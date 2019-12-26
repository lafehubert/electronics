// This program will run tests on a 16V8B GAL programmed to be a board-select 
// address-decoder (see pinout and PLD file below)

/*
 * ram_address_decoder.pld:
 * 
GAL16V8    ; address decoder for YA4B RAM Board
RamSelect  ;


CLK   A7    A6    A5    A4    J2    J1    J0    RD   GND
NC    CS0   CS1   OE    W0    W1    NC    BS    WR   VCC

/BS = A7 * J2 * A6 * J1 * A5 * J0 +
     A7 * J2 * A6 * J1 * /A5 * /J0 +
     A7 * J2 * /A6 * /J1 * A5 * J0 +
     A7 * J2 * /A6 * /J1 * /A5 * /J0 +
     /A7 * /J2 * A6 * J1 * A5 * J0 +
     /A7 * /J2 * A6 * J1 * /A5 * /J0 +
     /A7 * /J2 * /A6 * /J1 * A5 * J0 +
     /A7 * /J2 * /A6 * /J1 * /A5 * /J0

/CS0 = A7 * J2 * A6 * J1 * A5 * J0 * /A4 +
     A7 * J2 * A6 * J1 * /A5 * /J0 * /A4 +
     A7 * J2 * /A6 * /J1 * A5 * J0 * /A4 +
     A7 * J2 * /A6 * /J1 * /A5 * /J0 * /A4 +
     /A7 * /J2 * A6 * J1 * A5 * J0 * /A4 +
     /A7 * /J2 * A6 * J1 * /A5 * /J0 * /A4 +
     /A7 * /J2 * /A6 * /J1 * A5 * J0 * /A4 +
     /A7 * /J2 * /A6 * /J1 * /A5 * /J0 * /A4

/CS1 = A7 * J2 * A6 * J1 * A5 * J0 * A4 +
     A7 * J2 * A6 * J1 * /A5 * /J0 * A4 +
     A7 * J2 * /A6 * /J1 * A5 * J0 * A4 +
     A7 * J2 * /A6 * /J1 * /A5 * /J0 * A4 +
     /A7 * /J2 * A6 * J1 * A5 * J0 * A4 +
     /A7 * /J2 * A6 * J1 * /A5 * /J0 * A4 +
     /A7 * /J2 * /A6 * /J1 * A5 * J0 * A4 +
     /A7 * /J2 * /A6 * /J1 * /A5 * /J0 * A4

/OE = A7 * J2 * A6 * J1 * A5 * J0 * /RD +
     A7 * J2 * A6 * J1 * /A5 * /J0 * /RD +
     A7 * J2 * /A6 * /J1 * A5 * J0 * /RD +
     A7 * J2 * /A6 * /J1 * /A5 * /J0 * /RD +
     /A7 * /J2 * A6 * J1 * A5 * J0 * /RD +
     /A7 * /J2 * A6 * J1 * /A5 * /J0 * /RD +
     /A7 * /J2 * /A6 * /J1 * A5 * J0 * /RD +
     /A7 * /J2 * /A6 * /J1 * /A5 * /J0 * /RD

/W0 = A7 * J2 * A6 * J1 * A5 * J0 * /A4 * /WR * CLK +
     A7 * J2 * A6 * J1 * /A5 * /J0 * /A4 * /WR * CLK +
     A7 * J2 * /A6 * /J1 * A5 * J0 * /A4 * /WR * CLK +
     A7 * J2 * /A6 * /J1 * /A5 * /J0 * /A4 * /WR * CLK +
     /A7 * /J2 * A6 * J1 * A5 * J0 * /A4 * /WR * CLK +
     /A7 * /J2 * A6 * J1 * /A5 * /J0 * /A4 * /WR * CLK +
     /A7 * /J2 * /A6 * /J1 * A5 * J0 * /A4 * /WR * CLK +
     /A7 * /J2 * /A6 * /J1 * /A5 * /J0 * /A4 * /WR * CLK

/W1 = A7 * J2 * A6 * J1 * A5 * J0 * A4 * /WR * CLK +
     A7 * J2 * A6 * J1 * /A5 * /J0 * A4 * /WR * CLK +
     A7 * J2 * /A6 * /J1 * A5 * J0 * A4 * /WR * CLK +
     A7 * J2 * /A6 * /J1 * /A5 * /J0 * A4 * /WR * CLK +
     /A7 * /J2 * A6 * J1 * A5 * J0 * A4 * /WR * CLK +
     /A7 * /J2 * A6 * J1 * /A5 * /J0 * A4 * /WR * CLK +
     /A7 * /J2 * /A6 * /J1 * A5 * J0 * A4 * /WR * CLK +
     /A7 * /J2 * /A6 * /J1 * /A5 * /J0 * A4 * /WR * CLK

DESCRIPTION


This replaces a bunch of combinatorial logic
used for selecting one of two RAM chips based
on the A4 line, and routes the proper RD and WR
signals to the chips, and also controls a bus
transciever's output enable.
 *
 * pinout:

  
                                GAL16V8

                          -------\___/-------
                      CLK |  1           20 | VCC
                          |                 |
                       A7 |  2           19 | WR
                          |                 |
                       A6 |  3           18 | BS
                          |                 |
                       A5 |  4           17 | NC
                          |                 |
                       A4 |  5           16 | W1
                          |                 |
                       J2 |  6           15 | W0
                          |                 |
                       J1 |  7           14 | OE
                          |                 |
                       J0 |  8           13 | CS1
                          |                 |
                       RD |  9           12 | CS0
                          |                 |
                      GND | 10           11 | NC
                          -------------------

 */

