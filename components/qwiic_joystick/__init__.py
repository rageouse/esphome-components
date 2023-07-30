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

CONF_QWIIC_JOYSTICK = 'qwiic_joystick'

CONF_BUTTON                  = 'button'

CONF_X_AXIS_RAW              = 'x_axis_raw'
CONF_X_AXIS_RAW_CENTERED     = 'x_axis_raw_centered'
CONF_X_AXIS_8_BIT            = 'x_axis_8_bit'
CONF_X_AXIS_8_BIT_CENTERED   = 'x_axis_8_bit_centered'
CONF_X_AXIS_PERCENT          = 'x_axis_percent'
CONF_X_AXIS_PERCENT_CENTERED = 'x_axis_percent_centered'

CONF_Y_AXIS_RAW              = 'y_axis_raw'
CONF_Y_AXIS_RAW_CENTERED     = 'y_axis_raw_centered'
CONF_Y_AXIS_8_BIT            = 'y_axis_8_bit'
CONF_Y_AXIS_8_BIT_CENTERED   = 'y_axis_8_bit_centered'
CONF_Y_AXIS_PERCENT          = 'y_axis_percent'
CONF_Y_AXIS_PERCENT_CENTERED = 'y_axis_percent_centered'

CONF_RADIUS_SQUARED_RAW      = 'radius_squared_raw'
CONF_RADIUS_RAW              = 'radius_raw'
CONF_RADIUS_SQUARED_PERCENT  = 'radius_squared_percent'
CONF_RADIUS_PERCENT          = 'radius_percent'
CONF_THETA_DEGREES           = 'theta_degrees'
CONF_THETA_RADIANS           = 'theta_radians'

