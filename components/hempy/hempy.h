#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/application.h"

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
  void set_weight_sensor(sensor::Sensor *weight_sensor) { this->weight_sensor_ = weight_sensor; } // Store reference of the weight sensor belonging to this Hempy bucket
  void set_start_watering_weight(float start_watering_weight) { this->start_watering_weight_ = start_watering_weight; }
  void set_stop_watering_weight(float start_watering_weight) { this->stop_watering_weight_ = start_watering_weight; }
  float get_start_watering_weight() const { return this->start_watering_weight_; }
  float get_stop_watering_weight() const { return this->stop_watering_weight_; }
  void tare_weight_scale();
  const char *to_text_state(HempyStates state);

 private:
  sensor::Sensor *weight_sensor_;
  HempyStates State{HempyStates::IDLE}; // Stores the current state of the hempy bucket
  uint32_t StateTimer{millis()};      // Measures how much time is spent in a state
  float start_watering_weight_{0.0};
  float stop_watering_weight_{0.0};
  float tare_weight_offset_{0.0}; // Updated when tare_weight_scale() is called
};

// This function is called to register the custom component with ESPHome
/*
extern "C" void register_hempy() {
  esphome::ComponentFactory::get().register_component<HempyBucket>("hempy");
  }
  */

}  // namespace hempy
}  // namespace esphome