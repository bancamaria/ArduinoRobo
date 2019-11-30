#include <LiquidCrystal.h>
#include <string.h>
#include <EEPROM.h>
// LCD
const int RS = 2;
const int enable = 3;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
String menu[] = {"Start Game", "Highscore", "Settings"};

// joystick
const int xPin = A2;
const int yPin = A1;
const int pushPin = A0;
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

void selectMenuFunc() {
  int xValue = analogRead(xPin);
  if (xValue < minTreshold && xReset) {
    selectedMenu--;
    if (selectedMenu < 1)
      selectedMenu = 3;
    xReset = 0;
  }
  if (xValue > maxTreshold && xReset) {
    selectedMenu++;
    if (selectedMenu > 3)
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

void setup() {
  lcd.begin(16, 2);
  pinMode(pushPin, INPUT_PULLUP);
  Serial.begin(9600);
  //clearFunc();
  EEPROM.get(0, highscore);
  if(highscore == 0)
    strcpy(bestPlayer, "anon");
  else
    EEPROM.get(4, bestPlayer);
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
  if(Serial.available() > 0)
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
      if(currPlayer.length() == 0)
        currPlayer = "anon";
    }
    if(millis() - levelTime >= 5000) {
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
  }
}
