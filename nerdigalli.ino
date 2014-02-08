// #### NERDIGALLI #####
//
//      X X X X X
//


#include <LiquidCrystal.h>
char ch;

const int contrast = 15; // display contrast
const int player = 2; // number of players
const int space = 1000; // space between the players / between player one and the sensor (in millimeters)
const int interval = 1000; // interval between display changes in milliseconds
const int ignoredDistance = 1000; // the sensor ignores everything above this distance (in millimeters)

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// colors
const int LED_red = 8;
const int LED_green = 7;

// game options
char displayString[] = "x x x x x"; // = "x x x x x" as start value
int numberOfChangedSteps = 0;
boolean finished = false; //game is finished
boolean isRedOn = 0;
boolean isGreenOn = 0;
int intervenedPlayer = 0;

void setup() 
{
  Serial.begin(9600);
  Serial.println("LCD test with PWM contrast adjustment");
  pinMode(13,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(7,OUTPUT);
  analogWrite(6,contrast);
  analogWrite(9,28836);
  
  randomSeed(analogRead(0));

  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 3);
  // Print a message to the LCD.
  lcd.print("#### NERDIGALLI ####");
}

void loop() 
{
  if ((millis()/interval) > numberOfChangedSteps) {
    setNewValues();
  }

  if (/*TODO: sensor value*/ > ignoredDistance) {
    intervenedPlayer = getIntervenedPlayerNumber(/*TODO: sensor value*/sensorDistance);
    calculateResult();
  }
  
  lcd.setCursor(5, 2);
  lcd.print(displayString);
}

void serialEvent() {
  if (Serial.available()) {
    ch= Serial.read();
    
    if(ch=='N') {
      analogWrite(9,28836);
    }
    
    if(ch=='F') {
      analogWrite(9,0);
    }

  }
}

void setNewValues() {
  for(int i = 0; i < 5; i++) {
    int randomValue = random(65, 68);
    displayString[i*2] = randomValue;
  }
  
  isRedOn = random(0, 2);
  isGreenOn = random(0, 2);
  
  if(isRedOn) {
    digitalWrite(8,HIGH);
  } else {
    digitalWrite(8,LOW);
  }
  
  if(isGreenOn) {
    digitalWrite(7,HIGH);
  } else {    
    digitalWrite(7,LOW);
  }

  numberOfChangedSteps++;
  // Serial.println(numberOfChangedSteps);
}

void calculateResult()
{
  boolean winner = 0;
  int numberOfAs = 0;
  int numberOfBs = 0;
  int numberOfCs = 0;

  for (int i = 0; i < displayString.length; i++) {
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
        default:
          // do nothing
    }
  }

  if (isRedOn == false &&
      isGreenOn == true &&
      (numberOfAs == 3 ||
      numberOfBs == 3 ||
      numberOfCs == 3 )) {

    winner = 1;

    displayResultMessage(winner);
  }
}

int getIntervenedPlayerNumber(int sensorDistance)
{
  for (int i = 1; i <= player; i++) {
    if (i * space > sensorDistance) {
      intervenedPlayer = i;
    }
  }
}

void displayResultMessage(gameIsWon)
{
  char resultString[] = "Player X is a ";
  char resultTitle[6];

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
      default:
        // do something
  }

  if (gameIsWon) {
      // Winner
      resultTitle = "Winner";
    } else {
      //Loser!
      resultTitle = "Loser ";
    }
  lcd.setCursor(5, 2);
  lcd.print(resultString);
  lcd.setCursor(5, 3);
  lcd.print(resultTitle);
  lcd.setCursor(5, 1)
  lcd.print(displayString);
  delay(20000);
}
