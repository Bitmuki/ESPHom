#pragma once

#include "esphome/core/component.h"

namespace esphome {
namespace hempy {

class HempyComponent : public Component {
 public:
   void setup() override;
   void loop() override;
   void dump_config() override;
};

}  // namespace hempy
}  // namespace esphome