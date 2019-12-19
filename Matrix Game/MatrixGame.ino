#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "LedControl.h"
#include "Timer.h"
#include "pitches.h"

#define POTPIN A2 // controller potentiometer
#define PADSIZE 3
#define BALL_DELAY 200
#define GAME_DELAY 10
#define BOUNCE_VERTICAL 1
#define BOUNCE_HORIZONTAL -1
#define NEW_GAME_ANIMATION_SPEED 50
#define HIT_NONE 0
#define HIT_CENTER 1
#define HIT_LEFT 2
#define HIT_RIGHT 3

// LCD pins
const int d4 = 1;
const int d5 = 2;
const int d6 = 4;
const int d7 = 7;
const int RS = 8;
const int enable = 9;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
String menu[] = {"Start Game", "Highscore", "Settings", "Info"};

// joystick
const int xPin = A1;
const int yPin = A0;
const int pushPin = A3;
int xValue, yValue, pushValue;
bool xReset = true;
bool yReset = true;
int minTreshold = 400;
int maxTreshold = 600;
bool lastPushValue = 1;

// menu
int lives = 3;
int highscore = 0;
bool clickedMenu = 0;
int selectedMenu = 1;
int maxName = 15;
char bestPlayer[16];
String currPlayer;
unsigned long long int lastMillis = 0;
int LCDdelay = 300;
unsigned int startTime = 0;
unsigned int levelTime = 0;
int level = 0;
int score;
int i, j;
int startedFunc = 1;
int charNo = 0;
int pass = 0;
int nextChar;
int vec[16];
int screenInfo = 0;

// variables to hold the LED color
int rVal = 254;
int gVal = 1;
int bVal = 127;
int rDir = -1;
int gDir = 1;
int bDir = -1;
// constants to name the pins
const int rPin = 3;
const int gPin = 5;
const int bPin = 6;

// Santa Claus is coming to town
#define melodyPin 11
int santa_melody[] = {
  NOTE_G4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,
  NOTE_D4, NOTE_F4, NOTE_B3,
  NOTE_C4
};

int santa_tempo[] = {
  8,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 2,
  4, 4, 4, 4,
  4, 2, 4,
  1
};

int switchOne = 0;
int switchTwo = 0;
int switchThree = 0;


void selectMenuFunc() {
  int xValue = analogRead(xPin);
  if (xValue < minTreshold && xReset) {
    selectedMenu--;
    if (selectedMenu < 1)
      selectedMenu = 4;
    xReset = 0;
  }
  if (xValue > maxTreshold && xReset) {
    selectedMenu++;
    if (selectedMenu > 4)
      selectedMenu = 1;
    xReset = 0;
  }
  if (xReset == 0 && xValue >= minTreshold && xValue <= maxTreshold)
    xReset = 1;
  displayMenuFunc();
}

void saveHighscoreFunc() {
  EEPROM.put(0, highscore);
  EEPROM.put(4, bestPlayer);
}

void clearFunc() {
  for (i = 0; i < EEPROM.length(); i++)
    EEPROM.write(i, 0);
}

void displayMenuFunc() {
  if (millis() - lastMillis >= LCDdelay) {
    lastMillis = millis();
    lcd.clear();
    lcd.setCursor(0, 0);

    if (selectedMenu == 1)
      lcd.print(">");
    else
      lcd.print(" ");
    lcd.print("Start");

    if (selectedMenu == 2)
      lcd.print(">");
    else
      lcd.print(" ");
    lcd.print("Highscore");

    lcd.setCursor(0, 1);

    if (selectedMenu == 3)
      lcd.print(">");
    else
      lcd.print(" ");
    lcd.print("Settings");

    if (selectedMenu == 4)
      lcd.print(">");
    else
      lcd.print(" ");
    lcd.print("Info");
  }
}

bool pressedButtonFunc() {
  pushValue = digitalRead(pushPin);
  if (pushValue == 0 && lastPushValue == 1) {
    lastPushValue = 0;
    return 1;
  }
  if (pushValue && lastPushValue == 0)
    lastPushValue = 1;
  return 0;
}

