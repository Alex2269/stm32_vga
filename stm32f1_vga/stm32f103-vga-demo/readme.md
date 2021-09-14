# VGA-demo-on-bluepill
VGA demo on a STM32F103 board

```
For this code use makefile, no any IDE.
Tiny changes associated with avoiding all IDEs
```

see Originally this: https://github.com/abelykh0/stm32f103-vga-demo

## What it can do
There are 4 demos:
* Displaying color image (taken from http://www.worldofspectrum.org/pub/sinclair/screens/load/b/scr/BubbleBobble.scr)
* Text typing and moving cursor (font is taken from http://caxapa.ru/149446.html)
* Drawing (taken from https://github.com/Avamander/arduino-tvout/blob/master/examples/DemoNTSC/DemoNTSC.pde)
* Game (taken from http://http.debian.net/debian/pool/main/x/xonix/xonix_1.4.orig.tar.gz)

<a href="http://www.youtube.com/watch?feature=player_embedded&v=97oakB1NX68
" target="_blank"><img src="http://img.youtube.com/vi/97oakB1NX68/0.jpg" 
alt="VGA Demo on Bluepill" width="480" height="360" border="10" /></a>

Video

## Introduction
My idea of “fun” maybe odd, but here it is. This is a demo that displays a color VGA video with resolution of 256x192 and 64 colors without any specialized video hardware on a tiny STM32F103 microcontroller that has only 20 KB of RAM with CPU running at 72 MHz.

## How to connect wires

| PIN | Description | Connect To | Output |
| --- | ----------- | ---------- | ------ |
| PB08 | Red 1 | Resistor 680 Ohm | VGA red (1)
| PB09 | Red 2 | Resistor 470 Ohm | VGA red (1)
| PB12 | Green 1 | Resistor 680 Ohm | VGA green (2)
| PB13 | Green 2 | Resistor 470 Ohm | VGA green (2)
| PB14 | Blue 1 | Resistor 680 Ohm | VGA blue (3)
| PB15 | Blue 2 | Resistor 470 Ohm | VGA blue (3)
| PB0 | HSync | | VGA HSync (13)
| PB6 | VSync | | VGA VSync (14)
| PA0 | CLK | Resistor 2K2 to keyboard CLK and resistor 3K3 to GND
| PA1 | DATA | Resistor 2K2 to keyboard DATA and resistor 3K3 to GND
| G | Ground | | VGA Ground (5,6,7,8,10)

## Hardware

| Hardware      |    Qty|
| ------------- | -----:|
| STM32F103 board | 1
| VGA connector | 1
| PS/2 Keyboard | 1
| Power supply 5V DC for keyboard | 1
| Resistors 470 Ohm | 3
| Resistors 680 Ohm | 3
| Resistors 2.2 KOhm | 2
| Resistors 3.3 KOhm | 2
| Breadboard | 1
| Jumper wires | 18
| ST-Link v2 or clone | 1

## Software

[System Workbench for STM32](https://www.st.com/en/development-tools/sw4stm32.html/)
