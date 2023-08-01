#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace qwiic_joystick {

class QwiicJoystick : public PollingComponent, public i2c::I2CDevice {
 public:
 
  void set_button_sensor     (binary_sensor::BinarySensor *s) { button_sensor_  = s; }

  void set_x_axis_sensor          (sensor::Sensor *s) { x_axis_sensor_          = s; }
  void set_x_axis_centered_sensor (sensor::Sensor *s) { x_axis_centered_sensor_ = s; }

  void set_y_axis_sensor          (sensor::Sensor *s) { y_axis_sensor_          = s; }
  void set_y_axis_centered_sensor (sensor::Sensor *s) { y_axis_centered_sensor_ = s; }

  void set_radius_squared_sensor  (sensor::Sensor *s) { radius_squared_sensor_  = s; }
  void set_theta_sensor           (sensor::Sensor *s) { theta_sensor_           = s; }


  void setup() override;
  float get_setup_priority() const override;
  void update() override;

 protected:

  binary_sensor::BinarySensor *button_sensor_ {nullptr};
  
  sensor::Sensor *x_axis_sensor_              {nullptr};
  sensor::Sensor *x_axis_centered_sensor_     {nullptr};

  sensor::Sensor *y_axis_sensor_              {nullptr};
  sensor::Sensor *y_axis_centered_sensor_     {nullptr};

  sensor::Sensor *radius_squared_sensor_      {nullptr};

  sensor::Sensor *theta_sensor_               {nullptr};

  uint16_t center_x_ {512};
  uint16_t center_y_ {512};
  
  uint16_t old_x_ {512};
  uint16_t old_y_ {512};
  uint8_t  old_button_pressed_ {0}; 

 private:

};

}  // namespace qwiic_joystick
}  // namespace esphome

