
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define RED_BUTTON 2
#define YELLOW_BUTTON 3
#define GREEN_BUTTON 4
#define BLUE_BUTTON 5


#define RED 9
#define GREEN 10
#define BLUE 11



//reset pin
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

unsigned long gameUpdate;

//pong game
//ball (center)
uint8_t ballX = 64;
uint8_t ballY = 19;
uint8_t ballDirX = 1;
uint8_t ballDirY = 1;


const uint8_t PLAYER1_X = 112;
uint8_t player1Y = 14;
const uint8_t PLAYER2_X = 12;
uint8_t player2Y = 3;

uint8_t player1Score = 0;
uint8_t player2Score = 0;
const uint8_t PADDLE_HEIGHT = 8;
///////////////////////////////////////////
//dino game

const uint8_t DINO_HEIGHT = 10;
const uint8_t DINO_WIDTH = 10;
uint8_t foot = 0;
uint8_t nextFoot = 1;
bool inAir = false;
uint8_t speedY = 4;
uint8_t dinoX = 12;
uint8_t dinoY = 30-DINO_HEIGHT;
const unsigned long UPDATE_RATE = 16;
const uint8_t CACTUS_Y = 21;
uint8_t cactus1X = 64;
uint8_t cactus2X = 128;
uint8_t cactus3X = 178;
uint8_t cactus4X = 189;
uint8_t cactus5X = 250;
int dinoScore = 0;
uint8_t choose = 0;
unsigned long speedUpdate;
const unsigned long SPEED_RATE = 20000;
uint8_t cactusSpeed = 1;
bool isDead = false;


//snake game

int8_t dirX = 4;
int8_t dirY = 0;

struct Block{
  uint8_t x;
  uint8_t y;
  Block * next = NULL;
};

uint8_t fruitX = 9;
uint8_t fruitY = 9;
struct Block * snake = new Block();

void setup() 
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  pinMode(RED_BUTTON, INPUT);
  pinMode(YELLOW_BUTTON, INPUT);
  pinMode(GREEN_BUTTON, INPUT);
  pinMode(BLUE_BUTTON, INPUT);
  digitalWrite(RED_BUTTON, 1);
  digitalWrite(YELLOW_BUTTON, 1);
  digitalWrite(GREEN_BUTTON, 1);
  digitalWrite(BLUE_BUTTON, 1);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
}

void loop() 
{
  if(choose == 0){
    static bool r = false;
    static bool y = false;
    static bool g = false;
    static bool b = false;
    r |= (digitalRead(RED_BUTTON) == LOW);
    y |= (digitalRead(YELLOW_BUTTON) == LOW);
    g |= (digitalRead(GREEN_BUTTON) == LOW);
    b |= (digitalRead(BLUE_BUTTON) == LOW);
    choose = menu(r, y, g, b);
    r = false;
    y = false;
    g = false;
    b = false;
  }
  //setup for all games
  if(choose >= 1 && choose <= 4){
    choose = startGameSetup(choose);
  }
  if(choose == 5){
    choose = dino();
  }
  if(choose == 6){
    choose = snakeGame();
  }
  //5 - pong 1 player
  if(choose == 7){
    choose = pong1Player();
  }

  if(choose == 8){
    choose = pong2Players();
  }
  
  
}



void setColor(int red, int green, int blue)
{
  analogWrite(RED, red);
  analogWrite(GREEN, green);
  analogWrite(BLUE, blue);
  
}


uint8_t menu(bool r, bool y, bool g, bool b)
{
  display.setCursor(0,0);
  display.println("CHROME DINO - RED");
  display.println("SNAKE - YELLOW");
  display.println("PONG 1 PLAYER - GREEN");
  display.println("PONG 2 PLAYERS - BLUE");
  display.display();
  if(r){
    return 1;
  }
  if(y){
    return 2;
  }
  if(g){
    return 3;
  }
  if(b){
    return 4;
  }
  
  return 0;
}





