import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, sensor, i2c
from esphome import automation
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
    CONF_PUBLISH_INITIAL_STATE
)

CONF_QWIIC_TWIST = 'qwiic_twist'
CONF_ENCODER = "encoder"
CONF_BUTTON = "button"
CONF_ON_CLOCKWISE = "on_clockwise"
CONF_ON_ANTICLOCKWISE = "on_anticlockwise"

CODEOWNERS = ["@mcmanigle"]
MULTI_CONF = True

qwiic_twist_ns = cg.esphome_ns.namespace("qwiic_twist")
QwiicTwist     = qwiic_twist_ns.class_("QwiicTwist", i2c.I2CDevice, cg.PollingComponent)


TwistEncoderRestoreMode = qwiic_twist_ns.enum("TwistEncoderRestoreMode")

ENCODER_RESTORE_MODES = {
    "RESTORE_DEFAULT_ZERO": TwistEncoderRestoreMode.TWIST_ENCODER_RESTORE_DEFAULT_ZERO,
    "ALWAYS_ZERO": TwistEncoderRestoreMode.TWIST_ENCODER_ALWAYS_ZERO,
}

TwistEncoderSetValueAction = qwiic_twist_ns.class_(
    "TwistEncoderSetValueAction", automation.Action
)
TwistEncoderClockwiseTrigger = qwiic_twist_ns.class_(
    "TwistEncoderClockwiseTrigger", automation.Trigger
)
TwistEncoderAnticlockwiseTrigger = qwiic_twist_ns.class_(
    "TwistEncoderAnticlockwiseTrigger", automation.Trigger
)


DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["light", "sensor", "binary_sensor"]


encoder_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_STEPS,
    icon=ICON_ROTATE_RIGHT,
    accuracy_decimals=0,
).extend(
    {
        cv.Optional(CONF_PUBLISH_INITIAL_STATE, default=False): cv.boolean,
        cv.Optional(CONF_RESTORE_MODE, default="RESTORE_DEFAULT_ZERO"): cv.enum(
            ENCODER_RESTORE_MODES, upper=True, space="_"
        ),

        cv.Optional(CONF_ON_CLOCKWISE): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                    TwistEncoderClockwiseTrigger
                ),
            }
        ),
        cv.Optional(CONF_ON_ANTICLOCKWISE): automation.validate_automation(
            {
                cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                    TwistEncoderAnticlockwiseTrigger
                ),
            }
        ),
    }
)

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_QWIIC_TWIST): cv.declare_id(QwiicTwist),
        cv.Optional(CONF_ENCODER):       encoder_schema,
        cv.Optional(CONF_BUTTON):        binary_sensor.BINARY_SENSOR_SCHEMA,
        
    }
).extend(cv.polling_component_schema('50ms')).extend(i2c.i2c_device_schema(0x3f))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_QWIIC_TWIST])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    
    if CONF_ENCODER in config:
        encoder_config = config[CONF_ENCODER]
        if CONF_PUBLISH_INITIAL_STATE in encoder_config:
            cg.add(var.set_publish_initial_encoder_value(encoder_config[CONF_PUBLISH_INITIAL_STATE]))
        if CONF_RESTORE_MODE in encoder_config:
            cg.add(var.set_encoder_restore_mode(encoder_config[CONF_RESTORE_MODE]))
        for conf in encoder_config.get(CONF_ON_CLOCKWISE, []):
            trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
            await automation.build_automation(trigger, [], conf)
        for conf in encoder_config.get(CONF_ON_ANTICLOCKWISE, []):
            trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
            await automation.build_automation(trigger, [], conf)
        
        encoder = await sensor.new_sensor(encoder_config)
        cg.add(var.set_encoder_sensor(encoder))

    if CONF_BUTTON in config:
        button = await binary_sensor.new_binary_sensor(config[CONF_BUTTON])
        cg.add(var.set_button_sensor(button))

    


# @automation.register_action(
#     "sensor.rotary_encoder.set_value",
#     TwistEncoderSetValueAction,
#     cv.Schema(
#         {
#             cv.Required(CONF_ID): cv.use_id(sensor.Sensor),
#             cv.Required(CONF_VALUE): cv.templatable(cv.int_),
#         }
#     ),
# )
# async def sensor_template_publish_to_code(config, action_id, template_arg, args):
#     paren = await cg.get_variable(config[CONF_ID])
#     var = cg.new_Pvariable(action_id, template_arg, paren)
#     template_ = await cg.templatable(config[CONF_VALUE], args, int)
#     cg.add(var.set_value(template_))
#     return var
