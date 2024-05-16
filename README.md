# Automated Solar Tracking and Charging System with Blynk Integration 🌞☁️

## Project Overview 🗒️

This project involves the development of an IoT-enabled solar tracking system that optimizes the orientation of solar panels to maximize energy capture from the sun. The system includes Maximum Power Point Tracking (MPPT) for efficient battery charging, real-time battery capacity monitoring, and user alerts via the Blynk platform. Additionally, it features a 5V USB booster to support external load connections, such as charging a mobile device.

## Features 🌠

- **Solar Tracking**: Automatically adjusts the position of solar panels to follow the sun's trajectory throughout the day.
- **MPPT Module**: Ensures maximum efficiency in converting solar energy to electrical energy for battery charging.
- **Battery Management**: Monitors battery charge capacity using a voltage sensor.
- **IoT Monitoring**: Sends real-time data to the Blynk platform and alerts the user when the battery is nearly full.
- **External Load Support**: Includes a 5V USB booster to facilitate connections for external devices, such as mobile phones.

## Components 🌀
- Solar Panels
- Servo Motors (for tracking)
- MPPT Module
- Rechargeable Battery
- Voltage Sensor
- Microcontroller (e.g., Arduino, ESP8266/ESP32)
- Blynk Platform for IoT Integration
- 5V USB Booster

## Schematic Diagram

(Schematic Diagram)[https://drive.google.com/file/d/1xy6wegZcLUVimbRP5SgMEylFMIvNMCjG/view?usp=sharing]

## Getting Started 🚀

### Prerequisites 🧠

- Basic knowledge of Arduino programming
- Blynk account for IoT integration
- Required components (listed above)

### Installation ⤵️

1. **Clone the Repository:** 🤖

    ```bash
    git clone https://github.com/yourusername/solar-tracker.git
    cd solar-tracker
    ```

2. **Install Required Libraries:** 🌐

    Ensure you have the following Arduino libraries installed:
    - Servo
    - Blynk
    - MPPT

    You can install these libraries through the Arduino Library Manager.

3. **Upload the Code:** ➡️

    Open the `solar_tracker.ino` file in the Arduino IDE, configure your WiFi and Blynk credentials, and upload the code to your microcontroller.

4. **Hardware Setup:** 🔌

    - Connect the solar panels to the MPPT module.
    - Attach the servo motors to the solar panel mount.
    - Connect the MPPT output to the battery and the voltage sensor.
    - Connect the 5V USB booster to the battery for external load support.
    - Wire the microcontroller as per the schematic.

5. **Blynk Configuration:** ☁️

    - Create a new project in the Blynk app.
    - Add widgets to monitor the battery voltage and receive alerts.
    - Use the Blynk token in your Arduino code for connectivity.

## Usage

1. **Power Up the System:** 💡

    Ensure all connections are secure and power up the system. The solar tracker should begin adjusting the panel orientation towards the sun.

2. **Monitor via Blynk:** 📺

    Open the Blynk app on your smartphone to monitor real-time data and receive alerts about battery status.

3. **External Device Charging:** 📱

    Connect your mobile device or other USB-powered devices to the 5V USB booster for charging.

## Troubleshooting ⚠️

- **Servo Not Moving:** Check the connections and ensure the servo library is correctly installed.
- **No Data on Blynk:** Verify WiFi credentials and Blynk token in the Arduino code.
- **Battery Not Charging:** Ensure the MPPT module is correctly wired and functional.

## Contributing 🤝

Contributions are welcome! Please fork this repository and submit pull requests for any enhancements or bug fixes.

## License 🉑

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements 🙏

- Thanks to the open-source community for providing the tools and libraries used in this project.
- Special thanks to [Blynk](https://blynk.io) for their IoT platform.

---

For more information, visit the project [GitHub Repository](https://github.com/yourusername/solar-tracker).
