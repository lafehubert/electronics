// This program will run tests on 32-word RAM board for YA4B
// This uses the ram-select chip
/*
 * ram_selector_jumpers.pld:
 * 
GAL16V8    ; RAM-select for YA4B RAM Board
RamSelect  ;


CLK   A7    A6    A5    A4    J2    J1    J0    RD   GND
NC    CS0   CS1   OE    W0    W1    NC    BS    WR   VCC

/BS = A7 * J2 * A6 * J1 * A5 * J0 +
     A7 * J2 * A6 * J1 * /A5 * /J0 +
     A7 * J2 * /A6 * /J1 * A5 * J0 +
     A7 * J2 * /A6 * /J1 * /A5 * /J0 +
     /A7 * /J2 * A6 * J1 * A5 * J0 +
     /A7 * /J2 * A6 * J1 * /A5 * /J0 +
     /A7 * /J2 * /A6 * /J1 * A5 * J0 +
     /A7 * /J2 * /A6 * /J1 * /A5 * /J0

/CS0 = A7 * J2 * A6 * J1 * A5 * J0 * /A4 +
     A7 * J2 * A6 * J1 * /A5 * /J0 * /A4 +
     A7 * J2 * /A6 * /J1 * A5 * J0 * /A4 +
     A7 * J2 * /A6 * /J1 * /A5 * /J0 * /A4 +
     /A7 * /J2 * A6 * J1 * A5 * J0 * /A4 +
     /A7 * /J2 * A6 * J1 * /A5 * /J0 * /A4 +
     /A7 * /J2 * /A6 * /J1 * A5 * J0 * /A4 +
     /A7 * /J2 * /A6 * /J1 * /A5 * /J0 * /A4

/CS1 = A7 * J2 * A6 * J1 * A5 * J0 * A4 +
     A7 * J2 * A6 * J1 * /A5 * /J0 * A4 +
     A7 * J2 * /A6 * /J1 * A5 * J0 * A4 +
     A7 * J2 * /A6 * /J1 * /A5 * /J0 * A4 +
     /A7 * /J2 * A6 * J1 * A5 * J0 * A4 +
     /A7 * /J2 * A6 * J1 * /A5 * /J0 * A4 +
     /A7 * /J2 * /A6 * /J1 * A5 * J0 * A4 +
     /A7 * /J2 * /A6 * /J1 * /A5 * /J0 * A4

/OE = A7 * J2 * A6 * J1 * A5 * J0 * /RD +
     A7 * J2 * A6 * J1 * /A5 * /J0 * /RD +
     A7 * J2 * /A6 * /J1 * A5 * J0 * /RD +
     A7 * J2 * /A6 * /J1 * /A5 * /J0 * /RD +
     /A7 * /J2 * A6 * J1 * A5 * J0 * /RD +
     /A7 * /J2 * A6 * J1 * /A5 * /J0 * /RD +
     /A7 * /J2 * /A6 * /J1 * A5 * J0 * /RD +
     /A7 * /J2 * /A6 * /J1 * /A5 * /J0 * /RD

/W0 = A7 * J2 * A6 * J1 * A5 * J0 * /A4 * /WR * CLK +
     A7 * J2 * A6 * J1 * /A5 * /J0 * /A4 * /WR * CLK +
     A7 * J2 * /A6 * /J1 * A5 * J0 * /A4 * /WR * CLK +
     A7 * J2 * /A6 * /J1 * /A5 * /J0 * /A4 * /WR * CLK +
     /A7 * /J2 * A6 * J1 * A5 * J0 * /A4 * /WR * CLK +
     /A7 * /J2 * A6 * J1 * /A5 * /J0 * /A4 * /WR * CLK +
     /A7 * /J2 * /A6 * /J1 * A5 * J0 * /A4 * /WR * CLK +
     /A7 * /J2 * /A6 * /J1 * /A5 * /J0 * /A4 * /WR * CLK

/W1 = A7 * J2 * A6 * J1 * A5 * J0 * A4 * /WR * CLK +
     A7 * J2 * A6 * J1 * /A5 * /J0 * A4 * /WR * CLK +
     A7 * J2 * /A6 * /J1 * A5 * J0 * A4 * /WR * CLK +
     A7 * J2 * /A6 * /J1 * /A5 * /J0 * A4 * /WR * CLK +
     /A7 * /J2 * A6 * J1 * A5 * J0 * A4 * /WR * CLK +
     /A7 * /J2 * A6 * J1 * /A5 * /J0 * A4 * /WR * CLK +
     /A7 * /J2 * /A6 * /J1 * A5 * J0 * A4 * /WR * CLK +
     /A7 * /J2 * /A6 * /J1 * /A5 * /J0 * A4 * /WR * CLK

DESCRIPTION


This replaces a bunch of combinatorial logic
used for selecting one of two RAM chips based
on the A4 line, and routes the proper RD and WR
signals to the chips, and also controls a bus
transciever's output enable.
 *
 * pinout:

  
                                GAL16V8

                          -------\___/-------
                      CLK |  1           20 | VCC
                          |                 |
                       A7 |  2           19 | WR
                          |                 |
                       A6 |  3           18 | BS
                          |                 |
                       A5 |  4           17 | NC
                          |                 |
                       A4 |  5           16 | W1
                          |                 |
                       J2 |  6           15 | W0
                          |                 |
                       J1 |  7           14 | OE
                          |                 |
                       J0 |  8           13 | CS1
                          |                 |
                       RD |  9           12 | CS0
                          |                 |
                      GND | 10           11 | NC
                          -------------------

 */

