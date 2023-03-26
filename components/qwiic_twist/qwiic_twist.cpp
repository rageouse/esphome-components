#include "qwiic_twist.h"

namespace esphome {
namespace qwiic_twist {

void QwiicTwist::setup() {
  this->twist_rgb_->call_setup();
  this->twist_encoder_->call_setup();
}

void QwiicTwist::update() {
  this->twist_encoder_->update();
}


float QwiicTwist::get_setup_priority() const {
  return setup_priority::DATA;
}


} // namespace qwiic_twist
} // namespace esphome