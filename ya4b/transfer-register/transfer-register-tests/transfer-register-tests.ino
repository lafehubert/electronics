// This program will run tests on the YA4B transfer register
// The transfer register allows the transfer of values to and from 
// the 4-bit data registers to the 8-bit address registers.

// Address Bus
#define ADDR_0 22
#define ADDR_1 24
#define ADDR_2 26
#define ADDR_3 28
#define ADDR_4 30
#define ADDR_5 32
#define ADDR_6 34
#define ADDR_7 36

// Data Bus
#define DATA_0 23
#define DATA_1 25
#define DATA_2 27
#define DATA_3 29

// Control Signals
#define CTRL_LOAD_TH 40
#define CTRL_LOAD_TL 42

#define CTRL_OE_TH 44
#define CTRL_OE_TL 46

#define CTRL_OE_T 48

#define CTRL_BUS_SELECT 50

#define CTRL_CLK 52

#define STR_HIGH "high"
#define STR_LOW  "low"

int address_pins[8] = { ADDR_0, ADDR_1, ADDR_2, ADDR_3, ADDR_4, ADDR_5, ADDR_6, ADDR_7 };
int data_pins[4] = { DATA_0, DATA_1, DATA_2, DATA_3 };
const char* states[2] = { STR_LOW, STR_HIGH };

void setup() {
  Serial.begin(57600);
  Serial.println("Starting up...");
  Serial.println("");
  randomSeed(analogRead(0));
  setupPins();
  runTests();
}

void setupPins() {
  for (int i = CTRL_LOAD_TH; i <= CTRL_CLK; i+=2) {
    if ((i != CTRL_CLK && i) != (CTRL_BUS_SELECT)) {
      digitalWrite(i, HIGH);
    } else {
      digitalWrite(i, LOW);
    }
    pinMode(i, OUTPUT);
  }
  setDataInput();
  setAddressInput();
}

void setDataInput() {
  for (int i = DATA_0; i <= DATA_3; i+=2) {
    pinMode(i, INPUT_PULLUP);
  }
}

void setDataOutput() {
  for (int i = DATA_0; i <= DATA_3; i+=2) {
    pinMode(i, OUTPUT);
  }
}

void setAddressInput() {
  for (int i = ADDR_0; i <= ADDR_7; i+=2) {
    pinMode(i, INPUT_PULLUP);
  }
}

void setAddressOutput() {
  for (int i = ADDR_0; i <= ADDR_7; i+=2) {
    pinMode(i, OUTPUT);
  }
}

void runTests() {
  testLoadDataHigh();
  testLoadDataLow();
  testLoadAddress();
  testOutputEnable();
}

void foo() {
  Serial.println("Starting load from address test.");
  setAddressOutput();
  setAddress(0x01);
  digitalWrite(CTRL_BUS_SELECT, LOW);
  digitalWrite(ADDR_1, LOW);
}

void testLoadDataHigh() {
  Serial.println("Starting load high test.");
  int data = 1;
  while (data <= 0x8) {
    loadData(CTRL_LOAD_TH, data);
    delayMicroseconds(1);
    verifyData(CTRL_OE_TH, data);
    data = data << 1;
  }
}

void testLoadDataLow() {
  Serial.println("Starting load low test.");
  int data = 1;
  while (data <= 0x8) {
    loadData(CTRL_LOAD_TL, data);
    delayMicroseconds(1);
    verifyData(CTRL_OE_TL, data);
    data = data << 1;
  }
}

void testLoadAddress() {
  Serial.println("Starting load from address test.");
  int address = 1;
  while (address <= 0x80) {
    loadAddress(address);
    delayMicroseconds(1);
    verifyAddress(address);
    address = address << 1;
  }
}

void testOutputEnable() {
  Serial.println("Starting output enable test.");
  int address = random(0, 256);
  while (address != 255) {
    address = random(0, 256);
  }
  loadAddress(address);
  delayMicroseconds(1);
  int data_low = address & 0xf;
  int data_high = (address & 0xf0) >> 4;
  verifyAddress(address);
  verifyData(CTRL_OE_TL, data_low);
  verifyData(CTRL_OE_TH, data_high);

  digitalWrite(CTRL_OE_T, HIGH);
  digitalWrite(CTRL_OE_TL, HIGH);
  digitalWrite(CTRL_OE_TH, HIGH);

  int data = readData();
  if (data != 0xf) {
    char buf[80];
    sprintf(buf, "[FAIL] Expected 0xf, received %x", data);
    Serial.println(buf);
  } else {
    Serial.println("[PASS] All data lines are high.");
  }

  address = readData();
  if (address != 0xf) {
    char buf[80];
    sprintf(buf, "[FAIL] Expected 0xff, received %x", address);
    Serial.println(buf);
  } else {
    Serial.println("[PASS] All address lines are high.");
  }
}

void loadData(int loadPin, int data) {
  setDataOutput();
  setData(data);
  digitalWrite(CTRL_BUS_SELECT, HIGH);
  digitalWrite(loadPin, LOW);
  pulseClock();
  digitalWrite(loadPin, HIGH);
}

void verifyData(int outputEnablePin, int expected) {
  setDataInput();
  digitalWrite(outputEnablePin, LOW);
  int data = readData();
  digitalWrite(outputEnablePin, HIGH);
  if (data != expected) {
    char buf[80];
    sprintf(buf, "[FAIL] Expected %x, received %x", expected, data);
    Serial.println(buf);
  } else {
    Serial.println("[PASS] Load data passed.");
  }
}

void loadAddress(int data) {
  setAddressOutput();
  setAddress(data);
  digitalWrite(CTRL_BUS_SELECT, LOW);
  digitalWrite(CTRL_LOAD_TH, LOW);
  digitalWrite(CTRL_LOAD_TL, LOW);
  pulseClock();
  digitalWrite(CTRL_LOAD_TH, HIGH);
  digitalWrite(CTRL_LOAD_TL, HIGH);
}

void verifyAddress(int expected) {
  setAddressInput();
  digitalWrite(CTRL_OE_T, LOW);
  int address = readAddress();
  digitalWrite(CTRL_OE_T, HIGH);
  if (address != expected) {
    char buf[80];
    sprintf(buf, "[FAIL] Expected %0x, received %0x", expected, address);
    Serial.println(buf);
  } else {
    Serial.println("[PASS] Load from address passed.");
  }
}

void pulseClock() {
  digitalWrite(CTRL_CLK, LOW);
  delayMicroseconds(1);
  digitalWrite (CTRL_CLK, HIGH);
  delayMicroseconds(1);
  digitalWrite(CTRL_CLK, LOW);
}

void setAddress(int address) {
  int values[8];
  for (int i = 0; i < 8; i++) {
    values[i] = (address >> i) & 1;
    digitalWrite(address_pins[i], values[i]);
  }
}

int readAddress() {
  int address = 0;
  for (int i = 7; i >= 0; i--) {
    address = (address << 1) | digitalRead(address_pins[i]);
  }
  return address;
}

void setData(int data) {
  int values[4];
  for (int i = 0; i < 4; i++) {
    values[i] = (data >> i) & 1;
    digitalWrite(data_pins[i], values[i]);
  }
}

int readData() {
  int data = 0;
  for (int i = 3; i >= 0; i--) {
    data = (data << 1) | digitalRead(data_pins[i]);
  }
  return data;
}

void toBinary(int bits, int number, char *buf) {
  for (int i = 0; i < bits; i++) {
    buf[i] = '0' + ((number >> i) & 1);
    buf[bits] = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
