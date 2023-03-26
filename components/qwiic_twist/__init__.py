import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, sensor, i2c

CODEOWNERS = ["@mcmanigle"]
MULTI_CONF = True

qwiic_twist_ns = cg.esphome_ns.namespace("qwiic_twist")
QwiicTwist     = qwiic_twist_ns.class_("QwiicTwist", i2c.I2CDevice, cg.Component)

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["light", "sensor"]

CONF_QWIIC_TWIST = 'qwiic_twist'

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_QWIIC_TWIST): cv.declare_id(QwiicTwist)
    }
).extend(i2c.i2c_device_schema(0x3f))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_QWIIC_TWIST])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    
