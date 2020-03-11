/*
 * Implementation of the Ya4b test utilities
 */

 #include <Arduino.h>
 #include "Ya4bUtil.h"

void Ya4bUtil::setupPins()
{
  assertData(0);
  assertAddress(0);
  assertReadRegisterAddress(0);
  assertWriteRegisterAddress(0);
  assertValue(_readRegisterAddressPins, 4, 0);
  assertValue(_writeRegisterAddressPins, 4, 0);
  assertValue(_registerControlPins, 4, 0xF);
  assertValue(_transferControlPins, 6, 0b111111);
  digitalWrite(CPU_RESET, HIGH);
  digitalWrite(CPU_CLK, LOW);
  setDataOutput();
  setAddressOutput();
  setOutput(_readRegisterAddressPins, 4);
  setOutput(_writeRegisterAddressPins, 4);
  setOutput(_registerControlPins, 4);
  setOutput(_transferControlPins, 6);
  setOutput(_cpuSignalPins, 2);
}

void Ya4bUtil::resetControlPins() {
  assertValue(_registerControlPins, 4, 0xF);
  assertValue(_transferControlPins, 6, 0b111111);
  digitalWrite(CPU_RESET, HIGH);
  digitalWrite(CPU_CLK, LOW);
}

void Ya4bUtil::setDataInput()
{
  setInput(_dataPins, 4, INPUT_PULLUP);
}

void Ya4bUtil::setDataOutput()
{
  setOutput(_dataPins, 4);
}

void Ya4bUtil::assertData(int data)
{
  assertValue(_dataPins, 4, data);
}

int Ya4bUtil::readData()
{
  return readValue(_dataPins, 4);
}


void Ya4bUtil::setAddressInput()
{
  setInput(_addressPins, 8, INPUT_PULLUP);
}

void Ya4bUtil::setAddressOutput()
{
  setOutput(_addressPins, 8);
}

void Ya4bUtil::assertAddress(int address)
{
  setAddressOutput();
  assertValue(_addressPins, 8, address);
}

int Ya4bUtil::readAddress()
{
  return readValue(_addressPins, 8);
}


void Ya4bUtil::assertReadRegisterAddress(int address)
{
  assertValue(_readRegisterAddressPins, 4, address);
}

void Ya4bUtil::assertWriteRegisterAddress(int address)
{
  assertValue(_writeRegisterAddressPins, 4, address);
}


void Ya4bUtil::assertRegisterOutputEnable()
{
  digitalWrite(REG_A_OUTPUT_ENABLE, LOW);
}

void Ya4bUtil::assertRegisterLoad()
{
  digitalWrite(REG_B_LOAD, LOW);
}

void Ya4bUtil::assertRegisterCountEnable()
{
  digitalWrite(REG_B_COUNT_ENABLE, LOW);
}

void Ya4bUtil::assertRegisterCountUp()
{
  digitalWrite(REG_B_UP_DOWN, HIGH);
}

void Ya4bUtil::assertRegisterCountDown()
{
  digitalWrite(REG_B_UP_DOWN, LOW);
}


void Ya4bUtil::assertTransferOutputEnable()
{
  digitalWrite(REG_T_OUTPUT_ENABLE, LOW);
}

void Ya4bUtil::assertTransferLowOutputEnable()
{
  digitalWrite(REG_TH_OUTPUT_ENABLE, LOW);
}

void Ya4bUtil::assertTransferHighOutputEnable()
{
  digitalWrite(REG_TL_OUTPUT_ENABLE, LOW);
}

void Ya4bUtil::assertTransferInputFromAddressBus()
{
  digitalWrite(REG_T_ADDR_OR_DATA, LOW);
}

void Ya4bUtil::assertTransferInputFromDataBus()
{
  digitalWrite(REG_T_ADDR_OR_DATA, HIGH);
}

void Ya4bUtil::assertTransferLowLoad()
{
  digitalWrite(REG_TL_LOAD, LOW);
}

void Ya4bUtil::assertTransferHighLoad()
{
  digitalWrite(REG_TH_LOAD, LOW);
}

void Ya4bUtil::assertClock()
{
  digitalWrite(CPU_CLK, HIGH);
}

void Ya4bUtil::resetClock()
{
  digitalWrite(CPU_CLK, LOW);
}

void Ya4bUtil::pulseClock(int msDelay)
{
  resetClock();
  delay(msDelay);
  assertClock();
  delay(msDelay);
  resetClock();
}

void Ya4bUtil::assertReset()
{
  digitalWrite(CPU_RESET, LOW);
}

void Ya4bUtil::resetReset()
{
  digitalWrite(CPU_RESET, HIGH);
}

void Ya4bUtil::pulseReset(int msDelay)
{
  resetReset();
  delay(msDelay);
  assertReset();
  delay(msDelay);
  resetReset();
}

void Ya4bUtil::setInput(int pins[], int width, int readMode)
{
  for (int i = 0; i < width; i++)
  {
    pinMode(pins[i], readMode);
  }
}

void Ya4bUtil::setOutput(int pins[], int width)
{
  for (int i = 0; i < width; i++)
  {
    pinMode(pins[i], OUTPUT);
  }
}

void Ya4bUtil::assertValue(int pins[], int width, int value)
{
  for (int i = 0; i < width; i++) {
    digitalWrite(pins[i], (value >> i) & 1);
  }
}

int Ya4bUtil::readValue(int pins[], int width)
{
  int data = 0;
  for (int i = (width - 1); i >= 0; i--)
  {
    data |= (digitalRead(pins[i]) << i);
  }
  return data;
}
