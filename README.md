
Chhavi Fingerprint Sensor with ESPHome Documentation


Chhavi is an open-source project that offers an ESP32-based NFC and fingerprint device. This device has a range of applications, from security systems to access control. Our goal is to provide a seamless and accessible platform for developers and enthusiasts to explore and utilize NFC and fingerprint technologies.
This documentation will guide you through the process of setting up and running the Chhavi fingerprint sensor with ESPHome. ESPHome is an open-source platform that allows easy configuration and management of ESP8266/ESP32-based devices. By following these steps, you will be able to integrate Chhavi into your ESPHome-based project.



## Prerequisites

Before you begin, ensure you have the following prerequisites:

1. Chhavi Device: Ensure you have received the Chhavi device, either through distribution or assembly.
2. Computer: A Computer with ESPHome installed
3. USB cable: A USB cable for connecting the development board to your computer
4. Basic knowledge of ESPHome
5. Raspberry Pi board

## Setting Up

Prerequisites:

Before you begin, make sure you have the following prerequisites:
Python installed on your computer.

To verify it check python version using following command.
```bash
python --version
```

Installation Steps

Step 1: Clone the ESPHome Repository.
You can download ESPHome by cloning the official GitHub repository:

```bash
git clone https://github.com/esphome/esphome.git
```

Step 2: Complete the Installation
Navigate to the downloaded esphome folder and run the following command to install ESPHome:

```bash
cd esphome
python setup.py install
```

Step 3: Verify Installation
To confirm that ESPHome is installed correctly, check the version. It should be something like:
    Version: 2023.9.0-dev (or greater 2023.10.0-dev)

You can check the ESPHome version by running the following command:
```bash
esphome version
```

Additional Resources
For more detailed information and troubleshooting, please visit the official ESPHome documentation.

https://esphome.io/guides/getting_started_command_line.html.

## Running Chhavi Sensor with ESPHome

1. Edit the Configuration File:

    • Open the fingerprint-chhavi.yaml file from project directory in a text editor.

    • Configuration settings for the Chhavi sensor, including pins, communication settings, and any additional options required for your specific use case.

    • Refer to the ESPHome documentation for sensor configuration for guidance.
   
    • Configure the following details in the YAML file
        WiFi credentials (SSID and password) 
```bash
     wifi:
        ssid: "OnePlus 10 Pro 5G"
        password: "JerryTom
```
    

2. Compile the Configuration:

    • Open your terminal or command prompt.

    • Navigate to the directory where your fingerprint-chhavi.yaml file is located.

    • Run the following command to compile the configuration

        esphome compile fingerprint-chhavi.yaml 

    • This command generates the necessary build files for your ESP32 with the Chhavi sensor.

3.	Upload the Firmware:

    •	Once the compilation is successful, connect your ESP32 to your computer and put it in flashing mode.

    •	Run the following command to upload the firmware to your ESP32

        esphome upload fingerprint-chhavi.yaml 

    •	Ensure that your ESP32 is connected to your computer and in flashing mode.

4.	View Logs:

    • After uploading the firmware, you can monitor the behavior of your Chhavi sensor and troubleshoot any issues.

    • Run the following command to view real-time logs:

        esphome logs fingerprint-chhavi.yaml 

    • The logs will help you confirm that the sensor is functioning correctly or diagnose any problems.

5. Accessing the ESPHome Web UI:

   To configure Chhavi and monitor its behavior through the ESPHome Dashboard:

   • In your terminal, navigate to the directory where your ESPHome configuration files are located, typically in ~/.config/esphome/.

   • Run the following command to open the ESPHome Dashboard:

        esphome dashboard 

   If the Dashboard doesn't open automatically, you can access it by entering the IP address of your Chhavi device in your web browser's 
   address bar. The IP address is typically found in your router's connected devices list or using an IP scanning tool.
   Example: If your Chhavi device's IP address is 192.168.1.100, enter http://192.168.1.100 in your web browser.

   Ensure that your Chhavi device is on the same network as the computer you are using to access the Dashboard.

## User Interfaces

1. Display Version

    • To display the firmware version of the Chhavi sensor

    • The current firmware version of the Chhavi sensor will be displayed

2. Enroll Fingerprint

    • To enroll a fingerprint with a unique template ID

    • Press the "Enroll" button to start the fingerprint enrollment process.
    
    • Once successfully enrolled, the template ID will be associated with the fingerprint.

