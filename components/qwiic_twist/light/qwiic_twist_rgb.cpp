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

void QwiicTwistRGB::write_state(light::LightState *state) {
  float red, green, blue;
  state->current_values_as_rgb(&red, &green, &blue, false);
  
  uint8_t buf[] = { 0x0D, // 0x0D = Red; 0x0E = Green; 0x0F = Blue; register auto-increments
                    red * 255,
                    green * 255,
                    blue * 255
                  };

  if( this->parent_->write(buf, 4) != i2c::ERROR_OK )
    ESP_LOGCONFIG(TAG, "Error writing Qwiic Twist rgb value...");

}
    


} // namespace qwiic_twist
} // namespace esphome