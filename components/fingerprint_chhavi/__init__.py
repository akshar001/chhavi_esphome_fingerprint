import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome import pins
from esphome.components import spi
from esphome.components.esp32 import add_idf_sdkconfig_option
from esphome.const import (
    #CONF_COLOR,
    #CONF_COUNT,
    #CONF_FINGER_ID,
    CONF_ID,
    #CONF_NEW_PASSWORD,
    #CONF_NUM_SCANS,
    #CONF_ON_ENROLLMENT_FAILED,
    CONF_ON_ENROLLMENT_SCAN,
    CONF_ON_FINGER_SCAN_MATCHED,
    CONF_CLK_PIN,
    CONF_MISO_PIN,
    CONF_MOSI_PIN,
    CONF_CS_PIN,
  #  CONF_INTERRUPT_PIN, should be available in the next release
    CONF_RESET_PIN,
    #CONF_ON_FINGER_SCAN_UNMATCHED,
    #CONF_DELETE,
    #CONF_PASSWORD,
    CONF_SENSING_PIN,
    #CONF_SPEED,
    #CONF_STATE,
    CONF_TRIGGER_ID,
)
#from fingerprint_chhavi import HCP_comm_t
#ODEOWNERS = ["@OnFreund", "@loongyh"]
DEPENDENCIES = ["spi"]
AUTO_LOAD = ["binary_sensor", "sensor","text_sensor","button"]
MULTI_CONF = True
#CONF_FORCE_SW = "force_sw"
# class HCP_comm_t:
#     def __init__(self, value):
#         self.value = value

# class bool:
#     def __init__(self, value):
#         self.value = value

CONF_FINGERPRINT_CHHAVI_ID = "fingerprint_chhavi_id"
CONF_IRQ_PIN = "interrupt_pin"
CONF_CLOCK_SPEED = "clock_speed"  # spi clock speed
CONFIG_TX_GPIO_NUM = 21
CONFIG_RX_GPIO_NUM = 22

CONFIG_FPC_RST_GPIO = 0
# class HCP_comm_t:
#     def __init__(self):
#         self.write = None  # Placeholder for function pointer
#         self.read = None   # Placeholder for function pointer
#         self.phy_rx_timeout = 0
#         self.pkt_buffer = None
#         self.pkt_size_max = 0
#         self.pkt_size = 0
#         self.txrx_buffer = None
#         self.arg = None     # You need to define HCP_arg_t similarly
#         self.bep_result = None  # Define fpc_bep_result_t if not already defined

# Create an instance of the class
# hcp_comm_instance = HCP_comm_t()
CONF_FINGERPRINT_CHHAVI_ID = "fingerprint_chhavi_id"
CONF_ON_DELETE = "Delete_All"
CONF_VERSION = "Chhavi_version"
CONF_ON_WAIT = "wait"
spi_ns = cg.esphome_ns.namespace("spi")
SPIComponent = spi_ns.class_("SPIComponent", cg.Component)
SPIDevice = spi_ns.class_("SPIDevice")
SPIDataRate = spi_ns.enum("SPIDataRate")

fingerprint_chhavi_ns = cg.esphome_ns.namespace("fingerprint_chhavi")

FingerprintChhaviComponent = fingerprint_chhavi_ns.class_(
    "FingerprintChhaviComponent", cg.PollingComponent
)

FingerScanMatchedTrigger = fingerprint_chhavi_ns.class_(
    "FingerScanMatchedTrigger", automation.Trigger.template()
)

EnrollmentTrigger = fingerprint_chhavi_ns.class_(
    "EnrollmentTrigger", automation.Trigger.template()
)

VersionTrigger = fingerprint_chhavi_ns.class_(
    "VersionTrigger", automation.Trigger.template()
)

DeleteTrigger = fingerprint_chhavi_ns.class_(
    "DeleteTrigger", automation.Trigger.template()
)

WaitTrigger = fingerprint_chhavi_ns.class_(
    "WaitTrigger", automation.Trigger.template()
)


EnrollmentAction = fingerprint_chhavi_ns.class_("EnrollmentAction", automation.Action)
DeleteAction = fingerprint_chhavi_ns.class_("DeleteAction", automation.Action)
EnrollmentAction = fingerprint_chhavi_ns.class_("EnrollmentAction", automation.Action)
VersionAction = fingerprint_chhavi_ns.class_("VersionAction", automation.Action)
FingerScanMatchAction = fingerprint_chhavi_ns.class_("FingerScanMatchAction", automation.Action)
WaitAction = fingerprint_chhavi_ns.class_("WaitAction", automation.Action)
# FingerScanUnmatchedTrigger = fingerprint_chhavi_ns.class_(
#     "FingerScanUnmatchedTrigger", automation.Trigger.template()
# )
# EnrollmentDoneTrigger = fingerprint_chhavi_ns.class_(
#     "EnrollmentDoneTrigger", automation.Trigger.template(cg.uint16)
# )

