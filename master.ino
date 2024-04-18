#include <esp_now.h>
#include <WiFi.h>

#define ARRAY_SIZE 150
// Define the array to receive
int myArray[ARRAY_SIZE];
#define MAX_ARRAY_SIZE 62


String readString;

// slave ESP32's MAC address
uint8_t macArr[3][6] = {
  {0xF4, 0x12, 0xFA, 0x97, 0xDB, 0x10}, //uuid1
  {0x70, 0x04, 0x1D, 0xCD, 0x41, 0x60}, //uuid2
  {0x70, 0x04, 0x1D, 0xCE, 0x28, 0x54} //uuid3
};

int currEsp = 0;

void sendData(const uint8_t *data, int len, int currEsp){
  Serial.print(F("Sending data to "));
  Serial.println(currEsp);
  //Serial.println("Sending data to "+(currEsp));
  esp_now_send(macArr[currEsp-1], data, len);
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

  while(!Serial.available())
  {// do nothing
  }
  while(Serial.available())
  {Serial.read();
  }

  // Initialize WiFi
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);


  //Add ESP slaves from macArray
  esp_now_peer_info_t peerInfo;
  for(int macIndex=0;macIndex<sizeof(macArr)/sizeof(uint8_t);macIndex++){
      memset(&peerInfo, 0, sizeof(peerInfo));
      memcpy(peerInfo.peer_addr, macArr[macIndex], 6);
      peerInfo.channel = 0;
      peerInfo.encrypt = false;
      if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println(F("Failed to add peer"));
        return;
      }
      else
      {Serial.println(F("Added Peer "+(macIndex+1)));
      }
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
    sendData((const uint8_t*)readString.c_str(), readString.length(), 1);
    delay(10);
    sendData((const uint8_t*)readString.c_str(), readString.length(), 2);
    delay(10);
    sendData((const uint8_t*)readString.c_str(), readString.length(), 3);
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
