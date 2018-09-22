#define CLK 3
#define DIN 2

#define TOP 0
#define TOP_R 2
#define TOP_L 1
#define MID 3
#define BOT_R 5
#define BOT_L 4
#define BOT 6
#define DISP_ONE 14
#define DISP_TWO 7
#define DISP_THREE 0
#define AM 21
#define ONE_T 22
#define DOT_T 23
#define PM 24
#define DOT_B 25
#define ONE_B 26
#define NUM_SEGS 27

#define BUTTON_PIN 8
#define JUMP_TIME 100
#define LEVEL_UP 2000
#define END_TIME 3000

int SPAWN_PROB;
int spawnTimer;
int spawnTimeMin;
int endTimer;
int levelCount;

int numBullets;

bool midJump = false;
int jumpTimer;

bool reconfig = false;

int onSegs[NUM_SEGS];
int nextSegIndex;

int dieText[12];

int topArrayPath[4] = {
    DISP_THREE + TOP_R,
    DISP_TWO + TOP_R,
    DISP_ONE + TOP_R,
    ONE_T
  };
int botArrayPath[4] = {
    DISP_THREE + BOT_R,
    DISP_TWO + BOT_R,
    DISP_ONE + BOT_R,
    ONE_B
  };;

void initDodger() {
  SPAWN_PROB = 10;
  spawnTimeMin = 100;
  midJump = false;
  reconfig = false;
  jumpTimer = 0;
  nextSegIndex = 0; 
  levelCount = 0;
  numBullets = 0; 

  for(int a = 0; a < NUM_SEGS; a++) {
    onSegs[a] = -1;
  }
  nextSegIndex++;

  onSegs[0] = DISP_ONE + BOT_L/* Bottom of 1 */;
}

void onStart() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
  randomSeed(analogRead(0));


  initDodger();
}

void die() {
  setAllOff();
  writeNum(numBullets/100, DISP_ONE);
  writeNum((numBullets/10)%10, DISP_TWO);
  writeNum((numBullets/100)%10, DISP_THREE);

  if(endTimer > END_TIME) {
    initDodger();
  }

  endTimer++;
}

void reconfigure() {
  /* Left aligns all segs in the onSegs array, 
  * if one of them has gone out of bounds */

  int temp[NUM_SEGS];
  int tempIndex = 0;

  for(int a = 0; a < nextSegIndex; a++) {
    if(onSegs[a] != -1) {
        temp[tempIndex] = onSegs[a];
        tempIndex++;
    }
  }

  for (int a = 0; a < NUM_SEGS; a++) {
    onSegs[a] = temp[a];
  }
  nextSegIndex = tempIndex;
  reconfig = false;
}

int findPos(int arr[], int size, int el) {
  for(int a = 0; a < size; a++) 
    if(arr[a] == el)
      return a;
  
  return -1;
}

void updateLogic() {
  // Detect jump
  if(digitalRead(BUTTON_PIN) && !midJump) {
    onSegs[0] = ONE_T;
    midJump = true;
  }

  if(jumpTimer >= JUMP_TIME && spawnTimer >= spawnTimeMin) {
    onSegs[0] = ONE_B;
    midJump = false;
    spawnTimer = 0;
  }

  // Spawn new bullet
  if(random(100) < SPAWN_PROB) {
    int newBullet = DISP_THREE + TOP_R;

    if(random(100) < 50)
    newBullet = DISP_THREE + BOT_R;

    onSegs[nextSegIndex] = newBullet;
    nextSegIndex++;
    numBullets++;
  }

  // Re-position bullets
  for(int a = 1; a < nextSegIndex; a++) 
    if(onSegs[a] != -1) {
      if(onSegs[a] == ONE_T && midJump || onSegs[a] == ONE_B && !midJump) 
        die();

      int topIndex = findPos(topArrayPath, 4, onSegs[a]);
      int botIndex = findPos(botArrayPath, 4, onSegs[a]);
      if(topIndex >= 0) {
        if(topIndex < 3)
          onSegs[a] = topArrayPath[topIndex + 1];
        else {
          onSegs[a] = -1;
          reconfig = true;
        }
      }

      else if(botIndex >= 0) {
        if(botIndex < 3)
          onSegs[a] = botArrayPath[botIndex + 1];
        else {
          onSegs[a] = -1;
          reconfig = true;
        }
      }
      
      if(onSegs[a] != -1)
        (onSegs[a]);

      else {
        setSegOff(onSegs[a]);
      }
  }

  if(reconfig)
    reconfigure();

  if(midJump) {
    jumpTimer++;
  }

  if(levelCount % 1000 > LEVEL_UP)
    SPAWN_PROB += 5;
  
  levelCount++;
  spawnTimer++;
}

const int nums[10][7] = {
  {1, 1, 1, 0, 1, 1, 1}, //0
  {0, 1, 0, 0, 1, 0, 0}, //1
  {1, 1, 0, 1, 0, 1, 1}, //2
  {1, 0, 1, 1, 0, 1, 1}, //3
  {0, 1, 1, 1, 0, 1, 0}, //4
  {1, 0, 1, 1, 1, 0, 1}, //5
  {1, 0, 1, 1, 1, 1, 1}, //6
  {1, 0, 1, 0, 0, 1, 0}, //7
  {1, 1, 1, 1, 1, 1, 1}, //8
  {1, 1, 1, 1, 1, 0, 1}  //9
};
const int grounds[4] = {4, 5, 6, 7};

int count;
int vals[28];

void setup() {
  pinMode(CLK, OUTPUT);
  pinMode(DIN, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(grounds[i], OUTPUT); //set up pins
  }
  setAllOff();
  onStart();
}

void setAllOff() {
  for (int i = 0; i < 28; i++) {
    vals[i] = 0;
  }
}

void setSegOn(int seg) {
  vals[seg] = 1;
}
void setSegOff(int seg) {
  vals[seg] = 0;
}

void writeNum(int num, int disp_num) {
  for (int i = 0; i < 7; i++) {
    vals[disp_num + i] = nums[num][i];
  }
}

void writeVal(int val) {
  digitalWrite(CLK, LOW);
  digitalWrite(DIN, val);
  delayMicroseconds(1);
  digitalWrite(CLK, HIGH);
  delayMicroseconds(1);
  digitalWrite(CLK, LOW);
  delayMicroseconds(1);
  digitalWrite(DIN, LOW);
}
void loop() {
  updateLogic();
  for (int i = 0; i < 4; i++) {
    writeVal(0);
    for (int j = 6; j >= 0; j--) {
      writeVal(vals[i * 7 + j]);
    }
    digitalWrite(grounds[i], LOW);
    delay(2);
    digitalWrite(grounds[i], HIGH);
    delay(2);
    
  }

}
