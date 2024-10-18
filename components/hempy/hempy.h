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
  void set_start_watering_weight(number::Number *start_watering_weight) { this->start_watering_weight_ = start_watering_weight; }
  void set_stop_watering_weight(number::Number *stop_watering_weight) { this->stop_watering_weight_ = stop_watering_weight; }
  void set_waterpump(switch_::Switch *waterpump) { this->waterpump_ = waterpump; }  // Set the pump switch reference


 private:
  sensor::Sensor *weight_sensor_;  // Reference to the weight sensor
  number::Number *start_watering_weight_;  // Reference to start watering weight
  number::Number *stop_watering_weight_;   // Reference to stop watering weigh
  switch_::Switch *waterpump_;  // Reference to the relay controlling the water pump
  HempyStates State{HempyStates::IDLE}; // Stores the current state of the hempy bucket
  uint32_t StateTimer = 0;  // Track how much time is spent in one State
  uint32_t PumpOnTimer = 0;  // Track how long watering pump is on continuously (one water-drain cycle)
  uint32_t WateringTimer = 0;  // Track how long watering pump is on in total (all water-drain cycles)
  float BucketStartWeight = 0; //Store the start weight of the bucket before starting the water-drain cycles
  uint32_t DrainWaitTime = 30; //Seconds - how long to wait for the water to drain.  TODO: make this user configurable from HA dashboard
};

}  // namespace hempy
}  // namespace esphome