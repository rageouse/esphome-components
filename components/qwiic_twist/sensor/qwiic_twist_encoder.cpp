#include "qwiic_twist_encoder.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace qwiic_twist {

static const char *const TAG = "qwiic_twist.encoder";

void QwiicTwistEncoder::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Qwiic Twist Encoder '%s'...", this->name_.c_str());

  int16_t initial_value = 0;
  switch (this->restore_mode_) {
    case TWIST_ENCODER_RESTORE_DEFAULT_ZERO:
      this->rtc_ = global_preferences->make_preference<int32_t>(this->get_object_id_hash());
      if (!this->rtc_.load(&initial_value)) {
        uint8_t buff[2];
        if( parent_->readbuf(0x07, buff, 2) != i2c::ERROR_OK )
          ESP_LOGCONFIG(TAG, "Error reading encoder initial value for '%s'...", this->name_.c_str());
        initial_value = (buff[0] | buff[1] << 8);
      }
      break;
    case TWIST_ENCODER_ALWAYS_ZERO:
      initial_value = 0;
      this->set_value( initial_value );
      break;
  }
  
  this->store_.counter = initial_value;
  this->store_.last_read = initial_value;
  
  if( this->publish_initial_value_ )
    this->publish_state(initial_value);
}

void QwiicTwistEncoder::set_value(int16_t value, bool and_update /* = false */) {
  if( this->parent_->write16(0x07, value) != i2c::ERROR_OK )
    ESP_LOGCONFIG(TAG, "Error writing encoder value for '%s'...", this->name_.c_str());

  if( and_update ) {
    this->update();
  }
  else {
    this->store_.last_read = this->store_.counter;
    this->store_.counter = value;
  }
}

void QwiicTwistEncoder::update() {
  this->store_.last_read = this->store_.counter;

  uint8_t buff[2];
  if( parent_->readbuf(0x07, buff, 2) != i2c::ERROR_OK )
    ESP_LOGCONFIG(TAG, "Error reading encoder value for '%s'...", this->name_.c_str());

  int16_t value = (buff[0] | buff[1] << 8);
  
  if( value == this->store_.counter ) return;
  
  this->store_.counter = value;
  
  for( ; value > store_.last_read; value-- )
    this->on_clockwise_callback_.call();
  for( ; value < store_.last_read; value++ )
    this->on_anticlockwise_callback_.call();

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