void gameStatusFunc(int lives, int level, int score) {
  if (millis() - lastMillis > LCDdelay) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lives:"); lcd.print(lives);
    lcd.print(" Level:"); lcd.print(level);
    lcd.setCursor(0, 1);
    lcd.print("Score:"); lcd.print(score);
    lastMillis = millis();
  }
}

bool gameEndFunc() {
  if (startedFunc == false && millis() - startTime > 10000)
    return true;
  return false;
}

void getNameFunc() {
  if (Serial.available() > 0)
    currPlayer = Serial.readString();
}

void playGameFunc() {
  if (gameEndFunc()) {
    if (score > highscore) {
      highscore = score;
      strcpy(bestPlayer, currPlayer.c_str());
      saveHighscoreFunc();
    }
    if (millis() - lastMillis > LCDdelay) {
      lastMillis = millis();
      lcd.clear();
      // lcd.setCursor(0, 0);
      lcd.print("Congratulations!");
      lcd.setCursor(0, 1);
      lcd.print("Press for menu");
      if (pressedButtonFunc()) {
        clickedMenu = false;
        startedFunc = true;
      }
    }
  }
  else {
    if (startedFunc == true) {
      lives = 3;
      startTime = levelTime = millis();
      score = level * 3;
      startedFunc = false;
      if (currPlayer.length() == 0)
        currPlayer = "anon";
    }
    if (millis() - levelTime >= 5000) {
      levelTime = millis();
      level++;
      score += 3;
    }
    gameStatusFunc(lives, level, score);
  }
}

void moreSettingsFunc() {
  //Serial.println("Settings");
  if (millis() - lastMillis > LCDdelay) {
    lcd.clear();
    lcd.print("Level: "); lcd.print(level);
    lcd.setCursor(0, 1); lcd.print(currPlayer);
    lastMillis = millis();
  }
  xValue = analogRead(xPin);
  if (xValue < minTreshold && xReset) {
    level--;
    if (level < 0)
      level = 9;
    xReset = false;
  }
  else if (xValue > maxTreshold && xReset) {
    level++;
    if (level > 9)
      level = 0;
    xReset = 0;
  }
  else if (xValue >= minTreshold && xValue <= maxTreshold)
    xReset = true;
  getNameFunc();
  if (pressedButtonFunc())
    clickedMenu = false;
}

void infoFunc() {
  xValue = analogRead(xPin);
  if (xValue < minTreshold && xReset == false) {
    if (screenInfo > 0) {
      screenInfo--;
    }
    else {
      screenInfo = 3;
    }
    xReset = true;
  }
  if (xValue > maxTreshold && xReset == false) {
    if (screenInfo < 3) {
      screenInfo++;
    }
    else {
      screenInfo = 0;
    }
    xReset = true;
  }

  if (xValue >= minTreshold && xValue <= maxTreshold) {
    xReset = false;
  }
  delay(1);

  if (screenInfo == 0) {
    lcd.clear();
    lcd.print("Creator: Anca-Maria");
    lcd.setCursor(0, 1);
    lcd.print("Brancoveanu");
  }
  else if (screenInfo == 1) {
    lcd.clear();
    lcd.print("   Game name:  ");
    lcd.setCursor(0, 1);
    lcd.print("Atari Breakout");
  }
  else if (screenInfo == 2) {
    lcd.clear();
    lcd.print("  Github link: ");
    lcd.setCursor(0, 1);
    lcd.print("https://github.");

    yValue = analogRead(yPin);
    if (yValue > maxTreshold) {
      lcd.clear();
      lcd.print("  Github link: ");
      lcd.setCursor(0, 1);
      lcd.print("com/bancamaria  ");
      delay(1000);
      xReset = true;
    }
  }
  else if (screenInfo == 3) {
    lcd.clear();
    lcd.print("  Powered by:  ");
    lcd.setCursor(0, 1);
    lcd.print("@UnibucRobotics");
  }
  delay(50);
}

void highscoreFunc() {
  if (millis() - lastMillis > LCDdelay) {
    lastMillis = millis();
    lcd.clear();
    lcd.print(bestPlayer);
    lcd.print(" ");
    lcd.print(highscore);
    if (pressedButtonFunc())
      clickedMenu = false;
  }
}




