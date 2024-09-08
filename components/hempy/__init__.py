import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import core, components
from esphome.const import (    
    CONF_ID
)
from esphome.components import sensor

# Define a namespace for the component
hempy_ns = cg.esphome_ns.namespace('hempy')
HempyComponent = hempy_ns.class_('HempyComponent', cg.PollingComponent)

# Configuration schema
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(HempyComponent),
    cv.Required('weight_sensor'): cv.use_id(sensor.Sensor),
    cv.Optional('start_watering_weight', default=0.0): cv.float_,
    cv.Optional('stop_watering_weight', default=0.0): cv.float_,
}).extend(cv.polling_component_schema(default_update_interval="1s"))

# Code generation when configuring the component
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    weight_sensor = await cg.get_variable(config['weight_sensor'])
    cg.add(var.set_weight_sensor(weight_sensor))
    cg.add(var.set_start_watering_weight(config[CONF_START_WATERING_WEIGHT]))
    cg.add(var.set_stop_watering_weight(config[CONF_STOP_WATERING_WEIGHT]))
    await cg.register_component(var, config)
    #await cg.register_polling_component(var, config)