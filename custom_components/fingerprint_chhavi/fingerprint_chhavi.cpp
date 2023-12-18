#include "fingerprint_chhavi.h"
#include "esphome/core/log.h"
#define DEBUG
#include "esphome/core/application.h"

#include <string.h>
#include <stdio.h>
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_system.h"
#include "esp32/ulp.h"

//files from SDK
extern "C"{

    #include "platform.h"
    #include "bmlite_hal.h"
    #include "fpc_bep_types.h"
    spi_device_handle_t spi_test;
}

//static char be_version[100];
static char version_bep[100];
static HCP_comm_t hcp_chain;

//variables for home assistant update
bool button_pressed = false;
bool scan_pressed = false;
bool delete_pressed = false;

//nvs storage for fingerprint count
nvs_handle_t my_handle;
static int32_t counter = 0; // value will default to 0, if not set yet in NVS

namespace esphome {
namespace fingerprint_chhavi {

static bool match;
static fpc_bep_result_t res;
#define exit_if_err(c) { bep_result = c; if(bep_result || chain->bep_result) goto exit; }
#define ENABLE_PULL_UP 1
#define DATA_BUFFER_SIZE (1024*5)

static uint8_t hcp_txrx_buffer[MTU];
static uint8_t hcp_data_buffer[DATA_BUFFER_SIZE];
static uint32_t current_id = 10;

static SemaphoreHandle_t BMLITE_IS_BUSY;
TaskHandle_t xHandle_bmlite = NULL;
const TickType_t xDelay = 10 * 1000 / portTICK_PERIOD_MS; 
const gpio_num_t PIN = GPIO_NUM_9;

//spi pins
#define CONFIG_FPC_IRQ_GPIO 2
#define CONFIG_FPC_CS_GPIO 15
#define CONFIG_FPC_RST_GPIO 0
#define FPC_HOST    SPI2_HOST
#define DMA_CHAN    SPI_DMA_CH_AUTO  //FPC_H
static uint16_t template_id = 1;

//#define RECEIVE_TIMEOUT 10
bool HIGH = true;
#ifdef DEBUG
#define log_debug(format, ...) WRITE(format, ##__VA_ARGS__)
#else
#define log_debug(format, ...)
#endif
#define log_info(format, ...) WRITE(format, ##__VA_ARGS__)
#define log_error(format, ...) WRITE(format, ##__VA_ARGS__)
/** Maximum attempts for capture image */
static const uint8_t MAX_CAPTURE_ATTEMPTS = 15;
static const uint16_t CAPTURE_TIMEOUT = 3000;
void task_fpc_bmlite();
#define MAX_SINGLE_CAPTURE_ATTEMPTS 3

/*Function for writing in nvs storage*/
/**
 * The function `NVS_WRITE` increments a counter, opens an NVS handle, sets a value in NVS, commits the
 * changes, and then closes the handle.
 */
void FingerprintChhaviComponent::NVS_WRITE()
{
  esp_err_t err;
  counter++;
   err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        printf("Done\n");
    }
  err = nvs_set_i32(my_handle, "template_id", counter);
  ESP_LOGCONFIG(TAG,((err != ESP_OK) ? "Failed!\n" : "Done\n"));
        printf("Committing updates in NVS ... ");
        err = nvs_commit(my_handle);
       ESP_LOGCONFIG(TAG,((err != ESP_OK) ? "Failed!\n" : "Done\n"));
       nvs_close(my_handle);
}

/*Function for reading from nvs storage*/
/**
 * The function `NVS_READ` reads a value from the NVS (Non-Volatile Storage) and logs the result.
 */
void FingerprintChhaviComponent::NVS_READ(){
    esp_err_t err;
    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGCONFIG(TAG,"Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        ESP_LOGCONFIG(TAG,"Done\n");
    }
    err = nvs_get_i32(my_handle, "template_id", &counter);
    switch (err) {
            case ESP_OK:
                ESP_LOGD(TAG,"Done\n");
             //    printf("counter = % " PRIu32 "\n", counter);
                break;
            case ESP_ERR_NVS_NOT_FOUND:
                 ESP_LOGD(TAG,"The value is not initialized yet!\n");
                break;
            default :
                 ESP_LOGD(TAG,"Error (%s) reading!\n", esp_err_to_name(err));
        }
  nvs_close(my_handle);
}

void FingerprintChhaviComponent::setup() {
    ESP_LOGCONFIG(TAG,"In setup..");
    ESP_LOGCONFIG(TAG, "Setting up Chhavi Fingerprint Reader...");
  
    esp_err_t err = nvs_flash_init(); //nvs flash initilization
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );
    NVS_READ();
    esp_err_t ret;
    spi_transaction_t t;
     //Initialize the BM-Lite IO pins
     gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = ((1ULL << 15) | (1ULL << 0)); //GPIO_OUTPUT_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    //disable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;    
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_DISABLE; //GPIO_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = ((1ULL << 2)); //GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_DISABLE; //GPIO_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = ((1ULL << 9)); //GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    
    //spi bus configuration 
    spi_bus_config_t buscfg = {
    .mosi_io_num = 13,
    .miso_io_num = 12,
    .sclk_io_num = 14,
    .quadwp_io_num = -1,
    .quadhd_io_num = -1,
    .data4_io_num = -1,
    .data5_io_num = -1,
    .data6_io_num = -1,
    .data7_io_num = -1,
    .max_transfer_sz = 1000,
};
    //spi device configuration
    spi_device_interface_config_t devcfg = {};
        // Honestly, no clue what _CLOCK_SPEED is, so just use our own
        devcfg.clock_speed_hz = 10*1000*1000;
        devcfg.spics_io_num  = -1,
        devcfg.mode = 0; //SPI mode 0
        devcfg.spics_io_num = -1; //CS pin
        devcfg.queue_size = 7; //Not sure if needed

