import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID, CONF_PIN
from .. import qwiic_twist_ns, QwiicTwist, CONF_QWIIC_TWIST

QwiicTwistBinarySensor = seesaw_ns.class_(
    "QwiicTwistBinarySensor", binary_sensor.BinarySensor, cg.PollingComponent
)

CONFIG_SCHEMA = binary_sensor.BINARY_SENSOR_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(QwiicTwistBinarySensor),
        cv.GenerateID(CONF_QWIIC_TWIST): cv.use_id(QwiicTwist),
    }
).extend(cv.polling_component_schema('30ms'))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await binary_sensor.register_binary_sensor(var, config)
    qwiic_twist = await cg.get_variable(config[CONF_QWIIC_TWIST])
    cg.add(var.set_parent(qwiic_twist))