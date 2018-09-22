#define NUM_SEGS 27
#define JUMP_PIN
#define JUMP_TIME 1
#define RATE 10
#define LEVEL_UP = 2000
#define END_TIME

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
int nextSegInde;

int dieText[12];

int topArrayPath[4];
int botArrayPath[4];

void init() {
  SPAWN_PROB = 10;
  spawnTimeMin = 100;
  midJump = false;
  reconfig = false;
  jumpTimer = 0;
  nextSegIndex = 0; 
  levelCount = 0;
  numBullets = 0; 

  for(int a = 0; a < NUM_SEGS; a++) {
    onSegs = -1;
  }
  nextSegIndex++;

  onSegs[0] = /* Bottom of 1 */;
}

void onStart() {
  Serial.begin(9600);
  pinMode(JUMP_PIN, INPUT);
  randomSeed(analogRead(0));

  topArrayPath = {
    DISP_THREE + TOP_R,
    DISP_TWO + TOP_R,
    DISP_ONE + TOP_R,
    ONE_T
  };

  botArrayPath = {
    DISP_THREE + BOT_R,
    DISP_TWO + BOT_R,
    DISP_ONE + BOT_R,
    ONE_B
  };

  init();
}

void die() {
  setAllOff();
  drawNum(numBullets);

  if(endTimer > END_TIME) {
    init();
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
  onSegs = temp;
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
      if(onSegs[a] == ONE_T && midJump || onSegs[a] = ONE_B && !midJump) 
        die();

      int topIndex = findPos(topArrayPath, 4, onSegs[a]);
      int botIndex = findPos(botArrayPath, 4, onSegs[a]);
      if(topIndex >= 0) {
        if(topIndex < 3)
          onSegs[a] = topArrayPath[topIndex + 1)];
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
        segOff(onSegs[a]);
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
