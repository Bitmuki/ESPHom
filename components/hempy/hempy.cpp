#include "hempy.h"

namespace esphome {
namespace hempy {

HempyComponent::HempyComponent() : PollingComponent(1000) {}  // Constructor

void HempyComponent::setup() {
  ESP_LOGI("hempy", "Hempy component initialized");
}

void HempyComponent::update() {
  ESP_LOGI("hempy", "Hello World");
}

}  // namespace hempy
}  // namespace esphome