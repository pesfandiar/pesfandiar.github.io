---
layout: post
title: "Raspberry Pi Pico as AM Radio Transmitter"
description: ""
category:
tags: [Raspberry Pi Pico, Embedded, Radio Frequency]
---
{% include JB/setup %}

*Obligatory legalese: emitting radio waves, even low-powered and/or short-lived may not be permitted in your jurisdiction.*

Years ago, people figured out Raspberry Pi's can accidentally double as [FM radio transmitters](https://github.com/rm-hull/pifm) without a need for any radio front-end (if we don't count a single jumper wire working as an antenna). They achieved this by tying a GPIO pin to a software-controlled clock around 100 MHz to modulate audio. This created a low-powered FM radio transmitter. Due to the pin producing a square wave instead of a neat sine wave, it also emitted weaker harmonics at 300MHz, 500MHz, etc., but any basic FM radio could pick up the audio. I wondered if a similar feat could be achieved by much less powerful Raspberry Pi Pico microcontrollers.

While a Pico board doesn't have the same architecture that allows FM transmission on regular Pi boards, it has a unique feature: [PIO](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#group_hardware_pio). This allows running programs based on a minimal instruction set independent of the CPU at the system clock rate. A simple program ("turn the pin ON, turn the pin OFF") can then produce a square wave at half the system clock rate. Pico boards could be overclocked at ~200 MHz, allowing a similar carrier frequency, but the exact frequency cannot be controlled by software in any practical way. This rules out clean FM.

A Pico board can however allow better control at lower frequencies within the typical AM radio station (~1000 KHz). While square waves are well-suited for frequency-modulating any PCM audio file (ignoring the unwanted harmonics), they cannot be used well for amplitude modulation. They can only encode ON or OFF instead of different audio amplitudes (think 1-bit envelope). Alright, no emitting arbitrary ".wav" files. We can at least emit a single audio frequency at a time using [On-Off Keying](https://en.wikipedia.org/wiki/On%E2%80%93off_keying). Let's make some retro game sounds!

With assistance from LLMs, I wrote [this code](https://github.com/pesfandiar/PicoProjects/tree/main/projects/pico_am_radio) that emits the [Shave and A Haircut](https://en.wikipedia.org/wiki/Shave_and_a_Haircut) melody on a 1000 KHz carrier. Here's a basic AM radio picking up the audio:

<video controls style="width: 100%; height: auto; display: block;">
  <source src="/assets/videos/pico_am_radio_transmitter/shave_and_a_haircut.mp4" type="video/mp4" />
  Your browser does not support the video tag.
</video>

One of the pins is [programmed](https://github.com/pesfandiar/PicoProjects/blob/main/projects/pico_am_radio/squarewave.pio) to produce a constant 1000 KHz square wave using PIO. The main program then turns the PIO on and off at the intended frequency. e.g. to emit an A4 pitch (440 Hz), it turns the 1000 KHz square wave on and off 440 times a second. To the radio receiver, this means a 440 Hz square wave audio. Using this technique, it emits the well-known Shave and A Haircut melody on a loop.
