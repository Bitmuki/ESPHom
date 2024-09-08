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

 private:
  sensor::Sensor *weight_sensor_;
};

}  // namespace hempy
}  // namespace esphome