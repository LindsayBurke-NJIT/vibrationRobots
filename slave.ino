#include <esp_now.h>
#include <WiFi.h>

// ||------------------------------------------------|| IMU
#include <Wire.h>
#include <LSM6.h>
LSM6 imu;

// ||------------------------------------------------|| Vibration Motors
const int motorPin = 4;  // Pin for the Motor
// Motor Control Variables
const int motorRunTime = 3000;  // Motor run time - 2x IMU stabilization time

// ||------------------------------------------------|| Networking
// master ESP32's MAC address
uint8_t masterMacAddress[] = { 0xF4, 0x12, 0xFA, 0x97, 0xC5, 0x98};
String UUID = "1";

// ||------------------------------------------------|| Data storage
// const int MAX_READINGS = 150;  // Maximum number of readings to store
const int ARRAY_SIZE = 62;  // Size of each array

// int readings[MAX_READINGS];  // Array to store sensor readings
int X1[ARRAY_SIZE];
int X2[ARRAY_SIZE];
int X3[ARRAY_SIZE];

int Y1[ARRAY_SIZE];
int Y2[ARRAY_SIZE];
int Y3[ARRAY_SIZE];

int Z1[ARRAY_SIZE];
int Z2[ARRAY_SIZE];
int Z3[ARRAY_SIZE];

// flags
bool isReadyToSend = false;
bool isSentX = false;
bool isSentY = false;
bool isSentZ = false;

// timer stuff
unsigned long previousTime1 = 0;
unsigned long previousTime2 = 0;
unsigned long previousTime3 = 0;
const unsigned long dataInterval = 2000;  // 1 second interval

// Function to send data to the master
void sendData(const uint8_t* data, int len) {
  esp_now_send(masterMacAddress, data, len);
}

void sendArr(int* arrName, int ARRAY_SIZE) {
  if (esp_now_send(masterMacAddress, (uint8_t*)arrName, ARRAY_SIZE * sizeof(int)) != ESP_OK) {
    Serial.println("Error sending data");
  } else {
    Serial.println("Data sent successfully");
  }
  // Wait before sending again
  delay(100);
}

void sendString(String message) {
  // Convert the string to a char array
  char messageArray[message.length() + 1];
  message.toCharArray(messageArray, sizeof(messageArray));

  // Send the data using ESP-Now
  esp_now_send(masterMacAddress, (uint8_t*)messageArray, strlen(messageArray));
  delay(100);
}

void sendChar(char character) {
  // Send the character code
  if (esp_now_send(masterMacAddress, (uint8_t*)&character, sizeof(character)) != ESP_OK) {
    Serial.println("Error sending data");
  }
}

// Callback function for receiving data
void OnDataRecv(const uint8_t* mac, const uint8_t* data, int len) {
  Serial.print("Data: ");

  // Convert the received data to a string
  String receivedData = String((char*)data);
  Serial.println(receivedData);
  Serial.println(UUID);

  // Check if the received value is '23'
  if (receivedData.equals(UUID)) {
    Serial.println("...");
    // vibrate motors
    vibrateMotor(motorRunTime);
  } else {
    collectData();
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Booting up");

  // Initialize WiFi
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);

  // Add master ESP32
  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, masterMacAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }


  Wire.begin();

  if (!imu.init()) {
    Serial.println("Failed to detect and initialize IMU!");
    while (1);
  }
  imu.enableDefault();

  pinMode(motorPin, OUTPUT);
  stopMotors();
  runMotorDiagnostic();

  Serial.println("Arduino Ready");
}

void loop() {
  if (Serial.available()) {
    // Do nothing in the loop
    int selectID = Serial.read();

    // Convert data to a string
    String dataString = String(selectID);

    // Send data to the master
    sendData((const uint8_t*)dataString.c_str(), dataString.length());
    Serial.println(dataString);
  }

  if (isReadyToSend == true) {
    unsigned long currentTime = millis();

    // Call function 1 every 1 second
    if (currentTime - previousTime1 >= dataInterval && isSentX != true) {
      Serial.println("X");
      sendChar('X');
      delay(100);
      sendArr(X1, ARRAY_SIZE);
      delay(100);
      sendArr(X2, ARRAY_SIZE);
      delay(100);
      sendArr(X3, ARRAY_SIZE);
      delay(100);
      previousTime1 = currentTime;
      isSentX = true;
    }

    // Call function 2 every 1 second after function 1
    if (currentTime - previousTime2 >= dataInterval && currentTime - previousTime1 >= dataInterval && isSentY != true) {
      Serial.println("Y");
      sendChar('Y');
      sendArr(Y1, ARRAY_SIZE);
      delay(100);
      sendArr(Y2, ARRAY_SIZE);
      delay(100);
      sendArr(Y3, ARRAY_SIZE);
      delay(100);
      previousTime2 = currentTime;
      isSentY = true;
    }

    // Call function 3 every 1 second after function 2
    if (currentTime - previousTime3 >= dataInterval && currentTime - previousTime2 >= dataInterval && isSentZ != true) {
      Serial.println("Z");
      sendChar('Z');
      sendArr(Z1, ARRAY_SIZE);
      delay(100);
      sendArr(Z2, ARRAY_SIZE);
      delay(100);
      sendArr(Z3, ARRAY_SIZE);
      delay(100);
      previousTime3 = currentTime;
      isSentZ = true;
    }

    if (isSentX == true && isSentY == true && isSentZ == true) {
      isReadyToSend = false;
    }
  } else {
    previousTime1 = 0;
    previousTime2 = 0;
    previousTime3 = 0;
  }
}
