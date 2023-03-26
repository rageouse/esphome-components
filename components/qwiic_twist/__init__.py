import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, sensor, i2c
from esphome.const import CONF_COMPONENT_ID, CONF_LIGHT_ID, CONF_SENSOR_ID

qwiic_twist_ns = cg.esphome_ns.namespace("qwiic_twist")
QwiicTwist        = qwiic_twist_ns.class_("QwiicTwist", cg.Component)

QwiicTwistRGB     = QwiicTwist.twist_rgb_
QwiicTwistEncoder = QwiicTwist.twist_encoder_


# QwiicTwistRGB     = qwiic_twist_ns.class_("QwiicTwistRGB", light.LightOutput, cg.Component)
# QwiicTwistEncoder = qwiic_twist_ns.class_("QwiicTwistEncoder", sensor.Sensor, cg.PollingComponent)

DEPENDENCIES = ["light", "sensor", "i2c"]
AUTO_LOAD = ["light", "sensor"]

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(CONF_COMPONENT_ID): cv.declare_id(QwiicTwist),
            cv.GenerateID(CONF_LIGHT_ID):     cv.declare_id(QwiicTwistRGB),
            cv.GenerateID(CONF_SENSOR_ID):    cv.declare_id(QwiicTwistEncoder),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(light.RGB_LIGHT_SCHEMA)
    .extend(sensor.SENSOR_SCHEMA)
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_COMPONENT_ID])
    await cg.register_component(var, config)
    
    var = cg.new_Pvariable(config[CONF_LIGHT_ID])
    await light.register_light(var, config)

    var = cg.new_Pvariable(config[CONF_SENSOR_ID])
    await sensor.register_sensor(var, config)

