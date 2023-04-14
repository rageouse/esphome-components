#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"


namespace esphome {
namespace qwiic_twist {


class QwiicTwist : public i2c::I2CDevice, public Component {

  public:
    float get_setup_priority() const override;

    void setup() override;
  

  protected:
    friend class QwiicTwistRGB;
    friend class QwiicTwistEncoder;
    
    i2c::ErrorCode write8( uint8_t reg, uint8_t  value);
    i2c::ErrorCode write16(uint8_t reg, uint16_t value);
    i2c::ErrorCode write32(uint8_t reg, uint32_t value);
};


} // namespace qwiic_twist
} // namespace esphome