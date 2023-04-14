#include "qwiic_twist.h"

#include "esphome/core/log.h"

namespace esphome {
namespace qwiic_twist {

static const char *const TAG = "qwiic_twist";

float QwiicTwist::get_setup_priority() const {
  return setup_priority::IO;
}

void QwiicTwist::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Qwiic Twist...");
  uint8_t buf;
  if (this->read_register(0x00, &buf, 1) != i2c::ERROR_OK) {
    ESP_LOGCONFIG(TAG, "Failed to read unique identifier (0x00)!");
    this->mark_failed();
    return;
  }
  ESP_LOGCONFIG(TAG, "Qwiic Twist unique identifier is %u", buf);
}


} // namespace qwiic_twist
} // namespace esphome