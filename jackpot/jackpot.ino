#define NUM_SEGS 27
#define RATE 10
#define BLINK_RATE
#define BUTTON_PIN
#define BLINK_SEG 

int loopSegs[10];
int loopIndex = 0;

bool blinkOn = false;

void setup() {
  Serial.begin(9600);


  loopSegs = {/* Bot 1, Top 1, Top left 8, ... */};

  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  if(digitalRead(BUTTON_PIN)) {
    if(loopIndex == 0) {
      // win
    }

    // die
  }

  segDraw(loopSegs[loopIndex]);
  loopIndex = (loopIndex + 1) % 10;

  if(timer > BLINK_RATE) {
    blinkOn = !blinkOn;
    timer = 0;
  }

  if(blinkOn) {
    segDraw(/* PM */);
  }


  delay(RATE);
}