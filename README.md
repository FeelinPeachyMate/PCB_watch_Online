# PCB Watch

This repository contains the firmware source code and PCB production files for a custom-designed LED watch based on the LumiDial concept.

- **Firmware:** C++ code for the ATmega328P microcontroller (`PCB_Watch`)
- **Hardware:** PCB production files for manufacturing the watch PCB (`PCB_Watch_Files`)
- A tutorial can be found in the main directory

---

## Project Description

The PCB Watch is a battery-powered LED watch featuring a circular LED matrix display and a single-button interface.

- Original PCB concept: Taifur (LumiDial)
- Firmware written by: Antara
- Target microcontroller: ATmega328P

---

## Firmware Responsibilities

The firmware is responsible for:

- Driving a 9×8 circular LED matrix to display hours, minutes, and seconds
- Handling single-button input for:
  - LED on/off toggling
  - Entering and cycling edit modes
  - Adjusting hours, minutes, and seconds
  - Toggling Daylight Saving Time (DST)
- Communicating with a DS3231M real-time clock (RTC) over I²C
- Reducing power consumption by disabling unused MCU peripherals

---

## Repository Contents

### Firmware

- Written in C++
- Target: ATmega328P

Uses:
- DS3231M RTC library
- Low-power configuration (2 MHz internal RC oscillator)

Implements:
- LED matrix multiplexing
- Edit mode with blinking feedback
- Manual DST handling
- Software button debouncing

---

### PCB Production Files

- Custom PCB design (39.6 mm × 39.6 mm)
- Includes manufacturing-ready files (Gerbers)

Designed to support:
- ATmega328P (TQFP-32)
- DS3231M RTC
- Circular LED matrix
- Single tactile button
- Li-Po battery and TP4056 charger
- ISP programming pads

---

## Hardware Summary

- Microcontroller: ATmega328P
- RTC: DS3231MZ+
- Display: Circular 9×8 LED matrix
- Input: Single tactile push button
- Power: 3.7 V Li-Po battery with TP4056 charging module

---

## Notes and Limitations

- The RTC does not include a backup battery
- Timekeeping and DST are manually controlled
- The watch cannot be powered back on via software alone
- Some function names reflect earlier design intentions and may not fully match final behavior

---

## Credits

- Original PCB concept: Taifur (LumiDial)
- Firmware and modifications: Antara