3. Scan and Match Fingerprint

    • To scan and match a fingerprint and retrieve the associated template ID:
    
    • Click on the "Scan and Match " interface.
    
    • Place your finger on the sensor.
    
    • The system will scan and attempt to match your fingerprint with stored fingerprints.
    
    • If a match is found, the corresponding template ID will be displayed on Template number entity.

4. Delete Stored Fingerprints

    • To delete all stored fingerprints:
    
    • Click on the "Delete Stored Fingerprints" button.
    
    • All stored fingerprints will be removed from the system.
    
    • All Deleted entity will return 1 on successful deletion or 0 on unsuccessful deletion.

##  Raspberry Pi Setup for Home assistant server 

Follow these steps to set up Raspberry Pi for home automation:

1. Download and install Raspberry Pi Imager.
2. Insert an SD card and open Raspberry Pi Imager.
3. Select the home assistant operating system from other operating systems and flash it to the SD card.(path: home assistant and home automation ->home assistant-> select raspberry pi board )
4. Click "Write" to flash the Raspberry Pi OS Lite image to the SD card.
5. Eject the SD card from your computer and insert it into your Raspberry Pi.
6. Power on the Raspberry Pi and set up the Home Assistant server.
7. Access the server using the provided link.
    http://homeassistant.local:8123/

## Uploading Project Binaries to ESP32 

For Windows:

1. Download and install ESP Flash Tool from the official website.
    For windows : https://www.espressif.com/en/support/download/other-tools
2. Select the ESP32 chip and type 'develop'.
3. Upload three binary files with specific addresses.
    partitions.bin :  0x8000
	bootloader.bin : 0x1000
	firmware.bin : 0x10000
4. Choose the appropriate COM port and baud rate 460800.
5. Initiate the upload process.

For Linux:

1. Download the ESP-IDF from the provided link.
    ->https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html#get-started-get-esp-idf
2. Validate the installation by checking the version of ESP-IDF using the idf.py version command.
3. Execute the provided command with the necessary parameters to upload the binaries to the Chhavi device.
   ```bash
    esptool.py -p /dev/ttyUSB1 -b 460800 --before default_reset --after hard_reset --chip esp32 write_flash --flash_mode dio --flash_size detect --flash_freq 40m 0x1000 bootloader.bin 0x8000 partitions.bin 0x10000 firmware.bin
   ```
   
5. After successful flash you can see logs using following commands in esphome.
Navigate to esphome directory .
    esphome logs fingerprint-chhavi.yaml 
	

## Activating Soft-AP in Chhavi

After the firmware upload, the Chhavi device will activate its own fallback hotspot. Follow these steps to configure the Soft-AP:

1. Connect to the "fingerprint-chhavi" Wi-Fi network using the password "CHHAVIVEGG".
2. Access the Chhavi web configuration page using the IP address "192.168.4.1".
3. Provide your Wi-Fi credentials in the designated fields to connect Chhavi to your home network.

## Integrating Chhavi with Home Assistant

To integrate Chhavi with the Home Assistant server, ensure that both devices are on the same network. Follow these steps for seamless integration:

1. After setting up home assistant server uploading firmware to chhavi now by accessing home assistant web page you can see that your device will be discovered by home assistant. 
(You need to use same network connection for your home assistant server and device otherwise device will not be visible on server)

2. After discovery when you select device one dialogue box will be popped up for entering encryption key and password.
    encryption key : "besdMC9u8a3y+Js6wOK9oS4Lc/7a3fj4B3ShdaT/lqs="
    password: "chhavi"

3. Alternatively you can manually add device by going to settings-> add integration -> select esphome -> add device -> add ip as fingerprint-chhavi.local and port is as it is. (6053)

4. After that you will be needed to add all of its entity to dashboard.These entity will be visible on dashboard now you can access it and give command to device .
   
## Additional Resources

•	For hardware resources, such as design files and schematics, refer to our crowd-supply campaign page once active.

•	For firmware resources, explore the code in our repository.


## Contact and Support

•	For questions, issues, or sharing your Chhavi projects, contact us at contact@vicharak.in.

•	Thank you for joining us on this exciting Chhavi journey!

•	Remember, Chhavi is not just a device; it's a key to unlock endless possibilities.

•	Happy tinkering!

•	Discord server!
    https://discord.gg/BWkbF5Sg

