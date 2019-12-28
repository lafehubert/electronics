// This program will run tests on the YA4B address register
// The address register can store 8 bits, increment, decrement,
// and put its contents on the address bus.

// Address Bus
#define ADDR_0 22
#define ADDR_1 24
#define ADDR_2 26
#define ADDR_3 28
#define ADDR_4 30
#define ADDR_5 32
#define ADDR_6 34
#define ADDR_7 36

// Control Signals
#define CTRL_LOAD 40
#define CTRL_INC  42
#define CTRL_DEC 44
#define CTRL_OE  46
#define CTRL_CLK 48

#define STR_HIGH "high"
#define STR_LOW  "low"

int address_pins[8] = { ADDR_0, ADDR_1, ADDR_2, ADDR_3, ADDR_4, ADDR_5, ADDR_6, ADDR_7 };
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
  for (int i = CTRL_LOAD; i <= CTRL_CLK; i+=2) {
    if (i != CTRL_CLK) {
      digitalWrite(i, HIGH);
    } else {
      digitalWrite(i, LOW);
    }
    pinMode(i, OUTPUT);
  }
  setAddressInput();
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
  testLoad();
  testIncrement();
  testDecrement();
  testOutputEnable();
}

void testLoad() {
  Serial.println("Begin load address register tests...");
  boolean success = true;
  int address = 1;
  while (address <= 0x80) {
    loadAddress(address);
    delayMicroseconds(1);
    success = success && verifyAddress(address);
    address = address << 1;
  }
  if (success) {
    Serial.println("[PASS] Load address register tests passed.");
  } else {
    Serial.println("[FAIL] Load address register tests failed.");
  }
}

void testIncrement() {
  Serial.println("Begin increment address register tests...");
  boolean success = true;
  int address = 0;
  loadAddress(address);
  while (address < 256) {
    success = success && verifyAddress(address);
    incrementAddress();
    address++;
  }
  if (success) {
    Serial.println("[PASS] Increment address register tests passed.");
  } else {
    Serial.println("[FAIL] Increment address register tests failed.");
  }
}

void testDecrement() {
  Serial.println("Begin decrement address register tests...");
  boolean success = true;
  int address = 0xFF;
  loadAddress(address);
  while (address >= 0) {
    success = success && verifyAddress(address);
    decrementAddress();
    address--;
  }
  if (success) {
    Serial.println("[PASS] Decrement address register tests passed.");
  } else {
    Serial.println("[FAIL] Decrement address register tests failed.");
  }
}

void testOutputEnable() {
  Serial.println("Begin address register output enable tests.");
  boolean success = true;
  for (int address = 0; address < 255; address++) {
    loadAddress(address);
    verifyAddress(address);
    digitalWrite(CTRL_OE, HIGH);
    setAddressInput();
    int addr_bus = readAddress();
    if (addr_bus != 0xFF) {
      char buf[80];
      sprintf(buf, "    [FAIL] Expected 0xff, received %0x", addr_bus);
      Serial.println(buf);
      success = false;
    }
  }
  if (success) {
    Serial.println("[PASS] Address register output enable tests passed.");
  } else {
    Serial.println("[FAIL] Address register output enable tests failed.");
  }
}

void loadAddress(int data) {
  setAddressOutput();
  setAddress(data);
  digitalWrite(CTRL_LOAD, LOW);
  pulseClock();
  digitalWrite(CTRL_LOAD, HIGH);
}

void incrementAddress() {
  digitalWrite(CTRL_INC, LOW);
  pulseClock();
  digitalWrite(CTRL_INC, HIGH);
}

void decrementAddress() {
  digitalWrite(CTRL_DEC, LOW);
  pulseClock();
  digitalWrite(CTRL_DEC, HIGH);
}

boolean verifyAddress(int expected) {
  boolean success = true;
  setAddressInput();
  digitalWrite(CTRL_OE, LOW);
  int address = readAddress();
  digitalWrite(CTRL_OE, HIGH);
  if (address != expected) {
    char buf[80];
    sprintf(buf, "    [FAIL] Expected %0x, received %0x", expected, address);
    Serial.println(buf);
    success = false;
  }
  return success;
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

void toBinary(int bits, int number, char *buf) {
  for (int i = 0; i < bits; i++) {
    buf[i] = '0' + ((number >> i) & 1);
    buf[bits] = 0;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
