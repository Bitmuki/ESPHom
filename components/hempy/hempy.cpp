#include "esphome.h"

namespace esphome {
namespace hempy {

class HempyComponent : public PollingComponent {
 public:
  // Constructor that sets the update interval to 1 second (1000 ms)
  HempyComponent() : PollingComponent(1000) {}

  void setup() override {
    ESP_LOGI("hempy", "Hempy component initialized");
  }
