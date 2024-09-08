import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import sensor
from esphome.const import (
    CONF_CLK_PIN,
    CONF_GAIN,
    ICON_SCALE,
    STATE_CLASS_MEASUREMENT,
)

Hempy_ns = cg.esphome_ns.namespace("hempy")
HempySensor = Hempy_ns.class_("HempySensor", sensor.Sensor, cg.PollingComponent)

CONF_weight_sensor_dout_pin = "weight_sensor_dout_pin"

HempyGain = Hempy_ns.enum("HempyGain")
GAINS = {
    128: HempyGain.Hempy_GAIN_128,
    32: HempyGain.Hempy_GAIN_32,
    64: HempyGain.Hempy_GAIN_64,
}

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        HempySensor,
        icon=ICON_SCALE,
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.Required(CONF_weight_sensor_dout_pin): pins.gpio_input_pin_schema,
            cv.Required(CONF_CLK_PIN): pins.gpio_output_pin_schema,
            cv.Optional(CONF_GAIN, default=128): cv.enum(GAINS, int=True),
        }
    )
    .extend(cv.polling_component_schema("60s"))
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)

    weight_sensor_dout_pin = await cg.gpio_pin_expression(config[CONF_weight_sensor_dout_pin])
    cg.add(var.set_weight_sensor_dout_pin(weight_sensor_dout_pin))
    sck_pin = await cg.gpio_pin_expression(config[CONF_CLK_PIN])
    cg.add(var.set_sck_pin(sck_pin))
    cg.add(var.set_gain(config[CONF_GAIN]))