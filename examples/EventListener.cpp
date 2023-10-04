#include <Arduino.h>

#include "ArduinoJoystickLib.h"

// -----------------------------------------------------------------
const bool WAIT_FOR_SERIAL = true;
bool LOG_OUTPUT = true;

// -----------------------------------------------------------------
void printJoystickState(const uint8_t state) {
  if (state & JOYSTICK_STATE::JS_STATE_DOWN) {
    Serial.print("DOWN ");
  }
  if (state & JOYSTICK_STATE::JS_STATE_UP) {
    Serial.print("UP ");
  }
  if (state & JOYSTICK_STATE::JS_STATE_LEFT) {
    Serial.print("LEFT ");
  }
  if (state & JOYSTICK_STATE::JS_STATE_RIGHT) {
    Serial.print("RIGHT ");
  }
  if (state & JOYSTICK_STATE::JS_STATE_PRESSED) {
    Serial.print("BUTTON PRESSED ");
  }
}

// -----------------------------------------------------------------
class EventListener : public ArduinoJoystickEventListener {
 public:
  EventListener() : ArduinoJoystickEventListener(){};
  inline void onStateChange(uint8_t state) {
    Serial.print("state change: ");
    Serial.print(state);
    Serial.print(" --> ");
    printJoystickState(state);
    Serial.println();
  }
};

// -----------------------------------------------------------------
EventListener eventListener;
ArduinoJoystickEventSource js;

// -----------------------------------------------------------------
void startSerial() {
  Serial.begin(115200);
  while (!Serial && WAIT_FOR_SERIAL) {
    ;  // wait for serial port to connect. Needed for native USB port only
    delay(10);
  }
  delay(500);
}

// -----------------------------------------------------------------
void setup() {
  if (LOG_OUTPUT) {
    startSerial();
  }
  Serial.println("\nBegin");

  js.registerListener(&eventListener);

  js.setPins(A1, A2, A3);
  js.setup();

  js.printState();
};

// -----------------------------------------------------------------
void loop() {
  js.update();

  delay(100);
}
