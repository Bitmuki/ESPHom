#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/sensor/sensor.h"

#include <cinttypes>

namespace esphome {
namespace hempy {

enum HempyGain {
  Hempy_GAIN_128 = 1,
  Hempy_GAIN_32 = 2,
  Hempy_GAIN_64 = 3,
};

class HempySensor : public sensor::Sensor, public PollingComponent {
 public:
  void set_dout_pin(GPIOPin *dout_pin) { dout_pin_ = dout_pin; }
  void set_sck_pin(GPIOPin *sck_pin) { sck_pin_ = sck_pin; }
  void set_gain(HempyGain gain) { gain_ = gain; }

  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void update() override;

 protected:
  bool read_sensor_(uint32_t *result);

  GPIOPin *dout_pin_;
  GPIOPin *sck_pin_;
  HempyGain gain_{Hempy_GAIN_128};
};

}  // namespace hempy
}  // namespace esphome