# EnrollmentFailedTrigger = fingerprint_chhavi_ns.class_(
#     "EnrollmentFailedTrigger", automation.Trigger.template(cg.uint16)
# )# LEDControlAction = fingerprint_chhavi_ns.class_("LEDControlAction", automation.Action)
# AuraLEDControlAction = fingerprint_chhavi_ns.class_(
#     "AuraLEDControlAction", automation.Action
# )
CancelEnrollmentAction = fingerprint_chhavi_ns.class_(
    "CancelEnrollmentAction", automation.Action
)
# AuraLEDState = fingerprint_chhavi_ns.enum("GrowAuraLEDState", True)
# AURA_LED_STATES = {
#     "BREATHING": AuraLEDState.BREATHING,
#     "FLASHING": AuraLEDState.FLASHING,
#     "ALWAYS_ON": AuraLEDState.ALWAYS_ON,
#     "ALWAYS_OFF": AuraLEDState.ALWAYS_OFF,
#     "GRADUAL_ON": AuraLEDState.GRADUAL_ON,
#     "GRADUAL_OFF": AuraLEDState.GRADUAL_OFF,
# }
# validate_aura_led_states = cv.enum(AURA_LED_STATES, upper=True)
# AuraLEDColor = fingerprint_chhavi_ns.enum("GrowAuraLEDColor", True)
# AURA_LED_COLORS = {
#     "RED": AuraLEDColor.RED,
#     "BLUE": AuraLEDColor.BLUE,
#     "PURPLE": AuraLEDColor.PURPLE,
#     "GREEN": AuraLEDColor.GREEN,
#     "YELLOW": AuraLEDColor.YELLOW,
#     "CYAN": AuraLEDColor.CYAN,
#     "WHITE": AuraLEDColor.WHITE,
# }
# validate_aura_led_colors = cv.enum(AURA_LED_COLORS, upper=True)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(FingerprintChhaviComponent),
            cv.Optional(CONF_SENSING_PIN): pins.gpio_input_pin_schema,
            cv.Optional(CONF_CLK_PIN): pins.internal_gpio_output_pin_number,
            cv.Optional(CONF_MISO_PIN): pins.internal_gpio_input_pin_number,
            cv.Optional(CONF_MOSI_PIN): pins.internal_gpio_output_pin_number,
        #    cv.Optional(CONF_RESET_PIN): pins.internal_gpio_output_pin_number,
         #   cv.Optional(CONF_FORCE_SW, default=False): cv.boolean,
            cv.Required(CONF_CS_PIN): pins.internal_gpio_output_pin_number,
       #     cv.Optional(CONF_IRQ_PIN): pins.gpio_input_pin_schema,
            # default internally to -1 if not set (means disabled)
       #     cv.Optional(CONF_RESET_PIN): pins.internal_gpio_output_pin_number,
            # W5500 should operate stable up to 33.3 according to the datasheet.
            cv.Optional(CONF_CLOCK_SPEED, default=10): cv.int_range(1, 80), 
            
            #cv.Optional(CONF_PASSWORD): cv.uint32_t,
            #cv.Optional(CONF_NEW_PASSWORD): cv.uint32_t,
            cv.Optional(CONF_ON_FINGER_SCAN_MATCHED): automation.validate_automation(
                {
                    cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                    FingerScanMatchedTrigger
                    ),
                }
            ),
            # cv.Optional(CONF_ON_FINGER_SCAN_UNMATCHED): automation.validate_automation(
            #     {
            #         cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
            #             FingerScanUnmatchedTrigger
            #         ),
            #     }
            # ),
            cv.Optional(CONF_ON_ENROLLMENT_SCAN): automation.validate_automation(
                {
                    cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                        EnrollmentTrigger
                    ),
                }
            ),
            cv.Optional(CONF_ON_DELETE): automation.validate_automation(
                {
                    cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                        DeleteTrigger
                    ),
                }
            ),
             cv.Optional(CONF_VERSION): automation.validate_automation(
                {
                    cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                        VersionTrigger
                    ),
                }
            ),
            cv.Optional(CONF_ON_WAIT): automation.validate_automation(
                {
                    cv.GenerateID(CONF_TRIGGER_ID): cv.declare_id(
                        WaitTrigger
                    ),
                }
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
  #  .extend(cv.polling_component_schema("500ms"))
    .extend(spi.spi_device_schema(cs_pin_required=True))
)


