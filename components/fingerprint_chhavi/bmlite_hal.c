//  #define DEBUG_COMM 0
#ifdef DEBUG_COMM
#include <stdio.h>
#define LOG_DEBUG(...) printf(__VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif
#include "esp_log.h"
#include "esp_attr.h"
#include <stddef.h>
#include <string.h>
#include "esp_err.h"
 #include "driver/rmt.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include <stdlib.h>
#include "fpc_bep_types.h"
#include "platform.h"
#include "bmlite_hal.h"
#include "hcp_tiny.h"
#include "bmlite_if.h"
#include "bmlite_if_callbacks.h"
// #include "fingerprint_chhavi.h"
//spi_device_handle_t spi_test;
//extern spi_device_handle_t SPI();
//extern spi_device_handle_t spi_test;
//tatic const char *const TAG = "fingerprint_chhavi";
extern spi_device_handle_t spi_test;
#define exit_if_err(c) { bep_result = c; if(bep_result || chain->bep_result) goto exit; }
#define FPC_HOST    SPI2_HOST
#define DMA_CHAN    SPI_DMA_CH_AUTO  //FPC_HOST
#define CONFIG_FPC_IRQ_GPIO 2
#define CONFIG_FPC_CS_GPIO 15
#define CONFIG_FPC_RST_GPIO 0
#define DATA_BUFFER_SIZE (1024*5)
static uint8_t hcp_txrx_buffer[MTU];
static uint8_t hcp_data_buffer[DATA_BUFFER_SIZE];
static char version[100];
static uint16_t template_id;
static uint32_t current_id = 10;
   // static bool match;
//#define CONFIG_FPC_RSTN_GPIO 15
//extern void *getptr;
void hal_bmlite_reset(bool state)
{
    gpio_set_level(CONFIG_FPC_RST_GPIO, state);
    return;
}

/*

 * @brief SPI write-read
 * @param[in] Write buffer
 * @param[in] Read buffer
 * @param[in] Size
 * @param[in] Leave CS asserted
 * @return ::fpc_bep_result_t
 */

/**
 * @brief Set LED(s) status
 * @param[in] Status
 * @param[in] Status modifier
 */
// void hal_set_leds(platform_led_status_t status, uint16_t mode)
// {
//     ESP_LOGI(TAG, "-> FPC - Set LED with Status:%u | Mode:%u", status, mode);
// }

