import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ICON, ICON_KEY_PLUS
from . import CONF_FINGERPRINT_CHHAVI_ID, FingerprintChhaviComponent
from esphome.const import (
    CONF_LIGHT,
    CONF_MOTION,
    CONF_TIMEOUT,
    DEVICE_CLASS_LIGHT,
    DEVICE_CLASS_MOTION,
    CONF_ID,
)
from esphome.core import TimePeriod
DEPENDENCIES = ["fingerprint_chhavi"]
CONF_BUTTON = "button"

CONFIG_SCHEMA = binary_sensor.binary_sensor_schema().extend(
    {
        cv.GenerateID(CONF_FINGERPRINT_CHHAVI_ID): cv.use_id(FingerprintChhaviComponent),
        cv.Optional(CONF_ICON, default=ICON_KEY_PLUS): cv.icon,
        cv.Optional(CONF_BUTTON): binary_sensor.binary_sensor_schema().extend(
            {
                cv.Optional(CONF_TIMEOUT, default="200ms"): cv.All(
                    cv.positive_time_period_milliseconds,
                    cv.Range(max=TimePeriod(milliseconds=65535)),
                ),
            }
        ),
        cv.Optional(CONF_BUTTON): binary_sensor.binary_sensor_schema().extend(
            {
                cv.Optional(CONF_TIMEOUT, default="200ms"): cv.All(
                    cv.positive_time_period_milliseconds,
                    cv.Range(max=TimePeriod(milliseconds=65535)),
                ),
            }
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_FINGERPRINT_CHHAVI_ID])
    var = await binary_sensor.new_binary_sensor(config)
    cg.add(hub.set_enrolling_binary_sensor(var))

    if CONF_BUTTON in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_BUTTON])
        cg.add(parent.set_button(sens))
        cg.add(parent.set_button_timeout(config[CONF_BUTTON][CONF_TIMEOUT]))