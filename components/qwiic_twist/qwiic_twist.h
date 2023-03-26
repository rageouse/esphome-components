#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"

#include "esphome/core/hal.h"
#include "esphome/core/automation.h"
#include "esphome/components/sensor/sensor.h"

#include "qwiic_twist_rgb.h"
#include "qwiic_twist_encoder.h"


namespace esphome {
namespace qwiic_twist {

static const char *const TAG = "qwiic_twist";


class QwiicTwist : public Component {

  public:
    QwiicTwist() {}
    
    void setup() override;

    float get_setup_priority() const override;
    
    void set_twist_rgb(QwiicTwistRGB *twist_rgb) {
      this.twist_rgb_ = twist_rgb;
    }
    
    void set_twist_encoder(QwiicTwistEncoder *twist_encoder) {
      this.twist_encoder_ = twist_encoder;
    }

    // RGB LIGHT

    // ROTARY ENCODER
    void set_value(int16_t value) {
      this->twist_encoder_->set_value(value);
    }
    void set_restore_mode(TwistEncoderRestoreMode restore_mode) {
      this->twist_encoder_->set_restore_mode(restore_mode);
    }
    void set_publish_initial_value(bool publish_initial_value) {
      this->twist_encoder_->set_publish_initial_value(publish_initial_value);
    }


  
  protected:
    char i2c_address_ = 0x3F;

    QwiicTwistRGB     *twist_rgb_     { nullptr };
    QwiicTwistEncoder *twist_encoder_ { nullptr };

};


} // namespace qwiic_twist
} // namespace esphome