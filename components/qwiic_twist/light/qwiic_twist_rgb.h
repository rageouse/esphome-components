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
    
    light::LightTraits get_traits() override;

    void write_state(light::LightState *state) override;
    
    void set_parent(QwiicTwist *parent) {
      this.parent_ = parent;
    }

  protected:
    QwiicTwist *parent_ { nullptr };

};


} // namespace qwiic_twist
} // namespace esphome