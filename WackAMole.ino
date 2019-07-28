#include <LiquidCrystal.h>

LiquidCrystal lcd(A0,A1,5,4,3,2);

int blueButton = 13;
int blueLED = 12;

int redButton = 11;
int redLED = 10;

int greenButton = 8;
int greenLED = 9;

int yellowButton = 7;
int yellowLED = 6;

int buzzer = A5;

int strike = 0;
int score = 0;
int points = 100;
int pointDeduction = 20;

double lifeSpan = 3000;
unsigned long clockOne;
unsigned long clockTwo;
unsigned long gameTime;
unsigned long updateGame;
int getFaster = 10000;

bool stopGame = false;

void displayStrike() {
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("STRIKE ");
  lcd.print(strike);
  delay(1000);
}

void displayScore() {
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("SCORE: ");
  lcd.print(score);
}

class Mole {
public:
bool out;
int moleButton;
int moleLight;

unsigned long exposedTime = millis();
unsigned long escapeTime = exposedTime + lifeSpan;

Mole(int button, int LED) {
  moleButton = button;
  moleLight = LED;
  out = false;
}

void changeExposedTime() {
  exposedTime = millis();
  escapeTime = exposedTime + lifeSpan;
}

bool isOut(){
  return out;
}

void hide() {
  out = false;
  lightSwitch();
}

void popOut(){
  out = true;
  lightSwitch();
}

void lightSwitch(){
  if(out) {
    digitalWrite(moleLight, HIGH);
  }
  if(!out) {
   digitalWrite(moleLight, LOW); 
  }
}

void wasHit(bool buttonPressed) {
  if(out) {
    //checks if the mole has been out for too long
    if(gameTime > escapeTime) {
      strike++;
      tone(buzzer, 5, 2500);
      out = false;
      lightSwitch();
      score -= pointDeduction;
      displayStrike();
      delay(150);
    }
    //if pressed and mole is out
    else if(buttonPressed) {
      out = false;
      tone(buzzer, 16.35, 1000);
      lightSwitch();
      //got rid of a delay here
      score+=points;
      delay(150);
    }
  }
  //if button was pressed and mole isn't out
  else if (buttonPressed) {
    tone(buzzer, 5, 2500);
    score -= pointDeduction;
    delay(100);
  }
}
};

Mole blueMole(blueButton, blueLED);
Mole greenMole(greenButton, greenLED);
Mole redMole(redButton, redLED);
Mole yellowMole(yellowButton, yellowLED);

void updateGameDifficulty() {
if(gameTime > updateGame + getFaster) {
  lifeSpan*= 0.8;
  updateGame = millis();
  points+=25;
  pointDeduction+=10;
}
  gameTime = millis();
}

void flashMoles() {
  blueMole.popOut();
  greenMole.popOut();
  redMole.popOut();
  yellowMole.popOut();
  delay(1000);
  blueMole.hide();
  greenMole.hide();
  redMole.hide();
  yellowMole.hide();
  delay(1000); 
}

void endGame(){
  if(strike >= 5) {
    tone(buzzer, 5, 25000);
    flashMoles();
    flashMoles();

    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("GAME");
    lcd.setCursor(6,1);
    lcd.print("OVER");
    delay(5000);
    displayScore();
    stopGame = true;
  }
}

void randomMole() {
  int mole;
  randomSeed(analogRead(A3));
  mole = (int)random(0, 4);
  if(mole == 0){
    if(!blueMole.isOut()){
      blueMole.popOut();
      blueMole.changeExposedTime();
    }
  }
  if(mole == 1){
    if(!greenMole.isOut()){
      greenMole.popOut();
      greenMole.changeExposedTime();
    }
  }
  if(mole == 2){
    if(!redMole.isOut()){
      redMole.popOut();
      redMole.changeExposedTime();
    }
  }
  if(mole == 3){
    if(!yellowMole.isOut()){
      yellowMole.popOut();
      yellowMole.changeExposedTime();
    }
  }
}

void setup() {
pinMode(blueButton, INPUT_PULLUP);
pinMode(blueLED, OUTPUT);

pinMode(greenButton, INPUT_PULLUP);
pinMode(greenLED, OUTPUT);

pinMode(redButton, INPUT_PULLUP);
pinMode(redLED, OUTPUT);

pinMode(yellowButton, INPUT_PULLUP);
pinMode(yellowLED, OUTPUT);

clockOne = millis();
clockTwo = millis();

gameTime = millis();
updateGame = millis();

}

void loop() {
  if(!stopGame){
  displayScore();
  bool blueB = (digitalRead(blueButton) == 0);
  bool greenB = (digitalRead(greenButton) == 0);
  bool yellowB = (digitalRead(yellowButton) == 0);
  bool redB = (digitalRead(redButton) == 0);
  
  if(clockOne > clockTwo + 2500) {
    randomMole();
    clockTwo = millis();
  }
  
  blueMole.wasHit(blueB);
  greenMole.wasHit(greenB);
  yellowMole.wasHit(yellowB);
  redMole.wasHit(redB);
  
  updateGameDifficulty();
  endGame();
  clockOne = millis();
  }
  displayScore();
}
