#include "SendMitsubishiCode.h" // custom extension of https://github.com/z3t0/Arduino-IRremote => uses pin D3
#include "LowPower.h"           // library by rocketscream, https://github.com/rocketscream/Low-Power

// Codes for IR remote control
#define CODE_WIN          0x2F9C
#define CODE_HDMI         0x2F42
#define CODE_OFF          0x2F72
#define CODE_ON           0x2F4A
#define CODE_DPORT        0x2FAA
#define CODE_DVI          0x2F8C

const auto WAKEUP_FUNCTION = [](void){/* Do nothing, just wake up */};
const int  BUTTON_CODES[]  = {CODE_WIN, CODE_HDMI, CODE_OFF, CODE_ON, CODE_DPORT, CODE_DVI};
const int  BUTTON_PINS[]   = {7,        8,         9,        10,      11,         12      };
const int  INTERRUPT_PIN   = 2;

void setup() {
  Serial.begin(9600);

  // In default configuration all buttons are outputs set to low and we are checking
  // on interrupt pin (pulled up by internal resistor) if it set to ground
  configureInterruptMode();
}

void loop() {
  sleepUntilExternalInterrupt();

  // Change configuration to distinguish what button is actually pressed
  configureDistinctMode();

  // Find out what button has been pressed, send corresponding IR code and
  // wait until the button is released again
  for (int i = 0; i < sizeof(BUTTON_PINS) / sizeof(int); i++) { 
    if (digitalRead(BUTTON_PINS[i]) == LOW) {
      Serial.println(BUTTON_CODES[i], HEX);

      switch (BUTTON_PINS[i]) {
        case 9:
          dimMonitor();
          break;
        case 10:
          brightenMonitor();
          break;
        default:
          sendMitsubishiCode(BUTTON_CODES[i]);
      }

      while(digitalRead(BUTTON_PINS[i]) == LOW) {
        delay(100);
      }
    }
  }

  // Return configuration to interrupt mode
  configureInterruptMode();
}

void sleepUntilExternalInterrupt(void) {
  Serial.println("Going to sleep");
  delay(100); // must delay, otherwise arduino can't send the message before it falls to sleep

  // Execute the deep sleep mode
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

  // Once interrupt is triggered, arduino wakes up and continues from here
  Serial.println("Waking up");
}

void configureInterruptMode(void) {
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), WAKEUP_FUNCTION, LOW);
 
  for (int i = 0; i < sizeof(BUTTON_PINS) / sizeof(int); i++) {
    pinMode(BUTTON_PINS[i], OUTPUT);
    digitalWrite(BUTTON_PINS[i], LOW);
  }

  Serial.println("Configured interrupt mode");
}

void configureDistinctMode(void) {
  detachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN));

  for (int i = 0; i < sizeof(BUTTON_PINS) / sizeof(int); i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }

  pinMode(INTERRUPT_PIN, OUTPUT);
  digitalWrite(INTERRUPT_PIN, LOW);
 
  Serial.println("Configured district mode");
}
