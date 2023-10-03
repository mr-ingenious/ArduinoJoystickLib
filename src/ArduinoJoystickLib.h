
#ifndef __ARDUINO_JOYSTICK_LIB__
#define __ARDUINO_JOYSTICK_LIB__

#include "Arduino.h"

/**
 * Arduino Joystick Module Library.
 * @author Philipp Glass
 */

enum JOYSTICK_STATE {
  JS_STATE_PRESSED = 1,
  JS_STATE_LEFT = 2,
  JS_STATE_RIGHT = 4,
  JS_STATE_UP = 8,
  JS_STATE_DOWN = 16
};

struct jsCalibrationInfo {
  int vx_ref = 512;
  int vx_min = 2000;
  int vx_max = 0;

  int vy_ref = 512;
  int vy_min = 2000;
  int vy_max = 0;

  int sw_ref = 1024;
  int sw_min = 2000;
  int sw_max = 0;
};

class ArduinoJoystick {
 public:
  ArduinoJoystick();

  ArduinoJoystick(int vx_pin, int vy_pin, int sw_pin);

  void setPins(int vx_pin, int vy_pin, int sw_pin);
  void setup();

  struct jsCalibrationInfo calibrate();
  struct jsCalibrationInfo getCalibrationData();
  void setCalibrationData(struct jsCalibrationInfo calibData);

  uint8_t update();

  inline uint8_t getLastState() { return _state; }

  inline bool isRight() { return (_state & JS_STATE_RIGHT); }
  inline bool isLeft() { return (_state & JS_STATE_LEFT); }
  inline bool isUp() { return (_state & JS_STATE_UP); }
  inline bool isDown() { return (_state & JS_STATE_DOWN); }
  inline bool isButtonPressed() { return (_state & JS_STATE_PRESSED); }
  inline bool isAnyActivity() { return (_state > 0); };

  inline int getLastVX() { return _last_vx; };
  inline int getLastVY() { return _last_vy; };
  inline int getLastSW() { return _last_sw; };

  void printState();

 private:
  struct jsCalibrationInfo _c_data;
  int _vx_pin = 0, _vy_pin = 0, _sw_pin = 0;
  int _last_vx = 0, _last_vy = 0, _last_sw = 0;
  uint8_t _state = 0;
  bool isReady = false;
  bool isPreconfigured = false;
};

#endif