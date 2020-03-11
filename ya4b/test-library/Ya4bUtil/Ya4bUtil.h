/*
 * Test library for the Ya4b internal cpu bus and signals 
 * 
 * This only for the Mega
 */
#ifndef YA4BUTIL_H
#define YA4BUTIL_H

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
#define DATA_0 38
#define DATA_1 40
#define DATA_2 42
#define DATA_3 44

// Register Addresses
#define REG_A0 23
#define REG_A1 25
#define REG_A2 27
#define REG_A3 29

#define REG_B0 31
#define REG_B1 33
#define REG_B2 35
#define REG_B3 37 

// Register Signals

#define REG_B_COUNT_ENABLE  8
#define REG_B_UP_DOWN       9
#define REG_B_LOAD          10
#define REG_A_OUTPUT_ENABLE 11

// Transfer Register Signals
#define REG_T_OUTPUT_ENABLE  2
#define REG_TH_OUTPUT_ENABLE 3
#define REG_TL_OUTPUT_ENABLE 4
#define REG_T_ADDR_OR_DATA   5
#define REG_TL_LOAD          6
#define REG_TH_LOAD          7

// Other Signals

#define CPU_RESET 50
#define CPU_CLK   52

class Ya4bUtil
{
  public:
    void setupPins();
    void resetControlPins();
    
    void setDataInput();
    void setDataOutput();
    void assertData(int data);
    int  readData();
    
    void setAddressInput();
    void setAddressOutput();
    void assertAddress(int address);
    int  readAddress();

    void assertReadRegisterAddress(int address);
    void assertWriteRegisterAddress(int address);

    void assertRegisterOutputEnable();
    void assertRegisterLoad();
    void assertRegisterCountEnable();
    void assertRegisterCountUp();
    void assertRegisterCountDown();

    void assertTransferOutputEnable();
    void assertTransferLowOutputEnable();
    void assertTransferHighOutputEnable();
    void assertTransferInputFromAddressBus();
    void assertTransferInputFromDataBus();
    void assertTransferLowLoad();
    void assertTransferHighLoad();

    void assertClock();
    void resetClock();
    void pulseClock(int msDelay);
    void assertReset();
    void resetReset();
    void pulseReset(int msDelay);
    
  private:
    int _addressPins[8] = { ADDR_0, ADDR_1, ADDR_2, ADDR_3, ADDR_4, ADDR_5, ADDR_6, ADDR_7 };
    int _dataPins[4]    = { DATA_0, DATA_1, DATA_2, DATA_3 };
    
    int _readRegisterAddressPins[4]  = { REG_A0, REG_A1, REG_A2, REG_A3 };
    int _writeRegisterAddressPins[4] = { REG_B0, REG_B1, REG_B2, REG_B3 };

    int _registerControlPins[4] = { REG_A_OUTPUT_ENABLE, REG_B_LOAD, REG_B_COUNT_ENABLE, REG_B_UP_DOWN };

    int _transferControlPins[6] = { REG_T_OUTPUT_ENABLE, REG_TH_OUTPUT_ENABLE, REG_TL_OUTPUT_ENABLE, REG_T_ADDR_OR_DATA, REG_TL_LOAD, REG_TH_LOAD };

    int _cpuSignalPins[2] = { CPU_RESET, CPU_CLK };

    void setInput(int pins[], int width, int readMode);
    void setOutput(int pins[], int width);
    void assertValue(int pins[], int width, int value);
    int readValue(int pins[], int width);
    
};

#endif