int startGameSetup(int c){
  display.clearDisplay();
  speedUpdate = millis();
  dinoScore = 0;
  player1Score = 0;
  player2Score = 0;
  dinoX = 15;
  cactus1X = 64;
  cactus2X = 128;
  cactus3X = 178;
  cactus4X = 189;
  cactus5X = 250;
  isDead = false;
  cactusSpeed = 1;
  gameUpdate = millis();
  //snake
  snake->x = 5;
  snake->y = 5;
  struct Block * block2 = new Block();
  block2->x = 9;
  block2->y = 5;
  snake->next = block2;
  struct Block * block3 = new Block();
  block2->next = block3;
  block3->x = 13;
  block3->y = 5;

  //
  return c+4;
}




//snake

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


int snakeGame(){
  display.drawRect(0, 0, 128, 32, WHITE);
  unsigned long time = millis();
  static bool up = false;
  static bool down = false;
  static bool left = false;
  static bool right = false;
  up |= (digitalRead(GREEN_BUTTON) == LOW);
  down |= (digitalRead(BLUE_BUTTON) == LOW);
  left |= (digitalRead(RED_BUTTON) == LOW);
  right |= (digitalRead(YELLOW_BUTTON) == LOW);
  if(isDead == false && time > gameUpdate){
  struct Block * tmp;
  tmp = snake;
  while(tmp->next != NULL){  
    tmp = tmp->next;
  }
  struct Block * newOne = new Block();
  if(dirX == -4 && tmp->x == 1){
    newOne->x = 129;  
    newOne->y = tmp->y + dirY;
  }
  else if(dirY == -4 && tmp->y == 1){
    newOne->y = 29;
    newOne->x = tmp->x + dirX;
  }
  else{
      newOne->x = tmp->x + dirX;
      newOne->y = tmp->y + dirY;
  }
  if(newOne->x > 129){
  newOne->x = 1;
  }
  if(newOne->y > 29){
    newOne->y = 1;
  }
  struct Block *tmp2;
  tmp2 = snake;
  while(tmp2 != NULL){
    if(tmp2->x == newOne->x && tmp2->y == newOne->y){
      isDead = true;
      gameUpdate+=20000;
      break;
    }
    tmp2 = tmp2->next;
  }
  tmp->next = newOne;
  if(newOne->x != fruitX || newOne->y != fruitY){
  struct Block * toDelete = snake;
  delete toDelete;
  snake = snake->next;
  struct Block *tmpDisp;
  tmpDisp = snake;
  while(tmpDisp != NULL){
    display.fillRect(tmpDisp->x, tmpDisp->y, 4, 4, WHITE);
    tmpDisp = tmpDisp->next;
  }
  }
  else{
   fruitX = random(0,31)*4 + 1;
   fruitY = random(0,7) *4 + 1; 
   dinoScore+=1;
  }
  display.drawRect(fruitX, fruitY, 4, 4, WHITE);
  display.display();
  display.clearDisplay();
  
  if(up && dirY != 4){
    dirY = -4;
    dirX = 0;
  }
  if(down && dirY != -4){
    dirY = 4;
    dirX = 0;
  }
  if(left && dirX != 4){
    dirY = 0;
    dirX = -4;
  }
  if(right && dirX != -4){
    dirY = 0;
    dirX = 4;
  }
  gameUpdate += UPDATE_RATE;
  }
  if(isDead == true){
    if(snake->next != NULL){
    struct Block * tmp1 = snake->next;
    while(tmp1 != NULL){
      struct Block * tmp;
      tmp = tmp1;
      tmp1 = tmp1->next;
      delete tmp;
    }
    snake->next = NULL;
    }
    display.setCursor(0,2);
    display.print("You died!Final Score: ");
    display.setCursor(60,15);
    display.print(dinoScore);
    display.display();
    display.clearDisplay();
    up = false;
    down = false;
    left = false;
    right = false;
    if(time > gameUpdate+5000){  
      return 0;
    }
    
  }
  up = false;
  down = false;
  left = false;
  right = false;
  return 6;
}






















