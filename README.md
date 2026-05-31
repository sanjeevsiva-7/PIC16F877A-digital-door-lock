# PIC16F877A-digital-door-lock
# PIC16F877A Digital Security Lock System

## Overview

A password-protected digital door lock system developed using the PIC16F877A microcontroller. The system uses a 4×3 matrix keypad for password entry, a 16×2 LCD for user interaction, and a relay-based actuator to control the locking mechanism.

The firmware is written in Embedded C using MPLAB X IDE and the XC8 compiler. The project demonstrates keypad interfacing, LCD communication in 4-bit mode, password authentication logic, and relay control.

---

## Features

* 4×3 Matrix Keypad Interfacing
* 16×2 LCD Interfacing in 4-Bit Mode
* Password-Based Authentication
* Password Masking Using Asterisks (*)
* Relay-Based Lock Control
* Automatic Relocking After Successful Authentication
* Key Debouncing Using Software Logic
* Proteus Simulation Compatible

---

## Hardware Components

| Component                 | Description            |
| ------------------------- | ---------------------- |
| PIC16F877A                | Main Microcontroller   |
| 16×2 LCD                  | User Interface Display |
| 4×3 Matrix Keypad         | Password Input         |
| BC547 Transistor          | Relay Driver           |
| 5v  Relay                 | Lock Actuator Control  |
| 20 MHz Crystal Oscillator | System Clock           |
| 22 pF Capacitors          | Crystal Support        |
| 10 kΩ Resistors           | Keypad Pull-Up Network |

---

## Hardware Connections

### LCD Connections

| LCD Pin | PIC Pin |
| ------- | ------- |
| RS      | RB0     |
| EN      | RB1     |
| D4      | RB2     |
| D5      | RB3     |
| D6      | RB4     |
| D7      | RB5     |

### Keypad Connections

| Keypad Pin | PIC Pin |
| ---------- | ------- |
| Row A      | RD4     |
| Row B      | RD5     |
| Row C      | RD6     |
| Row D      | RD7     |
| Column 1   | RD0     |
| Column 2   | RD1     |
| Column 3   | RD2     |

### Relay Driver

| Signal        | PIC Pin |
| ------------- | ------- |
| Relay Control | RC0     |

---

## System Workflow

1. System powers up and initializes the LCD.
2. User enters a 4-digit password using the keypad.
3. Entered digits are displayed as `*` for privacy.
4. Password is compared with the stored master password.
5. If the password matches:

   * LCD displays **ACCESS GRANTED**
   * Relay is activated
   * Door remains unlocked for 5 seconds
   * Relay is deactivated automatically
6. If the password is incorrect:

   * LCD displays **ACCESS DENIED**
7. System resets and waits for the next user input.

---

## Embedded Concepts Demonstrated

* GPIO Configuration
* Matrix Keypad Scanning
* LCD Driver Development
* Bit Masking and Bit Manipulation
* State-Based Authentication Logic
* Relay Interfacing
* Software Debouncing
* Embedded C Programming
* Firmware Development for PIC Microcontrollers

---

## Development Environment

* MPLAB X IDE
* XC8 Compiler
* Proteus 8 Professional
* PIC16F877A Microcontroller

---

## Repository Structure

```text
.
├── firmware/
│   └── main.c
├── hardware/
│   ├── schematic.pdf
│   └── proteus_design.pdsprj
├── images/
│   ├── circuit.png
│   ├── access_granted.png
│   └── access_denied.png
└── README.md
```

## Future Improvements

* EEPROM-Based Password Storage
* Password Change Functionality
* Buzzer-Based Alerts
* Multiple User Support
* RFID Integration
* GSM Notification System

## Author

Sanjeev Siva
B.Tech Electronics and Communication Engineering (ECE)

Embedded Systems | Firmware Development | Microcontrollers

