# ESP32 Web Camera Streamer (Non-FIFO OV2640 / Standard ESP32 & ESP32-S3)

A lightweight Arduino IDE-compatible script to stream video over Wi-Fi using a standard ESP32 or ESP32-S3 development board paired with a low-cost, non-FIFO camera module (like the OV2640). 

This project was built as part of an ongoing journey reviving an **ESP32 Quadruped Robot** project, where GPIO pins are scarce and maximizing frame rate at a lower resolution ($320 \times 240$ QVGA) is key.

---
## 📺 Video Tutorial

[![Watch the tutorial](https://img.youtube.com/vi/hNfohX3zoXk/0.jpg)](https://www.youtube.com/watch?v=hNfohX3zoXk)
> **Click the image above to watch the full tutorial on YouTube.**

---

## Features

- **Low-Cost Camera Integration:** Utilizes non-FIFO camera modules that are typically harder to stream from.
- **Adjustable Resolutions:** Supports VGA, CIF, QVGA, and QQVGA formats. Configured out-of-the-box for **QVGA ($320 \times 240$)** to ensure high frame rates for robotics and real-time streaming.
- **Configurable JPEG Quality:** Fine-tune compression ratios to balance image clarity against Wi-Fi payload size.
- **Cross-Board Compatibility:** Tested and compatible with standard ESP32 development boards as well as the ESP32-S3.
- **Local Web Server:** Stream directly via browser using the board's local IP address.

---

## Hardware Requirements

- **Microcontroller:** Standard ESP32 Dev Board or ESP32-S3.
- **Camera Module:** Non-FIFO OV2640 camera module (supports up to VGA $640 \times 480$).
- **Power Source & Wiring Components** (Jumpers, breadboard, etc.)

---

## Getting Started

### 1. Prerequisites
- Install the [Arduino IDE](https://www.arduino.cc/).
- Ensure you have the **ESP32 Board Package** installed via the Arduino Board Manager.

### 2. Configuration
Open the provided Arduino script and update the following parameters before uploading:
- **Wi-Fi Credentials:** Enter your local network SSID and password.
- **Resolution:** Set to your preferred format (Recommended: `FRAMESIZE_QVGA` for $320 \times 240$ to optimize frame rates).
- **JPEG Quality:** Adjust the compression value (lower numbers mean higher image quality, but larger payload sizes).
- **Clock Settings:** Adjust the camera clock frequency if needed (note: pushing the clock too high without proper tuning may degrade image quality).

### 3. Wiring & Upload
1. Wire your non-FIFO camera module to your ESP32 / ESP32-S3 board. *(Check out the associated video guide linked below for detailed pinout guidance!)*
2. Connect your board via USB, select the correct board and port in the Arduino IDE, and upload the code.
3. Open the **Serial Monitor** (set to 115200 baud) to find the assigned local IP address once connected to Wi-Fi.

### OV7670 to Esp32s3
| Display Pin | ESP32-S3 GPIO | Function |
| :--- | :--- | :--- |
| PWDN_GPIO_NUM | connect to GND | - |
| RESET_GPIO_NUM | connect to 3.3v | - |
| XCLK_GPIO_NUM | GPIO **[6]** | - |
| SIOD_GPIO_NUM | GPIO **[2]** | - |
| SIOC_GPIO_NUM | GPIO **[1]** |- |
| Y9_GPIO_NUM | GPIO **[7]** | D7 |
| Y8_GPIO_NUM | GPIO **[8]** | D6 |
| Y7_GPIO_NUM | GPIO **[9]** | D5 |
| Y6_GPIO_NUM | GPIO **[10]** | D4 |
| Y5_GPIO_NUM | GPIO **[11]** | D3 |
| Y4_GPIO_NUM | GPIO **[12]** | D2 |
| Y3_GPIO_NUM | GPIO **[13]** | D1 |
| Y2_GPIO_NUM | GPIO **[14]** | D0 |
| VSYNC_GPIO_NUM | GPIO **[3]** | - |
| HREF_GPIO_NUM | GPIO **[4]** | - |
| PCLK_GPIO_NUM | GPIO **[5]** | - |

### 4. Accessing the Stream
Open any web browser on a device connected to the same Wi-Fi network, type in the IP address displayed in the Serial Monitor, and view your live stream!

---

## Video & Context
To watch the full build breakdown and wiring details, check out the video episode here: [https://www.youtube.com/watch?v=hNfohX3zoXk]

---

## 🤝 Support

If you found this project helpful, please consider:
* **Subscribing** to the YouTube Channel.
* Giving the video a **Like**.
* Starring this GitHub Repository!

* **YouTube:** [https://www.youtube.com/@DsnIndustries/videos]
* **Patreon:** [https://www.patreon.com/c/dsnIndustries]

Happy Making!

## Games
* **Maze Escape:** https://play.google.com/store/apps/details?id=com.DsnMechanics.MazeEscape
* **Air Hockey:** https://play.google.com/store/apps/details?id=com.DsnMechanics.AirHockey
* **Click Challenge:** https://play.google.com/store/apps/details?id=com.DsNMechanics.ClickChallenge
* **Flying Triangels:** https://play.google.com/store/apps/details?id=com.DsnMechanics.Triangle
* **SkyScrapper:** https://play.google.com/store/apps/details?id=com.DsnMechanics.SkyScraper
