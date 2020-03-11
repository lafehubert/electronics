#include "Ya4bUtil.h"

Ya4bUtil utils;

void setup() {
  Serial.begin(57600);
  Serial.println("Starting up...");
  utils.setupPins();
}

void loop() {
  assertAll();
  utils.setupPins();
}

void assertAll() 
{
  delay(1000);
  utils.assertAddress(0xFF);
  delay(1000);
  utils.assertReadRegisterAddress(0xF);
  delay(1000);
  utils.assertWriteRegisterAddress(0xF);
  delay(1000);
  utils.assertData(0xF);
  delay(1000);
  utils.pulseClock(1000L);
  utils.pulseReset(1000L);
  utils.assertClock();
  utils.assertReset();
  delay(1000);
  utils.assertRegisterOutputEnable();
  delay(1000);
  utils.assertRegisterLoad();
  delay(1000);
  utils.assertRegisterCountEnable();
  delay(1000);
  utils.assertRegisterCountUp();
  delay(1000);
  utils.assertRegisterCountDown();
  delay(1000);
  utils.assertTransferOutputEnable();
  delay(1000);
  utils.assertTranserLowOutputEnable();
  delay(1000);
  utils.assertTransferHighOutputEnable();
  delay(1000);
  utils.assertTransferInputFromDataBus();
  delay(1000);
  utils.assertTransferInputFromAddressBus();
  delay(1000);
  utils.assertTransferLowLoad();
  delay(1000);
  utils.assertTransferHighLoad();
  delay(1000);
}