async def to_code(config):
    cg.add_global(spi_ns.using)
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
   # cg.add(var.set_force_sw(config[CONF_FORCE_SW]))
    if CONF_CLK_PIN in config:
        clk = await cg.gpio_pin_expression(config[CONF_CLK_PIN])
        cg.add(var.set_clk(clk))
    if CONF_MISO_PIN in config:
        miso = await cg.gpio_pin_expression(config[CONF_MISO_PIN])
        cg.add(var.set_miso(miso))
    if CONF_MOSI_PIN in config:
        mosi = await cg.gpio_pin_expression(config[CONF_MOSI_PIN])
        cg.add(var.set_mosi(mosi))
    # cg.add(var.set_clk_pin(config[CONF_CLK_PIN]))
    # cg.add(var.set_miso_pin(config[CONF_MISO_PIN]))
    # cg.add(var.set_mosi_pin(config[CONF_MOSI_PIN]))
    # cg.add(var.set_cs_pin(config[CONF_CS_PIN]))
 #   cg.add(var.set_interrupt_pin(config[CONF_INTERRUPT_PIN]))
    

    # cg.add(var.set_clock_speed(config[CONF_CLOCK_SPEED]))

    # if CONF_PASSWORD in config:
    #     password = config[CONF_PASSWORD]
    #     cg.add(var.set_password(password))
    #  await spi.register_spi_device(var, config)
    #  cg.add(var.set_spi(spi_device))
    # spi_device = await spi.register_spi_device(var, config)
    # cg.add(var.set_spi(spi_device))

    # if CONF_NEW_PASSWORD in config:
    #     new_password = config[CONF_NEW_PASSWORD]
    #     cg.add(var.set_new_password(new_password))

    if CONF_SENSING_PIN in config:
        sensing_pin = await cg.gpio_pin_expression(config[CONF_SENSING_PIN])
        cg.add(var.set_sensing_pin(sensing_pin))

    add_idf_sdkconfig_option("CONFIG_SPI_MASTER_ISR_IN_IRAM", True)
    add_idf_sdkconfig_option("CONFIG_SPI_SLAVE_ISR_IN_IRAM", True)
    #add_idf_sdkconfig_option("CONFIG_ESP_TASK_WDT", True)
    # for conf in config.get(CONF_ON_FINGER_SCAN_MATCHED, []):
    #     trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
    #     await automation.build_automation(
    #         trigger, [], conf
    #     )

    # # for conf in config.get(CONF_ON_FINGER_SCAN_UNMATCHED, []):
    # #     trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
    # #     await automation.build_automation(trigger, [], conf)

    # for conf in config.get(CONF_ON_ENROLLMENT_SCAN, []):
    #     trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
    #     await automation.build_automation(
    #         trigger, [], conf
    #     )

    for conf in config.get(CONF_VERSION, []):
        trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
        await automation.build_automation(trigger, [], conf)

    # for conf in config.get(CONF_ON_ENROLLMENT_FAILED, []):
    #     trigger = cg.new_Pvariable(conf[CONF_TRIGGER_ID], var)
    #     await automation.build_automation(trigger, [(cg.uint16, "finger_id")], conf)

def spi_device_schema(cs_pin_required=True):
    """Create a schema for an SPI device.
    :param cs_pin_required: If true, make the CS_PIN required in the config.
    :return: The SPI device schema, `extend` this in your config schema.
    """
    schema = {
        cv.GenerateID(CONF_SPI_ID): cv.use_id(SPIComponent),
    }
    if cs_pin_required:
        schema[cv.Required(CONF_CS_PIN)] = pins.internal_gpio_output_pin_number
    else:
        schema[cv.Optional(CONF_CS_PIN)] = pins.internal_gpio_output_pin_number
    return cv.Schema(schema)


async def register_spi_device(var, config):
    parent = await cg.get_variable(config[CONF_SPI_ID])
    cg.add(var.set_spi_parent(parent))
    if CONF_CS_PIN in config:
        pin = await cg.gpio_pin_expression(config[CONF_CS_PIN])
        cg.add(var.set_cs_pin(pin))


def final_validate_device_schema(name: str, *, require_mosi: bool, require_miso: bool):
    hub_schema = {}
    if require_miso:
        hub_schema[
            cv.Required(
                CONF_MISO_PIN,
                msg=f"Component {name} requires this spi bus to declare a miso_pin",
            )
        ] = cv.valid
    if require_mosi:
        hub_schema[
            cv.Required(
                CONF_MOSI_PIN,
                msg=f"Component {name} requires this spi bus to declare a mosi_pin",
            )
        ] = cv.valid

    return cv.Schema(
        {cv.Required(CONF_SPI_ID): fv.id_declaration_match_schema(hub_schema)},
        extra=cv.ALLOW_EXTRA,
    )

@automation.register_action(
    "fingerprint_chhavi.enroll",
    EnrollmentAction,
    cv.maybe_simple_value(
        {
            cv.GenerateID(): cv.use_id(FingerprintChhaviComponent),
        },
    ),
)

