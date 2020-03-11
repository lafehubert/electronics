// This will test the address register control GALs

/*
 * This design is two GALs, a 16V8B for the output control and
 * a 22V10C for the input control
 * 
 * 
 */


// Jumpers

#define CTRL_J3 33
#define CTRL_J2 31

// Output Section

#define CTRL_OE 35
#define CTRL_A3 29
#define CTRL_A2 27
#define CTRL_A1 25
#define CTRL_A0 23

#define CO_ABS  30
#define CO_OE3  28
#define CO_OE2  26
#define CO_OE1  24
#define CO_OE0  22

// Input Section

#define CTRL_CE  51
#define CTRL_LD  49
#define CTRL_RST 47
#define CTRL_B3  45
#define CTRL_B2  43
#define CTRL_B1  41
#define CTRL_B0  39

#define CI_BBS 52
#define CI_LD3 50
#define CI_LD2 48
#define CI_LD1 46
#define CI_LD0 44
#define CI_CE3 42
#define CI_CE2 40
#define CI_CE1 38
#define CI_CE0 36


#define STR_HIGH "high"
#define STR_LOW  "low"

int jumper_pins[2]     = { CTRL_J2, CTRL_J3 };

int a_input_pins[5]    = { CO_OE0, CO_OE1, CO_OE2, CO_OE3, CO_ABS };
int a_output_pins[5]   = { CTRL_A0, CTRL_A1, CTRL_A2, CTRL_A3, CTRL_OE };
int a_address_pins[4]  = { CTRL_A0, CTRL_A1, CTRL_A2, CTRL_A3 };

int b_input_pins[9]    = { CI_LD0, CI_LD1, CI_LD2, CI_LD3, CI_CE0, CI_CE1, CI_CE2, CI_CE3, CI_BBS }; 
int b_output_pins[7]   = { CTRL_B0, CTRL_B1, CTRL_B2, CTRL_B3, CTRL_RST, CTRL_LD, CTRL_CE };
int b_address_pins[4]  = { CTRL_B0, CTRL_B1, CTRL_B2, CTRL_B3 };

const char* states[2] = { STR_LOW, STR_HIGH };

struct output_states {
  boolean a_bs;
  boolean oe_0;
  boolean oe_1;
  boolean oe_2;
  boolean oe_3;
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
  setupOutputSectionPins();
  setupInputSectionPins();
}

void setupJumperPins() {
  pinMode(CTRL_J3, OUTPUT);
  pinMode(CTRL_J2, OUTPUT);
}

void setupOutputSectionPins() {
  for (int i = 0; i < 5; i++) {
    pinMode(a_input_pins[i], INPUT);
  }
  for (int i = 0; i < 5; i++) {
    pinMode(a_output_pins[i], OUTPUT);
  }
}

void setupInputSectionPins() {
  for (int i = 0; i < 9; i++) {
    pinMode(b_input_pins[i], INPUT_PULLUP);
  }
  for (int i = 0; i < 7; i++) {
    pinMode(b_output_pins[i], OUTPUT);
  }
}

void resetControlLines() {
  resetJumpers();
  resetOutputSectionControlLines();
  resetInputSectionControlLines();
}

void resetJumpers() {
  digitalWrite(CTRL_J3, 1);
  digitalWrite(CTRL_J2, 0);
}

void resetOutputSectionControlLines() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(a_output_pins[i], HIGH);
  }
}

void resetInputSectionControlLines() {
  for (int i = 0; i < 7; i++) {
    digitalWrite(b_output_pins[i], HIGH);
  }
}

void setOutputRegisterAddress(int address) {
  setRegisterAddress(a_address_pins, address);
}

void setInputRegisterAddress(int address) {
  setRegisterAddress(b_address_pins, address);
}

void setRegisterAddress(int *address_pins, int address) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(address_pins[i], (address >> i) & 1);
  }
}

void setJumpers(int jumpers) {
  digitalWrite(CTRL_J2, jumpers & 1);
  digitalWrite(CTRL_J3, (jumpers >> 1) & 1);
}

void runTest() {
  testOutputBoardSelect();
  testOutputEnable();
  testInputBoardSelect();
  testInputLoad();
  testInputCountEnable();
  testInputReset();
}

void testOutputBoardSelect() {
  const char *testName = "output board select";
  printBegin(testName);
  boolean passed = true;
  resetControlLines();
  digitalWrite(CTRL_OE, HIGH);
  for (int jumpers = 0; jumpers < 4; jumpers++) {
    setJumpers(jumpers);
    for (int address = 0; address < 16; address++) {
      setOutputRegisterAddress(address);
      output_states states;
      readOutputStates(states);
      passed = validateOutputBoardSelect(address, states, getExpectedBoardSelectState(jumpers, address)) && passed;
    }
  }
  printResult(passed, testName);
}

