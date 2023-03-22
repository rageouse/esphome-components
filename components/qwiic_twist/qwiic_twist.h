#pragma once

#include "esphome.h"

namespace esphome {
namespace qwiic_twist {

class QwiicTwist : public rgb::RGBLightOutput {

  public:
    void set_red(output::FloatOutput *red)     override { ; }
    void set_green(output::FloatOutput *green) override { ; }
    void set_blue(output::FloatOutput *blue)   override { ; }
    
    void write_state(light::LightState *state) override {
      float red, green, blue;
      state->current_values_as_rgb(&red, &green, &blue, false);

      Wire.beginTransmission(i2c_address_);
      Wire.write(0x0D); // Red
      Wire.write(char(red*255));
      Wire.endTransmission();
      
      Wire.beginTransmission(i2c_address_);
      Wire.write(0x0E); // Green
      Wire.write(char(green*255));
      Wire.endTransmission();
      
      Wire.beginTransmission(i2c_address_);
      Wire.write(0x0F); // Blue
      Wire.write(char(blue*255));
      Wire.endTransmission();
    }
  
  protected:
    char i2c_address_ = 0x3F;

};

} // namespace qwiic_twist
} // namespace esphome