async def fingerprint_chhavi_enroll_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var
    # template_ = await cg.templatable(config[CONF_FINGER_ID], args, cg.uint16)
    # cg.add(var.set_finger_id(template_))
    # if CONF_NUM_SCANS in config:
    #     template_ = await cg.templatable(config[CONF_NUM_SCANS], args, cg.uint8)
    #     cg.add(var.set_num_scans(template_))
     


# @automation.register_action(
#     "fingerprint_chhavi.cancel_enroll",
#     CancelEnrollmentAction,
#     cv.Schema(
#         {
#             cv.GenerateID(): cv.use_id(FingerprintChhaviComponent),
#         }
#     ),
# )
# async def fingerprint_chhavi_cancel_enroll_to_code(config, action_id, template_arg, args):
#     var = cg.new_Pvariable(action_id, template_arg)
#     await cg.register_parented(var, config[CONF_ID])
#     return var

@automation.register_action(
    "fingerprint_chhavi_identify",
    FingerScanMatchAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(FingerprintChhaviComponent),
        }
    ),
)
async def fingerprint_chhavi_identify_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var

@automation.register_action(
    "fingerprint_chhavi_version",
    VersionAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(FingerprintChhaviComponent),
        }
    ),
)

async def fingerprint_chhavi_version_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var
# @automation.register_action(
#     "fingerprint_grow.delete",
#     DeleteAction,
#     cv.maybe_simple_value(
#         {
#             cv.GenerateID(): cv.use_id(FingerprintChhaviComponent),
#             cv.Required(CONF_FINGER_ID): cv.templatable(cv.uint16_t),
#         },
#         key=CONF_FINGER_ID,
#     ),
# )
# async def fingerprint_grow_delete_to_code(config, action_id, template_arg, args):
#     var = cg.new_Pvariable(action_id, template_arg)
#     await cg.register_parented(var, config[CONF_ID])

#     template_ = await cg.templatable(config[CONF_FINGER_ID], args, cg.uint16)
#     cg.add(var.set_finger_id(template_))
#     return var
@automation.register_action(
    "Chhavi_version",
    VersionAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(FingerprintChhaviComponent),
        }
    ),
)
async def version_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var

@automation.register_action(
    "fingerprint_chhavi_delete_all",
    DeleteAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(FingerprintChhaviComponent),
        }
    ),
)
async def fingerprint_chhavi_delete_all_to_code(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var

@automation.register_action(
    "fingerprint_chhavi_wait_for_finger",
    WaitAction,
    cv.Schema(
        {
            cv.GenerateID(): cv.use_id(FingerprintChhaviComponent),
        }
    ),
)
async def fingerprint_chhavi_wait_for_finger(config, action_id, template_arg, args):
    var = cg.new_Pvariable(action_id, template_arg)
    await cg.register_parented(var, config[CONF_ID])
    return var

# FINGERPRINT_GROW_LED_CONTROL_ACTION_SCHEMA = cv.maybe_simple_value(
#     {
#         cv.GenerateID(): cv.use_id(FingerprintChhaviComponent),
#         cv.Required(CONF_STATE): cv.templatable(cv.boolean),
#     },
#     key=CONF_STATE,
# )


# @automation.register_action(
#     "fingerprint_grow.led_control",
#     LEDControlAction,
#     FINGERPRINT_GROW_LED_CONTROL_ACTION_SCHEMA,
# )
# async def fingerprint_grow_led_control_to_code(config, action_id, template_arg, args):
#     var = cg.new_Pvariable(action_id, template_arg)
#     await cg.register_parented(var, config[CONF_ID])

#     template_ = await cg.templatable(config[CONF_STATE], args, cg.bool_)
#     cg.add(var.set_state(template_))
#     return var


# @automation.register_action(
#     "fingerprint_grow.aura_led_control",
#     AuraLEDControlAction,
#     cv.Schema(
#         {
#             cv.GenerateID(): cv.use_id(FingerprintChhaviComponent),
#             cv.Required(CONF_STATE): cv.templatable(validate_aura_led_states),
#             cv.Required(CONF_SPEED): cv.templatable(cv.uint8_t),
#             cv.Required(CONF_COLOR): cv.templatable(validate_aura_led_colors),
#             cv.Required(CONF_COUNT): cv.templatable(cv.uint8_t),
#         }
#     ),
# )
# async def fingerprint_grow_aura_led_control_to_code(
#     config, action_id, template_arg, args
# ):
#     var = cg.new_Pvariable(action_id, template_arg)
#     await cg.register_parented(var, config[CONF_ID])

#     for key in [CONF_STATE, CONF_SPEED, CONF_COLOR, CONF_COUNT]:
#         template_ = await cg.templatable(config[key], args, cg.uint8)
#         cg.add(getattr(var, f"set_{key}")(template_))
#     return var
