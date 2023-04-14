#include "qwiic_twist_rgb.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/log.h"
#include "esphome/components/light/light_output.h"


namespace esphome {
namespace qwiic_twist {

static const char *const TAG = "qwiic_twist.rgb";


light::LightTraits QwiicTwistRGB::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supported_color_modes({light::ColorMode::RGB});
  return traits;
}

uint8_t QwiicTwistRGB::red_float_to_uint8( float v ) {
    if( v < 0.01 ) return 0;
    v = v * (this->red_max_ - this->red_min_) + this->red_min_;
    return static_cast<uint8_t>(v * 255);
}
uint8_t QwiicTwistRGB::green_float_to_uint8( float v ) {
    if( v < 0.01 ) return 0;
    v = v * (this->green_max_ - this->green_min_) + this->green_min_;
    return static_cast<uint8_t>(v * 255);
}
uint8_t QwiicTwistRGB::blue_float_to_uint8( float v ) {
    if( v < 0.01 ) return 0;
    v = v * (this->blue_max_ - this->blue_min_) + this->blue_min_;
    return static_cast<uint8_t>(v * 255);
}

void QwiicTwistRGB::write_state(light::LightState *state) {
  float red, green, blue;
  state->current_values_as_rgb(&red, &green, &blue, false);
  
  uint8_t color_register = 0x0D; // 0x0D = Red; 0x0E = Green; 0x0F = Blue; register auto-increments
  std::array<uint8_t, 3> color_buf = { this->red_float_to_uint8(red),
                                       this->green_float_to_uint8(green),
                                       this->blue_float_to_uint8(blue)
                                     };

  if( not this->parent_->write_register(color_register, color_buf) )
    ESP_LOGCONFIG(TAG, "Error writing Qwiic Twist rgb value...");

}
    


} // namespace qwiic_twist
} // namespace esphome