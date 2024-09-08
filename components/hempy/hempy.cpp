#include "hempy.h"
#include "esphome/core/log.h"

namespace esphome {
namespace hempy {

static const char *TAG = "hempy";

void HempyComponent::setup() {
  ESP_LOGI(TAG, "Setting up Hempy component");
}

void HempyComponent::loop() {
  // Main logic of the component
}

void HempyComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Hempy component");
}

}  // namespace hempy
}  // namespace esphome