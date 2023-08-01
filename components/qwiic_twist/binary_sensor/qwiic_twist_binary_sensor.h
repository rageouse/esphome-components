#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../qwiic_twist.h"

namespace esphome {
namespace qwiic_twist {

class QwiicTwist;


const uint8_t twistStatusButtonClickedBit = 2;
const uint8_t twistStatusButtonPressedBit = 1;
const uint8_t twistStatusEncoderMovedBit  = 0;


class QwiicTwistBinarySensor : public binary_sensor::BinarySensor, public PollingComponent {
 public:
  void setup() override;
  void update() override;

  void set_parent(QwiicTwist *parent) { parent_ = parent; }

 protected:
  QwiicTwist *parent_;
  
  bool last_pressed_ {false};
  
  uint8_t get_status_byte();
  bool put_status_byte(uint8_t status_byte);
};

}  // namespace qwiic_twist
}  // namespace esphome