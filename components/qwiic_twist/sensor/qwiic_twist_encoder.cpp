#include "qwiic_twist_encoder.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace qwiic_twist {

static const char *const TAG = "qwiic_twist.encoder";

int16_t QwiicTwistEncoder::read_encoder_value() {
  uint8_t buf[] = {0, 0};
  
  if( not parent_->read_bytes(0x07, buf, 2) )
    return 0;
  
  return ( buf[0] | buf[1]<<8 );
}

void QwiicTwistEncoder::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Qwiic Twist Encoder '%s'...", this->name_.c_str());

  int16_t initial_value = 0;
  switch (this->restore_mode_) {
    case TWIST_ENCODER_RESTORE_DEFAULT_ZERO:
      this->rtc_ = global_preferences->make_preference<int32_t>(this->get_object_id_hash());
      if (!this->rtc_.load(&initial_value)) {
        initial_value = this->read_encoder_value();
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
  if( not this->parent_->write_byte_16(0x07, reinterpret_cast<uint16_t>(value)) )
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

  int16_t value = this->read_encoder_value();

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

