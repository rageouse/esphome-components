#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/automation.h"
#include "esphome/components/sensor/sensor.h"

#include "qwiic_twist_rgb.h"
#include "qwiic_twist_encoder.h"


namespace esphome {
namespace qwiic_twist {

static const char *const TAG = "qwiic_twist";


class QwiicTwist : public PollingComponent {

  public:
    QwiicTwist(): PollingComponent(50) {}
    
    void setup() override;
    void update() override;

    float get_setup_priority() const override;

    // RGB LIGHT

    // ROTARY ENCODER
    void set_value(int16_t value) {
      this->twist_encoder_->set_value(value);
    }
    void set_restore_mode(RotaryEncoderRestoreMode restore_mode) {
      this->twist_encoder_->set_restore_mode(restore_mode);
    }
    void set_publish_initial_value(bool publish_initial_value) {
      this->twist_encoder_->set_publish_initial_value(publish_initial_value);
    }

    void add_on_clockwise_callback(std::function<void()> callback) {
      this->twist_encoder_->add_on_clockwise_callback.add(callback);
    }
    void add_on_anticlockwise_callback(std::function<void()> callback) {
      this->twist_encoder_->add_on_anticlockwise_callback.add(callback);
    }

  
  protected:
    char i2c_address_ = 0x3F;

    QwiicTwistRGB     *twist_rgb_     = new QwiicTwistRGB(parent=this, name=this->name+" Light");
    QwiicTwistEncoder *twist_encoder_ = new QwiicTwistEncoder(parent=this, name=this->name+" Encoder");

};


} // namespace qwiic_twist
} // namespace esphome