//PONGS
////////////////////////////////////////////////////////////////////////////////

uint8_t pong1Player(){
  display.drawRect(0, 0, 128, 32, WHITE);
  bool update = false;
  unsigned long time = millis();
  static bool up = false;
  static bool down = false;
  up |= (digitalRead(RED_BUTTON) == LOW);
  down |= (digitalRead(BLUE_BUTTON) == LOW);
  update = ballUpdate(time);
  update = pong1PlayerUpdate(time, up, down);
  if(update){
    display.display();
    gameUpdate += UPDATE_RATE;
  }
  up = false;
  down = false;
  if(player1Score == 3 || player2Score == 3){
    display.clearDisplay();
    return 0;
  }
  return 7;
}

uint8_t pong2Players(){
  display.drawRect(0, 0, 128, 32, WHITE);
  bool update = false;
  unsigned long time = millis();
  static bool up1 = false;
  static bool down1 = false;
  static bool up2 = false;
  static bool down2 = false;
  up1 |= (digitalRead(GREEN_BUTTON) == LOW);
  down1 |= (digitalRead(BLUE_BUTTON) == LOW);
  up2 |= (digitalRead(RED_BUTTON) == LOW);
  down2 |= (digitalRead(YELLOW_BUTTON) == LOW);
  update = ballUpdate(time);
  update = pong2PlayersUpdate(time, up1, down1, up2, down2);
  if(update){
    display.display();
    gameUpdate += UPDATE_RATE;
  }
  up1 = false;
  down1 = false;
  up2 = false;
  down2 = false;
  if(player1Score == 3 || player2Score == 3){
    display.clearDisplay();
    return 0;
  }
  return 8;
}

bool ballUpdate(unsigned long time){
  if(time > gameUpdate){
    uint8_t newX = ballX + ballDirX + ballDirX;
    uint8_t newY = ballY + ballDirY + ballDirY;
    if(newX <= 1){
      player1Score += 1;
      ballDirX = -ballDirX;
      display.clearDisplay();
      display.setCursor(40, 13);
      display.print(player2Score);
      display.setCursor(80, 13);
      display.print(player1Score);
    }
    if(newX >= 127){
      player2Score += 1;
      ballDirX = -ballDirX;
      display.clearDisplay();
      display.setCursor(40, 13);
      display.print(player2Score);
      display.setCursor(80, 13);
      display.print(player1Score);
    }
    if(player2Score > player1Score){
        setColor(1,0,0);
      }
      if(player2Score == player1Score){
        setColor(0,0,1);
      }
      if(player2Score < player1Score){
        setColor(0,1,0);
      }
    //if hit bounds || hit player1 paddle || hit player2 paddle
    if((newX == PLAYER1_X && newY >= player1Y && newY <= player1Y + PADDLE_HEIGHT)
    || (newX == PLAYER2_X && newY >= player2Y && newY <= player2Y + PADDLE_HEIGHT)){
      ballDirX = -ballDirX;
    }
    //if hit bounds
    if(newY <= 1 || newY >= 31){
      ballDirY = -ballDirY;
    }

    display.drawPixel(ballX, ballY, BLACK);
    display.drawPixel(newX, newY, WHITE);
    ballX = newX;
    ballY = newY;
    return true;
}
return false;
}

