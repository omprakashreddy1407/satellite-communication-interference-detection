
flowchart TB
    P1["Mobile Phone A"] -->|Wi-Fi| A["ESP32 A"]
    P2["Mobile Phone B"] -->|Wi-Fi| B["ESP32 B"]

    A <-->|ESP-NOW Wireless Communication| B

    B -->|GPIO 21 SDA| O["OLED Display"]
    B -->|GPIO 22 SCL| O

    B -->|GPIO 18 + 220Ω| G["Green LED"]
    B -->|GPIO 19 + 220Ω| Y["Yellow LED"]
    B -->|GPIO 23 + 220Ω| R["Red LED"]

    B -->|GPIO 25 + 1kΩ| T["BC547 Transistor"]
    T -->|Switching| Z["Buzzer"]

    O --> D["Display Messages"]
    G --> S["Normal Status"]
    Y --> W["Weak Signal"]
    R --> I["Warning Status"]
    Z --> N["Audio Alert"]
