#include "qwiic_twist.h"

namespace esphome {
namespace qwiic_twist {

static const char *const TAG = "qwiic_twist";

float QwiicTwist::get_setup_priority() const {
  return setup_priority::IO;
}

void QwiicTwist::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Qwiic Twist...");
  if (this->write8(SEESAW_STATUS, SEESAW_STATUS_SWRST, 0xff) != i2c::ERROR_OK) {
    this->mark_failed();
    return;
  }
  
  uint8_t c = 0;
  this->readbuf(SEESAW_STATUS, SEESAW_STATUS_HW_ID, &c, 1);
  std::string cpu;
  if (c == SEESAW_HW_ID_SAMD09)
    cpu = "SAMD09";
  else if (c == SEESAW_HW_ID_TINY8X7)
    cpu = "TINY8X7";
  else
    cpu = "unknown";
  ESP_LOGCONFIG(TAG, "Hardware type is %s", cpu.c_str());
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
  i2c::ErrorCode err = this->write(reg, 1);
  if (err != i2c::ERROR_OK)
    return err;
  return this->read(buf, len);
}


} // namespace qwiic_twist
} // namespace esphome