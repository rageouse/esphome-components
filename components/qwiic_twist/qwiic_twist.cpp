#include "qwiic_twist.h"

namespace esphome {
namespace qwiic_twist {

void QwiicTwist::setup() override {
  this->twist_encoder_->setup();
}

void QwiicTwist::update() override {
  this->twist_encoder_->update();
}


float QwiicTwist::get_setup_priority() const {
  return setup_priority::DATA;
}


} // namespace qwiic_twist
} // namespace esphome