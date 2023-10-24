
## Uploading Project Binaries to ESP32

For Windows:

1.Download and install ESP Flash Tool from the official website. 
    https://www.espressif.com/en/support/download/other-tools

2.Select the ESP32 chip and type 'develop'.

3.Upload three binary files with specific addresses. 
   
    partitions.bin : 0x8000 
    bootloader.bin : 0x1000 
    firmware.bin : 0x10000

4.Choose the appropriate COM port and baud rate 460800.

5.Initiate the upload process pressing START button.

For Linux:

1.Download the ESP-IDF from the provided link.
  
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html#get-started-get-esp-idf

2.Validate the installation by checking the version of ESP-IDF using the idf.py version command.

3.Execute the provided command with the necessary parameters to upload the binaries to the Chhavi device.
  ```bash
 esptool.py -p /dev/ttyUSB1 -b 460800 --before default_reset --after hard_reset --chip esp32 write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 bootloader.bin 0x8000 partition-table.bin 0x10000 firmware.bin
After successful flash you can see logs using following commands in esphome. Navigate to esphome directory . esphome logs fingerprint-chhavi.yaml
  ```
4. After successful flash you can see logs using following commands in esphome.
  Navigate to esphome directory .
   ```bash
    esphome logs fingerprint-chhavi.yaml 
```

  Alternatively you can access logs using serial terminal by entering COM port and baudrate 115200.
