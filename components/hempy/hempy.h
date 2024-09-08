#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace hempy {

class HempyComponent : public PollingComponent {
 public:
  HempyComponent();
  void setup() override;
  void update() override;
};

}  // namespace hempy
}  // namespace esphome