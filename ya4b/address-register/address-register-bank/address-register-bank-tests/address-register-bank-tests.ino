/*
 * This program will run tests on the YA4B address register bank
 *
 * The address register bank contains four 8-bit address registers
 * addressed 00 - 11.  The address register bank can set a base 
 * register address of 00-11 (register addresses are 4 bits).
 *
 * The address register bank has an "A" side and a "B" side;
 * the "A" side controls which register is outputting on the 
 * address bus, and the "B" side controls which register is
 * being modified (by being incremented, decremented, or loaded).
 *
 * The reset signal to the board resets all registers to 0 and 
 * should be connected to the CPU reset signal.
 */

#include "Ya4bUtil.h"

#define STR_HIGH "high"
#define STR_LOW  "low"

Ya4bUtil utils;

#define BASE_REGISTER_ADDRESS 0b01

const char* states[2] = { STR_LOW, STR_HIGH };

void setup() 
{
  Serial.begin(57600);
  Serial.println("Starting up...");
  Serial.println("");
  randomSeed(analogRead(0));
  utils.setupPins();
  smokeTest();
  //runTests();
}

void smokeTest() {
  loadAddress(0, 0);
  delay(1000);
  for (int i = 0; i < 0x80; i++) {
    incrementAddress(0);
    delay(1000);
  }
}


void runTests() 
{
  utils.pulseReset(1);
  testLoad();
  testIncrement();
  //testDecrement();
  //testOutputEnable();
}

void testLoad() {
  Serial.println("Starting load test.");
  for (int registerAddress = 0; registerAddress < 4; registerAddress++) 
  {
    int address = 1;
    while (address <= 0x80) 
    {
      loadAddress(registerAddress, address);
      delayMicroseconds(1);
      verifyAddress("load", registerAddress, address);
      address = address << 1;
    }
  }
  resetRegisterAddresses();
}

void testIncrement() {
  Serial.println("Starting increment test.");
  for (int registerAddress = 0; registerAddress < 4; registerAddress++) 
  {
    int address = 1;
    while (address <= 0x80) 
    {
      loadAddress(registerAddress, address);
      delayMicroseconds(1);
      incrementAddress(registerAddress);
      delayMicroseconds(1);
      verifyAddress("increment", registerAddress, address + 1);
      address = address << 1;
    }
  }
  resetRegisterAddresses();
}

void testDecrement() {
  Serial.println("Starting decrement test.");
  for (int registerAddress = 0; registerAddress < 4; registerAddress++) 
  {
    int address = 1;
    while (address <= 0x80) 
    {
      loadAddress(registerAddress, address);
      delayMicroseconds(1);
      decrementAddress(registerAddress);
      delayMicroseconds(1);
      verifyAddress("decrement", registerAddress, address - 1);
      address = address << 1;
    }
  }
  resetRegisterAddresses();
}

/*
void testOutputEnable() 
{
  Serial.println("Starting output enable test.");
  int address = random(0, 256);
  while (address != 255) 
  {
    address = random(0, 256);
  }
  loadAddress(address);
  delayMicroseconds(1);
  int data_low = address & 0xf;
  int data_high = (address & 0xf0) >> 4;
  verifyAddress(address);
  verifyDataLow(data_low);
  verifyDataHigh(data_high);

  utils.resetControlPins();
  loadAddress(0x00);
  utils.resetControlPins();

  utils.setDataInput();
  delay(1);
  int data = utils.readData();
  if (data != 0xf) {
    char buf[80];
    sprintf(buf, "[FAIL] Expected 0xf, received %x", data);
    Serial.println(buf);
  } else {
    Serial.println("[PASS] All data lines are high.");
  }

  utils.setAddressInput();
  delay(1);
  address = utils.readAddress();
  if (address != 0xff) {
    char buf[80];
    sprintf(buf, "[FAIL] Expected 0xff, received %x", address);
    Serial.println(buf);
  } else {
    Serial.println("[PASS] All address lines are high.");
  }
}
*/

void loadAddress(int registerAddress, int address) 
{
  utils.setAddressOutput();
  utils.assertWriteRegisterAddress((BASE_REGISTER_ADDRESS << 2) + registerAddress);
  utils.assertAddress(address);
  utils.assertRegisterLoad();
  utils.pulseClock(1);
  utils.resetControlPins();
}

void incrementAddress(int registerAddress)
{
  utils.setAddressOutput();
  utils.assertWriteRegisterAddress((BASE_REGISTER_ADDRESS << 2) + registerAddress);
  utils.assertRegisterCountUp();
  utils.assertRegisterCountEnable();
  utils.pulseClock(1);
  utils.resetControlPins();
}

void decrementAddress(int registerAddress)
{
  utils.setAddressOutput();
  utils.assertWriteRegisterAddress((BASE_REGISTER_ADDRESS << 2) + registerAddress);
  utils.assertRegisterCountDown();
  utils.assertRegisterCountEnable();
  utils.pulseClock(1);
  utils.resetControlPins();  
}

void verifyAddress(const char *operation, int registerAddress, int expected) 
{
  utils.setAddressInput();
  utils.assertReadRegisterAddress((BASE_REGISTER_ADDRESS << 2) + registerAddress);
  utils.assertRegisterOutputEnable();
  int address = utils.readAddress();
  utils.resetControlPins();
  if (address != expected) 
  {
    char buf[80];
    sprintf(buf, "[FAIL] Expected %0x, received %0x", expected, address);
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

/*
void toBinary(int bits, int number, char *buf) 
{
  for (int i = 0; i < bits; i++) {
    buf[i] = '0' + ((number >> i) & 1);
    buf[bits] = 0;
  }
}
*/

void loop() {
  // put your main code here, to run repeatedly:

}
