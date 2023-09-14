cmake_minimum_required(VERSION 3.2)
project(fingrprint_chhavi)

idf_component_register(
   SRCS 
      "D:\Newfolder\chhavi_esphome_fingerprint\custom_components\fingerprint_chhavi\bmlite_if.c"
      "D:\Newfolder\chhavi_esphome_fingerprint\custom_components\fingerprint_chhavi\fpc_bmlite.c"
      "D:\Newfolder\chhavi_esphome_fingerprint\custom_components\fingerprint_chhavi\hcp_tiny.c"
      "D:\Newfolder\chhavi_esphome_fingerprint\custom_components\fingerprint_chhavi\fpc_crc.c"
      "D:\Newfolder\chhavi_esphome_fingerprint\custom_components\fingerprint_chhavi\platform.c"
      "D:\Newfolder\chhavi_esphome_fingerprint\custom_components\fingerprint_chhavi\bmlite_hal.c"
      "D:\Newfolder\chhavi_esphome_fingerprint\custom_components\fingerprint_chhavi\fingerprint_chhavi.cpp"

   INCLUDE_DIRS 
      "D:\Newfolder\chhavi_esphome_fingerprint\custom_components\fingerprint_chhavi"
)
