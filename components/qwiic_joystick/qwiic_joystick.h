#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace qwiic_joystick {

class QwiicJoystick : public PollingComponent, public i2c::I2CDevice {
 public:
 
  void set_button_sensor     (binary_sensor::BinarySensor *s) { button_sensor_                  = s; }

  void set_x_axis_raw_sensor              (sensor::Sensor *s) { x_axis_raw_sensor_              = s; }
  void set_x_axis_raw_centered_sensor     (sensor::Sensor *s) { x_axis_raw_centered_sensor_     = s; }
  void set_x_axis_8_bit_sensor            (sensor::Sensor *s) { x_axis_8_bit_sensor_            = s; }
  void set_x_axis_8_bit_centered_sensor   (sensor::Sensor *s) { x_axis_8_bit_centered_sensor_   = s; }
  void set_x_axis_percent_sensor          (sensor::Sensor *s) { x_axis_percent_sensor_          = s; }
  void set_x_axis_percent_centered_sensor (sensor::Sensor *s) { x_axis_percent_centered_sensor_ = s; }

  void set_y_axis_raw_sensor              (sensor::Sensor *s) { y_axis_raw_sensor_              = s; }
  void set_y_axis_raw_centered_sensor     (sensor::Sensor *s) { y_axis_raw_centered_sensor_     = s; }
  void set_y_axis_8_bit_sensor            (sensor::Sensor *s) { y_axis_8_bit_sensor_            = s; }
  void set_y_axis_8_bit_centered_sensor   (sensor::Sensor *s) { y_axis_8_bit_centered_sensor_   = s; }
  void set_y_axis_percent_sensor          (sensor::Sensor *s) { y_axis_percent_sensor_          = s; }
  void set_y_axis_percent_centered_sensor (sensor::Sensor *s) { y_axis_percent_centered_sensor_ = s; }

  void set_radius_squared_raw_sensor      (sensor::Sensor *s) { radius_squared_raw_sensor_      = s; }
  void set_radius_raw_sensor              (sensor::Sensor *s) { radius_raw_sensor_              = s; }
  void set_radius_squared_percent_sensor  (sensor::Sensor *s) { radius_squared_percent_sensor_  = s; }
  void set_radius_percent_sensor          (sensor::Sensor *s) { radius_percent_sensor_          = s; }

  void set_theta_degrees_sensor           (sensor::Sensor *s) { theta_degrees_sensor_           = s; }
  void set_theta_radians_sensor           (sensor::Sensor *s) { theta_radians_sensor_           = s; }


  void setup() override;
  float get_setup_priority() const override;
  void update() override;

 protected:

  binary_sensor::BinarySensor *button_sensor_     {nullptr};
  
  sensor::Sensor *x_axis_raw_sensor_              {nullptr};
  sensor::Sensor *x_axis_raw_centered_sensor_     {nullptr};
  sensor::Sensor *x_axis_8_bit_sensor_            {nullptr};
  sensor::Sensor *x_axis_8_bit_centered_sensor_   {nullptr};
  sensor::Sensor *x_axis_percent_sensor_          {nullptr};
  sensor::Sensor *x_axis_percent_centered_sensor_ {nullptr};

  sensor::Sensor *y_axis_raw_sensor_              {nullptr};
  sensor::Sensor *y_axis_raw_centered_sensor_     {nullptr};
  sensor::Sensor *y_axis_8_bit_sensor_            {nullptr};
  sensor::Sensor *y_axis_8_bit_centered_sensor_   {nullptr};
  sensor::Sensor *y_axis_percent_sensor_          {nullptr};
  sensor::Sensor *y_axis_percent_centered_sensor_ {nullptr};

  sensor::Sensor *radius_squared_raw_sensor_      {nullptr};
  sensor::Sensor *radius_raw_sensor_              {nullptr};
  sensor::Sensor *radius_squared_percent_sensor_  {nullptr};
  sensor::Sensor *radius_percent_sensor_          {nullptr};

  sensor::Sensor *theta_degrees_sensor_           {nullptr};
  sensor::Sensor *theta_radians_sensor_           {nullptr};

 private:

};

}  // namespace qwiic_joystick
}  // namespace esphome