bool pong1PlayerUpdate(unsigned long time, bool up, bool down){
  const uint8_t centerPaddle = PADDLE_HEIGHT >> 1;
  if(time > gameUpdate){
    display.drawFastVLine(PLAYER2_X, player2Y, PADDLE_HEIGHT, BLACK);
    if(player2Y + centerPaddle > ballY){
      player2Y -= 1;
    }
    if(player2Y + centerPaddle < ballY){
      player2Y += 1;
    }
    if(player2Y + PADDLE_HEIGHT > 31){
      player2Y = 31 - PADDLE_HEIGHT;
    }
    if(player2Y < 1){
      player2Y = 1;
    }
    display.drawFastVLine(PLAYER1_X, player1Y, PADDLE_HEIGHT, BLACK);
    if(up){
      player1Y -= 1;
    }
    if(down){
      player1Y += 1; 
    }

    if(player1Y + PADDLE_HEIGHT > 31){
      player1Y = 31 - PADDLE_HEIGHT;
    }
    if(player1Y < 1){
      player1Y = 1;
    }
    display.drawFastVLine(PLAYER2_X, player2Y, PADDLE_HEIGHT, WHITE);
    display.drawFastVLine(PLAYER1_X, player1Y, PADDLE_HEIGHT, WHITE);
    return true;
  }
  return false;
}


bool pong2PlayersUpdate(unsigned long time, bool up1, bool down1, bool up2, bool down2){
  const uint8_t centerPaddle = PADDLE_HEIGHT >> 1;
  if(time > gameUpdate){
    display.drawFastVLine(PLAYER1_X, player1Y, PADDLE_HEIGHT, BLACK);
    display.drawFastVLine(PLAYER2_X, player2Y, PADDLE_HEIGHT, BLACK);
    if(up1){
      player1Y -= 1;
    }
    if(down1){
      player1Y += 1; 
    }
    if(player1Y + PADDLE_HEIGHT > 31){
      player1Y = 31 - PADDLE_HEIGHT;
    }
    if(player1Y < 1){
      player1Y = 1;
    }
    if(up2){
      player2Y -= 1;
    }
    if(down2){
      player2Y += 1; 
    }
    if(player2Y + PADDLE_HEIGHT > 31){
      player2Y = 31 - PADDLE_HEIGHT;
    }
    if(player2Y < 1){
      player2Y = 1;
    }
    display.drawFastVLine(PLAYER2_X, player2Y, PADDLE_HEIGHT, WHITE);
    display.drawFastVLine(PLAYER1_X, player1Y, PADDLE_HEIGHT, WHITE);
    return true;
  }
  return false;
}

//end pongs
///////////////////////////////////////////////////
















//chrome dino

//##########################################3

