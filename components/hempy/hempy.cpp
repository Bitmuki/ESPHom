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
    float weight = this->weight_sensor_->state - tare_weight_offset_;  // Access the weight sensor's state and apply the offset
    ESP_LOGI("hempy", "Weight: %.2f kg, (start: %.2f, stop: %.2f)", weight,start_watering_weight_, stop_watering_weight_); // Log the weight in kg (or the unit configured)
  } else {
    ESP_LOGW("hempy", "No weight sensor available");
  }
}

// Calibrates the 0 reading to the current reaiding of the weight sensor
void HempyComponent::tare_weight_scale()
{
  if (this->weight_sensor_) {
    tare_weight_offset_ = this->weight_sensor_->state;  // store the current reading as the offset
    ESP_LOGI("hempy", "Offset: %.2f kg", tare_weight_offset_); // Log the weight in kg (or the unit configured)
  } 

}

}  // namespace hempy
}  // namespace esphome