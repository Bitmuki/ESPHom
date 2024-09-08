#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace hempy {

class HempyComponent : public PollingComponent {
 public:
  HempyComponent();
  void setup() override;
  void update() override;
  void set_weight_sensor(sensor::Sensor *sensor) { this->weight_sensor_ = sensor; }
  void set_start_watering_weight(float weight) { start_watering_weight_ = weight; }
  void set_stop_watering_weight(float weight) { stop_watering_weight_ = weight; }

 private:
  sensor::Sensor *weight_sensor_;
  float start_watering_weight_{0.0};
  float stop_watering_weight_{0.0};
};

}  // namespace hempy
}  // namespace esphome