uint8_t dino(){
  display.drawFastHLine(0,31,128,WHITE);

  if(foot == 0 || inAir == true){
    drawDino(dinoX, dinoY, foot);  
  }
  else{
    drawDino(dinoX, dinoY+1, foot);
  }
  unsigned long time = millis();
  

  if(isDead == false){
  static bool up = false;
  static bool right = false;
  static bool left = false;
  up |= (digitalRead(RED_BUTTON) == LOW);
  right |= (digitalRead(BLUE_BUTTON) == LOW);
  left |= (digitalRead(GREEN_BUTTON) == LOW);
  if(((dinoX + 3 <= cactus1X + 5 && dinoX+DINO_WIDTH >= cactus1X)
  ||  (dinoX + 3 <= cactus2X + 5 && dinoX+DINO_WIDTH >= cactus2X)
  ||  (dinoX + 3 <= cactus3X + 5 && dinoX+DINO_WIDTH >= cactus3X)
  ||  (dinoX + 3 <= cactus4X + 5 && dinoX+DINO_WIDTH >= cactus4X)
  ||  (dinoX + 3 <= cactus5X + 5 && dinoX+DINO_WIDTH >= cactus5X))
  &&  (dinoY+DINO_HEIGHT >= CACTUS_Y && dinoY <= CACTUS_Y + 10)){
    isDead = true;
    gameUpdate+= 5000;
  }
  if(time > gameUpdate){
  display.setCursor(60,2);
  display.print(dinoScore);
  display.display();
  display.clearDisplay();
  gameUpdate += UPDATE_RATE;
  dinoScore+=1;
  }  
  if(foot == 0 && nextFoot == 2){
    foot = 2;
  }
  else if(foot == 0 && nextFoot == 1){
    foot = 1;
  }
  else if(foot == 1){
    foot = 0;
    nextFoot = 2;
  }
  else if(foot == 2){
    foot = 0;
    nextFoot = 1;
  }

  if(dinoY == 20){
    speedY = 0;
    inAir = false;
  }
  if(up && inAir == false){
    inAir = true;
    speedY = -5;
  }
  dinoY+=speedY;
  if(inAir)
  speedY+=1;

  if(right && dinoX <= 123){
    dinoX += 4;
  }
  if(left && dinoX >= 4){
    dinoX -= 4;
  }
  up = false;
  right = false;
  left = false;


  if(time > speedUpdate && cactusSpeed < 7){
    
    cactusSpeed+=1;
    speedUpdate+=SPEED_RATE;
    
  }
  if(cactusSpeed == 2){
    setColor(0,1,0);
  }
  if(cactusSpeed == 3){
    setColor(0,1,1);
  }
  if(cactusSpeed == 4){
    setColor(0,0,1);
  }
  if(cactusSpeed == 5){
    setColor(1,0,1);
  }
  if(cactusSpeed == 6){
    setColor(1,0,0);
  }
  
  drawCactus(cactus1X, CACTUS_Y);
  cactus1X-=cactusSpeed;
  
  drawCactus(cactus2X, CACTUS_Y);
  cactus2X-=cactusSpeed;

  drawCactus(cactus3X, CACTUS_Y);
  cactus3X-=cactusSpeed;

  drawCactus(cactus4X, CACTUS_Y);
  cactus4X-=cactusSpeed;

  drawCactus(cactus5X, CACTUS_Y);
  cactus5X-=cactusSpeed;
  }
  else{
  display.setCursor(0,2);
  display.print("You died!Final Score: ");
  display.setCursor(60,15);
  display.print(dinoScore);
  display.display();
  display.clearDisplay();
    if(time > gameUpdate){
      return 0;
    }
  }
  
  return 5;
}

void drawDino(uint8_t x, uint8_t y, uint8_t a){
  display.drawPixel(5+x, y, WHITE);
  display.drawPixel(6+x, y, WHITE);
  display.drawPixel(7+x, y, WHITE);
  display.drawPixel(8+x, y, WHITE);

  display.drawPixel(4+x, 1+y, WHITE);
  display.drawPixel(5+x, 1+y, WHITE);
  display.drawPixel(7+x, 1+y, WHITE);
  display.drawPixel(8+x, 1+y, WHITE);
  display.drawPixel(9+x, 1+y, WHITE);

  display.drawPixel(x, 2+y, WHITE);
  display.drawPixel(4+x, 2+y, WHITE);
  display.drawPixel(5+x, 2+y, WHITE);
  display.drawPixel(6+x, 2+y, WHITE);
  display.drawPixel(7+x, 2+y, WHITE);
  display.drawPixel(8+x, 2+y, WHITE);
  display.drawPixel(9+x, 2+y, WHITE);

  display.drawPixel(x, 3+y, WHITE);
  display.drawPixel(4+x, 3+y, WHITE);
  display.drawPixel(5+x, 3+y, WHITE);
  display.drawPixel(6+x, 3+y, WHITE);
  display.drawPixel(7+x, 3+y, WHITE);
  display.drawPixel(8+x, 3+y, WHITE);
  display.drawPixel(9+x, 3+y, WHITE);

  display.drawPixel(x, 4+y, WHITE);
  display.drawPixel(1+x, 4+y, WHITE);
  display.drawPixel(3+x, 4+y, WHITE);
  display.drawPixel(4+x, 4+y, WHITE);
  display.drawPixel(5+x, 4+y, WHITE);
  display.drawPixel(6+x, 4+y, WHITE);

  display.drawPixel(x, 5+y, WHITE);
  display.drawPixel(1+x, 5+y, WHITE);
  display.drawPixel(2+x, 5+y, WHITE);
  display.drawPixel(3+x, 5+y, WHITE);
  display.drawPixel(4+x, 5+y, WHITE);
  display.drawPixel(5+x, 5+y, WHITE);
  display.drawPixel(6+x, 5+y, WHITE);
  display.drawPixel(7+x, 5+y, WHITE);
  display.drawPixel(8+x, 5+y, WHITE);  

  display.drawPixel(1+x, 6+y, WHITE);
  display.drawPixel(2+x, 6+y, WHITE);
  display.drawPixel(3+x, 6+y, WHITE);
  display.drawPixel(4+x, 6+y, WHITE);
  display.drawPixel(5+x, 6+y, WHITE);
  display.drawPixel(6+x, 6+y, WHITE);

  display.drawPixel(2+x, 7+y, WHITE);
  display.drawPixel(3+x, 7+y, WHITE);
  display.drawPixel(4+x, 7+y, WHITE);
  display.drawPixel(5+x, 7+y, WHITE);
  display.drawPixel(6+x, 7+y, WHITE);
  display.drawPixel(7+x, 7+y, WHITE);

  display.drawPixel(3+x, 8+y, WHITE);
  display.drawPixel(5+x, 8+y, WHITE);

  if(a == 0 || a == 1)
  display.drawPixel(3+x, 9+y, WHITE);
  if(a == 0 || a == 2)
  display.drawPixel(5+x, 9+y, WHITE); 
}