byte sad[8] = {0x3C, 0x42, 0xA5, 0x81, 0x99, 0xA5, 0x42, 0x3C};
byte smile[8] = {0x3C, 0x42, 0xA5, 0x81, 0xA5, 0x99, 0x42, 0x3C};

Timer timer;

LedControl lc = LedControl(12, 13, 10, 1);

byte direction; // Wind rose, 0 is north
int xball, yball;
int yball_prev;
byte xpad;
int ball_timer;
int currentMillis = 0;
int previousMillis = 0;

void setSprite(byte *sprite) {
  for (int r = 0; r < 8; r++) {
    lc.setRow(0, r, sprite[r]);
  }
}

void newGame() {
  lc.clearDisplay(0);
  // initial position
  xball = random(1, 7);
  yball = 1;
  direction = random(3, 6); // Go south
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      lc.setLed(0, r, c, HIGH);
      delay(NEW_GAME_ANIMATION_SPEED);
    }
  }
  setSprite(smile);
  currentMillis = millis();
  if (currentMillis - previousMillis >= 1500) {
    previousMillis = currentMillis;
  }
  lc.clearDisplay(0);
}

void setPad() {
  xpad = map(analogRead(POTPIN), 0, 1020, 8 - PADSIZE, 0);
}

void debug(const char* desc) {
#ifdef DEBUG
  Serial.print(desc);
  Serial.print(" XY: ");
  Serial.print(xball);
  Serial.print(", ");
  Serial.print(yball);
  Serial.print(" XPAD: ");
  Serial.print(xpad);
  Serial.print(" DIR: ");
  Serial.println(direction);
#endif
}

int checkBounce() {
  if (!xball || !yball || xball == 7 || yball == 6) {
    int bounce = (yball == 0 || yball == 6) ? BOUNCE_HORIZONTAL : BOUNCE_VERTICAL;
#ifdef DEBUG
    debug(bounce == BOUNCE_HORIZONTAL ? "HORIZONTAL" : "VERTICAL");
#endif
    return bounce;
  }
  return 0;
}

int getHit() {
  if (yball != 6 || xball < xpad || xball > xpad + PADSIZE) {
    return HIT_NONE;
  }
  if (xball == xpad + PADSIZE / 2) {
    return HIT_CENTER;
  }
  return xball < xpad + PADSIZE / 2 ? HIT_LEFT : HIT_RIGHT;
}

bool checkLoose() {
  return yball == 6 && getHit() == HIT_NONE;
}

void moveBall() {
  debug("MOVE");
  int bounce = checkBounce();
  if (bounce) {
    switch (direction) {
      case 0:
        direction = 4;
        break;
      case 1:
        direction = (bounce == BOUNCE_VERTICAL) ? 7 : 3;
        break;
      case 2:
        direction = 6;
        break;
      case 6:
        direction = 2;
        break;
      case 7:
        direction = (bounce == BOUNCE_VERTICAL) ? 1 : 5;
        break;
      case 5:
        direction = (bounce == BOUNCE_VERTICAL) ? 3 : 7;
        break;
      case 3:
        direction = (bounce == BOUNCE_VERTICAL) ? 5 : 1;
        break;
      case 4:
        direction = 0;
        break;
    }
    debug("->");
  }

  // Check hit: modify direction is left or right
  switch (getHit()) {
    case HIT_LEFT:
      if (direction == 0) {
        direction =  7;
      } else if (direction == 1) {
        direction = 0;
      }
      break;
    case HIT_RIGHT:
      if (direction == 0) {
        direction = 1;
      } else if (direction == 7) {
        direction = 0;
      }
      break;
  }

  // Check orthogonal directions and borders ...
  if ((direction == 0 && xball == 0) || (direction == 4 && xball == 7)) {
    direction++;
  }
  if (direction == 0 && xball == 7) {
    direction = 7;
  }
  if (direction == 4 && xball == 0) {
    direction = 3;
  }
  if (direction == 2 && yball == 0) {
    direction = 3;
  }
  if (direction == 2 && yball == 6) {
    direction = 1;
  }
  if (direction == 6 && yball == 0) {
    direction = 5;
  }
  if (direction == 6 && yball == 6) {
    direction = 7;
  }

  // "Corner" case
  if (xball == 0 && yball == 0) {
    direction = 3;
  }
  if (xball == 0 && yball == 6) {
    direction = 1;
  }
  if (xball == 7 && yball == 6) {
    direction = 7;
  }
  if (xball == 7 && yball == 0) {
    direction = 5;
  }

  yball_prev = yball;
  if (2 < direction && direction < 6) {
    yball++;
  } else if (direction != 6 && direction != 2) {
    yball--;
  }
  if (0 < direction && direction < 4) {
    xball++;
  } else if (direction != 0 && direction != 4) {
    xball--;
  }
  xball = max(0, min(7, xball));
  yball = max(0, min(6, yball));
  debug("AFTER MOVE");
}

