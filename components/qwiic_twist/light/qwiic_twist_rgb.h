#pragma once

#include "esphome.h"
#include "../qwiic_twist.h"

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
      this->parent_ = parent;
    }
    
    void set_red_min  ( float v ) { this->red_min_   = v; }
    void set_red_max  ( float v ) { this->red_max_   = v; }
    void set_green_min( float v ) { this->green_min_ = v; }
    void set_green_max( float v ) { this->green_max_ = v; }
    void set_blue_min ( float v ) { this->blue_min_  = v; }
    void set_blue_max ( float v ) { this->blue_max_  = v; }

  protected:
    QwiicTwist *parent_ { nullptr };
    
    float red_min_   { 0.14 };
    float red_max_   { 1.0  };
    float green_min_ { 0.14 };
    float green_max_ { 1.0  };
    float blue_min_  { 0.14 };
    float blue_max_  { 1.0  };
    
    uint8_t red_float_to_uint8( float v );
    uint8_t green_float_to_uint8( float v );
    uint8_t blue_float_to_uint8( float v );


};


} // namespace qwiic_twist
} // namespace esphome