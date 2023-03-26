#pragma once

#include <array>

#include "esphome.h"
#include "../qwiic_twist.h"

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/automation.h"
#include "esphome/components/sensor/sensor.h"


namespace esphome {
namespace qwiic_twist {

/// All possible restore modes for the rotary encoder
enum TwistEncoderRestoreMode {
  TWIST_ENCODER_RESTORE_DEFAULT_ZERO,  /// try to restore counter, otherwise set to zero
  TWIST_ENCODER_ALWAYS_ZERO,           /// do not restore counter, always set to zero
};

struct TwistEncoderSensorStore {

  volatile int16_t counter{0};
  int16_t last_read{0};
  
  bool first_read{true};

  std::array<int8_t, 8> rotation_events{};
  bool rotation_events_overflow{false};
};

class QwiicTwist;


class QwiicTwistEncoder : public sensor::Sensor, public PollingComponent {

  public:
    void set_value(int16_t value);
    void set_restore_mode(TwistEncoderRestoreMode restore_mode);
    void set_publish_initial_value(bool publish_initial_value);

    void setup() override;
    void update() override;

    void add_on_clockwise_callback(std::function<void()> callback) {
      this->on_clockwise_callback_.add(std::move(callback));
    }
    void add_on_anticlockwise_callback(std::function<void()> callback) {
      this->on_anticlockwise_callback_.add(std::move(callback));
    }

    void set_parent(QwiicTwist *parent) {
      this->parent_ = parent;
    }

  protected:
    QwiicTwist *parent_ { nullptr };

    TwistEncoderSensorStore store_{};
    TwistEncoderRestoreMode restore_mode_{TWIST_ENCODER_RESTORE_DEFAULT_ZERO};
    bool publish_initial_value_{false};
    
    CallbackManager<void()> on_clockwise_callback_;
    CallbackManager<void()> on_anticlockwise_callback_;
};


template<typename... Ts> class TwistEncoderSetValueAction : public Action<Ts...> {
 public:
  TwistEncoderSetValueAction(QwiicTwistEncoder *encoder) : encoder_(encoder) {}
  TEMPLATABLE_VALUE(int, value)

  void play(Ts... x) override { this->encoder_->set_value(this->value_.value(x...)); }

 protected:
  QwiicTwistEncoder *encoder_;
};

class TwistEncoderClockwiseTrigger : public Trigger<> {
 public:
  explicit TwistEncoderClockwiseTrigger(QwiicTwistEncoder *parent) {
    parent->add_on_clockwise_callback([this]() { this->trigger(); });
  }
};

class TwistEncoderAnticlockwiseTrigger : public Trigger<> {
 public:
  explicit TwistEncoderAnticlockwiseTrigger(QwiicTwistEncoder *parent) {
    parent->add_on_anticlockwise_callback([this]() { this->trigger(); });
  }
};


} // namespace qwiic_twist
} // namespace esphome