// outputs
#define OUT_A4 22
#define OUT_A5 24
#define OUT_A6 26
#define OUT_A7 28

#define OUT_J0 30
#define OUT_J1 32
#define OUT_J2 34

#define OUT_RD 36
#define OUT_WR 38
#define OUT_CLK 40

// inputs 
#define IN_BS 23
#define IN_CS0 25
#define IN_CS1 27
#define IN_OE 29
#define IN_W0 31
#define IN_W1 33

#define STR_HIGH "high"
#define STR_LOW  "low"

#define SIG_HIGH true
#define SIG_LOW false

int address_pins[4] = { OUT_A4, OUT_A5, OUT_A6, OUT_A7 };
int jumper_pins[3] = { OUT_J0, OUT_J1, OUT_J2 };
const char* states[2] = { STR_LOW, STR_HIGH };

struct input_state {
  int jumpers;
  int address;
  boolean rd;
  boolean wr;
  boolean clk;
};

struct output_state {
  boolean bs;
  boolean cs0;
  boolean cs1;
  boolean oe;
  boolean w0;
  boolean w1;
};

struct test_case {
  input_state input;
  output_state expected;
};

void setup() {
  Serial.begin(57600);
  setupPins();
  runTest();
}

void setupPins() {
  for (int i = OUT_A4; i <= OUT_CLK; i+=2) {
    pinMode(i, OUTPUT);
  }
  for (int i = IN_BS; i <= IN_W1; i+=2) {
    pinMode(i, INPUT);
  }
}

void runTest() {
  test_case cases[120];
  int next_case = setupTestCases(cases);
  for (int i = 0; i < next_case; i++) {
    char buf[80];
    sprintf(buf, "Running test case %d of %d", i, next_case);
    Serial.println(buf);
    runTestCase(cases[i], false);
    delay(1);
  }
  Serial.println("");
  Serial.println("Test cases complete!");
}

int setupTestCases(test_case *cases) {
  int next_case = 0;
  next_case = setupBoardNotSelectedCases(cases, next_case);
  next_case = setupBoardSelectedCases(cases, next_case);
  next_case = setupOutputEnableTestCases(cases, next_case);
  next_case = setupWriteEnableTestCases(cases, next_case);
  next_case = setupWriteEnableClockTestCases(cases, next_case);
  return next_case;
}

int setupBoardNotSelectedCases(test_case *cases, int next_case) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (i != j) {
        char buf[40];
        sprintf(buf, "Setting up case %d: j[%d], a[%d]", next_case, j, i << 1);
        Serial.println(buf);
        setupTestCase(cases[next_case++], j, i << 1, SIG_HIGH, SIG_HIGH, SIG_LOW,    SIG_HIGH, SIG_HIGH, SIG_HIGH, SIG_HIGH, SIG_HIGH, SIG_HIGH);
      }
    }
  }
  return next_case;
}