void gameOver() {
  setSprite(sad);
  delay(1500);
  lc.clearDisplay(0);
}

void drawGame() {
  if (yball_prev != yball) {
    lc.setRow(0, yball_prev, 0);
  }
  lc.setRow(0, yball, byte(1 << (xball)));
  byte padmap = byte(0xFF >> (8 - PADSIZE) << xpad) ;
#ifdef DEBUG
  //Serial.println(padmap, BIN);
#endif
  lc.setRow(0, 7, padmap);
}


void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13, HIGH);
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13, LOW);
}


void setup() {
  lcd.begin(16, 2);
  pinMode(pushPin, INPUT_PULLUP);
  Serial.begin(9600);
  //clearFunc();
  EEPROM.get(0, highscore);
  if (highscore == 0)
    strcpy(bestPlayer, "anon");
  else
    EEPROM.get(4, bestPlayer);


  pinMode(POTPIN, INPUT);
  lc.shutdown(0, false);
  // Set the brightness to a medium values
  lc.setIntensity(0, 8);
  // and clear the display
  lc.clearDisplay(0);
  randomSeed(analogRead(0));
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Pong");
#endif
  newGame();
  ball_timer = timer.every(BALL_DELAY, moveBall);

  // declare the pinModes
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);

  pinMode(11, OUTPUT); // Buzzer
}


void loop() {
   if (!clickedMenu) {
    if (pressedButtonFunc())
      clickedMenu = true;
    else {                                    // if(!pressedButtonFunc())
      selectMenuFunc();
    }
  }
  else {                                    // if(clickedMenu)
    //Serial.println("press");
    if (selectedMenu == 1)
      playGameFunc();
    else if (selectedMenu == 3)
      moreSettingsFunc();
    else if (selectedMenu == 2)
      highscoreFunc();
    else if (selectedMenu == 4)
      infoFunc();
  }


  timer.update();
  // Move pad
  setPad();
#ifdef DEBUG
  Serial.println(xpad);
#endif
  // Update screen
  drawGame();
  if (checkLoose()) {
    debug("LOOSE");
    gameOver();
    newGame();
  }
  delay(GAME_DELAY);


  analogWrite(rPin, rVal);
  analogWrite(gPin, gVal);
  analogWrite(bPin, bVal);
  // change the values of the LEDs
  rVal = rVal + rDir;
  gVal = gVal + gDir;
  bVal = bVal + bDir;
  // for each color, change direction if
  // you reached 0 or 255
  if (rVal >= 255 || rVal <= 0) {
    rDir = rDir * -1;
  }
  if (gVal >= 255 || gVal <= 0) {
    gDir = gDir * -1;
  }
  if (bVal >= 255 || bVal <= 0) {
    bDir = bDir * -1;
  }
  // slight delay so it doesn't rotate color too quicky
  delay(33);

   Serial.println(" 'Santa Claus is coming to town'");
  int size = sizeof(santa_melody) / sizeof(int);
  for (int thisNote = 0; thisNote < size; thisNote++) {
    // to calculate the note duration, take one second divided by the note type
    int noteDuration = 900 / santa_tempo[thisNote];
    buzz(melodyPin, santa_melody[thisNote], noteDuration);
    // to distinguish the notes, set a minimum time between them the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    buzz(melodyPin, 0, noteDuration);
  }
}
