// This will test the address register control GALs

/*
 * This design is two GALs, a 16V8B for the output control and
 * a 22V10C for the input control
 * 
 * 
 */


#define CTRL_RST 23
#define CTRL_LD  25
#define CTRL_CE  27
#define CTRL_B0  29
#define CTRL_B1  31
#define CTRL_J2  33
#define CTRL_B2  35
#define CTRL_J3  37
#define CTRL_B3  39


#define CI_BBS 22
#define CI_CE0 24
#define CI_CE1 26
#define CI_CE2 28
#define CI_CE3 30
#define CI_LD0 32
#define CI_LD1 34
#define CI_LD2 36
#define CI_LD3 38


#define STR_HIGH "high"
#define STR_LOW  "low"

int jumper_pins[2]     = { CTRL_J2, CTRL_J3 };

int input_pins[9]    = { CI_LD0, CI_LD1, CI_LD2, CI_LD3, CI_CE0, CI_CE1, CI_CE2, CI_CE3, CI_BBS }; 
int output_pins[7]   = { CTRL_B0, CTRL_B1, CTRL_B2, CTRL_B3, CTRL_RST, CTRL_LD, CTRL_CE };
int address_pins[4]  = { CTRL_B0, CTRL_B1, CTRL_B2, CTRL_B3 };

//const char* states[2] = { STR_LOW, STR_HIGH };

struct states {
  boolean b_bs;
  boolean ce_0;
  boolean ce_1;
  boolean ce_2;
  boolean ce_3;
  boolean ld_0;
  boolean ld_1;
  boolean ld_2;
  boolean ld_3;
};

void setup() {
  Serial.begin(57600);
  Serial.println("Starting up tests...");
  setupPins();
  runTest();
}


void setupPins() {
  setupJumperPins();
  setupInputPins();
  setupOutputPins();
}

void setupJumperPins() {
  pinMode(CTRL_J3, OUTPUT);
  pinMode(CTRL_J2, OUTPUT);
}

void setupInputPins() {
  for (int i = 0; i < 9; i++) {
    pinMode(input_pins[i], INPUT);
  }
}

void setupOutputPins() {
  for (int i = 0; i < 7; i++) {
    pinMode(output_pins[i], OUTPUT);
  }
}

void resetControlLines() {
  resetJumpers();
  for (int i = 0; i < 7; i++) {
    digitalWrite(output_pins[i], HIGH);
  }
}

void resetJumpers() {
  digitalWrite(CTRL_J3, 1);
  digitalWrite(CTRL_J2, 0);
}

void setRegisterAddress(int address) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(address_pins[i], (address >> i) & 1);
  }
}

void setJumpers(int jumpers) {
  digitalWrite(CTRL_J2, jumpers & 1);
  digitalWrite(CTRL_J3, (jumpers >> 1) & 1);
}

void runTest() {
  testInputBoardSelect();
  testInputLoad();
  testInputCountEnable();
  testInputReset();
}

void testInputBoardSelect() {
  const char *testName = "input board select";
  printBegin(testName);
  boolean passed = true;
  resetControlLines();
  for (int jumpers = 0; jumpers < 4; jumpers++) {
    setJumpers(jumpers);
    for (int address = 0; address < 16; address++) {
      setRegisterAddress(address);
      states states;
      readStates(states);
      passed = validateBoardSelect(address, states, getExpectedBoardSelectState(jumpers, address)) && passed;
    }
  }
  printResult(passed, testName);
}

void testInputLoad() {
  const char *testName = "input load";
  printBegin(testName);
  boolean passed = true;
  resetControlLines();
  for (int jumpers = 0; jumpers < 4; jumpers++) {
    for (int address = 0; address < 16; address++) {
      passed = testLoadState(jumpers, address) && passed;
    }
  }
  printResult(passed, testName);
}

void testInputCountEnable() {
  const char *testName = "input count enable";
  printBegin(testName);
  boolean passed = true;
  resetControlLines();
  for (int jumpers = 0; jumpers < 4; jumpers++) {
    for (int address = 0; address < 16; address++) {
      passed = testCountEnableState(jumpers, address) && passed;
    }
  }
  printResult(passed, testName);
}

void testInputReset() {
  const char *testName = "input reset";
  printBegin(testName);
  boolean passed = true;
  resetControlLines();
  for (int jumpers = 0; jumpers < 4; jumpers++) {
    for (int address = 0; address < 16; address++) {
      passed = testResetState(jumpers, address) && passed;
    }
  }
  printResult(passed, testName);
}