int setupBoardSelectedCases(test_case *cases, int next_case) {
  for (int i = 0; i < 8; i++) {
    char buf[40];
    sprintf(buf, "Setting up case %d: j[%d], a[%d]", next_case, i, i << 1);
    Serial.println(buf);
    setupTestCase(cases[next_case++], i, i << 1,       SIG_HIGH, SIG_HIGH, SIG_LOW,     SIG_LOW, SIG_LOW,  SIG_HIGH, SIG_HIGH, SIG_HIGH, SIG_HIGH);
    sprintf(buf, "Setting up case %d: j[%d], a[%d]", next_case, i, (i << 1) + 1);
    Serial.println(buf);
    setupTestCase(cases[next_case++], i, (i << 1) + 1, SIG_HIGH, SIG_HIGH, SIG_LOW,     SIG_LOW, SIG_HIGH, SIG_LOW,  SIG_HIGH, SIG_HIGH, SIG_HIGH);
  }
  return next_case;
}

int setupOutputEnableTestCases(test_case *cases, int next_case) {
  for (int i = 0; i < 8; i++) {
    char buf[40];
    sprintf(buf, "Setting up case %d: j[%d], a[%d] rd[low]", next_case, i, i << 1);
    Serial.println(buf);
    setupTestCase(cases[next_case++], i, i << 1,       SIG_LOW, SIG_HIGH, SIG_LOW,      SIG_LOW, SIG_LOW,  SIG_HIGH, SIG_LOW, SIG_HIGH, SIG_HIGH);
    sprintf(buf, "Setting up case %d: j[%d], a[%d] rd[low]", next_case, i, (i << 1) + 1);
    Serial.println(buf);
    setupTestCase(cases[next_case++], i, (i << 1) + 1, SIG_LOW, SIG_HIGH, SIG_LOW,      SIG_LOW, SIG_HIGH, SIG_LOW,  SIG_LOW, SIG_HIGH, SIG_HIGH);
  }
  return next_case;
}

int setupWriteEnableTestCases(test_case *cases, int next_case) {
  for (int i = 0; i < 8; i++) {
    char buf[40];
    sprintf(buf, "Setting up case %d: j[%d], a[%d] wr[low]", next_case, i, i << 1);
    Serial.println(buf);
    setupTestCase(cases[next_case++], i, i << 1,       SIG_HIGH, SIG_LOW, SIG_LOW,      SIG_LOW, SIG_LOW,  SIG_HIGH, SIG_HIGH, SIG_HIGH, SIG_HIGH);
    sprintf(buf, "Setting up case %d: j[%d], a[%d] wr[low]", next_case, i, (i << 1) + 1);
    Serial.println(buf);
    setupTestCase(cases[next_case++], i, (i << 1) + 1, SIG_HIGH, SIG_LOW, SIG_LOW,      SIG_LOW, SIG_HIGH, SIG_LOW,  SIG_HIGH, SIG_HIGH, SIG_HIGH);
  }
  return next_case;
}

int setupWriteEnableClockTestCases(test_case *cases, int next_case) {
  for (int i = 0; i < 8; i++) {
    char buf[40];
    sprintf(buf, "Setting up case %d: j[%d], a[%d] wr[low] clk[high]", next_case, i, i << 1);
    Serial.println(buf);
    setupTestCase(cases[next_case++], i, i << 1,       SIG_HIGH, SIG_LOW, SIG_HIGH,      SIG_LOW, SIG_LOW,  SIG_HIGH, SIG_HIGH, SIG_LOW, SIG_HIGH);
    sprintf(buf, "Setting up case %d: j[%d], a[%d] wr[low] clk[high]", next_case, i, (i << 1) + 1);
    Serial.println(buf);
    setupTestCase(cases[next_case++], i, (i << 1) + 1, SIG_HIGH, SIG_LOW, SIG_HIGH,      SIG_LOW, SIG_HIGH, SIG_LOW,  SIG_HIGH, SIG_HIGH, SIG_LOW);
  }
  return next_case;
}

