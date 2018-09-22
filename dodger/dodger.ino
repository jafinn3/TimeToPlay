#define NUM_SEGS 27
#define JUMP_PIN
#define JUMP_TIME 1
#define RATE 10
#define LEVEL_UP = 20
#define END_TIME

int SPAWN_PROB;
int spawnTimer;
int spawnTimeMin;
int endTimer;

bool midJump = false;
int jumpTimer;

bool reconfig = false;

int onSegs[NUM_SEGS];
int nextSegInde;

int dieText[12];

void init() {
  SPAWN_PROB = 10;
  spawnTimeMin = 100;
  midJump = false;
  reconfig = false;
  jumpTimer = 0;
  nextSegIndex = 0;  

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

  dieText = {
    /* Letters for "die" */
  };

  init();
}

void die() {
  setAllOff();
  drawNum(levelCount % 1000);

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

void updateLogic() {
  // Detect jump
  if(digitalRead(BUTTON_PIN) && !midJump) {
    onSegs[0] = /* Top of 1 */;
    midJump = true;
  }

  if(jumpTimer >= JUMP_TIME && spawnTimer >= spawnTimeMin) {
    onSegs[0] = /* Bot of 1 */;
    midJump = false;
    spawnTimer = 0;
  }

  // Spawn new bullet
  if(random(100) < SPAWN_PROB) {
    int newBullet = /* Top last 8 */;

    if(random(100) < 50)
    newBullet = /* Bottom of last 8 */;

    onSegs[nextSegIndex] = newBullet;
    nextSegIndex++;
  }

  // Re-position bullets
  for(int a = 1; a < nextSegIndex; a++) 
    if(onSegs[a] != -1) {
    onSegs[a] = /* Next seg to left */;

    if(onSegs[a] == onSegs[0]) 
      // die
    else if(/* next pos is out of bounds to left */) {
      onSegs[a] = -1;
      reconfig = true;
    }
    
    if(onSegs[a] != -1)
      segDraw(onSegs[a]);

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
  
  spawnTimer++;
}
