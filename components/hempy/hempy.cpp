#include "hempy.h"

namespace esphome {
namespace hempy {

HempyBucket::HempyBucket() : PollingComponent(1000), weight_sensor_(nullptr), start_watering_weight_(0.0), stop_watering_weight_(0.0) {}  // Constructor

void HempyBucket::setup() {
  id(start_watering_weight).add_on_state_callback([this](float value) {
    // Handle the change in start_watering_weight
    ESP_LOGD("custom", "Start watering weight updated to: %.2f", value);
});

id(stop_watering_weight).add_on_state_callback([this](float value) {
    // Handle the change in stop_watering_weight
    ESP_LOGD("custom", "Stop watering weight updated to: %.2f", value);
});

  ESP_LOGI("hempy", "Hempy component initialized with StartWateringWeight: %.2f, StopWateringWeight: %.2f",
           id(start_watering_weight).state,id(start_watering_weight).state);
}

void HempyBucket::update() {
   if (this->weight_sensor_) {
    float weight = this->weight_sensor_->state;  // Access the weight sensor's state and apply the offset
    ESP_LOGI("hempy", "State: %s, Weight: %.2f kg, (start: %.2f, stop: %.2f)", 
             to_text_state(State),weight,start_watering_weight_, stop_watering_weight_); // Log the weight in kg (or the unit configured)
  } else {
    ESP_LOGW("hempy", "No weight sensor available");
  }
  //update_state(State);
}

/*
void HempyBucket::update_state(HempyStates NewState)
{
  bool BlockOverWritingState = false; //Used when a state transitions to a new state
  if (State != NewState)
  {
    StateTimer = millis();                         ///< Start measuring the time spent in the new State
    memset(&LongMessage[0], 0, MaxLongTextLength); ///< clear variable

     ESP_LOGI("hempy", "State: %s, Weight: %.2f kg, (start: %.2f, stop: %.2f)", 
             to_text_state(State),weight,start_watering_weight_, stop_watering_weight_); 
             
    strcat(LongMessage, getName(F("state: ")));
    strcat(LongMessage, toText_hempyState(State));
    strcat_P(LongMessage, (PGM_P)F(" -> "));
    strcat(LongMessage, toText_hempyState(NewState));
    logToSerials(&LongMessage, true, 3);
  }

  BucketWeightSensor->readWeight(false); ///< Force Bucket weight update

  switch (NewState)
  {
  case HempyStates::DISABLED:
    BucketPump->disablePump();
    break;
  case HempyStates::IDLE:
    BucketPump->stopPump(true);
    if (BucketPump->getState() != WaterPumpStates::DISABLED && (DryWeight <= 0 || BucketWeightSensor->getWeight() <= DryWeight))
    {
      if (BucketWeightSensor->getWeight() >= DryWeight - *OverflowTarget) ///< Filters out waterings triggered by a disconnected weight sensor
      {
        if (BucketWasteReservoir->setReserved())
        {
          update_state(HempyStates::WATERING);
          BlockOverWritingState = true;
        }
      }
    }
    break;
  case HempyStates::WATERING:
    if (State != NewState)
    {
      BucketStartWeight = BucketWeightSensor->getWeight();              // Store the bucket weight before starting the pump
      if (State == HempyStates::IDLE || State == HempyStates::DISABLED) // First time entering the WATERING-DRIAINING cycles
      {
        PumpOnTimer = millis(); ///Start measuring the pump ON time for this cycle
        WateringTime = 0;       ///Reset the counter that tracks the total pump ON time during the watering process (multiple WATERING-DRAINING cycles)
      }
      if (State == HempyStates::DRAINING) /// The WATERING-DRIAINING cycles are already in progress
      {
        PumpOnTimer = millis(); ///Start measuring the pump ON time for this cycle
      }
      BucketPump->startPump(true);
    }
    if (BucketWeightSensor->getWeight(false) >= WetWeight && BucketWeightSensor->getWeight(false) - BucketStartWeight + BucketWasteReservoir->getWeightIncrease() >= *OverflowTarget) //Wet weight reached AND Target overflow's worth of water was added, wait for it to drain
    {
      WateringTime += millis() - PumpOnTimer;
      update_state(HempyStates::DRAINING);
      BlockOverWritingState = true;
    }
    if (WateringTime > ((uint32_t)BucketPump->getTimeOut() * 1000) || BucketPump->getState() == WaterPumpStates::DISABLED || BucketWasteReservoir->getState() == WasteReservoirStates::FULL) ///< Disable watering if: Timeout before the waste target was reached, pump failed or the waste reservoir got full
    {
      BucketWasteReservoir->clearReservation();
      update_state(HempyStates::DISABLED);
      BlockOverWritingState = true;
    }
    break;
  case HempyStates::DRAINING:
    BucketPump->stopPump();
    State = HempyStates::DRAINING;                                 //Store the new state immediately - Only important when DrainWaitTime is set to 0
    if (millis() - StateTimer > ((uint32_t)*DrainWaitTime * 1000)) ///< Waiting for the water to drain
    {
      if (BucketWasteReservoir->checkTarget(*OverflowTarget)) //Check if target overflow weight is reached
      {
        WetWeight = BucketWeightSensor->getWeight(); //Measure wet weight
        DryWeight = WetWeight - *EvaporationTarget;  //Calculate next watering weight
        BucketWasteReservoir->clearReservation();    ///< Free up the waste reservoir
        update_state(HempyStates::IDLE);
      }
      else
      {
        update_state(HempyStates::WATERING); /// Continue watering
      }
      BlockOverWritingState = true;
    }
    break;
  }

  if (State != NewState && !BlockOverWritingState)
  {
    State = NewState;
  }
}

*/

// Calibrates the 0 reading to the current reaiding of the weight sensor
/*
void HempyBucket::tare_weight_scale()
{
  if (this->weight_sensor_ && weight_sensor_->has_state()) {
    tare_weight_offset_ = this->weight_sensor_->state;  // store the current reading as the offset
    ESP_LOGI("hempy", "Offset: %.2f kg", tare_weight_offset_); // Log the weight in kg (or the unit configured)
  } 
}
*/

const char *HempyBucket::to_text_state(HempyStates state) {
  switch (state) {
  case HempyStates::DISABLED:
    return "DISABLED";
  case HempyStates::IDLE:
    return "IDLE";
  case HempyStates::WATERING:
    return "WATERING";
  case HempyStates::DRAINING:
    return "DRAINING";
  default:
    return "?";
  }
}

}  // namespace hempy
}  // namespace esphome