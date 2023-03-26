#include "qwiic_twist.h"

namespace esphome {
namespace qwiic_twist {

void QwiicTwist::setup() {
  this->twist_rgb_->setup();
  this->twist_encoder_->call_setup();
}


float QwiicTwist::get_setup_priority() const {
  return setup_priority::DATA;
}


} // namespace qwiic_twist
} // namespace esphome