fpc_bep_result_t hal_bmlite_spi_write_read(uint8_t *write, uint8_t *read, size_t size, bool leave_cs_asserted)
{
    static fpc_bep_result_t res;
//    LOG_DEBUG("Inside func spi_write_read");
//    spi_bus_c_t spi = spi_init();
 //    spi_transaction_t t;
 //    static fpc_bep_result_t res;
  //  spi_device_handle_t spi_t;
//spi_t = SPI();
 //    spi = spi_test;
    esp_err_t ret;
    spi_transaction_t t;
//   //  platform_init(NULL);

//     spi_bus_config_t buscfg = {
//     .mosi_io_num = 13,
//     .miso_io_num = 12,
//     .sclk_io_num = 14,
//     .quadwp_io_num = -1,
//     .quadhd_io_num = -1,
//     .data4_io_num = -1,
//     .data5_io_num = -1,
//     .data6_io_num = -1,
//     .data7_io_num = -1,
//     .max_transfer_sz = 1000,
// };
   
//      spi_device_interface_config_t devcfg = {};
//         // Honestly, no clue what _CLOCK_SPEED is, so just use our own
//         devcfg.clock_speed_hz = 10*10000 * 1000;
//         devcfg.spics_io_num  = -1,
//         devcfg.mode = 0; //SPI mode 0
//         devcfg.spics_io_num = -1; //CS pin
//         devcfg.queue_size = 7; //Not sure if needed


    
//     ret = spi_bus_initialize(FPC_HOST, &buscfg, DMA_CHAN);
//    ESP_ERROR_CHECK(ret);
//     ret = spi_bus_add_device(FPC_HOST, &devcfg, &spi);
//      ESP_ERROR_CHECK(ret);
// //   //  platform_init(NULL);
//    // esp_err_t ret;
    
   if (size==0) return 0;          //no need to send anything
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    
    gpio_set_level(CONFIG_FPC_CS_GPIO, 0);

    t.length=size*8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer=write;               //Data
    t.user=(void*)1;                //D/C needs to be set to 1
    //t.tx_data = write;
    t.rx_buffer=read;
    ret=spi_device_polling_transmit(spi_test , &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.

 //   LOG_DEBUG(" SPI return Code: %i", ret);

    if (!leave_cs_asserted){
        gpio_set_level(CONFIG_FPC_CS_GPIO, 1);
    }
  // LOG_DEBUG("Inside func");
    return res;
}

bool hal_bmlite_get_status(void)
{
    //int irq_status = gpio_get_level(CONFIG_FPC_IRQ_GPIO);
    //ESP_LOGI(TAG, "-> FPC - BM-Lite get IRQ status | IRQ: %i", irq_status);
    return gpio_get_level(GPIO_NUM_2); //irq_status;
}

/**
 * @brief Reads the system tick counter.
 *
 * @return Tick count since hal_timebase_init() call. [ms]
 */
hal_tick_t hal_timebase_get_tick(void)
{
    
    int tickCount = pdTICKS_TO_MS(xTaskGetTickCount());
    //ESP_LOGI(TAG, "-> FPC - Timebase get tickcount. %i", tickCount);
   
    return tickCount; //xTaskGetTickCount();
}

/**
 * @brief Busy wait.
 *
 * @param[in] ms  Time to wait [ms].
 * 0 => return immediately
 * 1 => wait at least 1ms etc.
 */
void hal_timebase_busy_wait(uint32_t ms)
{
    //ESP_LOGI(TAG, "-> FPC - Timebase busy wait. | Delay: %u", ms);
    vTaskDelay(pdMS_TO_TICKS(ms));
}

/**
 *  Optional functions for Buttons & Leds control
 */

/**
 * @brief Get button press time (msec)
 * 
 * @return ::uint32_t
 */
uint32_t hal_get_button_press_time(void){
    //ESP_LOGI(TAG, "-> FPC - Get button press time.");
    return 0;
}

/**
 * @brief Check if button was pressed and released.
 *
 * @return Button press time in milli seconds.
 */
uint32_t hal_check_button_pressed(void)
{
    //ESP_LOGI(TAG, "-> FPC - Check button pressed.");
    return 0;
}

void bmlite_on_start_enroll()
{
    LOG_DEBUG("-> FPC - Enroll Started.");
    // if (Serial) Serial.println(">> Start Enroll!");
    // u8g2log.print("\f");  //Clear the Screen log
    // u8g2log.println("> START ENROLLING,");
    // delay(100);
}

/**
 * Callback when the enrolling is done.
 */
void bmlite_on_finish_enroll(void)
{
   LOG_DEBUG( "-> FPC - Enroll Finished.");

    // u8g2log.println("> ENROLL FINISH .. OK");
    // //u8g2log.println("> FPC BM-Lite ____ OK");
    // // set the effect to play
    // haptic.setWaveform(0, 76);  // play effect 
    // haptic.setWaveform(1, 0);       // end waveform
    // // play the effect!
    // haptic.go();

    // //Notify the Base unit
    // if (Bluefruit.connected()){
    //       bleuart.printf("#03010100\r\n");
    // }
    // delay(100);
}

/**
 * Callback invoked when Enroll capture has started
 */
void bmlite_on_start_enrollcapture(void)
{
    LOG_DEBUG("-> FPC - Enrollcapture Started.");
    // if (Serial) Serial.println(">> Start Enroll Capture!");
    // u8g2log.println("> PLACE YOUR FINGER..");
    // // set the effect to play
    // haptic.setWaveform(0, 42);  // play effect 
    // haptic.setWaveform(1, 0);       // end waveform
    // // play the effect!
    // haptic.go();
    // delay(100);
}

/**
 * Callback invoked when a connection is dropped
 * @param capCount gives the number of captures completed! Usually it takes 3 captures the most.
 */
void bmlite_on_finish_enrollcapture()
{
   LOG_DEBUG("-> FPC - Enrollcapture Finished.");
    // Serial.println(">> Finish Enroll Capture!");

    // u8g2log.print("\f");  //Clear the Screen log
    // u8g2log.printf("> CAPTURE %d   .... OK\n\r", (capCount+1));
    
    // // set the effect to play
    // haptic.setWaveform(0, 52);  // play effect 
    // haptic.setWaveform(1, 0);       // end waveform
    //   // play the effect!
    // haptic.go();
    // delay(100);
}

/**
 * Callback invoked when the Finger identification has started.
 */
void bmlite_on_identify_start(void)
{
   LOG_DEBUG( "-> FPC - Identification Started.");
    // if (Serial) Serial.println(">> Start Identify!");
    
    // u8g2log.print("\f");  //Clear the Screen log
    // u8g2log.println("> IDENTIFY,");
    // u8g2log.println("> PLACE YOUR FINGER..");
    // delay(100);
}

/**
 * Callback invoked when the finger identification process has finished
 */
void bmlite_on_identify_finish(void)
{
    LOG_DEBUG( "-> FPC - Identification Finished.");
    // if (Serial) Serial.println(">> Finished Identify!");
        
    // //u8g2log.print("\f");  //Clear the Screen log
    // u8g2log.println("> IDENTIFY FINISH!");
    // delay(100);
}

/**
 * Callback invoked when an error has encountered during the finger reading
 * @param error shows the error code related to the commands which can be found in the 'bmlite_if_callbacks.h'
 * @param value is a descriptive reason code which is defined in 'fpc_bep_types.h'
 */
void bmlite_on_error(bmlite_error_t error, int32_t value)
{
    LOG_DEBUG( "-> FPC - Error No. %i | Code: %i", error, value);
    // if (value != FPC_BEP_RESULT_TIMEOUT) {
    //     //hal_set_leds(BMLITE_LED_STATUS_ERROR, false);
    //     //u8g2log.printf("> FPC ERROR: %d | %d\n\r", error, value);
    // } else {
    //     // Timeout - not really an error here
    //     //hal_set_leds(BMLITE_LED_STATUS_ERROR, true);
    //     //u8g2log.println("> FPC TIMEOUT ERROR!");
    // }
}