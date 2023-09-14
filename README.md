
Chhavi Fingerprint Sensor with ESPHome Documentation


Chhavi is an open-source project that offers an ESP32-based NFC and fingerprint device. This device has a range of applications, from security systems to access control. Our goal is to provide a seamless and accessible platform for developers and enthusiasts to explore and utilize NFC and fingerprint technologies.
This documentation will guide you through the process of setting up and running the Chhavi fingerprint sensor with ESPHome. ESPHome is an open-source platform that allows easy configuration and management of ESP8266/ESP32-based devices. By following these steps, you will be able to integrate Chhavi into your ESPHome-based project.



## Prerequisites

Before you begin, ensure you have the following prerequisites:

1. Chhavi Device: Ensure you have received the Chhavi device, either through distribution or assembly.
2. Computer: A Computer with ESPHome installed
3. USB cable: A USB cable for connecting the development board to your computer
4. Basic knowledge of ESPHome

## Setting Up

Download ESPHome : 
If you haven't already, you need to download and install ESPHome on your development computer. You can follow the installation instructions for your specific platform from the official ESPHome documentation: 
https://esphome.io/guides/getting_started_command_line.html.

## Running Chhavi Sensor with ESPHome

1. Edit the Configuration File:

    • Open the fingerprint-chhavi.yaml file in a text editor.

    • Configure the following details in the YAML file
        WiFi credentials (SSID and password) 

    • Configuration settings for the Chhavi sensor, including pins, communication settings, and any additional options required for your         specific use case.

    • Refer to the ESPHome documentation for sensor configuration for guidance.

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

