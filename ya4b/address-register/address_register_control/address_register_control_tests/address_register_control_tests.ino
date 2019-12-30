// This will test the address register control GAL
// ON HOLD until 74LS169s come in and I can do a redesign

/*
 * address_register_control.pld:
 * 
GAL16V8    ; Address register control for YA4B
ARCTRL  ;


CLK   CS    LD    INC   DEC   AO    R     NC    NC   GND
NC    NC    NC    RST   OE    DN    UP    LOAD  NC   VCC


/LOAD = /CS * /LD * CLK

UP = /CS * /INC * CLK

DN = /CS * /DEC * CLK

/OE = /CS * /AO

RST = /R

DESCRIPTION

Replaces 3 chips of combinatorial logic for a single
address register

PINOUT
                                GAL16V8

                          -------\___/-------
                      CLK |  1           20 | VCC
                          |                 |
                       CS |  2           19 | NC
                          |                 |
                       LD |  3           18 | LOAD
                          |                 |
                      INC |  4           17 | UP
                          |                 |
                      DEC |  5           16 | DN
                          |                 |
                       AO |  6           15 | OE
                          |                 |
                        R |  7           14 | RST
                          |                 |
                       NC |  8           13 | NC
                          |                 |
                       NC |  9           12 | NC
                          |                 |
                      GND | 10           11 | NC
                          -------------------
*/

// Control Lines
#define CTRL_R   40
#define CTRL_AO  42
#define CTRL_DEC 44
#define CTRL_INC 46
#define CTRL_LD  48
#define CTRL_CS  50
#define CTRL_CLK 52

// Outputs To Test
#define OUT_LOAD 2
#define OUT_UP   3
#define OUT_DOWN 4
#define OUT_OE   5
#define OUT_RST  6

#define STR_HIGH "high"
#define STR_LOW  "low"

const char* states[2] = { STR_LOW, STR_HIGH };

struct output_states {
  boolean load;
  boolean up;
  boolean down;
  boolean outputEnable;
  boolean reset;
};

void setup() {
  Serial.begin(57600);
  setupPins();
  runTest();
}

void setupPins() {
  for (int i = CTRL_R; i <= CTRL_CLK; i+=2) {
    pinMode(i, OUTPUT);
  }
  for (int i = OUT_LOAD; i <= OUT_RST; i++) {
    pinMode(i, INPUT);
  }
}

void resetControlLines() {
  for (int i = CTRL_R; i <= CTRL_CLK; i+=2) {
    if (i != CTRL_CLK) {
      digitalWrite(i, HIGH);
    } else {
      digitalWrite(i, LOW);
    }
  }
}

void runTest() {
  testChipSelect();
  testReset();
  testLoad();
}

void testChipSelect() {
  Serial.println("Beginning chip select test...");
  resetControlLines();
  digitalWrite(CTRL_CS, HIGH);
  digitalWrite(CTRL_LD, LOW);
  digitalWrite(CTRL_INC, LOW);
  digitalWrite(CTRL_DEC, LOW);
  digitalWrite(CTRL_AO, LOW);
  digitalWrite(CTRL_CLK, HIGH);
  output_states states;
  readOutputStates(states);
  if (validateOutputStates(states, HIGH, LOW, LOW, HIGH, LOW)) {
    Serial.println("[PASS] chip select test passed.");
  } else {
    Serial.println("[FAIL] chip select test failed.");
  }
}

void testReset() {
  Serial.println("Beginning reset test...");
  resetControlLines();
  digitalWrite(CTRL_R, LOW);
  output_states states;
  readOutputStates(states);
  if (validateOutputStates(states, HIGH, LOW, LOW, HIGH, HIGH)) {
    Serial.println("[PASS] reset test passed.");
  } else {
    Serial.println("[FAIL] reset test failed.");
  }
}

void testLoad() {
  Serial.println("Beginning load test...");
  resetControlLines();
  digitalWrite(CTRL_CS, LOW);
  digitalWrite(CTRL_LD, LOW);
  digitalWrite(CTRL_CLK, HIGH);
  output_states states;
  readOutputStates(states);
  if (validateOutputStates(states, LOW, LOW, LOW, HIGH, LOW)) {
    Serial.println("[PASS] load test passed.");
  } else {
    Serial.println("[FAIL] load test failed.");
  }
}

void testIncrement() {
  Serial.println("Beginning increment test...");
  resetControlLines();
  digitalWrite(CTRL_CS, LOW);
  digitalWrite(CTRL_INC, LOW);
  digitalWrite(CTRL_CLK, HIGH);
  output_states states;
  readOutputStates(states);
  if (validateOutputStates(states, HIGH, HIGH, LOW, HIGH, LOW)) {
    Serial.println("[PASS] increment test passed.");
  } else {
    Serial.println("[FAIL] increment test failed.");
  }
}

void testDecrement() {
  Serial.println("Beginning decrement test...");
  resetControlLines();
  digitalWrite(CTRL_CS, LOW);
  digitalWrite(CTRL_DEC, LOW);
  digitalWrite(CTRL_CLK, HIGH);
  output_states states;
  readOutputStates(states);
  if (validateOutputStates(states, HIGH, LOW, HIGH, HIGH, LOW)) {
    Serial.println("[PASS] increment test passed.");
  } else {
    Serial.println("[FAIL] increment test failed.");
  }
}
 

boolean validateOutputStates(output_states &states, boolean load, boolean up, boolean down, boolean outputEnable, boolean reset) {
  if ((states.load != load) ||
       (states.up != up) ||
       (states.down != down) ||
       (states.outputEnable != outputEnable) ||
       (states.reset != reset)) {
    char buf[80];
    sprintf(buf, "    [FAIL] Expected load(%1d), up(%1d), down(%1d), oe(%1d), reset(%1d)", load, up, down, outputEnable, reset);
    Serial.println(buf);
    sprintf(buf, "           Received load(%1d), up(%1d), down(%1d), oe(%1d), reset(%1d)", states.load, states.up, states.down, states.outputEnable, states.reset);
    Serial.println(buf);
    return false;    
  }
  return true;
}

void readOutputStates(output_states &states) {
  states.load         = digitalRead(OUT_LOAD);
  states.up           = digitalRead(OUT_UP);
  states.down         = digitalRead(OUT_DOWN);
  states.outputEnable = digitalRead(OUT_OE);
  states.reset        = digitalRead(OUT_RST);
}

void loop() {
  // put your main code here, to run repeatedly:

}
