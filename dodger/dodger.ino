#define NUM_SEGS 27
#define JUMP_PIN
#define JUMP_TIME 1
#define RATE 10
#define LEVEL_UP = 20

int SPAWN_PROB = 10;

bool midJump = false;
bool reconfig = false;

int jumpTimer = 0;

int onSegs[NUM_SEGS];
int nextSegIndex = 0;

void setup() {
  Serial.begin(9600);

  pinMode(JUMP_PIN, INPUT);

  onSegs = malloc(NUM_SEGS * sizeof(int));
  
  for(int a = 0; a < NUM_SEGS; a++) {
    onSegs = -1;
  }

  onSegs[0] = /* Bottom of 1 */;
  nextSegIndex++;


  randomSeed(analogRead(0));
}

void reconfigure() {
  /* Left aligns all segs in the onSegs array, 
  * if one of them has gone out of bounds */

  int temp[NUM_SEGS] = malloc(NUM_SEGS * sizeof(int));
  int tempIndex = 0;

  for(int a = 0; a < nextSegIndex; a++) {
  if(onSegs[a] != -1) {
      temp[tempIndex] = onSegs[a];
      tempIndex++;
  }
  }

  free(onSegs);
  onSegs = temp;
  nextSegIndex = tempIndex;
  reconfig = false;
}

void loop() {
  // Detect jump
  if(digitalRead(BUTTON_PIN) && !midJump) {
    onSegs[0] = /* Top of 1 */;
    midJump = true;
  }

  if(jumpTimer >= JUMP_TIME) {
    onSegs[0] = /* Bot of 1 */;
    midJump = false;
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
  }

  if(reconfig)
    reconfigure();

  if(midJump) {
    jumpTimer++;
  }

  if(levelCount > LEVEL_UP)
    SPAWN_PROB += 5;

  delay(RATE);
}