// outputs
#define ADDR_0 22
#define ADDR_1 24
#define ADDR_2 26
#define ADDR_3 28
#define ADDR_4 30
#define ADDR_5 32
#define ADDR_6 34
#define ADDR_7 36

#define SEL_0 38
#define SEL_1 40
#define SEL_2 42

#define CTRL_RD 44
#define CTRL_WR 46
#define CTRL_CLK 48

// data
#define DATA_0 23
#define DATA_1 25
#define DATA_2 27
#define DATA_3 29

#define STR_HIGH "high"
#define STR_LOW  "low"

#define SIG_HIGH true
#define SIG_LOW false

int address_pins[8] = { ADDR_0, ADDR_1, ADDR_2, ADDR_3, ADDR_4, ADDR_5, ADDR_6, ADDR_7 };
int jumper_pins[3] = { SEL_0, SEL_1, SEL_2 };
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
  for (int i = ADDR_0; i <= CTRL_CLK; i+=2) {
    pinMode(i, OUTPUT);
  }
  setDataInput();
  setRead(SIG_HIGH);
  setWrite(SIG_HIGH);
  setClock(SIG_LOW);
  setJumpers(0);
  setAddress(0);
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

void generateRandomData(int *data) {
  for (int i=0; i<32; i++) {
    data[i] = random(0,15);
  }
  char buf[120];
  sprintf(buf, "Data [ %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x, %x ]",
    data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
    data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[16],
    data[16], data[17], data[18], data[19], data[20], data[21], data[22], data[23],
    data[24], data[25], data[26], data[27], data[28], data[29], data[30], data[31]);
  Serial.println(buf);
}

void runTests() {
  runRandomDataTest();
  runControlLineReadTest();
  runControlLineWriteTest();
}

void runRandomDataTest() {
  Serial.println("Begin random data test.");
  int random_data[32];
  generateRandomData(random_data);
  int page = 0;
  setJumpers(page);
  Serial.println("Writing random data to RAM...");
  presetMemory(page, random_data);
  Serial.println("Reading and validating data in RAM...");
  if (validateMemory(page, random_data)) {
    Serial.println("[PASS] All memory read back correctly.");
  } else {
    Serial.println("[FAIL] Some reads failed.");
  }
}

