import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_RESOLUTION,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE,
    UNIT_STEPS,
    ICON_ROTATE_RIGHT,
    CONF_VALUE,
    CONF_PIN_A,
    CONF_PIN_B,
    CONF_TRIGGER_ID,
    CONF_RESTORE_MODE,
)
from .. import qwiic_twist_ns, QwiicTwist, CONF_QWIIC_TWIST

CONF_ON_CLOCKWISE = "on_clockwise"
CONF_ON_ANTICLOCKWISE = "on_anticlockwise"
CONF_PUBLISH_INITIAL_VALUE = "publish_initial_value"

TwistEncoderRestoreMode = qwiic_twist_ns.enum("TwistEncoderRestoreMode")
RESTORE_MODES = {
    "RESTORE_DEFAULT_ZERO": TwistEncoderRestoreMode.TWIST_ENCODER_RESTORE_DEFAULT_ZERO,
    "ALWAYS_ZERO": TwistEncoderRestoreMode.TWIST_ENCODER_ALWAYS_ZERO,
}

QwiicTwistEncoder = qwiic_twist_ns.class_("QwiicTwistEncoder", sensor.Sensor, cg.PollingComponent)

TwistEncoderSetValueAction = qwiic_twist_ns.class_(
    "TwistEncoderSetValueAction", automation.Action
)
TwistEncoderClockwiseTrigger = qwiic_twist_ns.class_(
    "TwistEncoderClockwiseTrigger", automation.Trigger
)
TwistEncoderAnticlockwiseTrigger = qwiic_twist_ns.class_(
    "TwistEncoderAnticlockwiseTrigger", automation.Trigger
)


CONFIG_SCHEMA = sensor.sensor_schema(
    QwiicTwistEncoder,
    unit_of_measurement=UNIT_STEPS,
    icon=ICON_ROTATE_RIGHT,
    accuracy_decimals=0,
).extend(
    {
        cv.GenerateID(): cv.declare_id(QwiicTwistEncoder),
        cv.GenerateID(CONF_QWIIC_TWIST): cv.use_id(QwiicTwist),
        
        cv.Optional(CONF_PUBLISH_INITIAL_VALUE, default=False): cv.boolean,
        cv.Optional(CONF_RESTORE_MODE, default="RESTORE_DEFAULT_ZERO"): cv.enum(
            RESTORE_MODES, upper=True, space="_"
        ),

        cv.Optional(CONF_ON_CLOCKWISE): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                    RotaryEncoderClockwiseTrigger
                ),
            }
        ),
        cv.Optional(CONF_ON_ANTICLOCKWISE): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                    RotaryEncoderAnticlockwiseTrigger
                ),
            }
        ),
    }
).extend(cv.polling_component_schema('60ms'))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    qwiic_twist = await cg.get_variable(config[CONF_QWIIC_TWIST])
    cg.add(var.set_parent(qwiic_twist))
    cg.add(var.set_publish_initial_value(config[CONF_PUBLISH_INITIAL_VALUE]))
    cg.add(var.set_restore_mode(config[CONF_RESTORE_MODE]))

    for conf in config.get(CONF_ON_CLOCKWISE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)
    for conf in config.get(CONF_ON_ANTICLOCKWISE, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)

@automation.register_action(
    "sensor.rotary_encoder.set_value",
    TwistEncoderSetValueAction,
    cv.Schema(
        {
            cv.Required(CONF_ID): cv.use_id(sensor.Sensor),
            cv.Required(CONF_VALUE): cv.templatable(cv.int_),
        }
    ),
)
async def sensor_template_publish_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    template_ = await cg.templatable(config[CONF_VALUE], args, int)
    cg.add(var.set_value(template_))
    return var

