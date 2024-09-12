#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace hempy {

enum class HempyStates
{
  DISABLED,
  IDLE,
  WATERING,
  DRAINING
};

class HempyComponent : public PollingComponent {
 public:
  HempyComponent();
  void setup() override;
  void update() override;
  void set_weight_sensor(sensor::Sensor *weight_sensor) { this->weight_sensor_ = weight_sensor; } // Store reference of the weight sensor belonging to this Hempy bucket
  void set_start_watering_weight(float start_watering_weight) { this->start_watering_weight_ = start_watering_weight; }
  void set_stop_watering_weight(float start_watering_weight) { this->stop_watering_weight_ = start_watering_weight; }
  float get_start_watering_weight() const { return this->start_watering_weight_; }
  float get_stop_watering_weight() const { return this->stop_watering_weight_; }
  void tare_weight_scale();
  const char *to_text_state();

 private:
  sensor::Sensor *weight_sensor_;
  HempyStates State = HempyStates::IDLE;
  float start_watering_weight_{0.0};
  float stop_watering_weight_{0.0};
  float tare_weight_offset_{0.0}; // Updated when tare_weight_scale() is called
};

}  // namespace hempy
}  // namespace esphome