    //spi bus initialization
    spi_bus_free(FPC_HOST);
    ret = spi_bus_initialize(FPC_HOST, &buscfg, DMA_CHAN); 
    ESP_ERROR_CHECK(ret);
    ret = spi_bus_add_device(FPC_HOST, &devcfg, &spi_test);
    ESP_ERROR_CHECK(ret);
  //  esp_err_t ret;

    ESP_LOGCONFIG(TAG,"spi");

    //hcp_chain initilization 
    hcp_chain.read = platform_bmlite_spi_receive;
    hcp_chain.write = platform_bmlite_spi_send;
    hcp_chain.pkt_buffer = hcp_data_buffer;
    hcp_chain.txrx_buffer = hcp_txrx_buffer;
    hcp_chain.pkt_size = 0;
    hcp_chain.pkt_size_max = sizeof(hcp_data_buffer);
    hcp_chain.phy_rx_timeout = 2000;
     ESP_LOGCONFIG(TAG, "Setting up ..");
    platform_init(NULL); 
    ESP_LOGCONFIG(TAG, "Setup completed..");
}

void FingerprintChhaviComponent::update() {
  //  const std::string vers = "Hello, World!";
    ESP_LOGI(TAG,"In update.."); 
    ESP_LOGCONFIG(TAG,"Total saved fingerprints = %d",counter-1);
    ESP_LOGI( TAG,"-> version.");
    this->version(); //version for firmware
 //  this->delete_all();
    //If button pressed for enroll function 
    if(button_pressed)
    {
      ESP_LOGD(TAG,"Inside func");
      this->enroll_fingerprint();
      //publishing state in text sensor
       if (this->text_press_ != nullptr) {
                this->text_press_->publish_state("Button");
            }
   //   delay(2000);
      ESP_LOGD(TAG,"out func");
      button_pressed = false; //set false after execution for next press
    }

    //if fingerscan button is pressed call fingerpscanmatch function
    if(scan_pressed)
    {
      ESP_LOGD(TAG,"Inside func");
      this->fingerscanmatch();
      ESP_LOGD(TAG,"out func");
      scan_pressed = false;
    }
    
    //If delete button is pressed call delete_all function
     if(delete_pressed)
    {
      ESP_LOGD(TAG,"Inside func");
      this->delete_all();
      ESP_LOGD(TAG,"out func");
      delete_pressed = false;
    }

//     memset(version_bep, 0, 100);
  //    this->version();
//     //   this->bep_version_callback_.call(&hcp_chain, be_version, 99);
//      ESP_LOGI( TAG,"-> BM-Lite method running.");
//     static fpc_bep_result_t res;
//  //   ESP_LOGI( TAG,"-> FPC - Version: %s", version_bep); 
//  //  this->enroll_fingerprint();
//   // this->finish_enrollment();
// //   this->fingerscanmatch();
 //  this->delete_all();
   ESP_LOGI( TAG,"-> Fingerprint");

}


