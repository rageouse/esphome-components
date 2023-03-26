import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, sensor, i2c
from esphome.const import CONF_COMPONENT_ID, CONF_LIGHT_ID, CONF_SENSOR_ID

qwiic_twist_ns = cg.esphome_ns.namespace("qwiic_twist")
QwiicTwist        = qwiic_twist_ns.class_("QwiicTwist", cg.Component)
QwiicTwistRGB     = qwiic_twist_ns.class_("QwiicTwistRGB", light.LightOutput, cg.Component)
QwiicTwistEncoder = qwiic_twist_ns.class_("QwiicTwistEncoder", sensor.Sensor, cg.PollingComponent)

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

async def register_light(var, config):
    if not CORE.has_id(config[CONF_ID]):
        var = cg.new_Pvariable(config[CONF_ID], var)
    cg.add(cg.App.register_light(var))
    await cg.register_component(var, config)
    await setup_light_core_(var, output_var, config)

async def to_code(config):
    qwiic_twist = cg.new_Pvariable(config[CONF_COMPONENT_ID])
    await cg.register_component(qwiic_twist, config)
    
    qwiic_twist_rgb = cg.new_Pvariable(config[CONF_LIGHT_ID])
    await cg.register_component(qwiic_twist_rgb, config)
    await light.register_light(qwiic_twist_rgb, config)
    cg.add(qwiic_twist_rgb.set_parent(qwiic_twist))

    qwiic_twist_encoder = cg.new_Pvariable(config[CONF_SENSOR_ID])
    await cg.register_component(qwiic_twist_encoder, config)
    await sensor.register_sensor(qwiic_twist_encoder, config)
    cg.add(qwiic_twist_encoder.set_parent(qwiic_twist))

