#pragma once

#include "esphome.h"  // Include ESPHome macros and functionality
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/number/number.h"
#include "esphome/components/switch/switch.h"

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
  HempyBucket(sensor::Sensor *weightSensor, number::Number *startWateringWeight, number::Number *stopWateringWeight, switch_::Switch *waterPump): PollingComponent(1000),WeightSensor(weightSensor),StartWateringWeight(startWateringWeight),StopWateringWeight(stopWateringWeight), WaterPump(waterPump) {}
  void setup() override;
  void update() override;
  void update_state(HempyStates NewState);
  const char *to_text_state(HempyStates state);
  /*
  void set_weight_sensor(sensor::Sensor *weight_sensor) { this->WeightSensor = weight_sensor; }
  void set_start_watering_weight(number::Number *start_watering_weight) { this->StartWateringWeight = start_watering_weight; }
  void set_stop_watering_weight(number::Number *stop_watering_weight) { this->StopWateringWeight = stop_watering_weight; }
  void set_waterpump(switch_::Switch *waterpump) { this->WaterPump = waterpump; }  // Set the pump switch reference
*/

 private:
  sensor::Sensor *WeightSensor;  // Reference to the weight sensor
  number::Number *StartWateringWeight;  // Reference to start watering weight
  number::Number *StopWateringWeight;   // Reference to stop watering weigh
  switch_::Switch *WaterPump;  // Reference to the relay controlling the water pump
  HempyStates State{HempyStates::IDLE}; // Stores the current state of the hempy bucket
  uint32_t StateTimer = 0;  // Track how much time is spent in one State
  uint32_t PumpOnTimer = 0;
  uint32_t WateringTimer = 0;  // Track how long watering pump is on in total (all water-drain cycles)
  uint32_t WateringTimeout = 30;  // seconds - Maximum total pump ON time (all water-drain cycles), if watering is not finished the pump is considered broken.  TODO: make this user configurable from HA dashboard
  uint32_t DrainWaitTime = 30; // seconds - how long to wait for the water to drain.  TODO: make this user configurable from HA dashboard
  float BucketStartWeight = 0; // Store the start weight of the bucket before starting the water-drain cycles
  float OverflowTarget = 0.1; //kg - How much water should drain into the waste reservoir before considering the watering cycle finished 

};

}  // namespace hempy
}  // namespace esphome