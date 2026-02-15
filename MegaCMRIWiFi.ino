#include <Wire.h> 
#include <CMRI.h>
#include <avr/wdt.h> 

int JMRIConnectedPin = 12;

bool watchdogReset = false;

// Pin Allocation for Mega 2560
// --------------------------------------------------------------------------
// Unusable Pins
// --------------------------------------------------------------------------
// Pin 0-1: Reserved for Serial0 Communication for debugging and programming.
// Pin 12 : Status LED
// Pin 13 : Onboard LED
// Pin 14-15 : Reserved for Serial3 Communication between Mega 2560 and ESP8266

// --------------------------------------------------------------------------
// Digital Pins
// --------------------------------------------------------------------------
// Pin 2, 3, 16, 17 : Panel 1
// Pin 4 - 7 : Panel 2
// Pin 8 - 11 : Panel 3
// Pin 18 - 21 : Panel 4
// Pin 22 - 25 : Panel 5
// Pin 26 - 29 : Panel 6
// Pin 30 - 33 : Panel 7
// Pin 34 - 37 : Panel 8
// Pin 38 - 41 : Panel 9
// Pin 42 - 45 : Panel 10
// Pin 46 - 49 : Panel 11
// Pin 50 - 53 : Panel 12

// --------------------------------------------------------------------------
// Analog Pins (Treated Digitally)
// --------------------------------------------------------------------------
// Pin A0 - A3 : Panel 13
// Pin A4 - A7 : Panel 14
// Pin A8 - A11 : Panel 15
// Pin A12 - A15 : Panel 16

String buffer = "";

bool cmriStreamActive = false;
bool cmriPollingStarted = false;

// Defining if panels are input or ouput

bool Panel1Input = false;
bool Panel2Input = false;
bool Panel3Input = true;
bool Panel4Input = false;

bool Panel5Input = true;
bool Panel6Input = true;
bool Panel7Input = true;
bool Panel8Input = true;

bool Panel9Input = true;
bool Panel10Input = true;
bool Panel11Input = true;
bool Panel12Input = true;

bool Panel13Input = false;
bool Panel14Input = false;
bool Panel15Input = false;
bool Panel16Input = false;

#define CMRI_ADDR 1 //CMRI node address in JMRI 

CMRI cmri(CMRI_ADDR, 128, 64, Serial3);

// Defining Sensor Pin Assignment for virtual block occupancy detection

// #define SN_INTXNW 4
// #define SN_INTXNE 5
// #define SN_HY1B_EOL 6
// #define SN_HY2C_EOL 7

// #define SN_HY1B_PARK 8
// #define SN_HY2C_PARK 9
// #define SN_HY1B_5 10
// #define SN_HY2C_SOL 11

// #define SN_HY1B_4 14
// #define SN_HY2B_EOL 15
// #define SN_HY1B_3 16
// #define SN_HY2B_PARK 17

// #define SN_HY1B_2 18
// #define SN_HY2B_2 19
// #define SN_HY1B_1 20
// #define SN_HY2B_1 21

// #define SN_HY1B_SOL 22
// #define SN_HY2B_SOL 23
// #define SN_HY1B_INTX 24
// #define SN_HY2A_EOL 25

// #define SN_HY1A_EOL 26
// #define SN_HY2A_PARK 27
// #define SN_HY1A_PARK 28
// #define SN_HY2A_5 29

// #define SN_HY1A_4 30
// #define SN_HY2A_4 31
// #define SN_HY1A_3 32
// #define SN_HY2A_3 33

// #define SN_HY1A_2 34
// #define SN_HY2A_2 35
// #define SN_HY1A_1 36
// #define SN_HY2A_1 37

// #define SN_HY1A_SOL 38
// #define SN_HY2A_SOL 39

// Defining variables to store virtual block occupancy status

// bool OC_INTXNW;
// bool OC_INTXNE;

// bool OC_HY1A;
// bool OC_HY1B;

// bool OC_HY2A;
// bool OC_HY2B;
// bool OC_HY2C;