ICON_RADIUS                  = 'mdi:radius-outline'
ICON_ANGLE                   = 'mdi:angle-acute'
ICON_BUTTON                  = 'mdi:radiobox-marked'

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
        
        cv.Optional(CONF_X_AXIS_RAW):              unitless_axis_schema('x'),
        cv.Optional(CONF_X_AXIS_RAW_CENTERED):     unitless_axis_schema('x'),
        cv.Optional(CONF_X_AXIS_8_BIT):            unitless_axis_schema('x'),
        cv.Optional(CONF_X_AXIS_8_BIT_CENTERED):   unitless_axis_schema('x'),
        cv.Optional(CONF_X_AXIS_PERCENT):           percent_axis_schema('x'),
        cv.Optional(CONF_X_AXIS_PERCENT_CENTERED):  percent_axis_schema('x'),
        
        cv.Optional(CONF_Y_AXIS_RAW):              unitless_axis_schema('y'),
        cv.Optional(CONF_Y_AXIS_RAW_CENTERED):     unitless_axis_schema('y'),
        cv.Optional(CONF_Y_AXIS_8_BIT):            unitless_axis_schema('y'),
        cv.Optional(CONF_Y_AXIS_8_BIT_CENTERED):   unitless_axis_schema('y'),
        cv.Optional(CONF_Y_AXIS_PERCENT):           percent_axis_schema('y'),
        cv.Optional(CONF_Y_AXIS_PERCENT_CENTERED):  percent_axis_schema('y'),
        
        cv.Optional(CONF_RADIUS_SQUARED_RAW):      radius_unitless_schema,
        cv.Optional(CONF_RADIUS_RAW):              radius_unitless_schema,
        cv.Optional(CONF_RADIUS_SQUARED_PERCENT):  radius_percent_schema,
        cv.Optional(CONF_RADIUS_PERCENT):          radius_percent_schema,
        
        cv.Optional(CONF_THETA_DEGREES):           angle_radians_schema,
        cv.Optional(CONF_THETA_RADIANS):           angle_degrees_schema,
    }
).extend(cv.polling_component_schema('30ms')).extend(i2c.i2c_device_schema(0x20))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_QWIIC_JOYSTICK])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    
    if CONF_BUTTON in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_BUTTON])
        cg.add(var.set_button_sensor(sens))
    
    if CONF_X_AXIS_RAW in config:
        sens = await sensor.new_sensor(config[CONF_X_AXIS_RAW])
        cg.add(var.set_x_axis_raw_sensor(sens))
    
    if CONF_X_AXIS_RAW_CENTERED in config:
        sens = await sensor.new_sensor(config[CONF_X_AXIS_RAW_CENTERED])
        cg.add(var.set_x_axis_raw_centered_sensor(sens))
    
    if CONF_X_AXIS_8_BIT in config:
        sens = await sensor.new_sensor(config[CONF_X_AXIS_8_BIT])
        cg.add(var.set_x_axis_8_bit_sensor(sens))
    
    if CONF_X_AXIS_8_BIT_CENTERED in config:
        sens = await sensor.new_sensor(config[CONF_X_AXIS_8_BIT_CENTERED])
        cg.add(var.set_x_axis_8_bit_centered_sensor(sens))
    
    if CONF_X_AXIS_PERCENT in config:
        sens = await sensor.new_sensor(config[CONF_X_AXIS_PERCENT])
        cg.add(var.set_x_axis_percent_sensor(sens))
    
    if CONF_X_AXIS_PERCENT_CENTERED in config:
        sens = await sensor.new_sensor(config[CONF_X_AXIS_PERCENT_CENTERED])
        cg.add(var.set_x_axis_percent_centered_sensor(sens))
    
    
    if CONF_Y_AXIS_RAW in config:
        sens = await sensor.new_sensor(config[CONF_Y_AXIS_RAW])
        cg.add(var.set_y_axis_raw_sensor(sens))
    
    if CONF_Y_AXIS_RAW_CENTERED in config:
        sens = await sensor.new_sensor(config[CONF_Y_AXIS_RAW_CENTERED])
        cg.add(var.set_y_axis_raw_centered_sensor(sens))
    
    if CONF_Y_AXIS_8_BIT in config:
        sens = await sensor.new_sensor(config[CONF_Y_AXIS_8_BIT])
        cg.add(var.set_y_axis_8_bit_sensor(sens))
    
    if CONF_Y_AXIS_8_BIT_CENTERED in config:
        sens = await sensor.new_sensor(config[CONF_Y_AXIS_8_BIT_CENTERED])
        cg.add(var.set_y_axis_8_bit_centered_sensor(sens))
    
    if CONF_Y_AXIS_PERCENT in config:
        sens = await sensor.new_sensor(config[CONF_Y_AXIS_PERCENT])
        cg.add(var.set_y_axis_percent_sensor(sens))
    
    if CONF_Y_AXIS_PERCENT_CENTERED in config:
        sens = await sensor.new_sensor(config[CONF_Y_AXIS_PERCENT_CENTERED])
        cg.add(var.set_y_axis_percent_centered_sensor(sens))
    
    
    if CONF_RADIUS_SQUARED_RAW in config:
        sens = await sensor.new_sensor(config[CONF_RADIUS_SQUARED_RAW])
        cg.add(var.set_radius_squared_raw_sensor(sens))
    
    if CONF_RADIUS_RAW in config:
        sens = await sensor.new_sensor(config[CONF_RADIUS_RAW])
        cg.add(var.set_radius_raw_sensor(sens))
    
    if CONF_RADIUS_SQUARED_PERCENT in config:
        sens = await sensor.new_sensor(config[CONF_RADIUS_SQUARED_PERCENT])
        cg.add(var.set_radius_squared_percent_sensor(sens))
    
    if CONF_RADIUS_PERCENT in config:
        sens = await sensor.new_sensor(config[CONF_RADIUS_PERCENT])
        cg.add(var.set_radius_percent_sensor(sens))
    

    if CONF_THETA_DEGREES in config:
        sens = await sensor.new_sensor(config[CONF_THETA_DEGREES])
        cg.add(var.set_theta_degrees_sensor(sens))
    
    if CONF_THETA_RADIANS in config:
        sens = await sensor.new_sensor(config[CONF_THETA_RADIANS])
        cg.add(var.set_theta_radians_sensor(sens))
    
