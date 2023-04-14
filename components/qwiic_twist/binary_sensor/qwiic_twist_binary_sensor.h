#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../qwiic_twist.h"

namespace esphome {
namespace qwiic_twist {

class QwiicTwist;


const byte twistStatusButtonClickedBit = 2;
const byte twistStatusButtonPressedBit = 1;
const byte twistStatusEncoderMovedBit = 0;


class QwiicTwistBinarySensor : public binary_sensor::BinarySensor, public PollingComponent {
 public:
  void setup() override;
  void update() override;

  void set_parent(QwiicTwist *parent) { parent_ = parent; }

 protected:
  QwiicTwist *parent_;
  
  uint8_t get_status_byte();
};

}  // namespace qwiic_twist
}  // namespace esphome