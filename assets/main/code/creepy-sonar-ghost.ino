/**
 * Creepy Proximity Ghost -> Bluetooth Speaker (A2DP Source)
 * Target: Raspberry Pi Pico W  (Arduino-Pico core 5.3.0 by Earle Philhower)
 * Library: <BluetoothAudio.h>  -> class A2DPSource
 *
 * IMPORTANT (WIRING):
 *  - HC-SR04 Ultrasonic:
 *      VCC  -> 5V (Pico VBUS if USB-powered)
 *      GND  -> GND (common with Pico)
 *      TRIG -> GP2  (3.3V output OK)
 *      ECHO -> GP3  (MUST be divided to 3.3V with a resistor divider)
 *        Example divider: 10k (from ECHO) -> node -> 15k (to GND), node -> GP3
 *        Power the sensor from 5V (not 3.3V) for reliable ranging.
 *
 *  - Bluetooth speaker: supply its MAC below and we connect via A2DPSource::connect(mac).
 *
 * DESIGN GOAL:
 *  - Stream a single, smooth "ghost" tone to a preset Bluetooth speaker
 *  - The tone is silent when distance >= FAR_CM.
 *  - As you get closer: pitch rises (300->900 Hz) and overall loudness increases.
 *  - A slow "breath" envelope adds life, but it’s gentle and smoothed.
 *  - Distance and params are smoothed so it never sounds "boily" or "jittery".
 */

#include <Arduino.h>
#include <BluetoothAudio.h>     // A2DPSource lives here
#include <math.h>

// Bluetooth A2DP Source
A2DPSource bt;

// TODO: REPLACE with your speaker’s MAC address bytes:
static const uint8_t SPEAKER_ADDR[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Audio format & block sizing
constexpr uint32_t SR        = 48000;   // Sample Rate
constexpr uint8_t  CHANNELS  = 2;       // interleaved stereo (mono duplicated)
constexpr size_t   FRAMES    = 2048;    // bigger block -> smoother (~42.7 ms @ 48k)

// Sonar pins (HC-SR04)
constexpr uint8_t  PIN_TRIG = 2;        // GP2
constexpr uint8_t  PIN_ECHO = 3;        // GP3 (through 10k:15k divider to 3.3V)

// Sonar behavior
constexpr uint32_t SONAR_PERIOD_MS  = 80;
constexpr uint32_t SONAR_TIMEOUT_US = 30000; // up to ~5m echo window for bring-up
static float g_lastDistanceCm = 100.0f;

// Define near/far range for activation & mapping
constexpr float NEAR_CM = 15.0f;
constexpr float FAR_CM  = 150.0f;   // SILENT when distance >= FAR_CM

// Very small sine Look-Up Table + linear interpolation
constexpr int      LUT_BITS = 8;              // 256 entries
constexpr uint32_t LUT_LEN  = (1u << LUT_BITS);
static int16_t     SINE_LUT[LUT_LEN];

// Convert frequency to phase increase per sample
static inline uint32_t phaseIncFromHz(float f) {
  return (uint32_t)((double)f * (4294967296.0 / (double)SR));  // f * 2^32 / SR
}

// Calculate sine from look-up table and linear interpolation
static inline int16_t lutSineLerp(uint32_t phase) {
  const uint32_t idx  = phase >> (32 - LUT_BITS);
  const uint32_t frac = (phase >> (32 - LUT_BITS - 8)) & 0xFF;
  const int16_t  a = SINE_LUT[idx & (LUT_LEN - 1)];
  const int16_t  b = SINE_LUT[(idx + 1) & (LUT_LEN - 1)];
  const int32_t  d = (int32_t)b - (int32_t)a;
  return (int16_t)(a + ((d * (int32_t)frac) >> 8));
}

struct Osc {
  uint32_t phase = 0;
  uint32_t inc   = 0;
  inline int16_t next() { phase += inc; return lutSineLerp(phase); }
};

// One carrier + one very slow "breath" Low-Frequency Oscillator for gentle amplitude shaping
static Osc oscCar;
static Osc oscBreath;

// Helpers
static inline float clampf(float x, float a, float b) { return x < a ? a : (x > b ? b : x); }
static inline float lerp(float a, float b, float t)   { return a + (b - a) * t; }

// Distance -> proximity in 0..1 (closer -> larger)
static inline float proximity01FromDistance(float cm) {
  float u = 1.0f - (cm - NEAR_CM) / (FAR_CM - NEAR_CM);
  return clampf(u, 0.0f, 1.0f);
}

// Simple sonar polling
static uint32_t g_lastSonarMs = 0;
static void pollSonar() {
  const uint32_t now = millis();
  if (now - g_lastSonarMs < SONAR_PERIOD_MS) return;
  g_lastSonarMs = now;

  digitalWrite(PIN_TRIG, LOW);  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH); delayMicroseconds(15);
  digitalWrite(PIN_TRIG, LOW);

  unsigned long dur = pulseIn(PIN_ECHO, HIGH, SONAR_TIMEOUT_US); // micro-sec
  if (dur == 0) { g_lastDistanceCm = FAR_CM; return; }
  g_lastDistanceCm = (float)dur / 58.0f;   // HC‑SR04: micro-sec -> cm
}

