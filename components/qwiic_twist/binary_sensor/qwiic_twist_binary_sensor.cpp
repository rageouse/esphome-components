#include "esphome/core/hal.h"
#include "qwiic_twist_binary_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace qwiic_twist {

static const char *const TAG = "qwiic_twist.binary_sensor";

uint8_t QwiicTwistBinarySensor::get_status_byte() {
  uint8_t status_byte = 0;
  
  if( this->parent_->read_byte(0x01, &status_byte) )
    return status_byte;
  
  ESP_LOGCONFIG(TAG, "Error reading qwiic twist status value for '%s'...", this->name_.c_str());
  return 0;
}

bool QwiicTwistBinarySensor::put_status_byte(uint8_t status_byte) {
  return this->parent_->write_byte(0x01, status_byte);
}


void QwiicTwistBinarySensor::setup() {
  uint8_t status_byte = this->get_status_byte();

  bool pressed = status_byte & (1 << twistStatusButtonPressedBit);
  
  this->publish_initial_state( pressed );
  
  this->put_status_byte( status_byte & ~(1 << twistStatusButtonPressedBit) );
}


void QwiicTwistBinarySensor::update() {
  uint8_t status_byte = this->get_status_byte();

  bool pressed = status_byte & (1 << twistStatusButtonPressedBit);
  bool clicked = status_byte & (1 << twistStatusButtonClickedBit);
    
  if(!pressed && !this->last_pressed_ && clicked) {
    this->publish_state( true );
    this->publish_state( false);
  }
  else {
    this->publish_state( pressed );
  }

  this->put_status_byte( status_byte & ~(1 << twistStatusButtonPressedBit)
                                     & ~(1 << twistStatusButtonClickedBit) );
  this->last_pressed_ = pressed;

}

}  // namespace qwiic_twist
}  // namespace esphome