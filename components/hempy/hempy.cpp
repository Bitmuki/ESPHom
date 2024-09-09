#include "hempy.h"

namespace esphome {
namespace hempy {

HempyComponent::HempyComponent() : PollingComponent(1000), weight_sensor_(nullptr), start_watering_weight_(0.0), stop_watering_weight_(0.0) {}  // Constructor

void HempyComponent::setup() {
  ESP_LOGI("hempy", "Hempy component initialized with StartWateringWeight: %.2f, StopWateringWeight: %.2f",
           start_watering_weight_, stop_watering_weight_);
}

void HempyComponent::update() {
   if (this->weight_sensor_) {
    float weight = this->weight_sensor_->state;  // Access the weight sensor's state
    ESP_LOGI("hempy", "Weight: %.2f kg, (start: %.2f, stop: %.2f)", weight,start_watering_weight_, stop_watering_weight_); // Log the weight in kg (or the unit configured)
  } else {
    ESP_LOGW("hempy", "No weight sensor available");
  }
}

}  // namespace hempy
}  // namespace esphome