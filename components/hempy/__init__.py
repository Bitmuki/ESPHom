import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import sensor

# Importing the Hempy component
from . import Hempy_ns, HempySensor, HempyGain

# Define the schema for validation
CONF_CLK_PIN = "clk_pin"
CONF_weight_sensor_dout_pin = "weight_sensor_dout_pin"
CONF_GAIN = "gain"

# Registering the Hempy namespace and component in ESPHome
hempy_ns = cg.esphome_ns.namespace("hempy")
HempySensor = hempy_ns.class_("HempySensor", sensor.Sensor, cg.PollingComponent)
HempyGain = hempy_ns.enum("HempyGain")

# Add your sensor configuration (from sensor.py)
CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(HempySensor),
            cv.Required(CONF_CLK_PIN): pins.gpio_output_pin_schema,
            cv.Required(CONF_weight_sensor_dout_pin): pins.gpio_input_pin_schema,
            cv.Optional(CONF_GAIN, default=128): cv.enum(
                {
                    128: HempyGain.Hempy_GAIN_128,
                    32: HempyGain.Hempy_GAIN_32,
                    64: HempyGain.Hempy_GAIN_64,
                },
                int=True,
            ),
        }
    ).extend(sensor.SENSOR_SCHEMA),
)

# Linking the C++ code generator (from sensor.py)
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    clk_pin = await cg.gpio_pin_expression(config[CONF_CLK_PIN])
    cg.add(var.set_sck_pin(clk_pin))

    weight_sensor_dout_pin = await cg.gpio_pin_expression(config[CONF_weight_sensor_dout_pin])
    cg.add(var.set_weight_sensor_dout_pin(weight_sensor_dout_pin))

    cg.add(var.set_gain(config[CONF_GAIN]))

# Finally, register this as a component in ESPHome
HEMPY_SENSOR_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(HempySensor),
    }
).extend(CONFIG_SCHEMA)
