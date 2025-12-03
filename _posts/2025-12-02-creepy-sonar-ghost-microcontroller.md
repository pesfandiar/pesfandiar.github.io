---
layout: post
title: "Sonar-Powered Creepy Halloween Ghost"
description: ""
category:
tags: [Raspberry Pi Pico, Arduino, Embedded]
---
{% include JB/setup %}

I recently bought a [Raspberry Pi Pico W](https://www.raspberrypi.com/documentation/microcontrollers/pico-series.html#raspberry-pi-pico-w), which comes with a Bluetooth module, along with an HC-SR04 ultrasonic rangefinder. I wanted to hide it inside Halloween decorations and program it to make a creepy ghost sound that intensified as my child's Halloween party guests approached it. The children might have already aged out of being scared by it, but it was a technical success after all. I'm sharing the source code and some of the details here.

The ultrasonic rangefinder works by emitting high-frequency sound pulses and measuring how long it takes the echos to return from nearby objects. The emission and measurement has to be manually driven by the microcontroller. The microcontroller's job is to measure the distance regularly and if someone is standing within the range, produce a creepy ghost sound; a sine wave within 300-600 Hz with some extra low-frequency modulation to give it a "breathing" effect.

## Circuitry

The rangefinder requires 5V and Ground that could be easily obtained from Pico's VBUS and GND pins. Its TRIG pin, to drive wave emissions, can be controlled by a regular GPIO pin (GP2) as 3.3V is enough, but its ECHO pin outputs 5V. In order to prevent the rangefinder from frying the board by sending 5V into the receiving GPIO pin (GP3), I use a simple voltage divider using a 10K and a 15K ohm resistors. ECHO is separated from GP3 by 10k ohm, which is in turn pulled down to GND by 15k ohm. This brings down the voltage to about 3V, which reliably registers as high for a GPIO pin.

There isn't anything else to the circuit besides powering the board via USB.

![The circuit on a breadboard](/assets/images/creepy_sonar/circuit.jpg)

## Source code

Instead of using the standard C/C++ SDK, I opted for [Arduino-Pico v5.3.0](https://github.com/earlephilhower/arduino-pico). You can still program the board in C++ (Arduino styled), but it comes with a convenient Bluetooth driver and the IDE makes debugging via the serial port simpler. The entire [source code](/assets/main/code/creepy-sonar-ghost.ino) is inside a single file.

The setup defines the I/O pins and makes a connection to the Bluetooth speaker as an A2DPSource. For connection to the Bluetooth speaker, I specified my speaker's MAC address for reliability and simplicity. The main loop is in charge of driving the rangefinder and calculating and streaming the ghost sound, so to keep it tight and performant, I pre-calculate the sine function for 256 values (with linear interpolation between them) during the setup and store them in a look-up table.

The main loop polls the ultrasonic sensor by sending a 10-15 micro-second TRIG pulse and measuring how long the ECHO pin stays high. From this round-trip time, I compute the distance. The results are surprisingly accurate. If an object is within the range (15-150 cm), it starts generating a sine wave, whose frequency (300 to 900 Hz) and amplitude are a function of distance. This creates the illusion that the "ghost" is screaming harder as you get closer. As the pure sine wave can sound a bit artificial, I add a low-frequency oscillator to the mix. I maintain two separate phase accumulators; one for the main tone and one for the modulation, and read both from the lookup table. I apply smoothing and clamping functions to avoid clicks or harsh transitions. This is the part where generative AI and a few iterations helped me get the best results.

The audio output is then calculated at the sample rate of 48KHz. The left and right channels have the same value (mono) and I use a 2048-frame buffer for streaming the audio via Bluetooth. Raspberry Pi Pico is not exactly a high-performance microcontroller, but it could handle driving the sonar sensor and Bluetooth streaming in the same loop with occasional buffer underrun (resulting in clicks). To avoid that, a real-time OS (e.g. FreeRTOS) could be used to separate the sensing and streaming tasks, and ensuring the streaming takes priority over sensing (no major change in distance is expected anyway).