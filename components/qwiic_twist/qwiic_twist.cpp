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
  if (this->readbuf(0x00, &buf, 1) != i2c::ERROR_OK) {
    ESP_LOGCONFIG(TAG, "Failed to read unique identifier (0x00)!");
    this->mark_failed();
    return;
  }
  ESP_LOGCONFIG(TAG, "Qwiic Twist unique identifier is %u", buf);
}


i2c::ErrorCode QwiicTwist::write8(uint8_t reg, uint8_t value) {
  uint8_t buf[2] = {reg, value};
  return this->write(buf, 2);
}

i2c::ErrorCode QwiicTwist::write16(uint8_t reg, uint16_t value) {
  uint8_t buf[3] = {reg, (uint8_t)(value >> 8), (uint8_t)value};
  return this->write(buf, 3);
}

i2c::ErrorCode QwiicTwist::write32(uint8_t reg, uint32_t value) {
  uint8_t buf[5] = {reg, (uint8_t)(value >> 24), (uint8_t)(value >> 16),
                    (uint8_t)(value >> 8), (uint8_t)value};
  return this->write(buf, 5);
}

i2c::ErrorCode QwiicTwist::readbuf(uint8_t reg, uint8_t *buf, uint8_t len) {
  i2c::ErrorCode err = this->write(&reg, 1);
  if (err != i2c::ERROR_OK)
    return err;
  return this->read(buf, len);
}


} // namespace qwiic_twist
} // namespace esphome