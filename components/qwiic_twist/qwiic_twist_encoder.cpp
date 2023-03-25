#pragma once

#include "esphome.h"
#include "qwiic_twist.h"

#include "qwiic_twist_encoder.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace qwiic_twist {



void QwiicTwistEncoder::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Qwiic Twist Encoder '%s'...", this->name_.c_str());

  Wire.beginTransmission(this->i2c_address_);
  Wire.write(0x07);
  Wire.endTransmission();
  Wire.requestFrom(this->i2c_address_, 2);
  byte buff[2];
  Wire.readBytes(buff, 2);
  int16_t initial_value = buff[0] + buff[1] << 8;

  this->store_.counter = initial_value;
  this->store_.last_read = initial_value;
}

void QwiicTwistEncoder::set_value(int16_t value) {
  this->store_.counter = value;
  
  char msb = (char)((value >> 8) & 0xFF);
  char lsb = (char)( value       & 0xFF);
  
  Wire.beginTransmission(this->i2c_address_);
  Wire.write(0x07);
  Wire.write(lsb);
  Wire.write(msb);
  Wire.endTransmission();
}

void QwiicTwistEncoder::update() {
  this->store_.last_read = this->store_.counter;
  
  Wire.beginTransmission(this->i2c_address_);
  Wire.write(0x07);
  Wire.endTransmission();
  Wire.requestFrom(this->i2c_address_, 2);
  byte buff[2];
  Wire.readBytes(buff, 2);
  int16_t value = buff[0] + buff[1] << 8;

  this->store_.counter = value;
  this->publish_state(value);
}

void QwiicTwistEncoder::set_restore_mode(TwistEncoderRestoreMode restore_mode) {
  this->restore_mode_ = restore_mode;
}

void QwiicTwistEncoder::set_publish_initial_value(bool publish_initial_value) {
  this->publish_initial_value_ = publish_initial_value;
}


} // namespace qwiic-twist
} // namespace esphome

