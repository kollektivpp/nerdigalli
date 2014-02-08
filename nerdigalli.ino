// #### NERDIGALLI #####
//
//      X X X X X
//


#include "Ultrasonic.h"
#include <LiquidCrystal.h>
char ch;

const int contrast = 15; // display contrast
const int player = 2; // number of players
const int space = 20; // space between the players / between player one and the sensor (in centimeter)
const int interval = 2000; // interval between display changes in milliseconds
const int ignoredDistance = 80; // the sensor ignores everything above this distance (in cent)

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Ultrasonic ultrasonic(13,10);

// colors
const int LED_red = 8;
const int LED_green = 7;

// game options
char displayString[] = "x x x x x"; // = "x x x x x" as start value
int numberOfChangedSteps = 0;
int sensorDistance = 1000;
boolean finished = false; //game is finished
boolean isRedOn = 0;
boolean isGreenOn = 0;
int intervenedPlayer = 0;

void setup() 
{
  Serial.begin(9600);

  pinMode(13,OUTPUT);
  pinMode(LED_red,OUTPUT);
  pinMode(LED_green,OUTPUT);
  analogWrite(6,contrast);
  analogWrite(9,28836);
  
  randomSeed(analogRead(0));

  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 4);
  // Print heading
  lcd.print("#### NERDIGALLI ####");
  showStartSequenz();
}

void loop() 
{
  if(!finished) {
    sensorDistance = ultrasonic.Ranging(CM);
    
    if ((millis()/interval) > numberOfChangedSteps) {
      setNewValues();
    }
  
    if (sensorDistance < ignoredDistance) {
      intervenedPlayer = ceil(sensorDistance / space) + 1;
      calculateResult();
    }
    
    lcd.setCursor(5, 2);
    lcd.print(displayString);
  } else {
    clearDisplay();
    showStartSequenz();
    delay(1000);
    finished = false;
  }
}

void setNewValues() {
  for(int i = 0; i < 5; i++) {
    int randomValue = random(65, 68);
    displayString[i*2] = randomValue;
  }
  
  isRedOn = random(0, 2);
  isGreenOn = random(0, 2);
  
  if(isRedOn == 1) {
    digitalWrite(LED_red,HIGH);
  } else {
    digitalWrite(LED_red,LOW);
  }
  
  if(isGreenOn == 1) {
    digitalWrite(LED_green,HIGH);
  } else {    
    digitalWrite(LED_green,LOW);
  }

  numberOfChangedSteps++;
}

void calculateResult()
{
  boolean winner = 0;
  int numberOfAs = 0;
  int numberOfBs = 0;
  int numberOfCs = 0;

  for (int i = 0; i < sizeof(displayString); i++) {
    switch (displayString[i]) {
        case 65:
          numberOfAs++;
          break;
        case 66:
          numberOfBs++;
          break;
        case 67:
          numberOfCs++;
          break;
    }
  }

  if (isRedOn == false &&
      isGreenOn == true &&
      (numberOfAs == 3 ||
      numberOfBs == 3 ||
      numberOfCs == 3 )) {

    Serial.println("Winner winner chicken dinner");
    winner = 1;
  }
  
  displayResultMessage(winner);
}

void displayResultMessage(boolean gameIsWon)
{
  char resultString[] = "Player X is a ";
  char resultTitle[7];

  switch (intervenedPlayer) {
      case 1:
        resultString[7] = 49;
        break;
      case 2:
        resultString[7] = 50;
        break;
      case 3:
        resultString[7] = 51;
        break;
      case 4:
        resultString[7] = 52;
        break;
  }

  if (gameIsWon) {    
      // Winner
      String winnerString = "Winner";
      winnerString.toCharArray(resultTitle, 7);
    } else {
      //Loser!
      String loserString = "loser";
      loserString.toCharArray(resultTitle, 7);
    }
    
  lcd.setCursor(5, 1);
  lcd.print(displayString);
  lcd.setCursor(5, 2);
  lcd.print(resultString);
  lcd.setCursor(5, 3);
  lcd.print(resultTitle);
  
  finished = true;
  delay(2000);
}

void showStartSequenz() {
  delay(1000);
  lcd.setCursor(9, 2);
  lcd.print('3');
  delay(1000);
  lcd.setCursor(9, 2);
  lcd.print('2');
  delay(1000);
  lcd.setCursor(9, 2);
  lcd.print('1');
  delay(1000);
  lcd.setCursor(9, 2);
  lcd.print("GO");
  delay(1000);
}

void clearDisplay() {
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print("                    ");
}
