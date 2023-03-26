import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, STATE_CLASS_NONE
from .. import qwiic_twist_ns, QwiicTwist, CONF_QWIIC_TWIST

QwiicTwistEncoder = qwiic_twist_ns.class_("QwiicTwistEncoder", sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = sensor.sensor_schema(
    accuracy_decimals=1,
    state_class=STATE_CLASS_NONE,
).extend(
    {
        cv.GenerateID(): cv.declare_id(QwiicTwistEncoder),
        cv.GenerateID(CONF_QWIIC_TWIST): cv.use_id(QwiicTwist),
    }
).extend(cv.polling_component_schema('60ms'))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    qwiic_twist = await cg.get_variable(config[CONF_QWIIC_TWIST])
    cg.add(var.set_parent(qwiic_twist))
