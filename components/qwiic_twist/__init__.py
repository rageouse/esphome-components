import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, sensor, i2c
from esphome.const import CONF_OUTPUT_ID

qwiic_twist_ns = cg.esphome_ns.namespace("qwiic_twist")
QwiicTwist        = qwiic_twist_ns.class_("QwiicTwist", cg.Component)
QwiicTwistRGB     = qwiic_twist_ns.class_("QwiicTwistRGB", light.LightOutput, cg.Component)
QwiicTwistEncoder = qwiic_twist_ns.class_("QwiicTwistEncoder", sensor.Sensor, cg.PollingComponent)

DEPENDENCIES = ["light", "sensor", "i2c"]
AUTO_LOAD = ["light", "sensor"]

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(QwiicTwist),
            cv.GenerateID(CONF_OUTPUT_ID+'RGB'): cv.declare_id(QwiicTwistRGB),
            cv.GenerateID(CONF_OUTPUT_ID+'Encoder'): cv.declare_id(QwiicTwistEncoder),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID+'RGB'])
    await light.register_light(var, config)
    
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID+'Encoder'])
    await sensor.register_sensor(var, config)

