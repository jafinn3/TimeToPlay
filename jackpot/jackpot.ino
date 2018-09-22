#define NUM_SEGS 27
#define BLINK_RATE
#define BUTTON_PIN
#define BLINK_SEG 
#define END_TIME

int loopSegs[10];
int loopIndex = 0;

int timer;
bool blinkOn;
bool winner;
bool loser;
int endTimer;

int winText[14];
int dieText[12];

void init() {
  timer = 0;
  blinkOn = false;
  winner = false;
  loser = false;
  endTimer = 0;
  setAllOff();
}

void onStart() {
  Serial.begin(9600);
  winText = {
    DISP_ONE + TOP_L,
    DISP_ONE + MID,
    DISP_ONE + TOP_R,
    DISP_ONE + BOT_R,
    DISP_ONE + BOT,

    DISP_TWO + TOP_L,
    DISP_TWO + MID,
    DISP_TWO + TOP_R,
    DISP_TWO + BOT_R,
    DISP_TWO + BOT_L,

    DISP_THREE + TOP_L,
    DISP_THREE + MID,
    DISP_THREE + TOP_R,
    DISP_THREE + BOT_R,
    DISP_THREE + BOT
  };

  dieText = {
    DISP_ONE + MID,
    DISP_ONE + TOP_R,
    DISP_ONE + BOT_R,
    DISP_ONE + BOT,
    DISP_ONE + BOT_L,

    DISP_TWO + TOP_R,
    DISP_TWO + BOT_R,

    DISP_THREE + MID,
    DISP_THREE + TOP_R,
    DISP_THREE + BOT_R,
    DISP_THREE + BOT,
    DISP_THREE + BOT_L
  };

  loopSegs = {
    DISP_ONE + BOT_L,
    DISP_ONE + TOP_L,
    DISP_ONE + TOP,
    DISP_TWO + TOP,
    DISP_THREE + TOP,
    DISP_THREE + TOP_R,
    DISP_THREE + BOT_R,
    DISP_THREE + BOT,
    DISP_TWO + BOT,
    DISP_ONE + BOT
  };
  pinMode(BUTTON_PIN, INPUT);
  init();
}

void win() {
  setAllOff();
  // Print "yay"
  for(int a = 0; a < 14; a++) {
    segOn(winText[a]);
  }

  if(endTimer > END_TIME) {
    init();
  }

  endTimer++;
}

void die() {
  setAllOff();
  // Print "die"
  for(int a = 0; a < 12; a++) {
    segOn(dieText[a]);
  }

  if(endTimer > END_TIME) {
    init();
  }

  endTimer++;
}

void updateLogic() {
  if(winner)
    win();
  else if(loser)
    die();
  else {
    if(digitalRead(BUTTON_PIN)) {
      if(loopIndex == 0) {
        winner = true;
      }

      loser = true;
    }

    segOn(loopSegs[loopIndex]);
    if(loopIndex != 0)
      segOff(loopSegs[loopIndex - 1]);
    else
      segOff(loogSegs[9]);

    loopIndex = (loopIndex + 1) % 10;

    if(timer > BLINK_RATE) {
      blinkOn = !blinkOn;
      timer = 0;
    }

    if(blinkOn) {
      segOn(ONE_B);
    }

    else {
      segOff(ONE_B);
    }

    timer++;
  }
}