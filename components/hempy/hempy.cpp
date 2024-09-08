#include "hempy.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace hempy {

static const char *const TAG = "hempy";

void HempySensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Hempy '%s'...", this->name_.c_str());
  this->sck_pin_->setup();
  this->weight_sensor_dout_pin_->setup();
  this->sck_pin_->digital_write(false);

  // Read sensor once without publishing to set the gain
  this->read_sensor_(nullptr);
}

void HempySensor::dump_config() {
  LOG_SENSOR("", "Hempy", this);
  LOG_PIN("  DOUT Pin: ", this->weight_sensor_dout_pin_);
  LOG_PIN("  SCK Pin: ", this->sck_pin_);
  LOG_UPDATE_INTERVAL(this);
}
float HempySensor::get_setup_priority() const { return setup_priority::DATA; }
void HempySensor::update() {
  uint32_t result;
  if (this->read_sensor_(&result)) {
    int32_t value = static_cast<int32_t>(result);
    ESP_LOGD(TAG, "'%s': Got value %" PRId32, this->name_.c_str(), value);
    this->publish_state(value);
  }
}
bool HempySensor::read_sensor_(uint32_t *result) {
  if (this->weight_sensor_dout_pin_->digital_read()) {
    ESP_LOGW(TAG, "Hempy is not ready for new measurements yet!");
    this->status_set_warning();
    return false;
  }

  uint32_t data = 0;
  bool final_dout;

  {
    InterruptLock lock;
    for (uint8_t i = 0; i < 24; i++) {
      this->sck_pin_->digital_write(true);
      delayMicroseconds(1);
      data |= uint32_t(this->weight_sensor_dout_pin_->digital_read()) << (23 - i);
      this->sck_pin_->digital_write(false);
      delayMicroseconds(1);
    }

    // Cycle clock pin for gain setting
    for (uint8_t i = 0; i < this->gain_; i++) {
      this->sck_pin_->digital_write(true);
      delayMicroseconds(1);
      this->sck_pin_->digital_write(false);
      delayMicroseconds(1);
    }
    final_dout = this->weight_sensor_dout_pin_->digital_read();
  }

  if (!final_dout) {
    ESP_LOGW(TAG, "Hempy DOUT pin not high after reading (data 0x%" PRIx32 ")!", data);
    this->status_set_warning();
    return false;
  }

  this->status_clear_warning();

  if (data & 0x800000ULL) {
    data |= 0xFF000000ULL;
  }

  if (result != nullptr)
    *result = data;
  return true;
}

}  // namespace hempy
}  // namespace esphome