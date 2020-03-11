/*
 * This program will run tests on the YA4B transfer register
 * The transfer register allows the transfer of values to and from 
 * the 4-bit data registers to the 8-bit address registers.
 * 
 */

#include "Ya4bUtil.h"

#define STR_HIGH "high"
#define STR_LOW  "low"

Ya4bUtil utils;

const char* states[2] = { STR_LOW, STR_HIGH };

void setup() {
  Serial.begin(57600);
  Serial.println("Starting up...");
  Serial.println("");
  randomSeed(analogRead(0));
  utils.setupPins();
  runTests();
}


void runTests() {
  testLoadDataHigh();
  testLoadDataLow();
  testLoadAddress();
  testOutputEnable();
}

void testLoadDataHigh() {
  Serial.println("Starting load high test.");
  int data = 1;
  while (data <= 0x8) {
    loadDataHigh(data);
    delayMicroseconds(1);
    verifyDataHigh(data);
    data = data << 1;
  }
}

void testLoadDataLow() {
  Serial.println("Starting load low test.");
  int data = 1;
  while (data <= 0x8) {
    loadDataLow(data);
    delayMicroseconds(1);
    verifyDataLow(data);
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

void loadDataHigh(int data)
{
  utils.setDataOutput();
  utils.assertData(data);
  utils.assertTransferInputFromDataBus();
  utils.assertTransferHighLoad();
  utils.pulseClock(1);
  utils.resetControlPins();
}

void loadDataLow(int data)
{
  utils.setDataOutput();
  utils.assertData(data);
  utils.assertTransferInputFromDataBus();
  utils.assertTransferLowLoad();
  utils.pulseClock(1);
  utils.resetControlPins();
}

void verifyDataHigh(int expected)
{
  utils.setDataInput();
  utils.assertTransferHighOutputEnable();
  int data = utils.readData();
  utils.resetControlPins();
  if (data != expected) {
    char buf[80];
    sprintf(buf, "[FAIL] Expected %x, received %x", expected, data);
    Serial.println(buf);
  } else {
    Serial.println("[PASS] Load data passed.");
  }
}

void verifyDataLow(int expected)
{
  utils.setDataInput();
  utils.assertTransferLowOutputEnable();
  int data = utils.readData();
  utils.resetControlPins();
  if (data != expected) {
    char buf[80];
    sprintf(buf, "[FAIL] Expected %x, received %x", expected, data);
    Serial.println(buf);
  } else {
    Serial.println("[PASS] Load data passed.");
  }
}

void loadAddress(int data) {
  utils.setAddressOutput();
  utils.assertAddress(data);
  utils.assertTransferInputFromAddressBus();
  utils.assertTransferLowLoad();
  utils.assertTransferHighLoad();
  utils.pulseClock(1);
  utils.resetControlPins();
}

void verifyAddress(int expected) {
  utils.setAddressInput();
  utils.assertTransferOutputEnable();
  int address = utils.readAddress();
  utils.resetControlPins();
  if (address != expected) {
    char buf[80];
    sprintf(buf, "[FAIL] Expected %0x, received %0x", expected, address);
    Serial.println(buf);
  } else {
    Serial.println("[PASS] Load from address passed.");
  }
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
