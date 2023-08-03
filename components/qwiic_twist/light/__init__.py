import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome.const import CONF_OUTPUT_ID, CONF_GAMMA_CORRECT
from .. import qwiic_twist_ns, QwiicTwist, CONF_QWIIC_TWIST

CONF_RED_MIN   = "red_min"
CONF_RED_MAX   = "red_max"
CONF_GREEN_MIN = "green_min"
CONF_GREEN_MAX = "green_max"
CONF_BLUE_MIN  = "blue_min"
CONF_BLUE_MAX  = "blue_max"

QwiicTwistRGB = qwiic_twist_ns.class_("QwiicTwistRGB", light.LightOutput, cg.Component)

CONFIG_SCHEMA = light.RGB_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID):   cv.declare_id(QwiicTwistRGB),
        cv.GenerateID(CONF_QWIIC_TWIST): cv.use_id(QwiicTwist),
        cv.Optional(CONF_RED_MIN  ,     default=0.00): cv.zero_to_one_float,
        cv.Optional(CONF_RED_MAX  ,     default=0.80): cv.zero_to_one_float,
        cv.Optional(CONF_GREEN_MIN,     default=0.00): cv.zero_to_one_float,
        cv.Optional(CONF_GREEN_MAX,     default=1.00): cv.zero_to_one_float,
        cv.Optional(CONF_BLUE_MIN ,     default=0.00): cv.zero_to_one_float,
        cv.Optional(CONF_BLUE_MAX ,     default=0.95): cv.zero_to_one_float,
        cv.Optional(CONF_GAMMA_CORRECT, default=1.00): cv.positive_float,
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    await cg.register_component(var, config)
    await light.register_light(var, config)
    qwiic_twist = await cg.get_variable(config[CONF_QWIIC_TWIST])
    cg.add(var.set_parent(qwiic_twist))
    
    if CONF_RED_MIN   in config:
        cg.add(var.set_red_min(config[CONF_RED_MIN]))
    if CONF_RED_MAX   in config:
        cg.add(var.set_red_max(config[CONF_RED_MAX]))
    if CONF_GREEN_MIN in config:
        cg.add(var.set_green_min(config[CONF_GREEN_MIN]))
    if CONF_GREEN_MAX in config:
        cg.add(var.set_green_max(config[CONF_GREEN_MAX]))
    if CONF_BLUE_MIN  in config:
        cg.add(var.set_blue_min(config[CONF_BLUE_MIN]))
    if CONF_BLUE_MAX  in config:
        cg.add(var.set_blue_max(config[CONF_BLUE_MAX]))