void runControlLineReadTest() {
  Serial.println("Begin control line read test.");
  int all_zeros[32];
  memset(all_zeros, 0, sizeof(int) * 32);
  int page = 0;
  setJumpers(page);
  presetMemory(page, all_zeros);
  boolean pass = true;
  if (validateMemory(page, all_zeros)) {
    for (page = 1; page < 8; page++) {
      for (int address = 0; address < 32; address++) {
        int data = readWord(page*32 + address);
        if (data == 15) {
          char buf[80];
          sprintf(buf, "[FAIL] Expected 15, got %d", data);
          Serial.println(buf);
          pass = false; 
        }
      }
    }
  } else {
    Serial.println("[FAIL] Preconditions - writes or reads failed.");
    pass = false;
  }
  if (pass) {
    Serial.println("[PASS] Control line read tests passed.");
  } else {
    Serial.println("[FAIL] Control line read tests failed.");
  }
}

void runControlLineWriteTest() {
  Serial.println("Begin control line write test.");
  int random_data[32];
  generateRandomData(random_data);
  int page = 0;
  setJumpers(page);
  Serial.println("Writing random data to RAM...");
  presetMemory(page, random_data);
  boolean pass = true;
  if (validateMemory(page, random_data)) {
    int all_zeros[32];
    memset(all_zeros, 0, sizeof(int) * 32);
    for (page = 1; page < 8; page++) {
      char buf[80];
      sprintf(buf, "Testing writes to page %d", page);
      Serial.println(buf);
      presetMemory(page, all_zeros);
      if (!validateMemory(0, random_data)) {
        sprintf(buf, "[FAIL] Writing to page %d changed data in page 0", page);
        Serial.println(buf);
        pass = false;
      }
    }
  } else {
    Serial.println("[FAIL] Preconditions - writes or reads failed.");
    pass = false;
  }
  if (pass) {
    Serial.println("[PASS] Control line write tests passed.");
  } else {
    Serial.println("[FAIL] Control line write tests failed.");
  }
}

void presetMemory(int page, const int* values) {
  for (int address = 0; address < 32; address++) {
    writeWord(page * 32 + address, values[address]);
  }
}

boolean validateMemory(int page, const int* values) {
  boolean pass = true;
  for (int address = 0; address < 32; address++) {
    int data = readWord(page * 32 + address);
    if (data != values[address]) {
     char buf[80];
      sprintf(buf, "[FAIL]: expected %x, read %x, at page %d, address %d", values[address], data, page, address);
      Serial.println(buf);
      pass = false;
    }
  }
  return pass;
}

void writeWord(int address, int data) {
  setAddress(address);
  setData(data & 0xF);
  doWrite();
}

int readWord(int address) {
  setAddress(address);
  return doRead();
}

void doWrite() {
  setDataOutput();
  setWrite(SIG_LOW);
  delay(1);
  setClock(SIG_HIGH);
  delayMicroseconds(100);
  setClock(SIG_LOW);
  delay(1);
  setWrite(SIG_HIGH);
}

int doRead() {
  setDataInput();
  setRead(SIG_LOW);
  delay(1);
  int data = readData();
  setRead(SIG_HIGH);
  return data;
}

void setRead(boolean read) {
  digitalWrite(CTRL_RD, read);
}

void setWrite(boolean write) {
  digitalWrite(CTRL_WR, write);
}

void setClock(boolean clock) {
  digitalWrite(CTRL_CLK, clock);
}

void setJumpers(int address) {
  int values[3];
  for (int i = 0; i < 3; i++) {
    values[i] = (address >> i) & 1;
    digitalWrite(jumper_pins[i], values[i]);
  }
}

void setAddress(int address) {
  int values[8];
  for (int i = 0; i < 8; i++) {
    values[i] = (address >> i) & 1;
    digitalWrite(address_pins[i], values[i]);
  }
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
