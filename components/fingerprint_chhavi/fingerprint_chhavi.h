#pragma once
#define FINGERPRINT_CHHAVI_H
#ifdef FINGERPRINT_CHHAVI_H


//#include "esphome.h"
// #include <driver/gpio.h>
#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/spi/spi.h"
#include "esphome/components/button/button.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/core/helpers.h" 
#include  "esphome/components/text_sensor/text_sensor.h"
#include "driver/rmt.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include <stdlib.h>

//Libraries for NVS permenant storage
#include "esp_system.h"
#include <inttypes.h>
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_spiffs.h"
#include "esp_log.h"
#include "spiffs_config.h"

#include <sys/unistd.h>
#include <sys/stat.h>
#include  <string.h>
#include <vector>
#include <stdint.h>
#include <stdbool.h>
#include <functional>

extern "C"{
 // #include "esphome/components/fingerprint_chhavi/fingerprint_chhavi.h"
  #include "esphome/components/fingerprint_chhavi/bmlite_if.h"
  #include "esphome/components/fingerprint_chhavi/hcp_tiny.h"
}


#define REMOVE_ID_ALL_TEMPLATES 0U
#define FORMAT_SPIFFS_IF_FAILED true
namespace esphome {
namespace fingerprint_chhavi{

static const char *const TAG = "fingerprint_chhavi";

class FingerprintChhaviComponent : public sensor::Sensor ,public PollingComponent{
 public:
  FingerprintChhaviComponent() : PollingComponent(15000) { }
 // void loop() override;
  void setup() override ;
  void dump_config() override;
  void update() override;
  void finish_enrollment();
  void enroll_fingerprint();
  void fingerscanmatch();
  void version();
  void wait();
  void delete_all();
  void NVS_WRITE();
  void NVS_READ();
  FingerprintChhaviComponent(spi_host_device_t spi_bus, gpio_num_t cs_pin);
  
// CallbackManager<void()> enrollment_callback_;
  CallbackManager<void()> enrollment_scan_callback_;
  CallbackManager<void()> enrollment_done_callback_;
 // CallbackManager<void(HCP_comm_t , uint32_t , uint16_t, bool )> identify_callback_;
  CallbackManager<void(HCP_comm_t, uint16_t)> sensor_wait_finger_present_callback_;
  CallbackManager<void(HCP_comm_t,uint16_t)> sensor_wait_finger_not_present_callback_;
  CallbackManager<void(HCP_comm_t,uint16_t)> bep_capture_callback_;
  CallbackManager<void(void)> version_callback_;
  CallbackManager<void(void)> finger_scan_matched_callback_;
  CallbackManager<void(HCP_comm_t)> bep_template_remove_callback_;
  // CallbackManager<void(HCP_comm_t,uint16_t)>bep_template_save_callback_;
  CallbackManager<void()>bep_template_remove_all_callback_;
 // CallbackManager<void(HCP_comm_t)> bep_sensor_reset_callback;
  CallbackManager<void(HCP_comm_t,uint16_t,uint16_t)> bmlite_send_cmd_callback_;
  CallbackManager<void(void)>platform_bmlite_reset_callback_;

  void add_on_enrollment_done_callback(std::function<void()> callback) {
    this->enrollment_done_callback_.add(std::move(callback));
  }

  void sensor_wait_finger_present(std::function<void(HCP_comm_t , uint16_t )>callback){
    this->sensor_wait_finger_present_callback_.add(std::move(callback));
  }

  void sensor_wait_finger_not_present(std::function<void(HCP_comm_t , uint16_t)>callback){
    this->sensor_wait_finger_not_present_callback_.add(std::move(callback));
  }

  void bep_capture(std::function<void(HCP_comm_t , uint16_t )>callback){
    this->bep_capture_callback_.add(std::move(callback));
  }

  void bep_template_remove(std::function<void()>callback){
    this->bep_template_remove_all_callback_.add(std::move(callback));
  }

  void add_version_callback(std::function<void()>callback){
    this->version_callback_.add(std::move(callback));
  }

   void finger_scan_matched_callback(std::function<void()>callback){
    this->finger_scan_matched_callback_.add(std::move(callback));
  }

  void bmlite_send_cmd_callback(std::function<void(HCP_comm_t , uint16_t , uint16_t)>callback){
    this->bmlite_send_cmd_callback_.add(std::move(callback));
  }

  void platform_bmlite_reset_callback(std::function<void(void)> callback){
    this->platform_bmlite_reset_callback_.add(std::move(callback));
  }