void FingerprintChhaviComponent::dump_config() {
  ESP_LOGD(TAG, "CHHAVI_FINGERPRINT_READER:");
}

//Function for finger scan match 
void FingerprintChhaviComponent::fingerscanmatch() {
    if(counter == 0)
    {
      ESP_LOGI(TAG, "No fingerprint present");
    }
    static uint16_t template_d = -1;
    res = bep_identify_finger(&hcp_chain, 500, &template_d, &match);
    ESP_LOGW(TAG, "-> BM-Lite - Finger Wait Res: %i.", res);
     if (res == FPC_BEP_RESULT_OK)
     {
        if (match == true){
            ESP_LOGI(TAG, "-> BM-Lite - Match Found. Template ID: %i", template_d);
            if (this->finger_scan_matched_sensor_ != nullptr) {
                this->finger_scan_matched_sensor_->publish_state(template_d);
            }

            ESP_LOGI(TAG, "Identify completed");
        }
        else {
            ESP_LOGI(TAG, "-> BM-Lite - No Match Found. Template ID: %i", template_d);
             if (this->finger_scan_matched_sensor_ != nullptr) {
                this->finger_scan_matched_sensor_->publish_state(0);
            ESP_LOGI(TAG, "Identity not found");
            }
        }
    }
    
}

void FingerprintChhaviComponent::wait() {
   
}

//Delete all stored fingerprint
void FingerprintChhaviComponent::delete_all() {
    if(counter == 0)
    {
      ESP_LOGI(TAG, "No fingerprint present");
    }
     ESP_LOGI(TAG, "Deleting all stored fingerprints");
    res = bep_template_remove_all(&hcp_chain);
    if(res == FPC_BEP_RESULT_OK)
    {
        ESP_LOGI(TAG, "Deleted all fingerprints");
        if (this->remove_all_sensor_ != nullptr) {
            this->remove_all_sensor_->publish_state(1);
        }
        template_id = 0;
        counter = 0;
        NVS_WRITE();
    }
    else
    {
       ESP_LOGI(TAG, "Unsuccessfull Delete");
        if (this->remove_all_sensor_ != nullptr) {
            this->remove_all_sensor_->publish_state(0);
        }
    }

}

//Version printing
void FingerprintChhaviComponent::version(){
   res =  bep_version(&hcp_chain, version_bep, 99);
   if(res == FPC_BEP_RESULT_OK)
   {
         ESP_LOGI( TAG,"-> FPC - Version: %s", version_bep); 
   }
    if (this->version_sensor_ != nullptr) {
    this->version_sensor_->publish_state(0.2);
    }
    ESP_LOGI(TAG, "firmware version 0.2");

}

//For enrolling finger in device
void FingerprintChhaviComponent::enroll_fingerprint()
{
  
  ESP_LOGI(TAG, "Starting enrollment in slot ");
  res = bep_enroll_finger(&hcp_chain);
//  finger_id++;
  ESP_LOGI(TAG, "-> BM-Lite - Enroll Finger Res: %i.", res);
  if(res == FPC_BEP_RESULT_OK)
  {
    
    res = bep_template_save(&hcp_chain, template_id);
    if(res == FPC_BEP_RESULT_OK)
    {
      template_id = counter;
      ESP_LOGI(TAG, "-> BM-Lite - Save Template with id: %i.", template_id);
      if (this->finger_scan_matched_sensor_ != nullptr) {
                this->finger_scan_matched_sensor_->publish_state(template_id);
            }
         NVS_WRITE();
         template_id++;
         if (this->enrolling_binary_sensor_ != nullptr) {
                  this->enrolling_binary_sensor_->publish_state(true);
        }
       ESP_LOGI(TAG, "Completed enrollment in slot ");
    }
   if (this->enrolling_binary_sensor_ != nullptr) {
                  this->enrolling_binary_sensor_->publish_state(true);
        }
        ESP_LOGI(TAG, "enrollment in slot ");
       this->finish_enrollment();
  }  
}

void FingerprintChhaviComponent::finish_enrollment() {
 // this->enrollment_done_callback_.call();
  if (this->enrolling_binary_sensor_ != nullptr) {
    this->enrolling_binary_sensor_->publish_state(false);
  }
  ESP_LOGI(TAG, "Finished enrollment");
}


}
}

