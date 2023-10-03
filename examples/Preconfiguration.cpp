#include <Arduino.h>
#include "ArduinoJoystickLib.h"

// -----------------------------------------------------------------

const bool WAIT_FOR_SERIAL = true;
bool LOG_OUTPUT = true;

ArduinoJoystick js;

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

  /* set configuration beforehand */
  jsCalibrationInfo calibData;

  calibData.vx_ref = 467;
  calibData.vy_ref = 464;
  calibData.sw_ref = 1020;

  js.setPins(A1, A2, A3);

  /* set configuration */
  js.setCalibrationData(calibData);
  
  js.setup();

  js.printState();
};


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
void printJoystickRawValues() {
  Serial.print("vx=");
  Serial.print(js.getLastVX());
  Serial.print(", vy=");
  Serial.print(js.getLastVY());
  Serial.print(", sw=");
  Serial.print(js.getLastSW());
}

unsigned long lastStatsTs = 0;
// -----------------------------------------------------------------
void loop() {
  const uint8_t jsState = js.update();
  if (js.isAnyActivity()) {
    Serial.print("JS STATE CHANGED: ");
    Serial.print(jsState);
    Serial.print(" [");
    printJoystickState(jsState);
    Serial.print("] -- [");
    printJoystickRawValues();
    Serial.println("]");
  }

  delay(100);

  if (millis() - lastStatsTs > 5000) {
    js.printState();
    lastStatsTs = millis();
  }
}
