#include "hempy.h"

namespace esphome {
namespace hempy {

HempyComponent::HempyComponent() : PollingComponent(1000), weight_sensor_(nullptr) {}  // Constructor

void HempyComponent::setup() {
  ESP_LOGI("hempy", "Hempy component initialized");
}

void HempyComponent::update() {
   if (this->weight_sensor_) {
    float weight = this->weight_sensor_->state;  // Access the weight sensor's state
    ESP_LOGI("hempy", "Weight: %.2f kg", weight); // Log the weight in kg (or the unit configured)
  } else {
    ESP_LOGW("hempy", "No weight sensor available");
  }
}

}  // namespace hempy
}  // namespace esphome