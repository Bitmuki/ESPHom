#include "hempy.h"

namespace esphome {
namespace hempy {

HempyBucket::HempyBucket(sensor::Sensor *weightSensor, number::Number *startWateringWeight,
                number::Number *stopWateringWeight, switch::Switch *waterPump)
        : PollingComponent(1000),  // Polling every 1000ms (1 second)
          WeightSensor(weightSensor), StartWateringWeight(startWateringWeight),
          StopWateringWeight(stopWateringWeight), WaterPumpaterPump_(waterPump) {}

void HempyBucket::setup() {

  ESP_LOGI("hempy", "Hempy component initialized with StartWateringWeight: %.2f, StopWateringWeight: %.2f",
          this->StartWateringWeight->state, this->StopWateringWeight->state);
}

void HempyBucket::update() {
  update_state(State);
  ESP_LOGI("hempy", "State: %s, Weight: %.2f kg, (start: %.2f, stop: %.2f)", 
             to_text_state(State), this->WeightSensor->state, this->StartWateringWeight->state, this->StopWateringWeight->state); // Log the weight in kg (or the unit configured)
}

void HempyBucket::update_state(HempyStates NewState){
  bool BlockOverWritingState = false; //Used when a state transitions to a new state
  if (State != NewState)
  {
    StateTimer = millis();                         ///< Start measuring the time spent in the new State
    ESP_LOGI("hempy", "State: %s -> %s", to_text_state(State),to_text_state(NewState));
  }

  //BucketWeightSensor->readWeight(false); ///< Force Bucket weight update

  switch (NewState)
  {
  case HempyStates::DISABLED:
    if(this->WaterPump->is_on())
      this->WaterPump->turn_off();
    break;
  case HempyStates::IDLE:
     if(this->WaterPump->is_on())
      this->WaterPump->turn_off();
    if (StartWateringWeight->state <= 0 || this->WeightSensor->state <= StartWateringWeight->state)
    {  //BucketPump->getState() != WaterPumpStates::DISABLED && 
      //if (BucketWeightSensor->getWeight() >= DryWeight - *OverflowTarget) ///< Filters out waterings triggered by a disconnected weight sensor
      {
        //if (BucketWasteReservoir->setReserved())
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
      BucketStartWeight = this->WeightSensor->state;              // Store the bucket weight before starting the pump
      if (State == HempyStates::IDLE || State == HempyStates::DISABLED) // First time entering the WATERING-DRIAINING cycles
      {
        PumpOnTimer = millis(); ///Start measuring the pump ON time for this cycle
        WateringTimer = 0;       ///Reset the counter that tracks the total pump ON time during the watering process (multiple WATERING-DRAINING cycles)
      }
      if (State == HempyStates::DRAINING) /// The WATERING-DRIAINING cycles are already in progress
      {
        PumpOnTimer = millis(); ///Start measuring the pump ON time for this cycle
      }
      this->WaterPump->turn_on();
    }
    if (this->WeightSensor->state >= StopWateringWeight->state) // TODO: add new logic instead of: && BucketWeightSensor->getWeight(false) - BucketStartWeight + BucketWasteReservoir->getWeightIncrease() >= *OverflowTarget) //Wet weight reached AND Target overflow's worth of water was added, wait for it to drain
    {
      WateringTimer += millis() - PumpOnTimer;
      update_state(HempyStates::DRAINING);
      BlockOverWritingState = true;
    }
    /* TODO: Add timeout for water pump
    if (WateringTimer > ((uint32_t)BucketPump->getTimeOut() * 1000) || BucketPump->getState() == WaterPumpStates::DISABLED || BucketWasteReservoir->getState() == WasteReservoirStates::FULL) ///< Disable watering if: Timeout before the waste target was reached, pump failed or the waste reservoir got full
    {
      BucketWasteReservoir->clearReservation();
      update_state(HempyStates::DISABLED);
      BlockOverWritingState = true;
    }
    */
    break;
  case HempyStates::DRAINING:
    this->WaterPump->turn_off();
    State = HempyStates::DRAINING;                                 //Store the new state immediately - Only important when DrainWaitTime is set to 0
    if (millis() - StateTimer > ((uint32_t)DrainWaitTime * 1000)) ///< Waiting for the water to drain
    {
      /*      
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
      */
      update_state(HempyStates::IDLE); //TODO: Create new logic for measuring the weight of water that drained away
      BlockOverWritingState = true;
    }
    break;
  }

  if (State != NewState && !BlockOverWritingState)
  {
    State = NewState;
  }
}

// Calibrates the 0 reading to the current reaiding of the weight sensor
/*
void HempyBucket::tare_weight_scale()
{
  if (this->WeightSensor && WeightSensor->has_state()) {
    tare_weight_offset_ = this->WeightSensor->state;  // store the current reading as the offset
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