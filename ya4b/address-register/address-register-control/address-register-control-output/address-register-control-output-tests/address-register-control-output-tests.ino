// This will test the address register control GALs

/*
 * This design is two GALs, a 16V8B for the output control and
 * a 22V10C for the input control
 * 
 * 
 */


// Jumpers

#define CTRL_J3 33
#define CTRL_J2 29

// Output Section

#define CTRL_OE 23
#define CTRL_A0 25
#define CTRL_A1 27
#define CTRL_A2 31
#define CTRL_A3 35


#define CO_ABS  37
#define CO_OE0  39
#define CO_OE1  41
#define CO_OE2  43
#define CO_OE3  45

#define STR_HIGH "high"
#define STR_LOW  "low"

int jumper_pins[2]     = { CTRL_J2, CTRL_J3 };

int input_pins[5]    = { CO_OE0, CO_OE1, CO_OE2, CO_OE3, CO_ABS };
int output_pins[5]   = { CTRL_A0, CTRL_A1, CTRL_A2, CTRL_A3, CTRL_OE };
int address_pins[4]  = { CTRL_A0, CTRL_A1, CTRL_A2, CTRL_A3 };

//const char* state_strings[2] = { STR_LOW, STR_HIGH };

struct states {
  boolean a_bs;
  boolean oe_0;
  boolean oe_1;
  boolean oe_2;
  boolean oe_3;
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
  for (int i = 0; i < 5; i++) {
    pinMode(input_pins[i], INPUT);
  }
}

void setupOutputPins() {
  for (int i = 0; i < 5; i++) {
    pinMode(output_pins[i], OUTPUT);
  }
}

void resetControlLines() {
  resetJumpers();
  for (int i = 0; i < 5; i++) {
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
  for (int i = 0; i < 16; i++) {
    testBoardSelect();
  }
  for (int i = 0; i < 16; i++) {
    testOutputEnable();
  }
}

void testBoardSelect() {
  const char *testName = "output board select";
  printBegin(testName);
  boolean passed = true;
  resetControlLines();
  digitalWrite(CTRL_OE, HIGH);
  for (int jumpers = 0; jumpers < 4; jumpers++) {
    setJumpers(jumpers);
    for (int address = 0; address < 16; address++) {
      setRegisterAddress(address);
      delayForSettle();
      states states;
      readStates(states);
      passed = validateBoardSelect(address, states, getExpectedBoardSelectState(jumpers, address)) && passed;
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
      passed = testState(jumpers, address) && passed;
    }
  }
  printResult(passed, testName);
}

boolean testState(int jumpers, int address) {
  states states;
  
  digitalWrite(CTRL_OE, LOW);
  setJumpers(jumpers);
  setRegisterAddress(address);
  delayForSettle();
  readStates(states);
  return validateStates(states, 
          getExpectedBoardSelectState(jumpers, address), 
          getExpectedRegisterSelect(0, jumpers, address), 
          getExpectedRegisterSelect(1, jumpers, address), 
          getExpectedRegisterSelect(2, jumpers, address), 
          getExpectedRegisterSelect(3, jumpers, address));
}

void delayForSettle() {
  delayMicroseconds(5);
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

boolean validateStates(states &states, boolean a_bs, boolean oe_0, boolean oe_1, boolean oe_2, boolean oe_3) {
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

boolean validateBoardSelect(int address, states &states, boolean a_bs) {
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

void printStates(states &states) {
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

void readStates(states &states) {
  states.a_bs = digitalRead(CO_ABS);
  states.oe_0 = digitalRead(CO_OE0);
  states.oe_1 = digitalRead(CO_OE1);
  states.oe_2 = digitalRead(CO_OE2);
  states.oe_3 = digitalRead(CO_OE3);
}

void loop() {
  // put your main code here, to run repeatedly:

}
