# Arduino Function Generator Project

This project implements a **function generator** using an Arduino and an LCD.  
It can generate different waveforms such as **Sine, Triangle, Square, Sawtooth, and Pulse** with adjustable frequency and duty cycle (for pulse).  

The project also includes **Proteus simulation files** and real oscilloscope screenshots/videos for verification.

---

## Project Contents

- **Arduino/**  
  Contains the Arduino `.ino` code file for the function generator.  

- **Protues/**  
  Contains the Proteus simulation project files.  

- **Media/**  
  Contains images and videos of the waveforms captured from the oscilloscope.  
  - `oscilloscope_sine.png` – Sine waveform  
  - `oscilloscope_triangle.png` – Triangle waveform  
  - `oscilloscope_square.png` – Square waveform  
  - `oscilloscope_sawtooth.png` – Sawtooth waveform  
  - `FrequencyControl.mp4` – Video showing frequency control  
  - `waveformControl.mp4` – Video showing waveform selection  

---

## Features

- Select **waveform type** (Sine, Triangle, Square, Sawtooth, Pulse) using a button.  
- Adjust **frequency** using a button (predefined steps: 50, 100, 150, 200, 250, 300 Hz).  
- Adjust **duty cycle** for the Pulse waveform (10% to 90%).  
- Display current waveform, frequency, and duty cycle on a 16x2 LCD.  

---

## Arduino Code Overview

- **Buttons:**  
  - `BTN_FREQ` – change frequency  
  - `BTN_WAVE` – change waveform  
- **Waveform Tables:**  
  Arrays containing samples for each waveform.  
- **Timer Setup:**  
  Uses Arduino Timer1 to generate precise waveform output.  
- **Interrupt Service Routine (ISR):**  
  Sends the waveform samples to `PORTD` to generate the output.  
- **LCD Display:**  
  Updates with current waveform, frequency, and duty cycle.  

---

## How to Run

1. Open the Arduino `.ino` file in Arduino IDE.  
2. Connect the buttons and LCD as specified in the code.  
3. Upload the code to an Arduino board.  
4. Run the Proteus simulation if you want to test virtually.  
5. Observe the output waveform on an oscilloscope or in the simulation.  

---

## Notes

- Make sure the Arduino is connected correctly with the DAC pins.  
- Pulse waveform duty cycle only works when Pulse is selected.  
- All media files are captured from the real oscilloscope or simulation for reference.  

---

## Author

**Sama Yasin** – Electronics Engineering Student