void setup() { 

  uint8_t mcusr_mirror = MCUSR;

  // Check if watchdog caused the reset
  if (mcusr_mirror & (1 << WDRF)) {
    watchdogReset = true;
  }

  // Clear all reset flags
  MCUSR = 0;

  // Disable watchdog (important to avoid immediate reset)
  wdt_disable();

  Serial.begin(115200);

  Serial.println("-------------------------------");
  Serial.println("Starting Mega Initialization");

  if (watchdogReset) {
    Serial.println("Mega last reset was caused by the Watchdog Timer.");
  }

  // Setting up Pin 12 for Input ESP8266 JMRI Connect indication

  pinMode(JMRIConnectedPin, INPUT_PULLUP); 
  pinMode(13, OUTPUT);
  
  // Disable interrupts while configuring WDT
  cli();

  // Enable watchdog with 2‑second timeout
  wdt_enable(WDTO_2S);

  // Re-enable interrupts
  sei();

  Serial.println("Mega watchdog enabled (2s timeout)");


  // Setup Panel 1
  if (Panel1Input) {
    pinMode(2, INPUT_PULLUP); 
    pinMode(3, INPUT_PULLUP); 
    pinMode(16, INPUT_PULLUP); 
    pinMode(17, INPUT_PULLUP); 
  }
  else {
    pinMode(2, OUTPUT); 
    pinMode(3, OUTPUT); 
    pinMode(16, OUTPUT); 
    pinMode(17, OUTPUT); 
  }

  // Setup Panel 2
  if (Panel2Input) {
    pinMode(4, INPUT_PULLUP); 
    pinMode(5, INPUT_PULLUP); 
    pinMode(6, INPUT_PULLUP); 
    pinMode(7, INPUT_PULLUP); 
  }
  else {
    pinMode(4, OUTPUT); 
    pinMode(5, OUTPUT); 
    pinMode(6, OUTPUT); 
    pinMode(7, OUTPUT); 
  }

  // Setup Panel 3
  if (Panel3Input) {
    pinMode(8, INPUT_PULLUP); 
    pinMode(9, INPUT_PULLUP); 
    pinMode(10, INPUT_PULLUP); 
    pinMode(11, INPUT_PULLUP); 
  }
  else {
    pinMode(8, OUTPUT); 
    pinMode(9, OUTPUT); 
    pinMode(10, OUTPUT); 
    pinMode(11, OUTPUT); 
  }

// Setup Panel 4
  if (Panel4Input) {
    pinMode(18, INPUT_PULLUP); 
    pinMode(19, INPUT_PULLUP); 
    pinMode(20, INPUT_PULLUP); 
    pinMode(21, INPUT_PULLUP); 
  }
  else {
    pinMode(18, OUTPUT); 
    pinMode(19, OUTPUT); 
    pinMode(20, OUTPUT); 
    pinMode(21, OUTPUT); 
  }

// Setup Panel 5
  if (Panel5Input) {
    pinMode(22, INPUT_PULLUP); 
    pinMode(23, INPUT_PULLUP); 
    pinMode(24, INPUT_PULLUP); 
    pinMode(25, INPUT_PULLUP); 
  }
  else {
    pinMode(22, OUTPUT); 
    pinMode(23, OUTPUT); 
    pinMode(24, OUTPUT); 
    pinMode(25, OUTPUT); 
  }

// Setup Panel 6
  if (Panel6Input) {
    pinMode(26, INPUT_PULLUP); 
    pinMode(27, INPUT_PULLUP); 
    pinMode(28, INPUT_PULLUP); 
    pinMode(29, INPUT_PULLUP); 
  }
  else {
    pinMode(26, OUTPUT); 
    pinMode(27, OUTPUT); 
    pinMode(28, OUTPUT); 
    pinMode(29, OUTPUT); 
  }

// Setup Panel 7
  if (Panel7Input) {
    pinMode(30, INPUT_PULLUP); 
    pinMode(31, INPUT_PULLUP); 
    pinMode(32, INPUT_PULLUP); 
    pinMode(33, INPUT_PULLUP); 
  }
  else {
    pinMode(30, OUTPUT); 
    pinMode(31, OUTPUT); 
    pinMode(32, OUTPUT); 
    pinMode(33, OUTPUT); 
  }

// Setup Panel 8
  if (Panel8Input) {
    pinMode(34, INPUT_PULLUP); 
    pinMode(35, INPUT_PULLUP); 
    pinMode(36, INPUT_PULLUP); 
    pinMode(37, INPUT_PULLUP); 
  }
  else {
    pinMode(34, OUTPUT); 
    pinMode(35, OUTPUT); 
    pinMode(36, OUTPUT); 
    pinMode(37, OUTPUT); 
  }

// Setup Panel 9
  if (Panel9Input) {
    pinMode(38, INPUT_PULLUP); 
    pinMode(39, INPUT_PULLUP); 
    pinMode(40, INPUT_PULLUP); 
    pinMode(41, INPUT_PULLUP); 
  }
  else {
    pinMode(38, OUTPUT); 
    pinMode(39, OUTPUT); 
    pinMode(40, OUTPUT); 
    pinMode(41, OUTPUT); 
  }

 // Setup Panel 10
  if (Panel10Input) {
    pinMode(42, INPUT_PULLUP); 
    pinMode(43, INPUT_PULLUP); 
    pinMode(44, INPUT_PULLUP); 
    pinMode(45, INPUT_PULLUP); 
  }
  else {
    pinMode(42, OUTPUT); 
    pinMode(43, OUTPUT); 
    pinMode(44, OUTPUT); 
    pinMode(45, OUTPUT); 
  }

 // Setup Panel 11
  if (Panel11Input) {
    pinMode(46, INPUT_PULLUP); 
    pinMode(47, INPUT_PULLUP); 
    pinMode(48, INPUT_PULLUP); 
    pinMode(49, INPUT_PULLUP); 
  }
  else {
    pinMode(46, OUTPUT); 
    pinMode(47, OUTPUT); 
    pinMode(48, OUTPUT); 
    pinMode(49, OUTPUT); 
  }

 // Setup Panel 12
  if (Panel12Input) {
    pinMode(50, INPUT_PULLUP); 
    pinMode(51, INPUT_PULLUP); 
    pinMode(52, INPUT_PULLUP); 
    pinMode(53, INPUT_PULLUP); 
  }
  else {
    pinMode(50, OUTPUT); 
    pinMode(51, OUTPUT); 
    pinMode(52, OUTPUT); 
    pinMode(53, OUTPUT); 
  }

 // Setup Panel 13
  if (Panel13Input) {
    pinMode(A0, INPUT_PULLUP); 
    pinMode(A1, INPUT_PULLUP); 
    pinMode(A2, INPUT_PULLUP); 
    pinMode(A3, INPUT_PULLUP); 
  }
  else {
    pinMode(A0, OUTPUT); 
    pinMode(A1, OUTPUT); 
    pinMode(A2, OUTPUT); 
    pinMode(A3, OUTPUT); 
  }

 // Setup Panel 14
  if (Panel14Input) {
    pinMode(A4, INPUT_PULLUP); 
    pinMode(A5, INPUT_PULLUP); 
    pinMode(A6, INPUT_PULLUP); 
    pinMode(A7, INPUT_PULLUP); 
  }
  else {
    pinMode(A4, OUTPUT); 
    pinMode(A5, OUTPUT); 
    pinMode(A6, OUTPUT); 
    pinMode(A7, OUTPUT); 
  }

 // Setup Panel 15
  if (Panel15Input) {
    pinMode(A8, INPUT_PULLUP); 
    pinMode(A9, INPUT_PULLUP); 
    pinMode(A10, INPUT_PULLUP); 
    pinMode(A11, INPUT_PULLUP); 
  }
  else {
    pinMode(A8, OUTPUT); 
    pinMode(A9, OUTPUT); 
    pinMode(A10, OUTPUT); 
    pinMode(A11, OUTPUT); 
  }

 // Setup Panel 16
  if (Panel15Input) {
    pinMode(A12, INPUT_PULLUP); 
    pinMode(A13, INPUT_PULLUP); 
    pinMode(A14, INPUT_PULLUP); 
    pinMode(A15, INPUT_PULLUP); 
  }
  else {
    pinMode(A12, OUTPUT); 
    pinMode(A13, OUTPUT); 
    pinMode(A14, OUTPUT); 
    pinMode(A15, OUTPUT); 
  }

  // Start communication with ESP8266 WiFi 
  Serial3.begin(115200);

  WaitJMRIConnected();

  digitalWrite(13, LOW);

  Serial.println("Mega starting CMRI processing.");

} 

