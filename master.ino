#include <esp_now.h>
#include <WiFi.h>

#define ARRAY_SIZE 150
// Define the array to receive
int myArray[ARRAY_SIZE];
#define MAX_ARRAY_SIZE 62

// slave ESP32's MAC address
uint8_t mac1[] = { 0xF4, 0x12, 0xFA, 0x97, 0xDB, 0x10 };
uint8_t mac2[] = { 0x70, 0x04, 0x1D, 0xCD, 0x41, 0x60 };
uint8_t mac3[] = { 0xF4, 0x12, 0xFA, 0x97, 0x96, 0xD0 };
uint8_t mac4[] = { 0x70, 0x04, 0x1D, 0xCE, 0x28, 0x54 };
uint8_t mac5[] = { 0xF4, 0x12, 0xFA, 0x97, 0x97, 0x94 };

String readString;

void sendData1(const uint8_t* data, int len) {
  Serial.println("Sending data to 1");
  esp_now_send(mac1, data, len);
  //  delay(100);
}
void sendData2(const uint8_t* data, int len) {
  Serial.println("Sending data to 2");
  esp_now_send(mac2, data, len);
  //  delay(100);
}
void sendData3(const uint8_t* data, int len) {
  Serial.println("Sending data to 3");
  esp_now_send(mac3, data, len);
  //  delay(100);
}
void sendData4(const uint8_t* data, int len) {
  Serial.println("Sending data to 4");
  esp_now_send(mac4, data, len);
  //  delay(100);
}
void sendData3(const uint8_t* data, int len) {
  Serial.println("Sending data to 5");
  esp_now_send(mac5, data, len);
  //  delay(100);
}

// Callback function for receiving data
void OnDataRecv(const uint8_t* mac, const uint8_t* data, int len) {
  // Check if the received data is an array
  if (len == MAX_ARRAY_SIZE * sizeof(int)) {
    int *receivedArray = (int *)data;
    // Process the received array
    //    Serial.println("Received an array:");
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
      Serial.print(receivedArray[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
  // Check if the received data is a string
  else if (len > 0) {
    // Check if the received data is a single character
    if (len == sizeof(char)) {
      char receivedChar = *((char*)data);
      // Process the received character
      Serial.print("Received a character: ");
      Serial.println(receivedChar);
    }
    // Process the received string
    else {
      String receivedString = String((const char*)data);
      Serial.print("Received a string: ");
      Serial.println(receivedString);
    }
  }

  // Handle received data
  //  Serial.print("Received data from: ");
  //  for (int i = 0; i < 6; ++i) {
  //    Serial.print(mac[i], HEX);
  //    if (i < 5) {
  //      Serial.print(":");
  //    }
  //  }
  //  Serial.println();
  //
  //  // Print received data
  //  Serial.print("Data: ");
  //  for (int i = 0; i < len; ++i) {
  //    Serial.print((char)data[i]);
  //  }
  //  Serial.println();
}

void setup() {
  Serial.begin(115200);

  // Initialize WiFi
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);

  // Add slave ESP32
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, mac1, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  // Add slave ESP32
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, mac2, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  // Add slave ESP32
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, mac3, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
    // Add slave ESP32
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, mac4, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
    // Add slave ESP32
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, mac5, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the String readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }
  if (readString.length() > 0) {
    // Send data to the master
    sendData1((const uint8_t*)readString.c_str(), readString.length());
    delay(10);
    sendData2((const uint8_t*)readString.c_str(), readString.length());
    delay(10);
    sendData3((const uint8_t*)readString.c_str(), readString.length());
    Serial.println(readString);
    sendData4((const uint8_t*)readString.c_str(), readString.length());
    Serial.println(readString);
    sendData5((const uint8_t*)readString.c_str(), readString.length());
    Serial.println(readString);
    delay(10);
    readString = "";
  }
  //  if (Serial.available()) {
  // Do nothing in the loop
  //    int selectID = Serial.read();
  //
  //    // Convert sensor data to a string
  //    char dataString = char(selectID);



  // Send data to the master
  //    sendData1((const uint8_t*)&dataString, sizeof(dataString));
  //    delay(10);
  //    sendData2((const uint8_t*)&dataString, sizeof(dataString));
  //    delay(10);
  //    sendData3((const uint8_t*)&dataString, sizeof(dataString));
  //    Serial.println(dataString);
  //    delay(10);
  //  }
}
