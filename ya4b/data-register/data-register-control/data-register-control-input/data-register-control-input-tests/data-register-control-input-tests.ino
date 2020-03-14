// This will test the address register control GALs

/*
 * This design is two GALs, a 16V8B for the output control and
 * a 22V10C for the input control
 * 
 * 
 */


#define CTRL_RST 23
#define CTRL_LD  25
#define CTRL_B0  27
#define CTRL_B1  29
#define CTRL_J2  31
#define CTRL_B2  33
#define CTRL_J3  35
#define CTRL_B3  37


#define CI_BBS 22
#define CI_LD0 24
#define CI_LD1 26
#define CI_LD2 28
#define CI_LD3 30
#define CI_RST 32


#define STR_HIGH "high"
#define STR_LOW  "low"

int jumper_pins[2]     = { CTRL_J2, CTRL_J3 };

int input_pins[6]    = { CI_LD0, CI_LD1, CI_LD2, CI_LD3, CI_RST, CI_BBS }; 
int output_pins[6]   = { CTRL_B0, CTRL_B1, CTRL_B2, CTRL_B3, CTRL_RST, CTRL_LD };
int address_pins[4]  = { CTRL_B0, CTRL_B1, CTRL_B2, CTRL_B3 };

//const char* states[2] = { STR_LOW, STR_HIGH };

struct states {
  boolean b_bs;
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
  for (int i = 0; i < 6; i++) {
    pinMode(input_pins[i], INPUT);
  }
}

void setupOutputPins() {
  for (int i = 0; i < 6; i++) {
    pinMode(output_pins[i], OUTPUT);
  }
}

void resetControlLines() {
  resetJumpers();
  for (int i = 0; i < 6; i++) {
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
  testBoardSelect();
  testLoad();
  testReset();
}

void testBoardSelect() {
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

void testLoad() {
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

void testReset() {
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
          getExpectedLoadState(0, jumpers, address), 
          getExpectedLoadState(1, jumpers, address), 
          getExpectedLoadState(2, jumpers, address), 
          getExpectedLoadState(3, jumpers, address));
}



boolean testResetState(int jumpers, int address) {
  states states;
  
  digitalWrite(CTRL_RST, LOW);
  int state = digitalRead(CI_RST);
  
  return state == HIGH;
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

boolean getExpectedLoadState(int regno, int jumpers, int address) {
  if (address == ((jumpers << 2) + regno)) {
    return HIGH;
  }
  return LOW;
}

boolean validateStates(states &states, boolean b_bs, boolean ld_0, boolean ld_1, boolean ld_2, boolean ld_3) {
  if ((states.b_bs != b_bs) ||
       (states.ld_0 != ld_0) ||
       (states.ld_1 != ld_1) ||
       (states.ld_2 != ld_2) ||
       (states.ld_3 != ld_3)) {
    char buf[160];
    sprintf(buf, "    [FAIL] Expected b_bs(%1d), ld_0(%1d), ld_1(%1d), ld_2(%1d), ld_3(%1d)", b_bs, ld_0, ld_1, ld_2, ld_3);
    Serial.println(buf);
    sprintf(buf, "           Received b_bs(%1d), ld_0(%1d), ld_1(%1d), ld_2(%1d), ld_3(%1d)", states.b_bs, states.ld_0, states.ld_1, states.ld_2, states.ld_3);
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
  states.ld_0 = digitalRead(CI_LD0);
  states.ld_1 = digitalRead(CI_LD1);
  states.ld_2 = digitalRead(CI_LD2);
  states.ld_3 = digitalRead(CI_LD3);
}

void loop() {
  // put your main code here, to run repeatedly:

}
