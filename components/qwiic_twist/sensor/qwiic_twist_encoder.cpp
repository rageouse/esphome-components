#include "qwiic_twist_encoder.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace qwiic_twist {

static const char *const TAG = "qwiic_twist.encoder";

void QwiicTwistEncoder::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Qwiic Twist Encoder '%s'...", this->name_.c_str());

  uint8_t buff[2];
  if( this->parent_->readbuf(0x07, buff, 2) != i2c::ERROR_OK )
    ESP_LOGCONFIG(TAG, "Error reading initial value for '%s'...", this->name_.c_str());

  int16_t initial_value = (buff[0] | buff[1] << 8);
  
  
  
  this->store_.counter = initial_value;
  this->store_.last_read = initial_value;
}

void QwiicTwistEncoder::set_value(int16_t value) {
  this->store_.last_read = this->store_.counter;
  this->store_.counter = value;
  
  if( this->parent_->write16(0x07, value) != i2c::ERROR_OK )
    ESP_LOGCONFIG(TAG, "Error writing encoder value for '%s'...", this->name_.c_str());;
}

void QwiicTwistEncoder::update() {
  this->store_.last_read = this->store_.counter;

  uint8_t buff[2];
  if( parent_->readbuf(0x07, buff, 2) != i2c::ERROR_OK )
    ESP_LOGCONFIG(TAG, "Error reading encoder value for '%s'...", this->name_.c_str());

  int16_t value = (buff[0] | buff[1] << 8);

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

