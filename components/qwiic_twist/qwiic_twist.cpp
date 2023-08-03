#include "qwiic_twist.h"

#include "esphome/core/log.h"

namespace esphome {
namespace qwiic_twist {

static const char *const TAG = "qwiic_twist";

const uint8_t zeros[] = {0, 0, 0, 0, 0, 0};

float QwiicTwist::get_setup_priority() const {
  return setup_priority::IO;
}

void QwiicTwist::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Qwiic Twist at %#2x...", this->address_);
  uint8_t buf;
  
  TwistBytes bytes = this->read_twist_bytes();
  
  if( bytes.readError || bytes.writeError ) {
    ESP_LOGCONFIG(TAG, "- Failed to read or write to Qwiic Twist! Giving up.");
    this->mark_failed();
    return;
  }
  ESP_LOGCONFIG(TAG, "- Qwiic Twist unique identifier is %u", bytes.id);
  
  if( this->encoder_sensor_ ) {
      ESP_LOGCONFIG(TAG, "- Setting up Qwiic Twist Encoder...");

      int16_t initial_value = 0;
      switch (this->encoder_restore_mode_) {
        case TWIST_ENCODER_RESTORE_DEFAULT_ZERO:
          initial_value = bytes.encoderCount;
          break;
        case TWIST_ENCODER_ALWAYS_ZERO:
          initial_value = 0;
          this->write_encoder_count( initial_value );
          break;
      }
  
      this->last_encoder_count_   = initial_value;
  
      if( this->publish_initial_encoder_value_ )
        this->encoder_sensor_->publish_state(initial_value);
  }
  else {
      ESP_LOGCONFIG(TAG, "- Skipping Qwiic Twist Encoder; no sensor defined...");
  }

  if( this->button_sensor_ ) {
      ESP_LOGCONFIG(TAG, "- Setting up Qwiic Twist Button...");

      this->button_sensor_->publish_initial_state( bytes.buttonPressed );
  }
  else {
      ESP_LOGCONFIG(TAG, "- Skipping Qwiic Twist Button; no sensor defined...");
  }
  
}


void QwiicTwist::update() {
  TwistBytes bytes = this->read_twist_bytes();
  
  if( this->encoder_sensor_ && bytes.encoderCount != this->last_encoder_count_ ) {
    this->last_encoder_count_ = bytes.encoderCount;

    for( ; bytes.encoderCount > this->last_encoder_count_; bytes.encoderCount-- )
      this->on_clockwise_callback_.call();
    for( ; bytes.encoderCount < this->last_encoder_count_; bytes.encoderCount++ )
      this->on_anticlockwise_callback_.call();

    this->encoder_sensor_->publish_state(this->last_encoder_count_);
  }

  if( this->button_sensor_ ) {
    if( !bytes.buttonPressed && bytes.buttonClicked && !this->last_button_pressed_ ) {
      this->button_sensor_->publish_state( true );
      this->button_sensor_->publish_state( false);
    }
    else {
      this->button_sensor_->publish_state( bytes.buttonPressed );
    }
    
    this->last_button_pressed_ = bytes.buttonPressed;
  }  
}


TwistBytes QwiicTwist::read_twist_bytes(bool and_reset /* = true */) {
  TwistBytes bytes;
  
  if( !this->read_bytes(0x00, this->buf, 13) ) {
    ESP_LOGE(TAG, "Unable to read Qwiic Twist bytes for twist at %#2x...", this->address_);
    bytes.readError = true;
    return bytes;
  }
  
  bytes.id                    = this->buf[ 0];
  bytes.status                = this->buf[ 1];
  bytes.buttonPressed         = this->buf[ 1] & 1<<3;
  bytes.buttonClicked         = this->buf[ 1] & 1<<2;
  bytes.buttonInterrupt       = this->buf[ 1] & 1<<1;
  bytes.encoderInterrupt      = this->buf[ 1] & 1<<0;
  bytes.firmware              = this->buf[ 2] << 8 | this->buf[ 3];
  bytes.interruptEnable       = this->buf[ 4];
  bytes.encoderCount          = this->buf[ 6] << 8 | this->buf[ 5];
  bytes.encoderDifference     = this->buf[ 8] << 8 | this->buf[ 7];
  bytes.timeSinceLastMovement = this->buf[10] << 8 | this->buf[ 9];
  bytes.timeSinceLastButton   = this->buf[12] << 8 | this->buf[11];
  
  if(and_reset) {
    if(! this->write_bytes(0x01, zeros, 1) ) {  // reset status byte
        ESP_LOGE(TAG, "Unable to reset status byte for Qwiic Twist at %#2x...", this->address_);
        bytes.writeError = true;
    }
    if(! this->write_bytes(0x07, zeros, 6) ) {  // reset encoderDifference, timeSinceLastMovement, timeSinceLastButton
        ESP_LOGE(TAG, "Unable to reset bytes for Qwiic Twist at %#2x...", this->address_);
        bytes.writeError = true;
    }
  }
  
  return bytes;
}

bool QwiicTwist::write_encoder_count(int16_t c) {
  if(! this->write_byte_16(0x05, c) ) }
    ESP_LOGE(TAG, "Unable to write encoder bytes for Qwiic Twist at %#2x...", this->address_);
    return false;
  }
  return true;
}



} // namespace qwiic_twist
} // namespace esphome