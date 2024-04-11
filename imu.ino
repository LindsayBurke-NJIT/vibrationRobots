
// ||------------------------------------------------|| IMU
int x, y, z;
int prev_x = 0, prev_y = 0, prev_z = 0;

// ||------------------------------------------------|| IMU stabilization
unsigned long startTime;
unsigned long currentTime;
double stabilizationTime = 3000;
double interval = 3000;

bool printFlag = true;

// ||------------------------------------------------|| Data storage
const int MAX_READINGS = 150;  // Maximum number of readings to store
// const int ARRAY_SIZE = 62;     // Size of each array

int readings[MAX_READINGS];  // Array to store sensor readings
// int X1[ARRAY_SIZE];
// int X2[ARRAY_SIZE];
// int X3[ARRAY_SIZE];

// int Y1[ARRAY_SIZE];
// int Y2[ARRAY_SIZE];
// int Y3[ARRAY_SIZE];

// int Z1[ARRAY_SIZE];
// int Z2[ARRAY_SIZE];
// int Z3[ARRAY_SIZE];

int readingsCount = 0;  // Current number of readings
int XCount = 0;         // Current number of elements in array1
int YCount = 0;         // Current number of elements in array2
int ZCount = 0;         // Current number of elements in array3

bool sendingFlag = true;

void rawReadings() {
  for (int i = 0; i < 100; i++) {
    imu.read();
    x = imu.a.x;
    y = imu.a.y;
    z = imu.a.z;
    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.println(z);
  }
}

void quickRawReadings() {
  for (int i = 0; i < 100; i++) {
    imu.read();
    x = imu.a.x;
    y = imu.a.y;
    z = imu.a.z;
    Serial.print(x);
    Serial.print(" ");
    Serial.print(y);
    Serial.print(" ");
    Serial.println(z);
  }
}


void collectData() {
  // unsigned long timeDiff = millis() - startTime;
  // Serial.println(timeDiff);

  // if (timeDiff >= stabilizationTime && timeDiff <= interval + stabilizationTime) {
  //   printFlag = true;
  // } else {
  //   printFlag = false;
  // }

  sendingFlag = true;

  while (readingsCount < MAX_READINGS) {
    // Serial.println(readingsCount);

    imu.read();
    x = imu.a.x;
    y = imu.a.y;
    z = imu.a.z;

    if (XCount < ARRAY_SIZE) {
      X1[XCount] = x;
      XCount++;
    } else if (XCount >= ARRAY_SIZE && XCount < ARRAY_SIZE * 2) {
      X2[XCount - ARRAY_SIZE] = x;
      XCount++;
    } else if (XCount >= ARRAY_SIZE * 2 && XCount < ARRAY_SIZE * 3) {
      X3[XCount - (ARRAY_SIZE * 2)] = x;
      XCount++;
    }

    if (YCount < ARRAY_SIZE) {
      Y1[YCount] = y;
      YCount++;
    } else if (YCount >= ARRAY_SIZE && YCount < ARRAY_SIZE * 2) {
      Y2[YCount - ARRAY_SIZE] = y;
      YCount++;
    } else if (YCount >= ARRAY_SIZE * 2 && YCount < ARRAY_SIZE * 3) {
      Y3[YCount - (ARRAY_SIZE * 2)] = y;
      YCount++;
    }

    if (ZCount < ARRAY_SIZE) {
      Z1[ZCount] = z;
      ZCount++;
    } else if (ZCount >= ARRAY_SIZE && ZCount < ARRAY_SIZE * 2) {
      Z2[ZCount - ARRAY_SIZE] = z;
      ZCount++;
    } else if (ZCount >= ARRAY_SIZE * 2 && ZCount < ARRAY_SIZE * 3) {
      Z3[ZCount - (ARRAY_SIZE * 2)] = z;
      ZCount++;
    }
    readingsCount++;
  }

  if (sendingFlag == true) {
    Serial.println(XCount);
    Serial.println(YCount);
    Serial.println(ZCount);
    // Serial.println("X1");
    // for (int i = 0; i < ARRAY_SIZE; i++) {
    //   Serial.println(Y1[i]);
    // }
    // Serial.println("X2");
    // for (int i = 0; i < ARRAY_SIZE; i++) {
    //   Serial.println(Y2[i]);
    // }
    // Serial.println("X3");
    // for (int i = 0; i < ARRAY_SIZE; i++) {
    //   Serial.println(Y3[i]);
    // }

    int multipliedValue = UUID.toInt() * 3000;
    delay(multipliedValue);

    // Send ESP Header
    String myString = "ESP" + UUID;
    sendString(myString);

    isReadyToSend = true;
    sendingFlag = false;
  }
}
