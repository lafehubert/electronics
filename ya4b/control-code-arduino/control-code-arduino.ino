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

#define BASE_REGISTER_ADDRESS 0b00

const char* states[2] = { STR_LOW, STR_HIGH };

void setup() {
  Serial.begin(57600);
  Serial.println("Starting up...");
  Serial.println("");
  randomSeed(analogRead(0));
  utils.setupPins();
  runProgram();
}


void runProgram() {
  initialize();
  moveTransferLowToDataRegister(1);
  moveDataRegisterToTransferLow(3);
}

void initialize() {
  utils.pulseReset(1);
  loadTransferAddress(0x55);
  loadData(0, 1);
  loadData(1, 2);
  loadData(2, 4);
  loadData(3, 8);
  utils.resetControlPins();
  utils.setDataInput();
  utils.setAddressInput();
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

void loadTransferDataHigh(int data)
{
  utils.setDataOutput();
  utils.assertData(data);
  utils.assertTransferInputFromDataBus();
  utils.assertTransferHighLoad();
  utils.pulseClock(1);
  utils.resetControlPins();
}

void loadTransferDataLow(int data)
{
  utils.setDataOutput();
  utils.assertData(data);
  utils.assertTransferInputFromDataBus();
  utils.assertTransferLowLoad();
  utils.pulseClock(1);
  utils.resetControlPins();
}

void loadTransferAddress(int data) {
  utils.setAddressOutput();
  utils.assertAddress(data);
  utils.assertTransferInputFromAddressBus();
  utils.assertTransferLowLoad();
  utils.assertTransferHighLoad();
  utils.pulseClock(1);
  utils.resetControlPins();
}

void moveTransferLowToDataRegister(int registerAddress) {
  utils.assertTransferLowOutputEnable();
  utils.assertWriteRegisterAddress(registerAddress);
  utils.assertRegisterLoad();
  utils.pulseClock(1);
  utils.resetControlPins();
}

void moveDataRegisterToTransferLow(int registerAddress) {
  utils.assertReadRegisterAddress(registerAddress);
  utils.assertRegisterOutputEnable();
  utils.assertTransferInputFromDataBus();
  utils.assertTransferLowLoad();
  utils.pulseClock(1);
  utils.resetControlPins();
}

void loop() {
  // put your main code here, to run repeatedly:

}
