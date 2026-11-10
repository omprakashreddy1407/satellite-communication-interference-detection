# satellite-communication-interference-detection
ESP32-based Satellite Communication Interference Detection and Monitoring System using OLED display, LEDs, buzzer, real receiver measurements, and safe software-based interference simulation.

# 🛰️ Smart Satellite Communication Interface
### ESP32-Based Wireless Messaging Prototype

![Project](https://img.shields.io/badge/Project-ESP32-blue)
![Domain](https://img.shields.io/badge/Domain-Embedded%20%26%20IoT-green)
![Communication](https://img.shields.io/badge/Communication-ESP--NOW-orange)
![Platform](https://img.shields.io/badge/Platform-Arduino%20IDE-blue)

## 📌 Project Overview

The Smart Satellite Communication Interface is an ESP32-based wireless messaging system designed to provide communication when normal internet and mobile networks are unavailable.

The project uses two ESP32 boards communicating directly through ESP-NOW technology. Users can send and receive messages using a mobile phone web page.

The system displays messages on an OLED screen and provides alerts using LEDs and a buzzer.

**Note:** This is an educational satellite-style communication prototype. It currently uses ESP-NOW between ESP32 boards, not an actual satellite connection.

## 🎯 Problem Statement

People in remote areas and disaster-affected locations may lose internet and mobile network connectivity.

Our project aims to provide a low-cost wireless messaging solution without depending on internet connectivity.

## 💡 Proposed Solution

- Two ESP32 boards communicate using ESP-NOW.
- Mobile phones connect to ESP32 Wi-Fi hotspots.
- Users send messages through a web interface.
- Messages are transmitted wirelessly to the other ESP32.
- OLED displays received messages.
- LEDs and buzzer provide alerts.
- Acknowledgement confirms message delivery.
- Two-way messaging allows users to reply.

## ⭐ Main Features

- Two-way wireless messaging
- ESP32-to-ESP32 communication
- Mobile phone web interface
- Message delivery acknowledgement
- Live signal strength (RSSI)
- OLED display
- LED status indicators
- Buzzer notification
- No internet required for ESP-NOW communication

## 🔧 Hardware Components

| Component | Quantity |
|---|---|
| ESP32 Development Board | 2 |
| OLED Display 0.96 inch | 1 or as required |
| LEDs | As required |
| Buzzer | 1 |
| Resistors | As required |
| Breadboard | 1 |
| Jumper Wires | As required |

## 💻 Software Requirements

- Arduino IDE
- ESP32 Board Package 3.x
- Embedded C++
- ESP-NOW
- WebServer
- Adafruit SSD1306
- Adafruit GFX

## ⚙️ Working Principle

1. Power ON both ESP32 boards.
2. Establish communication using ESP-NOW.
3. Connect the mobile phone to the ESP32 Wi-Fi hotspot.
4. Open the messaging web page.
5. Type and send a message.
6. The receiving ESP32 receives the message.
7. OLED displays the received message.
8. LEDs and buzzer indicate the status.
9. An acknowledgement is sent to the sender.
10. Users can send replies through the same system.

## 📊 Block Diagram

```text
       MOBILE PHONE A
              |
         Wi-Fi Hotspot
              |
           ESP32 A
              |
       ESP-NOW Wireless
       Communication
              |
           ESP32 B
              |
       +------+------+ 
       |      |      |
      OLED   LED   BUZZER
       |
  Message Display
              |
       MOBILE PHONE B
```

## 📡 Communication Packet Types

- **Heartbeat:** Checks whether the other ESP32 is active.
- **Message:** Sends text messages.
- **Acknowledgement:** Confirms message delivery.
- **Reply:** Sends a response to the received message.

## 🚀 Project Development Phases

### Phase 1 – Completed
- ESP-NOW communication between two ESP32 boards.
- OLED display integration.
- LED and buzzer alerts.

### Phase 2 – Current
- Mobile phone web chat.
- Two-way messaging.
- Signal-strength bars.
- Message acknowledgement.

### Phase 3 – Planned
- TFT display integration.
- LoRa long-range communication option.
- Final testing and demonstration.
- Project documentation.

## 🌍 Applications

- Disaster management communication
- Remote-area communication
- Emergency messaging concepts
- Field team communication
- Embedded systems learning
- IoT wireless communication

## 💰 Estimated Project Cost

Approximately ₹1,000, depending on component prices and availability.

## 🔮 Future Scope

- Improve communication range.
- Integrate LoRa technology.
- Develop an improved mobile interface.
- Add message history storage.
- Improve signal monitoring.
- Add more display features.

## 🔒 Safety Note

This project is designed for wireless messaging and communication monitoring. It does not generate jamming signals or intentionally interfere with satellite or mobile communication systems.

## 📁 Repository Structure

```text
Smart-Satellite-Communication/
│
├── README.md
│
├── Arduino_Code/
│   ├── ESP32_A/
│   └── ESP32_B/
│
├── Circuit_Diagram/
│
├── Block_Diagram/
│
├── Project_Images/
│
└── Documentation/
```

## 👨‍🎓 Project Details

- **Project Name:** Smart Satellite Communication Interface
- **Controller:** ESP32
- **Communication:** ESP-NOW
- **Programming Language:** Embedded C++
- **Software:** Arduino IDE
- **Domain:** Embedded Systems & IoT
- **Course:** ECE1512 – Design Thinking in Practice

---

## ⭐ Thank You!

Thank you for visiting our project repository.

We welcome suggestions and improvements!