boolean testLoadState(int jumpers, int address) {
  states states;
  
  digitalWrite(CTRL_LD, LOW);
  setJumpers(jumpers);
  setRegisterAddress(address);
  
  readStates(states);
  return validateStates(states, 
          getExpectedBoardSelectState(jumpers, address), 
          getExpectedRegisterSelect(0, jumpers, address), 
          getExpectedRegisterSelect(1, jumpers, address), 
          getExpectedRegisterSelect(2, jumpers, address), 
          getExpectedRegisterSelect(3, jumpers, address),
          HIGH,
          HIGH,
          HIGH,
          HIGH);
}

boolean testCountEnableState(int jumpers, int address) {
  states states;
  
  digitalWrite(CTRL_CE, LOW);
  setJumpers(jumpers);
  setRegisterAddress(address);
  
  readStates(states);
  return validateStates(states, 
          getExpectedBoardSelectState(jumpers, address),
          HIGH,
          HIGH,
          HIGH,
          HIGH,
          getExpectedRegisterSelect(0, jumpers, address), 
          getExpectedRegisterSelect(1, jumpers, address), 
          getExpectedRegisterSelect(2, jumpers, address), 
          getExpectedRegisterSelect(3, jumpers, address));
}

boolean testResetState(int jumpers, int address) {
  states states;
  
  digitalWrite(CTRL_RST, LOW);
  setJumpers(jumpers);
  setRegisterAddress(address);
  
  readStates(states);
  return validateStates(states, 
          getExpectedBoardSelectState(jumpers, address),
          LOW,
          LOW,
          LOW,
          LOW,
          HIGH,
          HIGH,
          HIGH,
          HIGH);
}

void printBegin(const char* testName) {
  char buf[80];
  sprintf(buf, "Beginning %s...", testName);
  Serial.println(buf);
}

void printResult(boolean passed, const char* testName) {
  char buf[80];
  sprintf(buf, "%s %s %s", passed ? "[PASS]" : "[FAIL]", testName, passed ? "passed." : "failed.");
  Serial.println(buf);
}

boolean getExpectedBoardSelectState(int jumpers, int address) {
  if (jumpers == (address >> 2)) {
    return LOW;
  }
  return HIGH;
}

boolean getExpectedRegisterSelect(int regno, int jumpers, int address) {
  if (address == ((jumpers << 2) + regno)) {
    return LOW;
  }
  return HIGH;
}

boolean validateStates(states &states, boolean b_bs, boolean ld_0, boolean ld_1, boolean ld_2, boolean ld_3, boolean ce_0, boolean ce_1, boolean ce_2, boolean ce_3) {
  if ((states.b_bs != b_bs) ||
       (states.ld_0 != ld_0) ||
       (states.ld_1 != ld_1) ||
       (states.ld_2 != ld_2) ||
       (states.ld_3 != ld_3) ||
       (states.ce_0 != ce_0) ||
       (states.ce_1 != ce_1) ||
       (states.ce_2 != ce_2) ||
       (states.ce_3 != ce_3)) {
    char buf[160];
    sprintf(buf, "    [FAIL] Expected b_bs(%1d), ld_0(%1d), ld_1(%1d), ld_2(%1d), ld_3(%1d), ce_0(%1d), ce_1(%1d), ce_2(%1d), ce_3(%1d)", b_bs, ld_0, ld_1, ld_2, ld_3, ce_0, ce_1, ce_2, ce_3);
    Serial.println(buf);
    sprintf(buf, "           Received b_bs(%1d), ld_0(%1d), ld_1(%1d), ld_2(%1d), ld_3(%1d), ce_0(%1d), ce_1(%1d), ce_2(%1d), ce_3(%1d)", states.b_bs, states.ld_0, states.ld_1, states.ld_2, states.ld_3, states.ce_0, states.ce_1, states.ce_2, states.ce_3);
    Serial.println(buf);
    return false;    
  }
  return true;
}

boolean validateBoardSelect(int address, states &states, boolean b_bs) {
  if (states.b_bs != b_bs) {
    char buf[80];
    sprintf(buf, "    [FAIL] Address %0x, expected b_bs(%1d)", address, b_bs);
    Serial.println(buf);
    sprintf(buf, "           Received b_bs(%1d)", states.b_bs);
    Serial.println(buf);
    return false;
  }
  return true;
}

void printValue(const char* param, int value) {
  char buf[80];
  sprintf(buf, "    %s: %1d,", param, value);
  Serial.println(buf);
}

void readStates(states &states) {
  states.b_bs = digitalRead(CI_BBS);
  states.ce_0 = digitalRead(CI_CE0);
  states.ce_1 = digitalRead(CI_CE1);
  states.ce_2 = digitalRead(CI_CE2);
  states.ce_3 = digitalRead(CI_CE3);
  states.ld_0 = digitalRead(CI_LD0);
  states.ld_1 = digitalRead(CI_LD1);
  states.ld_2 = digitalRead(CI_LD2);
  states.ld_3 = digitalRead(CI_LD3);
}

void loop() {
  // put your main code here, to run repeatedly:

}
