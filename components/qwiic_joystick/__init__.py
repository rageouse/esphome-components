import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, binary_sensor, i2c
from esphome.const import ICON_ACCELERATION, ICON_ACCELERATION_X, ICON_ACCELERATION_Y, ICON_ACCELERATION_Z
from esphome.const import UNIT_EMPTY, UNIT_PERCENT, UNIT_DEGREES, STATE_CLASS_MEASUREMENT

CODEOWNERS = ["@mcmanigle"]
MULTI_CONF = True

qwiic_joystick_ns = cg.esphome_ns.namespace("qwiic_joystick")
QwiicJoystick     = qwiic_joystick_ns.class_("QwiicJoystick", i2c.I2CDevice, cg.PollingComponent)

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["sensor"]

CONF_QWIIC_JOYSTICK  = 'qwiic_joystick'

CONF_BUTTON          = 'button'

CONF_X_AXIS          = 'x_axis'
CONF_X_AXIS_CENTERED = 'x_axis_centered'

CONF_Y_AXIS          = 'y_axis'
CONF_Y_AXIS_CENTERED = 'y_axis_centered'

CONF_RADIUS_SQUARED  = 'radius_squared'
CONF_THETA           = 'theta'

ICON_RADIUS          = 'mdi:radius-outline'
ICON_ANGLE           = 'mdi:angle-acute'
ICON_BUTTON          = 'mdi:radiobox-marked'

def unitless_axis_schema(axis=None):
    icon = ICON_ACCELERATION
    if axis == 'x': icon = ICON_ACCELERATION_X
    if axis == 'y': icon = ICON_ACCELERATION_Y
    if axis == 'z': icon = ICON_ACCELERATION_Z
    return sensor.sensor_schema(
        unit_of_measurement=UNIT_EMPTY,
        icon=icon,
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT,
    )

def percent_axis_schema(axis=None):
    icon = ICON_ACCELERATION
    if axis == 'x': icon = ICON_ACCELERATION_X
    if axis == 'y': icon = ICON_ACCELERATION_Y
    if axis == 'z': icon = ICON_ACCELERATION_Z
    return sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        icon=icon,
        accuracy_decimals=1,
        state_class=STATE_CLASS_MEASUREMENT,
    )

radius_unitless_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_EMPTY,
    icon=ICON_RADIUS,
    accuracy_decimals=0,
    state_class=STATE_CLASS_MEASUREMENT,
)

radius_percent_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_PERCENT,
    icon=ICON_RADIUS,
    accuracy_decimals=1,
    state_class=STATE_CLASS_MEASUREMENT,
)

angle_radians_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_EMPTY,
    icon=ICON_ANGLE,
    accuracy_decimals=2,
    state_class=STATE_CLASS_MEASUREMENT,
)

angle_degrees_schema = sensor.sensor_schema(
    unit_of_measurement=UNIT_DEGREES,
    icon=ICON_ANGLE,
    accuracy_decimals=0,
    state_class=STATE_CLASS_MEASUREMENT,
)

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_QWIIC_JOYSTICK): cv.declare_id(QwiicJoystick),
        
        cv.Optional(CONF_BUTTON): binary_sensor.binary_sensor_schema(icon=ICON_BUTTON),
        
        cv.Optional(CONF_X_AXIS):              unitless_axis_schema('x'),
        cv.Optional(CONF_X_AXIS_CENTERED):     unitless_axis_schema('x'),
        
        cv.Optional(CONF_Y_AXIS):              unitless_axis_schema('y'),
        cv.Optional(CONF_Y_AXIS_CENTERED):     unitless_axis_schema('y'),
        
        cv.Optional(CONF_RADIUS_SQUARED):      radius_unitless_schema,
        
        cv.Optional(CONF_THETA):           angle_radians_schema,
    }
).extend(cv.polling_component_schema('50ms')).extend(i2c.i2c_device_schema(0x20))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_QWIIC_JOYSTICK])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    
    if CONF_BUTTON in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_BUTTON])
        cg.add(var.set_button_sensor(sens))
    
    
    if CONF_X_AXIS in config:
        sens = await sensor.new_sensor(config[CONF_X_AXIS])
        cg.add(var.set_x_axis_sensor(sens))
    
    if CONF_X_AXIS_CENTERED in config:
        sens = await sensor.new_sensor(config[CONF_X_AXIS_CENTERED])
        cg.add(var.set_x_axis_centered_sensor(sens))
        
    
    if CONF_Y_AXIS in config:
        sens = await sensor.new_sensor(config[CONF_Y_AXIS])
        cg.add(var.set_y_axis_sensor(sens))
    
    if CONF_Y_AXIS_CENTERED in config:
        sens = await sensor.new_sensor(config[CONF_Y_AXIS_CENTERED])
        cg.add(var.set_y_axis_centered_sensor(sens))
        
    
    if CONF_RADIUS_SQUARED in config:
        sens = await sensor.new_sensor(config[CONF_RADIUS_SQUARED])
        cg.add(var.set_radius_squared_sensor(sens))
    

    if CONF_THETA in config:
        sens = await sensor.new_sensor(config[CONF_THETA])
        cg.add(var.set_theta_sensor(sens))
