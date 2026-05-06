# PCB Watch | sponsored by PCBway!

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

# Pcbway – Sponsership

First, I want to thank PCBway, and personally Liam from the marketing team for reaching out and offering this opportunity! Thank you so much. Also thank you for being patient with me as it took quite some time to arrange everything, especially due to my internship and university stuff.
If you are looking for a friendly and good place to order your PCB’s I would advice PCBway.

## Ordering process & Service
The ordering process went smoothly and showed alot more options than the pervious place I had ordered which was not only fun but also very useful!

From various surface finishes to fit to your exact needs, to many fun different PCB colours!

Every option is also supported with a green questionmark which contains all the explanation a beginner could need to understand. 
<img width="678" height="519" alt="image" src="https://github.com/user-attachments/assets/6e8db32e-dc00-4e26-b828-c1cba94b70ba" />
 Figure 1 Example explanation

During the development process the team made sure to reach out to confirm various things, they pay great attention to details and ensure that the client will be happy with the final result. From more complex confirms such as polarity to lower priorities such as silkscreen designs. 

Upon discovering certain things, they accept a corrected file and continue onwards. 

During Chinese new year they made sure to update the customer on the state of the PCB delivery and wished a happy new year.
Once finished the PCB’s arrived fairly quick. 

## First impression
The PCB’s arrived in static free bags ready to use, no need to click them out of any frame or anything, giving them a nice rounded clean ridge.

The silk screen looked very nice and even could handle small details like very tiny a kaomoji. The gold finish looked very neat.
<img width="466" height="457" alt="image" src="https://github.com/user-attachments/assets/f83cef6d-1bf6-497b-a6ee-b1068076ba06" />
Figure 2 PCB front
 
Overall, the components were attached sturdy, as I could use the button as leverage to pull the PCB out the watch case. 

Programming using the programming pads was also a pleasant surprise, as the previous one had gotten scratched up and dented from the pogo pins I had used, but the PCB from PCBway showed no such damage, as can be seen down below.   
<img width="441" height="433" alt="image" src="https://github.com/user-attachments/assets/1445b049-924a-4202-8fbb-9c2a034e794e" />
Figure 3 First prototype | damaged pads after use
<img width="389" height="444" alt="image" src="https://github.com/user-attachments/assets/3c48094e-d456-418a-884d-3b16b09d41db" />
Figure 4 | PCBway programming pads after use
## Usage
My first prototype was produced by a different PCB company, which will not be stated. This PCB was alright, but as the soldering pads broke off multiple times during production it did show some lack of quality. Recently the one that was gifted to a dear one, also had a pad break off the PCB, I got to first hand try the new ones delivered by the PCBway sponsorship!

First I want to mention how easy it is to get solder on the pads, which is very pleasant. Especially for beginners. 
Also, as I had made a small mistake in the orientation of a component ( my mistake truly I sent the wrong version ) I had to fix the orientation of an 8 pin chip. This task was deemed surprisingly easy and I was able to complete it on all the models I received within an hour, I believe this also speaks for the quality, and the ease for beginners aswell. As it is not hard to correct your own mistakes! 😊

## Conclusion
Here you can see the PCB back in its case, fully working! Overall I would highly recommend PCBway and would definitely order my future custom PCB’s at them! They deliver quality, the staff is kind and precise, and the price to quality ratio is good!

Once again thank you so much and if you are ordering yours: https://www.pcbway.com/ 
 <img width="761" height="747" alt="image" src="https://github.com/user-attachments/assets/03029f43-6e8b-4bf5-b29f-45a4f5ecb532" />
Figure 5 PCB front in case

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