void setupTestCase(test_case &tcase, int jumpers, int address, boolean rd, boolean wr, boolean clk, boolean bs, boolean cs0, boolean cs1, boolean oe, boolean w0, boolean w1) {
  tcase.input.jumpers = jumpers;
  tcase.input.address = address;
  tcase.input.rd = rd;
  tcase.input.wr = wr;
  tcase.input.clk = clk;

  tcase.expected.bs = bs;
  tcase.expected.cs0 = cs0;
  tcase.expected.cs1 = cs1;
  tcase.expected.oe = oe;
  tcase.expected.w0 = w0;
  tcase.expected.w1 = w1;
}

void runTestCase(test_case &tcase, boolean print_pass) {
  setState(&tcase.input);
  delay(1);
  output_state actual;
  readState(&actual);
  if (different(&tcase.expected, &actual)) {
    printFail(&tcase.input, &tcase.expected, &actual);
  } else if (print_pass) {
    printPass(&tcase.input, &actual);
  }
}

boolean different(output_state *expected, output_state *actual) {
  return (expected->bs != actual->bs) ||
         (expected->cs0 != actual->cs0) ||
         (expected->cs1 != actual->cs1) ||
         (expected->oe != actual->oe) ||
         (expected->w0 != actual->w0) ||
         (expected->w1 != actual->w1); 
}

void printFail(input_state *input, output_state *expected, output_state *actual) {
  printInput(input);
  char buf[120];
  sprintf(buf,"    FAIL - expected bs:%4s cs0:%4s cs1:%4s oe:%4s w0:%4s w1:%4s;\n           received bs:%4s cs0:%4s cs1:%4s oe:%4s w0:%4s w1:%4s.",
      states[expected->bs], states[expected->cs0], states[expected->cs1], states[expected->oe], states[expected->w0], states[expected->w1], 
      states[actual->bs], states[actual->cs0], states[actual->cs1], states[actual->oe], states[actual->w0], states[actual->w1]);
  Serial.println(buf);
}

void printPass(input_state *input, output_state *expected) {
  printInput(input);
  char buf[120];
  sprintf(buf, "    pass - received bs:%s cs0:%s cs1:%s oe:%s w0:%s w1:%s.",
      states[expected->bs], states[expected->cs0], states[expected->cs1], states[expected->oe], states[expected->w0], states[expected->w1]);
  Serial.println(buf);
}

void setState(input_state *input) {
  setJumpers(input->jumpers);
  setAddress(input->address);
  setRead(input->rd);
  setWrite(input->wr);
  setClock(input->clk);
}

void printInput(input_state *input) {
  char addressBuf[5];
  toBinary(4, input->address, addressBuf);
  char jumperBuf[4];
  toBinary(3, input->jumpers, jumperBuf);
  char buf[80];
  sprintf(buf, "Input - address:%s, jumpers:%s, rd:%s, wr:%s, clk:%s", addressBuf, jumperBuf, states[input->rd], states[input->wr], states[input->clk]);
  Serial.println(buf);
}

void setRead(boolean read) {
  digitalWrite(OUT_RD, read);
}

void setWrite(boolean write) {
  digitalWrite(OUT_WR, write);
}

void setClock(boolean clock) {
  digitalWrite(OUT_CLK, clock);
}

void setJumpers(int address) {
  int values[3];
  for (int i = 0; i < 3; i++) {
    values[i] = (address >> i) & 1;
    digitalWrite(jumper_pins[i], values[i]);
  }
}

void setAddress(int address) {
  int values[4];
  for (int i = 0; i < 4; i++) {
    values[i] = (address >> i) & 1;
    digitalWrite(address_pins[i], values[i]);
  }
}

void readState(output_state *output) {
  output->bs = digitalRead(IN_BS);
  output->cs0 = digitalRead(IN_CS0);
  output->cs1 = digitalRead(IN_CS1);
  output->oe = digitalRead(IN_OE);
  output->w0 = digitalRead(IN_W0);
  output->w1 = digitalRead(IN_W1);
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