  void add_on_enrollment_scan_callback(std::function<void()> callback) {
    this->enrollment_scan_callback_.add(std::move(callback));
  }
   void set_enrolling_binary_sensor(binary_sensor::BinarySensor *enrolling_binary_sensor) {
    this->enrolling_binary_sensor_ = enrolling_binary_sensor;
  }
  void set_Chhavi_version_sensor(sensor::Sensor *version_sensor) { this->version_sensor_ = version_sensor; }
  void set_on_finger_scan_matched_sensor(sensor::Sensor *finger_scan_matched_sensor){this->finger_scan_matched_sensor_ = finger_scan_matched_sensor;}
  void set_Delete_All_sensor(sensor::Sensor *remove_sensor){this->remove_all_sensor_ = remove_sensor;}
  void set_fingerprint_enrolling(button::Button *scan_pressed){this->scan_pressed_ = scan_pressed;}
  void set_fingerscan_matched(button::Button *button_press){this->button_pressed_ = button_press;}
  void set_fingerprint_delete(button::Button *delete_press){this->delete_pressed_ = delete_press;}
  text_sensor::TextSensor *fingerprint_state{nullptr};

 protected:

  uint8_t enrollment_buffers_ = 5;
  std::vector<uint8_t> data_ = {};
  uint8_t interrupt_pin_;
  int reset_pin_ = -1;
  int phy_addr_ = -1;
  int clock_speed_ = 10000000;
  binary_sensor::BinarySensor *enrolling_binary_sensor_{nullptr};
  button::Button *button_pressed_{nullptr};
  button::Button *scan_pressed_{nullptr};
  button::Button *delete_pressed_{nullptr};
  sensor::Sensor *version_sensor_{nullptr};
  sensor::Sensor *finger_scan_matched_sensor_{nullptr};
  sensor::Sensor *remove_all_sensor_{nullptr};

};

class FingerScanMatchedTrigger : public Trigger<> {
 public:
  explicit FingerScanMatchedTrigger(FingerprintChhaviComponent *parent) {
    parent->finger_scan_matched_callback([this]() { this->trigger(); });
  }
};

class VersionTrigger : public Trigger<> {
 public: 
  explicit VersionTrigger(FingerprintChhaviComponent *parent) {
    parent->add_version_callback([this]() { this->trigger(); });
  }
};

class EnrollmentTrigger : public Trigger<> {
 public:
  explicit EnrollmentTrigger(FingerprintChhaviComponent *parent) {
    parent->add_on_enrollment_scan_callback([this]() { this->trigger(); });
  }
};

class DeleteTrigger : public Trigger<> {
 public:
  explicit DeleteTrigger(FingerprintChhaviComponent *parent) {
    parent->bep_template_remove([this]() { this->trigger();});
  }
};

class EnrollmentDoneTrigger : public Trigger<> {
 public:
  explicit EnrollmentDoneTrigger(FingerprintChhaviComponent *parent) {
    parent->add_on_enrollment_done_callback([this]() { this->trigger(); });
  }
};

template<typename... Ts> class EnrollmentAction : public Action<Ts...>, public Parented<FingerprintChhaviComponent> {
 public:
  void play(Ts... x) override {
      this->parent_->enroll_fingerprint();
  }
};

template<typename... Ts>
class CancelEnrollmentAction : public Action<Ts...>, public Parented<FingerprintChhaviComponent> {
 public:
  void play(Ts... x) override { this->parent_->finish_enrollment(); }
};

template<typename... Ts> class FingerScanMatchAction : public Action<Ts...>, public Parented<FingerprintChhaviComponent> {
 public:
  void play(Ts... x) override {
    this->parent_-> fingerscanmatch();
  }
};

template<typename... Ts>
class VersionAction : public Action<Ts...>, public Parented<FingerprintChhaviComponent> {
 public:
  void play(Ts... x) override {
   this->parent_->version();
   }
};

template<typename... Ts>
class WaitAction : public Action<Ts...>, public Parented<FingerprintChhaviComponent> {
 public:
 TEMPLATABLE_VALUE(HCP_comm_t, chain)
  void play(Ts... x) override { this->parent_->wait(); }
};

template<typename... Ts>
class DeleteAction : public Action<Ts...>, public Parented<FingerprintChhaviComponent> {
 public:
  void play(Ts... x) override { this->parent_->delete_all(); }
};

}  // namespace fingerprint_grow
}  // namespace esphome
#endif