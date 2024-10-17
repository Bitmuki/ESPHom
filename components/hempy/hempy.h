#pragma once

#include "esphome.h"  // Include ESPHome macros and functionality
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/application.h"
#include "esphome/components/number/number.h"

namespace esphome {
namespace hempy {

enum class HempyStates
{
  DISABLED,
  IDLE,
  WATERING,
  DRAINING
};

class HempyBucket : public PollingComponent {
 public:
  HempyBucket();
  void setup() override;
  void update() override;
  void update_state(HempyStates NewState);
  const char *to_text_state(HempyStates state);

 private:
  sensor::Sensor *weight_sensor_;  // Reference to the weight sensor
  HempyStates State{HempyStates::IDLE}; // Stores the current state of the hempy bucket
  uint32_t StateTimer{millis()};      // Measures how much time is spent in a state


}  // namespace hempy
}  // namespace esphome