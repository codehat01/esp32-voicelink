#include <WiFi.h>
#include <WiFiUdp.h>
#include <driver/i2s.h>

// Configuration
#define I2S_BCLK 26           // I2S BCLK pin connected to esp32
#define I2S_LRC 25            // I2S LRC/WS pin connected to esp32
#define I2S_DOUT 22           // I2S Data out pin connected to esp32
#define SAMPLE_RATE 8000      // Audio sample rate in Hz
#define WIFI_TIMEOUT 10000    // WiFi connection timeout in ms
#define UDP_PACKET_SIZE 512   // Size of UDP packet

// I2S configuration
#define I2S_PORT I2S_NUM_0

// WiFi credentials
const char* ssid = "your ssid here";
const char* password = "password here";
const int udpPort = 1234; //ur wished port
WiFiUDP udp;
unsigned long lastWifiCheck = 0;
uint8_t packetBuffer[UDP_PACKET_SIZE];
bool connectToWiFi();
void checkWiFiConnection();
void initI2S();
void processAudioPacket(int packetSize);
void setup() {
  Serial.begin(115200);
  Serial.println("\nESP32 Audio Receiver Starting...");
  WiFi.mode(WIFI_STA);
  if (!connectToWiFi()) {
    Serial.println("Initial WiFi connection failed. Will retry in loop...");
  } else {
    udp.begin(udpPort);
    Serial.println("UDP server started on port " + String(udpPort));
  }
  initI2S();
}

void loop() {
  if (millis() - lastWifiCheck > 10000) {
    checkWiFiConnection();
    lastWifiCheck = millis();
  }
  if (WiFi.status() == WL_CONNECTED) {
    int packetSize = udp.parsePacket();
    if (packetSize) {
      processAudioPacket(packetSize);
    }
  }
  delay(1);
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
    if (connectToWiFi()) {
      udp.begin(udpPort);
      Serial.println("UDP server restarted on port " + String(udpPort));
    }
  }
}

void initI2S() {
  Serial.println("Initializing I2S...");
  esp_err_t err;
  
  // I2S configuration
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
  };
  
  // I2S pin configuration
  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_BCLK,
    .ws_io_num = I2S_LRC,
    .data_out_num = I2S_DOUT,
    .data_in_num = I2S_PIN_NO_CHANGE
  };
  
  // Install and start I2S driver
  err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  if (err != ESP_OK) {
    Serial.printf("Failed to install I2S driver: %d\n", err);
    return;
  }
  
  // Set I2S pins
  err = i2s_set_pin(I2S_PORT, &pin_config);
  if (err != ESP_OK) {
    Serial.printf("Failed to set I2S pins: %d\n", err);
    return;
  }
  
  Serial.println("I2S initialized successfully");
}

void processAudioPacket(int packetSize) {
  int bytesRead = udp.read(packetBuffer, min(packetSize, UDP_PACKET_SIZE));
  Serial.print("Received audio packet: ");
  Serial.print(bytesRead);
  Serial.println(" bytes");
  size_t i2s_bytes_written = 0;
  int16_t samples[UDP_PACKET_SIZE];
  int sampleCount = 0;
  for (int i = 0; i < bytesRead; i += 2) {
    if (i + 1 < bytesRead) {
      int16_t sample = packetBuffer[i] | (packetBuffer[i + 1] << 8);
      if (sample >= 2048) {
        sample = (sample - 2048) * 16;
      } else {
        sample = (sample - 2048) * 16;
      }
      samples[sampleCount++] = sample;  // Left channel
      samples[sampleCount++] = sample;  // Right channel
    }
  }
  if (sampleCount > 0) {
    i2s_write(I2S_PORT, samples, sampleCount * sizeof(int16_t), &i2s_bytes_written, 100);
    Serial.printf("Wrote %d bytes to I2S\n", i2s_bytes_written);
  }
}