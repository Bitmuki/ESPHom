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
  void set_weight_sensor(sensor::Sensor *weight_sensor) { this->weight_sensor_ = weight_sensor; }
  void set_start_watering_weight(number::Number *start_weight) { this->start_watering_weight_ = start_weight; }
  void set_stop_watering_weight(number::Number *stop_weight) { this->stop_watering_weight_ = stop_weight; }


 private:
  sensor::Sensor *weight_sensor_;  // Reference to the weight sensor
  number::Number *start_watering_weight_;  // Reference to start watering weight
  number::Number *stop_watering_weight_;   // Reference to stop watering weight
  HempyStates State{HempyStates::IDLE}; // Stores the current state of the hempy bucket
};

}  // namespace hempy
}  // namespace esphome