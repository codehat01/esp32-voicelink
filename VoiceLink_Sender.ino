#include <WiFi.h>
#include <WiFiUdp.h>

// Configuration
#define MIC_PIN 39              // Analog pin for KY-037 microphone
#define SAMPLE_RATE 8000        // Audio sample rate in Hz
#define WIFI_TIMEOUT 10000      // WiFi connection timeout in ms
#define WIFI_RETRY_DELAY 5000   // Delay between WiFi reconnection attempts
#define UDP_PACKET_SIZE 512     // Size of UDP packet (multiple samples for efficiency)

// WiFi credentials
const char* ssid = "your ssid here";
const char* password = "passwordhere";
const char* receiverIP = "esp32ip"; 
const int udpPort = 1234;

// Global variables
WiFiUDP udp;
unsigned long lastWifiCheck = 0;
unsigned long lastSampleTime = 0;
int sampleInterval;

// Buffer for audio samples
uint16_t audioBuffer[UDP_PACKET_SIZE/2];
int bufferIndex = 0;

// Function prototypes
bool connectToWiFi();
void sendAudioData();
void checkWiFiConnection();

void setup() {
  Serial.begin(115200);
  Serial.println("\nESP32 Audio Sender Starting...");
  sampleInterval = 1000000 / SAMPLE_RATE;
  analogReadResolution(12);
  WiFi.mode(WIFI_STA);
  if (!connectToWiFi()) {
    Serial.println("Initial WiFi connection failed. Will retry in loop...");
  }
}
void loop() {
  if (millis() - lastWifiCheck > 10000) {
    checkWiFiConnection();
    lastWifiCheck = millis();
  }
  if (WiFi.status() == WL_CONNECTED) {
    if (micros() - lastSampleTime >= sampleInterval) {
      lastSampleTime = micros();
      int micValue = analogRead(MIC_PIN);
      if (abs(micValue - 2048) < 50) {
        micValue = 2048;
      }
      audioBuffer[bufferIndex++] = (uint16_t)micValue;
      if (bufferIndex >= UDP_PACKET_SIZE/2) {
        sendAudioData();
        bufferIndex = 0;
      }
    }
  }
}
bool connectToWiFi() {
  Serial.print("Connecting to WiFi ");
  Serial.print(ssid);
  unsigned long startAttemptTime = millis();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && 
         millis() - startAttemptTime < WIFI_TIMEOUT) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    return true;
  } else {
    Serial.println("\nFailed to connect to WiFi!");
    WiFi.disconnect();
    return false;
  }
}
void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Attempting to reconnect...");
    WiFi.disconnect();
    delay(1000);
    connectToWiFi();
  }
}

void sendAudioData() {
  if (WiFi.status() == WL_CONNECTED) {
    udp.beginPacket(receiverIP, udpPort);
    udp.write((uint8_t*)audioBuffer, UDP_PACKET_SIZE);
    if (udp.endPacket()) {
      //Uncomment for debugging
      // Serial.println("Packet sent");
    } else {
      Serial.println("Failed to send packet");
    }
  }
}