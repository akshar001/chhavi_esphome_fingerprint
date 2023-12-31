import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_CAPACITY,
    CONF_FINGERPRINT_COUNT,
    CONF_LAST_CONFIDENCE,
    CONF_LAST_FINGER_ID,
    CONF_SECURITY_LEVEL,
    CONF_STATUS,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_ACCOUNT,
    ICON_ACCOUNT_CHECK,
    ICON_DATABASE,
    ICON_FINGERPRINT,
    ICON_SECURITY,
)
from . import CONF_FINGERPRINT_CHHAVI_ID, FingerprintChhaviComponent
from . import CONF_VERSION
from . import CONF_ON_FINGER_SCAN_MATCHED, CONF_ON_DELETE, CONF_ON_TEXT

DEPENDENCIES = ["fingerprint_chhavi"]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_FINGERPRINT_CHHAVI_ID): cv.use_id(FingerprintChhaviComponent),
        cv.Optional(CONF_FINGERPRINT_COUNT): sensor.sensor_schema(
            icon=ICON_FINGERPRINT,
            accuracy_decimals=0,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_VERSION): sensor.sensor_schema(
            icon=ICON_FINGERPRINT,
            accuracy_decimals=1,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_ON_DELETE): sensor.sensor_schema(
            icon=ICON_DATABASE,
            accuracy_decimals=0,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_ON_TEXT): sensor.sensor_schema(
            icon=ICON_DATABASE,
            accuracy_decimals=0,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_ON_FINGER_SCAN_MATCHED): sensor.sensor_schema(
            icon=ICON_FINGERPRINT,
            accuracy_decimals=0,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_STATUS): sensor.sensor_schema(
            accuracy_decimals=0,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_CAPACITY): sensor.sensor_schema(
            icon=ICON_DATABASE,
            accuracy_decimals=0,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_SECURITY_LEVEL): sensor.sensor_schema(
            icon=ICON_SECURITY,
            accuracy_decimals=0,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_LAST_FINGER_ID): sensor.sensor_schema(
            icon=ICON_ACCOUNT,
            accuracy_decimals=0,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_LAST_CONFIDENCE): sensor.sensor_schema(
            icon=ICON_ACCOUNT_CHECK,
            accuracy_decimals=0,
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_FINGERPRINT_CHHAVI_ID])

    for key in [
        CONF_FINGERPRINT_COUNT,
        CONF_STATUS,
        CONF_CAPACITY,
        CONF_SECURITY_LEVEL,
        CONF_LAST_FINGER_ID,
        CONF_LAST_CONFIDENCE,
        CONF_VERSION,
        CONF_ON_FINGER_SCAN_MATCHED,
        CONF_ON_DELETE,
        CONF_ON_TEXT
    ]:
        if key not in config:
            continue
        conf = config[key]
        sens = await sensor.new_sensor(conf)
        cg.add(getattr(hub, f"set_{key}_sensor")(sens))
