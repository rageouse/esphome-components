#pragma once

#include "esphome/core/hal.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/i2c/i2c.h"


namespace esphome {
namespace qwiic_twist {


struct TwistBytes {
  uint8_t  id                    {0};
  uint8_t  status                {0};
  bool     buttonPressed     {false};
  bool     buttonClicked     {false};
  bool     buttonInterrupt   {false};
  bool     encoderInterrupt  {false};
  uint16_t firmware              {0};
  uint8_t  interruptEnable       {0};
  int16_t  encoderCount          {0};
  int16_t  encoderDifference     {0};
  uint16_t timeSinceLastMovement {0};
  uint16_t timeSinceLastButton   {0};
  bool     readError         {false};
  bool     writeError        {false};
}


// restore modes for the rotary encoder
enum TwistEncoderRestoreMode {
  TWIST_ENCODER_RESTORE_DEFAULT_ZERO,  // try to restore counter, otherwise set to zero
  TWIST_ENCODER_ALWAYS_ZERO,           // do not restore counter, always set to zero
};


class QwiicTwist : public i2c::I2CDevice, public PollingComponent {

  public:
    float get_setup_priority() const override;

    void setup() override;    
    void update() override;
    
    void set_encoder_restore_mode(TwistEncoderRestoreMode v) {
      this->encoder_restore_mode_ = v;
    }
    void set_publish_initial_encoder_value(bool v) {
      this->publish_initial_encoder_value = v;
    }
    
    void add_on_clockwise_callback(std::function<void()> callback) {
      this->on_clockwise_callback_.add(std::move(callback));
    }
    void add_on_anticlockwise_callback(std::function<void()> callback) {
      this->on_anticlockwise_callback_.add(std::move(callback));
    }
    
    void set_encoder_sensor(sensor::Sensor s) {
      this->encoder_sensor_ = s;
    }
    void set_button_sensor(binary_sensor::BinarySensor s) {
      this->button_sensor_ = s;
    }

  protected:
    uint8_t buf[13];
    
    TwistBytes read_twist_bytes(bool and_reset = true);
    
    sensor::Sensor *encoder_sensor_             {nullptr};
    binary_sensor::BinarySensor *button_sensor_ {nullptr};

    int16_t last_encoder_count_  {0};
    bool    last_button_pressed_ {false};

    TwistEncoderRestoreMode encoder_restore_mode_{TWIST_ENCODER_RESTORE_DEFAULT_ZERO};    
    bool publish_initial_encoder_value_ {false};

    bool write_encoder_count(int16_t c);
    
    CallbackManager<void()> on_clockwise_callback_;
    CallbackManager<void()> on_anticlockwise_callback_;
    
    bool put_status_byte(uint8_t status_byte);

    i2c::ErrorCode write8( uint8_t reg, uint8_t  value);
    i2c::ErrorCode write16(uint8_t reg, uint16_t value);
    i2c::ErrorCode write32(uint8_t reg, uint32_t value);
};



template<typename... Ts> class TwistEncoderSetValueAction : public Action<Ts...> {
 public:
  TwistEncoderSetValueAction(QwiicTwist *encoder) : encoder_(encoder) {}
  TEMPLATABLE_VALUE(int, value)

  void play(Ts... x) override { this->encoder_->set_value(this->value_.value(x...)); }

 protected:
  QwiicTwist *encoder_;
};

class TwistEncoderClockwiseTrigger : public Trigger<> {
 public:
  explicit TwistEncoderClockwiseTrigger(QwiicTwist *parent) {
    parent->add_on_clockwise_callback([this]() { this->trigger(); });
  }
};

class TwistEncoderAnticlockwiseTrigger : public Trigger<> {
 public:
  explicit TwistEncoderAnticlockwiseTrigger(QwiicTwist *parent) {
    parent->add_on_anticlockwise_callback([this]() { this->trigger(); });
  }
};


} // namespace qwiic_twist
} // namespace esphome