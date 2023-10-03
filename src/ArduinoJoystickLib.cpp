#include "ArduinoJoystickLib.h"

ArduinoJoystick::ArduinoJoystick(int vx_pin, int vy_pin, int sw_pin) {
  setPins(vx_pin, vy_pin, sw_pin);
}

ArduinoJoystick::ArduinoJoystick() {}

void ArduinoJoystick::setPins(int vx_pin, int vy_pin, int sw_pin) {
  _vx_pin = vx_pin;
  _vy_pin = vy_pin;
  _sw_pin = sw_pin;
}

uint8_t ArduinoJoystick::update() {
  if (isReady) {
    _state = 0;

    _last_vx = analogRead(_vx_pin);

    if (_c_data.vx_min > _last_vx) {
      _c_data.vx_min = _last_vx;
    };

    if (_c_data.vx_max < _last_vx) {
      _c_data.vx_max = _last_vx;
    };

    if (_last_vx < (_c_data.vx_ref - 50)) {
      _state |= JS_STATE_LEFT;
    } else if (_last_vx > (_c_data.vx_ref + 50)) {
      _state |= JS_STATE_RIGHT;
    }

    _last_vy = analogRead(_vy_pin);

    if (_c_data.vy_min > _last_vy) {
      _c_data.vy_min = _last_vy;
    };

    if (_c_data.vy_max < _last_vy) {
      _c_data.vy_max = _last_vy;
    };

    if (_last_vy < (_c_data.vy_ref - 50)) {
      _state |= JS_STATE_UP;
    } else if (_last_vy > (_c_data.vy_ref + 50)) {
      _state |= JS_STATE_DOWN;
    }

    _last_sw = analogRead(_sw_pin);

    if (_c_data.sw_min > _last_sw) {
      _c_data.sw_min = _last_sw;
    };

    if (_c_data.sw_max < _last_sw) {
      _c_data.sw_max = _last_sw;
    };

    if (_last_sw < (_c_data.sw_ref - ((float)_c_data.sw_ref / (float)2))) {
      _state |= JS_STATE_PRESSED;
    }
  }

  return _state;
}

void ArduinoJoystick::setup() {
  pinMode(_vx_pin, INPUT_PULLUP);
  pinMode(_vy_pin, INPUT_PULLUP);
  pinMode(_sw_pin, INPUT_PULLUP);

  if (!isPreconfigured) {
    calibrate();
  } else {
    isReady = true;
  }
}

struct jsCalibrationInfo ArduinoJoystick::calibrate() {
  isReady = false;

  _c_data.vx_ref = analogRead(_vx_pin);
  _c_data.vy_ref = analogRead(_vy_pin);
  _c_data.sw_ref = analogRead(_sw_pin);

  isReady = true;

  return getCalibrationData();
}

struct jsCalibrationInfo ArduinoJoystick::getCalibrationData() {
  return _c_data;
}

void ArduinoJoystick::setCalibrationData(struct jsCalibrationInfo calibData) {
  isPreconfigured = true;
  _c_data = calibData;
}

void ArduinoJoystick::printState() {
  Serial.print("current state: ");
  Serial.print(_state);

  Serial.print(", vx_ref: ");
  Serial.print(_c_data.vx_ref);

  Serial.print(", vy_ref: ");
  Serial.print(_c_data.vy_ref);

  Serial.print(", sw_ref: ");
  Serial.print(_c_data.sw_ref);
  Serial.print("\n");

  Serial.print("vx [");
  Serial.print(_c_data.vx_min);
  Serial.print(" .. ");
  Serial.print(_c_data.vx_max);
  Serial.print("], vy [");
  Serial.print(_c_data.vy_min);
  Serial.print(" .. ");
  Serial.print(_c_data.vy_max);
  Serial.print("], sw [");
  Serial.print(_c_data.sw_min);
  Serial.print(" .. ");
  Serial.print(_c_data.sw_max);
  Serial.print("]\n");
}