// Smooth ghost parameters: only smooth distance, then map to pitch + loudness.
static float d_cm_sm     = FAR_CM;    // smoothed distance
static float prox_sm     = 0.0f;      // smoothed proximity
static float f_car_sm    = 220.0f;    // smoothed carrier Hz
static float amp_sm      = 0.0f;      // smoothed amplitude 0..1

// Tuning
constexpr float MASTER_GAIN    = 0.60f;   // overall loudness
constexpr float FREQ_MIN_HZ    = 150.0f;
constexpr float FREQ_MAX_HZ    = 450.0f;
constexpr float BREATH_MIN_HZ  = 0.12f;   // very slow
constexpr float BREATH_MAX_HZ  = 0.40f;

// Arduino Setup
void setup() {
  // (Optional) Serial debug
  Serial.begin(115200);
  // while (!Serial) delay(10);  // comment/uncomment as you like

  // Build sine LUT
  for (uint32_t i=0; i<LUT_LEN; ++i) {
    double th = (2.0 * M_PI * i) / (double)LUT_LEN;
    SINE_LUT[i] = (int16_t)lrint(32767.0 * sin(th));
  }

  // Sonar pins
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  // Bluetooth source
  bt.begin();
  // Connect by MAC (keep trying in loop if dropped)
  bt.connect(SPEAKER_ADDR);

  // Init oscillators
  oscCar.inc    = phaseIncFromHz(440.0f);
  oscBreath.inc = phaseIncFromHz(BREATH_MIN_HZ);
}

// Arduino Loop
void loop() {
  // 1) Update sonar distance
  pollSonar();

  // 2) Smooth distance & derive proximity
  //    Very gentle smoothing to avoid steps
  const float a_d   = 0.15f;                           // distance smoothing
  d_cm_sm = d_cm_sm + a_d * (g_lastDistanceCm - d_cm_sm);

  float prox = proximity01FromDistance(d_cm_sm);
  const float a_p   = 0.20f;                           // proximity smoothing
  prox_sm = (1.0f - a_p) * prox_sm + a_p * prox;

  // 3) Map to pitch + loudness
  //    - Silence when prox_sm == 0 (distance >= FAR_CM)
  //    - Pitch: 300->900 Hz smoothly with proximity
  //    - Loudness: quadratic for natural swell (nearer = much louder)
  float f_car_target = lerp(FREQ_MIN_HZ, FREQ_MAX_HZ, prox_sm);
  const float a_fc   = 0.10f;                          // pitch smoothing
  f_car_sm = (1.0f - a_fc) * f_car_sm + a_fc * f_car_target;

  float amp_target   = prox_sm * prox_sm;              // 0..1, quadratic
  const float a_amp  = 0.20f;                          // amplitude smoothing
  amp_sm = (1.0f - a_amp) * amp_sm + a_amp * amp_target;

  // 4) Breath rate goes slightly faster as you get close, but stays slow.
  float f_breath = lerp(BREATH_MIN_HZ, BREATH_MAX_HZ, prox_sm);
  oscBreath.inc  = phaseIncFromHz(f_breath);
  float breath   = 0.75f + 0.25f * ((float)oscBreath.next() / 32767.0f); // 0.5..1.0 range

  // 5) Prepare audio block
  static int16_t buf[FRAMES * CHANNELS];

  // Silent when far: write zeros (keeps A2DP happy, no glitches)
  bool silent = (prox_sm <= 0.0005f);  // tiny hysteresis
  if (silent) {
    memset(buf, 0, sizeof(buf));
  } else {
    // Update carrier increment for this block
    oscCar.inc = phaseIncFromHz(f_car_sm);

    for (size_t i=0; i<FRAMES; ++i) {
      float car = (float)oscCar.next() / 32767.0f;   // -1..1
      float a   = MASTER_GAIN * amp_sm * breath;     // 0..~0.6
      float y   = a * car;                           // final mono
      // soft clip guard
      if (y >  0.999f) y =  0.999f;
      if (y < -0.999f) y = -0.999f;
      int16_t s = (int16_t)lrintf(y * 32767.0f);
      buf[2*i + 0] = s;  // L
      buf[2*i + 1] = s;  // R
    }
  }

  // 6) Stream to A2DP
  size_t nbytes = sizeof(buf);
  const uint8_t* p = reinterpret_cast<const uint8_t*>(buf);
  while (nbytes) {
    int w = bt.write(p, nbytes);
    if (w > 0) { p += w; nbytes -= w; }
    else       { delay(1); }
  }

  // debug once per block:
  // Serial.printf("d=%.1f cm  prox=%.2f  f=%.1f Hz  amp=%.2f  breath=%.2f  %s\n",
  //               g_lastDistanceCm, prox_sm, f_car_sm, amp_sm, breath, silent?"(silent)":"");
}
