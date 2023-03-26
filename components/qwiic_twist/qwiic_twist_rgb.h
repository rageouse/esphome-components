#pragma once

#include "esphome.h"
#include "qwiic_twist.h"

#include "esphome/core/component.h"
#include "esphome/components/light/light_output.h"


namespace esphome {
namespace qwiic_twist {

class QwiicTwist;


class QwiicTwistRGB : public light::LightOutput, public Component {

  public:
    
    QwiicTwistRGB(QwiicTwist *parent)
        : parent_{ parent }
        {}
    
    QwiicTwistRGB()
        : QwiicTwistRGB { nullptr }
        {}
    
    light::LightTraits get_traits() override {
      auto traits = light::LightTraits();
      traits.set_supported_color_modes({light::ColorMode::RGB});
      return traits;
    }

    void write_state(light::LightState *state) override {
      float red, green, blue;
      state->current_values_as_rgb(&red, &green, &blue, false);

      Wire.beginTransmission(this->i2c_address_);
      Wire.write(0x0D); // 0x0D = Red; 0x0E = Green; 0x0F = Blue; register auto-increments
      Wire.write(char(red*255));
      Wire.write(char(green*255));
      Wire.write(char(blue*255));
      Wire.endTransmission();
    }
    
    void set_parent(QwiicTwist *parent) {
      this.parent_ = parent;
    }
    
  protected:
    QwiicTwist *parent_ { nullptr };
    char i2c_address_ = 0x3F;

};


} // namespace qwiic_twist
} // namespace esphome