/*
 * This program will run tests on the YA4B data register bank
 * 
 * The data bank register contains four 4-bit data registers,
 * address 00-01.  The data register bank can set a base
 * register address of 00-11 (register addreses are 4 bits).
 *
 * The data register bank has an "A" side and a "B" side;
 * the "A" side controls which register is outputting on the 
 * data bus, and the "B" side controls which register is
 * being loaded from the data bus.
 *
 * The reset signal to the board resets all registers to 0 and 
 * should be connected to the CPU reset signal.
 */

#include "Ya4bUtil.h"

#define STR_HIGH "high"
#define STR_LOW  "low"

Ya4bUtil utils;

#define BASE_REGISTER_ADDRESS 0b00

const char* states[2] = { STR_LOW, STR_HIGH };

void setup() 
{
  Serial.begin(57600);
  Serial.println("Starting up...");
  Serial.println("");
  randomSeed(analogRead(0));
  utils.setupPins();
  runTests();
  //smokeTest();
}

void smokeTest() {
  utils.pulseReset(1);
  loadData(0, 4);
  delayMicroseconds(1);
  utils.setDataInput();
  //utils.assertReadRegisterAddress(0);
  digitalWrite(REG_A0, 0);
  digitalWrite(REG_A1, 0);
  digitalWrite(REG_A2, 0);
  digitalWrite(REG_A3, 0);
  utils.assertRegisterOutputEnable();
  int data = utils.readData();
  Serial.println("done");
}

void runTests() 
{
  utils.pulseReset(1);
  delay(1);
  testLoad();
  testOutputEnable();
}

void testLoad() {
  Serial.println("Starting load test.");
  for (int registerAddress = 0; registerAddress < 4; registerAddress++) 
  {
    int data = 1;
    while (data <= 0x8) 
    {
      loadData(registerAddress, data);
      delayMicroseconds(1);
      verifyData("load", registerAddress, data);
      data = data << 1;
    }
  }
  resetRegisterAddresses();
  Serial.println("Done with load test.\n");
}


void testOutputEnable() 
{
  Serial.println("Starting output enable test.");
  int data[4];
  setRandomData(data);
  delayMicroseconds(1);
  utils.setDataInput();
  for (int i = 0; i < 4; i++) {
    verifyData("outputEnable", i, data[i]);
  }
  Serial.println("Done with output enable test.\n");
}

void setRandomData(int *data) {
  for (int i = 0; i <= 0b11; i++) {
    do {
      data[i] = random(0,16);
    } while (data[i] != 0xF && notExists(data, i, data[i]));
    loadData(i, data[i]);
  }
}

boolean notExists(const int *data, int max, int value) {
  for (int i=0; i < max; i++) {
    if (data[i] == value) {
      return true;
    }
  }
  return false;
}


void loadData(int registerAddress, int data) 
{
  utils.setDataOutput();
  utils.assertWriteRegisterAddress((BASE_REGISTER_ADDRESS << 2) + registerAddress);
  utils.assertData(data);
  utils.assertRegisterLoad();
  utils.pulseClock(1);
  utils.resetControlPins();
}

void verifyData(const char *operation, int registerAddress, int expected) 
{
  utils.setDataInput();
  utils.assertReadRegisterAddress((BASE_REGISTER_ADDRESS << 2) + registerAddress);
  utils.assertRegisterOutputEnable();
  int data = utils.readData();
  utils.resetControlPins();
  if (data != expected) 
  {
    char buf[80];
    sprintf(buf, "[FAIL] Expected %0x, received %0x", expected, data);
    Serial.println(buf);
  } 
  else 
  {
    char buf[80];
    sprintf(buf, "[PASS] Operation %s passed.", operation);
    Serial.println(buf);
  }
}

void resetRegisterAddresses() {
  utils.assertReadRegisterAddress(0);
  utils.assertWriteRegisterAddress(0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