void drawCactus(uint8_t x, uint8_t y){

  display.drawPixel(3+x, y, WHITE);
  display.drawPixel(4+x, y, WHITE);

  display.drawPixel(3+x, y+1, WHITE);
  display.drawPixel(4+x, y+1, WHITE);

  display.drawPixel(3+x, y+2, WHITE);
  display.drawPixel(4+x, y+2, WHITE);
  display.drawPixel(x, y+2, WHITE);
  display.drawPixel(1+x, y+2, WHITE);

  display.drawPixel(3+x, y+3, WHITE);
  display.drawPixel(4+x, y+3, WHITE);
  display.drawPixel(x, y+3, WHITE);
  display.drawPixel(1+x, y+3, WHITE);
  display.drawPixel(6+x, y+3, WHITE);
  display.drawPixel(7+x, y+3, WHITE);

  display.drawPixel(3+x, y+4, WHITE);
  display.drawPixel(4+x, y+4, WHITE);
  display.drawPixel(x, y+4, WHITE);
  display.drawPixel(1+x, y+4, WHITE);
  display.drawPixel(6+x, y+4, WHITE);
  display.drawPixel(7+x, y+4, WHITE);
  display.drawPixel(2+x, y+4, WHITE);

  display.drawPixel(3+x, y+5, WHITE);
  display.drawPixel(4+x, y+5, WHITE);
  display.drawPixel(x, y+5, WHITE);
  display.drawPixel(1+x, y+5, WHITE);
  display.drawPixel(6+x, y+5, WHITE);
  display.drawPixel(7+x, y+5, WHITE);
  display.drawPixel(2+x, y+5, WHITE);
  
  display.drawPixel(3+x, y+6, WHITE);
  display.drawPixel(4+x, y+6, WHITE);
  display.drawPixel(5+x, y+6, WHITE);
  display.drawPixel(6+x, y+6, WHITE);
  display.drawPixel(7+x, y+6, WHITE);

  display.drawPixel(3+x, y+7, WHITE);
  display.drawPixel(4+x, y+7, WHITE);

  display.drawPixel(3+x, y+8, WHITE);
  display.drawPixel(4+x, y+8, WHITE);

  display.drawPixel(3+x, y+9, WHITE);
  display.drawPixel(4+x, y+9, WHITE);
}
