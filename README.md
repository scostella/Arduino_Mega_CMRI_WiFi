# Arduino_Mega_CMRI_WiFi

Arduino Mega 2560 sketch that exposes GPIO to **JMRI** or other C/MRI supporting software via the  
**C/MRI (CMRInet)** protocol, using an **ESP8266 Wi‑Fi bridge** connected on
**Serial3**.

This sketch uses the **ArduinoCMRI** library to emulate a C/MRI node, allowing
JMRI to read physical inputs (sensors, pushbuttons) and control outputs
(relays, LEDs, signals, control panels) on a model railroad layout.

---

## Repository Contents

- `Arduino_Mega_CMRI_WiFi.ino` — main Arduino sketch

---

## Overview

This sketch:

- Runs on an **Arduino Mega 2560**
- Communicates with an **ESP8266** over **Serial3** (pins 14/15)
- Presents **64 C/MRI bits** to JMRI (16 panels × 4 I/O each)
- Allows each panel to be independently configured as **input or output**
- Supports **analog pins used as digital inputs/outputs**
- Uses a **2‑second watchdog timer** to recover from lost communication

The ESP8266 handles the Wi‑Fi/TCP connection to JMRI.  
ESP8266 firmware is **not included** in this repository.

---

## Hardware Assumptions

### Arduino Mega 2560
- Communicates with ESP8266 via Serial3

### ESP8266 (Wi‑Fi Bridge)
- Bridges Serial3 traffic to JMRI over Wi‑Fi
- Drives a JMRI connection‑status signal into the Mega

---

## Wiring

### Reserved Pins
```markdown
|  Pin  | Purpose |
|-------|------------------------------------------------------|
|  0-1  | Serial0 (USB programming / debugging)                |
|   12  | JMRI connection status input from ESP8266            |
|   13  | Onboard LED visual display of JMRI connection status |
| 14-15 | Serial3 communication to ESP8266                     |
```

### Serial3 (Mega ↔ ESP8266)

| Mega Pin | Function | ESP8266-ESP01 | ESP8266-ESP01 Pin |
|--------|----------|---------|
| 14 | TX3 | UTX | 1 |
| 15 | RX3 | URX | 8 |
| GND | Ground | GND |

> ⚠️ ESP8266 uses **3.3 V logic**.  
> Use proper level shifting if required, the Arduino Mega CMRI WiFi Shield provides this level shifting.

### JMRI Connection Status

- **Mega pin 12** (`JMRIConnectedPin`)
  - Configured as `INPUT_PULLUP`
  - Driven HIGH by ESP8266 when JMRI is connected
- **Mega pin 13**
  - Onboard LED mirrors JMRI connection state

### Digital Pin Panels

| Panel | Pins |
|--------|----------|---------|
| Panel 1 | 2, 3, 16, 17 |
| Panel 2 | 4, 5, 6, 7 |
| Panel 3 | 8, 9, 10, 11 |
| Panel 4 | 18, 19, 20, 21 |
| Panel 5 | 22, 23, 24, 25 |
| Panel 6 | 26, 27, 28, 29 |
| Panel 7 | 30, 31, 32, 33 |
| Panel 8 | 34, 35, 36, 37 |
| Panel 9 | 38, 39, 40, 41 |
| Panel 10 | 42, 43, 44, 45 |
| Panel 11 | 46, 47, 48, 49 |
| Panel 12 | 50, 51, 52, 53 |
| Panel 13 | A0, A1, A2, A3 |
| Panel 14 | A4, A5, A6, A7 |
| Panel 15 | A8, A9, A10, A11 |
| Panel 16 | A12, A13, A14, A15 |

---

## Dependencies

- Arduino IDE (or PlatformIO)
- **ArduinoCMRI** library (`CMRI.h`)

---

## C/MRI Configuration

### Node Address
Configure the node address on the following line, default is 1.  As each network connected Arduino Mega is a separate connection and separate loop.  You should be able to leave this as Node 1 for each instance, but can change if it provides better organization for your use.

```cpp
#define CMRI_ADDR 1
CMRI cmri(CMRI_ADDR, 128, 64, Serial3);

### Panel operation (Input/Output)
Change value to True if the panel connected provides input (IR Sensor, Tortoise Feedback) the and false if it's an output (Accessory Controller, Light Controller, Tortoise Control).

```cpp
bool Panel1Input  = false;
bool Panel2Input  = false;
bool Panel3Input  = true;
bool Panel4Input  = false;