void loop(){ 

  // Serial.println("CMRI processing.");

  cmriStreamActive = cmri.process();

  // if(cmriStreamActive) {
  //   cmriPollingStarted = true;
  // }

  // if(!cmriPollingStarted) {
  //   wdt_reset();
  // }

  if(cmriStreamActive) {
    wdt_reset();
  }

  if (digitalRead(JMRIConnectedPin)) {
    digitalWrite(13, HIGH);
  }
  else {
    digitalWrite(13, LOW);
  }


  // Process Panel 1 - CMRI Addresses 1001 - 1004
  if (Panel1Input) {
    cmri.set_bit(0, !digitalRead(2));
    cmri.set_bit(1, !digitalRead(3));
    cmri.set_bit(2, !digitalRead(16));
    cmri.set_bit(3, !digitalRead(17));
  }
  else {
    digitalWrite(2, cmri.get_bit(0));
    digitalWrite(3, cmri.get_bit(1));
    digitalWrite(16, cmri.get_bit(2));
    digitalWrite(17, cmri.get_bit(3));
  }

  // Process Panel 2 - CMRI Addresses 1005 - 1008
  if (Panel2Input) {
    cmri.set_bit(4, !digitalRead(4));
    cmri.set_bit(5, !digitalRead(5));
    cmri.set_bit(6, !digitalRead(6));
    cmri.set_bit(7, !digitalRead(7));
  }
  else {
    digitalWrite(4, cmri.get_bit(4));
    digitalWrite(5, cmri.get_bit(5));
    digitalWrite(6, cmri.get_bit(6));
    digitalWrite(7, cmri.get_bit(7));
  }

  // Process Panel 3 - CMRI Addresses 1009 - 1012
  if (Panel3Input) {
    cmri.set_bit(8, !digitalRead(8));
    cmri.set_bit(9, !digitalRead(9));
    cmri.set_bit(10, !digitalRead(10));
    cmri.set_bit(11, !digitalRead(11));
  }
  else {
    digitalWrite(8, cmri.get_bit(8));
    digitalWrite(9, cmri.get_bit(9));
    digitalWrite(10, cmri.get_bit(10));
    digitalWrite(11, cmri.get_bit(11));
  }

  // Process Panel 4 - CMRI Addresses 1013 - 1016
  if (Panel4Input) {
    cmri.set_bit(12, !digitalRead(18));
    cmri.set_bit(13, !digitalRead(19));
    cmri.set_bit(14, !digitalRead(20));
    cmri.set_bit(15, !digitalRead(21));
  }
  else {
    digitalWrite(18, cmri.get_bit(12));
    digitalWrite(19, cmri.get_bit(13));
    digitalWrite(20, cmri.get_bit(14));
    digitalWrite(21, cmri.get_bit(15));
  }

  // Process Panel 5 - CMRI Addresses 1017 - 1020
  if (Panel5Input) {
    cmri.set_bit(16, !digitalRead(22));
    cmri.set_bit(17, !digitalRead(23));
    cmri.set_bit(18, !digitalRead(24));
    cmri.set_bit(19, !digitalRead(25));
  }
  else {
    digitalWrite(22, cmri.get_bit(16));
    digitalWrite(23, cmri.get_bit(17));
    digitalWrite(24, cmri.get_bit(18));
    digitalWrite(25, cmri.get_bit(19));
  }

  // Process Panel 6 - CMRI Addresses 1021 - 1024
  if (Panel6Input) {
    cmri.set_bit(20, !digitalRead(26));
    cmri.set_bit(21, !digitalRead(27));
    cmri.set_bit(22, !digitalRead(28));
    cmri.set_bit(23, !digitalRead(29));
  }
  else {
    digitalWrite(26, cmri.get_bit(20));
    digitalWrite(27, cmri.get_bit(21));
    digitalWrite(28, cmri.get_bit(22));
    digitalWrite(29, cmri.get_bit(23));
  }

  // Process Panel 7 - CMRI Addresses 1025 - 1028
  if (Panel7Input) {
    cmri.set_bit(24, !digitalRead(30));
    cmri.set_bit(25, !digitalRead(31));
    cmri.set_bit(26, !digitalRead(32));
    cmri.set_bit(27, !digitalRead(33));
  }
  else {
    digitalWrite(30, cmri.get_bit(24));
    digitalWrite(31, cmri.get_bit(25));
    digitalWrite(32, cmri.get_bit(26));
    digitalWrite(33, cmri.get_bit(27));
  }

  // Process Panel 8 - CMRI Addresses 1029 - 1032
  if (Panel8Input) {
    cmri.set_bit(28, !digitalRead(34));
    cmri.set_bit(29, !digitalRead(35));
    cmri.set_bit(30, !digitalRead(36));
    cmri.set_bit(31, !digitalRead(37));
  }
  else {
    digitalWrite(34, cmri.get_bit(28));
    digitalWrite(35, cmri.get_bit(29));
    digitalWrite(36, cmri.get_bit(30));
    digitalWrite(37, cmri.get_bit(31));
  }

  // Process Panel 9 - CMRI Addresses 1033 - 1036
  if (Panel9Input) {
    cmri.set_bit(32, !digitalRead(38));
    cmri.set_bit(33, !digitalRead(39));
    cmri.set_bit(34, !digitalRead(40));
    cmri.set_bit(35, !digitalRead(41));
  }
  else {
    digitalWrite(38, cmri.get_bit(32));
    digitalWrite(39, cmri.get_bit(33));
    digitalWrite(40, cmri.get_bit(34));
    digitalWrite(41, cmri.get_bit(35));
  }

  // Process Panel 10 - CMRI Addresses 1037 - 1040
  if (Panel10Input) {
    cmri.set_bit(36, !digitalRead(42));
    cmri.set_bit(37, !digitalRead(43));
    cmri.set_bit(38, !digitalRead(44));
    cmri.set_bit(39, !digitalRead(45));
  }
  else {
    digitalWrite(42, cmri.get_bit(36));
    digitalWrite(43, cmri.get_bit(37));
    digitalWrite(44, cmri.get_bit(38));
    digitalWrite(45, cmri.get_bit(39));
  }

  // Process Panel 11 - CMRI Addresses 1041 - 1044
  if (Panel11Input) {
    cmri.set_bit(40, !digitalRead(46));
    cmri.set_bit(41, !digitalRead(47));
    cmri.set_bit(42, !digitalRead(48));
    cmri.set_bit(43, !digitalRead(49));
  }
  else {
    digitalWrite(46, cmri.get_bit(40));
    digitalWrite(47, cmri.get_bit(41));
    digitalWrite(48, cmri.get_bit(42));
    digitalWrite(49, cmri.get_bit(43));
  }

  // Process Panel 12 - CMRI Addresses 1045 - 1048
  if (Panel12Input) {
    cmri.set_bit(44, !digitalRead(50));
    cmri.set_bit(45, !digitalRead(51));
    cmri.set_bit(46, !digitalRead(52));
    cmri.set_bit(47, !digitalRead(53));
  }
  else {
    digitalWrite(50, cmri.get_bit(44));
    digitalWrite(51, cmri.get_bit(45));
    digitalWrite(52, cmri.get_bit(46));
    digitalWrite(53, cmri.get_bit(47));
  }

  // Process Panel 13 - CMRI Addresses 1049 - 1052
  if (Panel13Input) {
    cmri.set_bit(48, checkAnalogThreshold(analogRead(A0)));
    cmri.set_bit(49, checkAnalogThreshold(analogRead(A1)));
    cmri.set_bit(50, checkAnalogThreshold(analogRead(A2)));
    cmri.set_bit(51, checkAnalogThreshold(analogRead(A3)));
  }
  else {
    digitalWrite(A0, cmri.get_bit(48));
    digitalWrite(A1, cmri.get_bit(49));
    digitalWrite(A2, cmri.get_bit(50));
    digitalWrite(A3, cmri.get_bit(51));
  }

  // Process Panel 14 - CMRI Addresses 1053 - 1056
  if (Panel14Input) {
    cmri.set_bit(52, checkAnalogThreshold(analogRead(A4)));
    cmri.set_bit(53, checkAnalogThreshold(analogRead(A5)));
    cmri.set_bit(54, checkAnalogThreshold(analogRead(A6)));
    cmri.set_bit(55, checkAnalogThreshold(analogRead(A7)));
  }
  else {
    digitalWrite(A4, cmri.get_bit(52));
    digitalWrite(A5, cmri.get_bit(53));
    digitalWrite(A6, cmri.get_bit(54));
    digitalWrite(A7, cmri.get_bit(55));
  }

  // Process Panel 15 - CMRI Addresses 1057 - 1060
  if (Panel15Input) {
    cmri.set_bit(56, checkAnalogThreshold(analogRead(A8)));
    cmri.set_bit(57, checkAnalogThreshold(analogRead(A9)));
    cmri.set_bit(58, checkAnalogThreshold(analogRead(A10)));
    cmri.set_bit(59, checkAnalogThreshold(analogRead(A11)));
  }
  else {
    digitalWrite(A8, cmri.get_bit(56));
    digitalWrite(A9, cmri.get_bit(57));
    digitalWrite(A10, cmri.get_bit(58));
    digitalWrite(A11, cmri.get_bit(59));
  }

  // Process Panel 16 - CMRI Addresses 1061 - 1064
  if (Panel15Input) {
    cmri.set_bit(60, checkAnalogThreshold(analogRead(A12)));
    cmri.set_bit(61, checkAnalogThreshold(analogRead(A13)));
    cmri.set_bit(62, checkAnalogThreshold(analogRead(A14)));
    cmri.set_bit(63, checkAnalogThreshold(analogRead(A15)));
  }
  else {
    digitalWrite(A12, cmri.get_bit(60));
    digitalWrite(A13, cmri.get_bit(61));
    digitalWrite(A14, cmri.get_bit(62));
    digitalWrite(A15, cmri.get_bit(63));
  }


// Processing Virtual Block Occupancy

  // OC_INTXNW = false;
  // OC_INTXNE = false;

  // OC_HY1A = false;
  // OC_HY1B = false;

  // OC_HY2A = false;
  // OC_HY2B = false;
  // OC_HY2C = false;

  // if(!digitalRead(SN_INTXNW)) {
  //   OC_INTXNW = true;
  // }

  // if(!digitalRead(SN_INTXNE)) {
  //   OC_INTXNE = true;
  // }

  // if(!digitalRead(SN_HY1A_EOL) || !digitalRead(SN_HY1A_PARK) || !digitalRead(SN_HY1A_4) || !digitalRead(SN_HY1A_3) || !digitalRead(SN_HY1A_2) || !digitalRead(SN_HY1A_1) || !digitalRead(SN_HY1A_SOL)) {
  //   OC_HY1A = true;
  // }

  // if(!digitalRead(SN_HY1B_EOL) || !digitalRead(SN_HY1B_PARK) || !digitalRead(SN_HY1B_5) || !digitalRead(SN_HY1B_4) || !digitalRead(SN_HY1B_3) || !digitalRead(SN_HY1B_2) || !digitalRead(SN_HY1B_1) || !digitalRead(SN_HY1B_SOL) || !digitalRead(SN_HY1B_INTX)) {
  //   OC_HY1B = true;
  // }

  // if(!digitalRead(SN_HY2A_EOL) || !digitalRead(SN_HY2A_PARK) || !digitalRead(SN_HY2A_5) || !digitalRead(SN_HY2A_4) || !digitalRead(SN_HY2A_3) || !digitalRead(SN_HY2A_2) || !digitalRead(SN_HY2A_1) || !digitalRead(SN_HY2A_SOL)) {
  //   OC_HY2A = true;
  // }

  // if(!digitalRead(SN_HY2B_EOL) || !digitalRead(SN_HY2B_PARK) || !digitalRead(SN_HY2B_2) || !digitalRead(SN_HY2B_1) || !digitalRead(SN_HY2B_SOL)) {
  //   OC_HY2B = true;
  // }

  // if(!digitalRead(SN_HY2C_EOL) || !digitalRead(SN_HY2C_PARK) || !digitalRead(SN_HY2C_SOL)){
  //   OC_HY2C = true;
  // }

  // Writing Virtual Block Occupancy Detection to CMRI

  // cmri.set_bit(100, OC_INTXNW); // CMRI ADDRESS 1101
  // cmri.set_bit(101, OC_INTXNE); // CMRI ADDRESS 1102
  // cmri.set_bit(102, OC_HY1B); // CMRI ADDRESS 1103
  // cmri.set_bit(103, OC_HY2C); // CMRI ADDRESS 1104
  // cmri.set_bit(104, OC_HY1A); // CMRI ADDRESS 1105
  // cmri.set_bit(105, OC_HY2A); // CMRI ADDRESS 1106
  // cmri.set_bit(106, OC_HY2B); // CMRI ADDRESS 1107


} 

void WaitJMRIConnected() {
  buffer = "";  // clear any old data

  Serial.println("Mega initialization complete, waiting for Wifi connection, Server Start, and JMRI to connect.");

  while (true) {
    while (Serial3.available()) {
      char c = Serial3.read();
      Serial.write(c);      // forward to Serial

      buffer += c;          // accumulate text

      // keep buffer small
      if (buffer.length() > 50) {
        buffer.remove(0, buffer.length() - 50);
      }

    }
    
    // check for trigger phrase
    if (digitalRead(JMRIConnectedPin)) {
      return;   // exit the function once detected
    }

    // Reset (kick) the watchdog so it doesn't reset the board 
    wdt_reset();

  }
}

int checkAnalogThreshold(int value) {
  if (value < 512) {
    return 0;
  } else {
    return 1;
  }
}