void testOutputEnable() {
  const char *testName = "output enable";
  printBegin(testName);
  boolean passed = true;
  resetControlLines();
  for (int jumpers = 0; jumpers < 4; jumpers++) {
    for (int address = 0; address < 16; address++) {
      passed = testOutputState(jumpers, address) && passed;
    }
  }
  printResult(passed, testName);
}

boolean testOutputState(int jumpers, int address) {
  output_states states;
  
  digitalWrite(CTRL_OE, LOW);
  setJumpers(jumpers);
  setOutputRegisterAddress(address);
  
  readOutputStates(states);
  return validateOutputSectionStates(states, 
          getExpectedBoardSelectState(jumpers, address), 
          getExpectedRegisterSelect(0, jumpers, address), 
          getExpectedRegisterSelect(1, jumpers, address), 
          getExpectedRegisterSelect(2, jumpers, address), 
          getExpectedRegisterSelect(3, jumpers, address));
}

void testInputBoardSelect() {
  const char *testName = "input board select";
  printBegin(testName);
  boolean passed = true;
  resetControlLines();
  for (int jumpers = 0; jumpers < 4; jumpers++) {
    setJumpers(jumpers);
    for (int address = 0; address < 16; address++) {
      setInputRegisterAddress(address);
      output_states states;
      readOutputStates(states);
      passed = validateInputBoardSelect(address, states, getExpectedBoardSelectState(jumpers, address)) && passed;
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
      passed = testInputLoadState(jumpers, address) && passed;
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
      passed = testInputCountEnableState(jumpers, address) && passed;
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
      passed = testInputResetState(jumpers, address) && passed;
    }
  }
  printResult(passed, testName);
}

boolean testInputLoadState(int jumpers, int address) {
  output_states states;
  
  digitalWrite(CTRL_LD, LOW);
  setJumpers(jumpers);
  setInputRegisterAddress(address);
  
  readOutputStates(states);
  return validateInputSectionStates(states, 
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

boolean testInputCountEnableState(int jumpers, int address) {
  output_states states;
  
  digitalWrite(CTRL_CE, LOW);
  setJumpers(jumpers);
  setInputRegisterAddress(address);
  
  readOutputStates(states);
  return validateInputSectionStates(states, 
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

boolean testInputResetState(int jumpers, int address) {
  output_states states;
  
  digitalWrite(CTRL_RST, LOW);
  setJumpers(jumpers);
  setInputRegisterAddress(address);
  
  readOutputStates(states);
  return validateInputSectionStates(states, 
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



boolean validateOutputSectionStates(output_states &states, boolean a_bs, boolean oe_0, boolean oe_1, boolean oe_2, boolean oe_3) {
  if ((states.a_bs != a_bs) ||
       (states.oe_0 != oe_0) ||
       (states.oe_1 != oe_1) ||
       (states.oe_2 != oe_2) ||
       (states.oe_3 != oe_3)) {
    char buf[80];
    sprintf(buf, "    [FAIL] Expected a_bs(%1d), oe_0(%1d), oe_1(%1d), oe_2(%1d), oe_3(%1d)", a_bs, oe_0, oe_1, oe_2, oe_3);
    Serial.println(buf);
    sprintf(buf, "           Received a_bs(%1d), oe_0(%1d), oe_1(%1d), oe_2(%1d), oe_3(%1d)", states.a_bs, states.oe_0, states.oe_1, states.oe_2, states.oe_3);
    Serial.println(buf);
    return false;    
  }
  return true;
}

boolean validateInputSectionStates(output_states &states, boolean b_bs, boolean ld_0, boolean ld_1, boolean ld_2, boolean ld_3, boolean ce_0, boolean ce_1, boolean ce_2, boolean ce_3) {
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

boolean validateOutputBoardSelect(int address, output_states &states, boolean a_bs) {
  if (states.a_bs != a_bs) {
    char buf[80];
    sprintf(buf, "    [FAIL] Address %0x, expected a_bs(%1d)", address, a_bs);
    Serial.println(buf);
    sprintf(buf, "           Received a_bs(%1d)", states.a_bs);
    Serial.println(buf);
    return false;
  }
  return true;
}

boolean validateInputBoardSelect(int address, output_states &states, boolean b_bs) {
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

void printOutputStates(output_states &states) {
  Serial.println("Output: {");
  printValue("a_bs", states.a_bs);
  printValue("oe_3", states.oe_3);
  printValue("oe_2", states.oe_2);
  printValue("oe_1", states.oe_1);
  printValue("oe_0", states.oe_0);
  Serial.println("}");
}

void printValue(const char* param, int value) {
  char buf[80];
  sprintf(buf, "    %s: %1d,", param, value);
  Serial.println(buf);
}

void readOutputStates(output_states &states) {
  states.a_bs = digitalRead(CO_ABS);
  states.oe_0 = digitalRead(CO_OE0);
  states.oe_1 = digitalRead(CO_OE1);
  states.oe_2 = digitalRead(CO_OE2);
  states.oe_3 = digitalRead(CO_OE3);

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
