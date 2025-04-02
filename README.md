# 🎙️ VoiceLink - Wireless ESP32 Audio Transceiver System

<p align="center">
  <img src="https://raw.githubusercontent.com/codehat01/esp32-voicelink/refs/heads/main/assets/logo2.png" alt="VoiceLink ESP32 System">
</p>


## 📡 Introduction

Welcome to VoiceLink it  is a lightweight, DIY wireless audio communication system using ESP32 microcontrollers. This project creates a simple yet effective two-way audio transmission system similar to walkie-talkies, leveraging WiFi for communication instead of traditional radio frequencies. The system consists of two ESP32 units - one configured as an audio sender (with microphone) and another as a receiver (with speaker).

Perfect for:
- DIY home intercom systems
- Educational projects for IoT and audio streaming
- Remote monitoring applications
- Maker projects requiring wireless audio transmission

## ✨ Features

- **Real-time audio transmission** over WiFi network
- **Robust connection handling** with automatic reconnection
- **Low latency** audio streaming using UDP protocol
- **Power efficient** design for portable applications
- **User-configurable audio parameters** for different microphones/speakers
- **Simple, modifiable codebase** for custom projects
- **Automatic error recovery** for reliable operation

## 🔌 Hardware Requirements

### Components Needed
- 2× ESP32 development boards
- 1× KY-037 sound sensor module (or similar microphone)
- 1× MAX98357A I2S amplifier or similar
- 1× 8Ω speaker
- Jumper wires
- Power supply (USB or battery)

### Connection Diagram

#### Sender Unit (Microphone)
```
┌─────────────┐           ┌─────────────┐
│             │           │   KY-037    │
│             │           │  Microphone │
│    ESP32    │◄──────────┤   Module    │
│  (Sender)   │   Analog  │             │
│             │   Pin 39  │             │
└─────────────┘           └─────────────┘
```

#### Receiver Unit (Speaker)
```
┌─────────────┐      ┌────────────┐      ┌───────────┐
│             │ BCLK │            │      │           │
│             ├─────►│            │      │           │
│             │ LRC  │ MAX98357A  │ Out  │  Speaker  │
│    ESP32    ├─────►│    I2S     ├─────►│   (8Ω)    │
│ (Receiver)  │ DIN  │ Amplifier  │      │           │
│             ├─────►│            │      │           │
└─────────────┘      └────────────┘      └───────────┘
```

![Circuit Diagram](https://raw.githubusercontent.com/codehat01/esp32-voicelink/refs/heads/main/assets/Circuit.png)

## 📚 Software Dependencies

### Required Libraries
- WiFi.h (Arduino ESP32 Core)
- WiFiUdp.h (Arduino ESP32 Core)
- driver/i2s.h (ESP32 Core)
- BluetoothA2DPSink.h (for Bluetooth audio option)

### Development Environment
- Arduino IDE (1.8.x or later)
- ESP32 Arduino Core (2.0.0 or later)

## 💻 Installation

1. **Install Required Libraries**
   - Install the ESP32 board package in Arduino IDE
   - Install the ESP32-A2DP library if using Bluetooth functionality

2. **Configure the Network Settings**
   - Update WiFi SSID and password in both sender and receiver code
   - Set the receiver's IP address in the sender code

3. **Upload Code to Devices**
   - Upload `VoiceLink_Sender.ino` to the microphone unit
   - Upload `VoiceLink_Receiver.ino` to the speaker unit

4. **Monitor Serial Output**
   - Use serial monitor at 115200 baud rate to verify connections

## 🚀 Usage

1. Power up both ESP32 devices
2. Wait for WiFi connection (indicated by serial output)
3. Speak into the microphone connected to the sender ESP32
4. Audio will be transmitted to and played by the receiver ESP32

## 🔍 System Details

### Sender Unit
The sender ESP32 reads analog values from the microphone at a fixed sample rate, packages them into UDP packets, and transmits them over WiFi to the receiver.

**Key Features:**
- Adjustable sampling rate
- Noise filtering
- Automatic WiFi reconnection
- Efficient packet transmission

### Receiver Unit
The receiver ESP32 listens for incoming UDP packets, processes the audio data, and outputs it to a speaker via the I2S protocol.

**Key Features:**
- I2S digital audio output
- Audio buffer management
- Signal processing capabilities
- WiFi connection monitoring

## 📊 Performance

- **Sample Rate:** 8000 Hz  
- **Bit Depth:** 16-bit  
- **Latency:** 80-400ms (varies with WiFi signal strength)  
- **Range:** Up to 50m, with increasing packet loss beyond 30m  
- **Battery Life:** ~4 hours on a 1000mAh LiPo battery (estimated)  
- **Audio Quality:** MOS 2.5-4.9 (improves with higher bitrate)  

![Performance Chart](https://raw.githubusercontent.com/codehat01/esp32-voicelink/refs/heads/main/assets/Performance.png)


## 📸 Screenshots & Demo

### Serial Monitor Output Sender:
![Serial Monitor Output](https://raw.githubusercontent.com/codehat01/esp32-voicelink/refs/heads/main/assets/sender.png)


### Serial Monitor Output Receiver:
![Serial Monitor Output](https://raw.githubusercontent.com/codehat01/esp32-voicelink/refs/heads/main/assets/receiver.png)
![Serial Monitor Output](https://raw.githubusercontent.com/codehat01/esp32-voicelink/refs/heads/main/assets/receiver1.png)




### 🎥 Demo Video  
🔗 [Watch the VoiceLink Demo Video](https://streamable.com/xqneb8)

## 🛠️ Troubleshooting

| Issue | Possible Solution |
|-------|-------------------|
| No WiFi connection | Check SSID and password in code |
| No audio output | Verify speaker connections and I2S pins |
| Poor audio quality | Adjust sampling rate or implement filtering |
| High latency | Reduce buffer size or optimize WiFi settings |
| Connection drops | Improve WiFi signal or implement retry logic |

## 🔄 Future Improvements

- [ ] Add volume control
- [ ] Implement noise cancellation
- [ ] Create 3D printable enclosure
- [ ] Add battery monitoring
- [ ] Support for multiple receivers
- [ ] Web interface for configuration
- [ ] Add encryption for secure communications

## 🤝 Contributing

Contributions are welcome! Feel free to fork this repository and submit pull requests.

1. Fork the Project
2. Create your feature branch (`git checkout -b feature/newfeature`)
3. Commit your changes (`git commit -m 'Add some newfeature'`)
4. Push to the branch (`git push origin feature/newfeature`)
5. Open a pull request

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 👏 Acknowledgments

- Thanks to the ESP32 community for excellent documentation
- Inspired by various IoT audio projects
- Special thanks to all open-source audio processing libraries

## 📬 Contact

Project Link: [https://github.com/codehat01/esp32-voicelink](https://github.com/codehat01/esp32-voicelink)

---
Made with ❤️ and ESP32