bool Panel5Input  = true;
bool Panel6Input  = true;
bool Panel7Input  = true;
bool Panel8Input  = true;

bool Panel9Input  = true;
bool Panel10Input = true;
bool Panel11Input = true;
bool Panel12Input = true;

bool Panel13Input = false;
bool Panel14Input = false;
bool Panel15Input = false;
bool Panel16Input = false;

### C/MRI Bit Mapping
CMRI Address reflects value for Node 1, for other Node values, replace the leading 1 with that Node value.

| C/MRI Bit | Panel | Arduino Mega Pin | CMRI Address |
|----------:|:------|:------------------|:------------------------|
| 0 | Panel 1 | D2 | 1001 |
| 1 | Panel 1 | D3 | 1002 |
| 2 | Panel 1 | D16 | 1003 |
| 3 | Panel 1 | D17 | 1004 |
| 4 | Panel 2 | D4 | 1005 |
| 5 | Panel 2 | D5 | 1006 |
| 6 | Panel 2 | D6 | 1007 |
| 7 | Panel 2 | D7 | 1008 |
| 8 | Panel 3 | D8 | 1009 |
| 9 | Panel 3 | D9 | 1010 |
| 10 | Panel 3 | D10 | 1011 |
| 11 | Panel 3 | D11 | 1012 |
| 12 | Panel 4 | D18 | 1013 |
| 13 | Panel 4 | D19 | 1014 |
| 14 | Panel 4 | D20 | 1015 |
| 15 | Panel 4 | D21 | 1016 |
| 16 | Panel 5 | D22 | 1017 |
| 17 | Panel 5 | D23 | 1018 |
| 18 | Panel 5 | D24 | 1019 |
| 19 | Panel 5 | D25 | 1020 |
| 20 | Panel 6 | D26 | 1021 |
| 21 | Panel 6 | D27 | 1022 |
| 22 | Panel 6 | D28 | 1023 |
| 23 | Panel 6 | D29 | 1024 |
| 24 | Panel 7 | D30 | 1025 |
| 25 | Panel 7 | D31 | 1026 |
| 26 | Panel 7 | D32 | 1027 |
| 27 | Panel 7 | D33 | 1028 |
| 28 | Panel 8 | D34 | 1029 |
| 29 | Panel 8 | D35 | 1030 |
| 30 | Panel 8 | D36 | 1031 |
| 31 | Panel 8 | D37 | 1032 |
| 32 | Panel 9 | D38 | 1033 |
| 33 | Panel 9 | D39 | 1034 |
| 34 | Panel 9 | D40 | 1035 |
| 35 | Panel 9 | D41 | 1036 |
| 36 | Panel 10 | D42 | 1037 |
| 37 | Panel 10 | D43 | 1038 |
| 38 | Panel 10 | D44 | 1039 |
| 39 | Panel 10 | D45 | 1040 |
| 40 | Panel 11 | D46 | 1041 |
| 41 | Panel 11 | D47 | 1042 |
| 42 | Panel 11 | D48 | 1043 |
| 43 | Panel 11 | D49 | 1044 |
| 44 | Panel 12 | D50 | 1045 |
| 45 | Panel 12 | D51 | 1046 |
| 46 | Panel 12 | D52 | 1047 |
| 47 | Panel 12 | D53 | 1048 |
| 48 | Panel 13 | A0 | 1049 |
| 49 | Panel 13 | A1 | 1050 |
| 50 | Panel 13 | A2 | 1051 |
| 51 | Panel 13 | A3 | 1052 |
| 52 | Panel 14 | A4 | 1053 |
| 53 | Panel 14 | A5 | 1054 |
| 54 | Panel 14 | A6 | 1055 |
| 55 | Panel 14 | A7 | 1056 |
| 56 | Panel 15 | A8 | 1057 |
| 57 | Panel 15 | A9 | 1058 |
| 58 | Panel 15 | A10 | 1059 |
| 59 | Panel 15 | A11 | 1060 |
| 60 | Panel 16 | A12 | 1061 |


