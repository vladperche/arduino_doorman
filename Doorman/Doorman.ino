#include "PCA9685.h"
#include "7Seg4Dig.h"

//To disable TEST MODE, comment the line bellow
//#define DEBUG
#define PAUSE 2000
#define PIN_SELECT 7
#define PIN_POTENC A2

//Define each servo port
#define NUM_SERVOS 5
const int servoPort[] = { 0, 1, 2, 4, 5 };
int servoInitPos[] = { 0, 0, 0, 0, 0 };
int servoCurrPos[] = { 0, 0, 0, 0, 0 };
int servoMovePos[] = { 0, 0, 0, 0, 0 };

const int servoLimits[NUM_SERVOS][2] {
  { 0, 180 },
  { 0, 180 },
  { 0, 180 },
  { 0, 180 },
  { 0, 125 }
};

//Define Eye's axes index of Ports
#define EYE_RIGHT_X 1
#define EYE_RIGHT_Y 0
#define EYE_LEFT_X 3
#define EYE_LEFT_Y 4
#define EYE_LIDS 2

#define PIN_SENSOR_PIR 2
#define BLINK_OPEN 8000 //Maximum time between eye's lid movement
#define BLINK_RANGE 1000 //Maximum time range to randomize eye's lid movement
#define BLINK_CLOSE 200 //Maximum time of eye's lid will close during blinks
#define EYES_MOVE 3000 //Maximum time between eyes movement
#define EYES_RANGE 800 //Maximum time range to randomize eyes movement

//uncomment this line if using a Common Anode LED
#define COMMON_ANODE

#define PIN_LED_RED 10
#define PIN_LED_GRE 6
#define PIN_LED_BLU 5

void setup() {
  initServos();
  displayBrightMax();

  pinMode(PIN_SENSOR_PIR, INPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GRE, OUTPUT);
  pinMode(PIN_LED_BLU, OUTPUT);
  
  #if defined(DEBUG)
    Serial.begin(9600);
    pinMode(PIN_SELECT, INPUT_PULLUP);
    pinMode(PIN_POTENC, INPUT);
    
    displaySegments(dsp_debu);
    delay(1000);
    
    displaySegments(dsp_red);
    setEyeColor(255, 0, 0);
    delay(PAUSE);

    displaySegments(dsp_gree);
    setEyeColor(0, 255, 0);
    delay(PAUSE);

    displaySegments(dsp_blue);
    setEyeColor(0, 0, 255);
    delay(PAUSE);
  #endif
  
  setEyeColor(0,0,0);
}

void loop() {
  #if defined(DEBUG)
    testServos();
  #else
    wakeUp();
    eyesOnFire();
    blinkEyeLids();
    moveEyes();
  #endif
  moveServos();
  delay(20);
}

bool isAwake = false;
unsigned long lastBlink;
unsigned long lastEyesMove;
int oldPir = LOW;
void wakeUp()
{
  int curPir = digitalRead(PIN_SENSOR_PIR);
  if(oldPir != curPir)
  {
    if(curPir == HIGH) //acordou
    {
      #if defined(DEBUG)
        Serial.println("Acordou");
      #endif
      isAwake = true;
      lastBlink = millis() - BLINK_OPEN;
      lastEyesMove = millis() - EYES_MOVE;
    }
    else // dormiu
    {
      #if defined(DEBUG)
        Serial.println("Dormiu");
      #endif
      isAwake = false;
    }
    oldPir = curPir;
  }
}

void eyesOnFire()
{
  if(isAwake)
  {
    int brightness = random(120);
    int red = brightness+135;
    int green = brightness/7;
    int blue = brightness/17;
    setEyeColor(red,green,blue);
  }
  else
  {
    setEyeColor(1,1,1);
  }
}

bool isBlinking = false;
int nextBlink = BLINK_OPEN - random(BLINK_RANGE);
void blinkEyeLids()
{
  if(isAwake)
  {
    unsigned long currBlink = millis();
    if(currBlink - lastBlink > nextBlink && !isBlinking)
    {
      servoMovePos[EYE_LIDS] = 0; //fecha
      isBlinking = true;
      lastBlink = millis();
      nextBlink = BLINK_OPEN - random(BLINK_RANGE);
    }
    
    if(currBlink - lastBlink > BLINK_CLOSE && isBlinking)
    {
      servoMovePos[EYE_LIDS] = 180; //abre
      isBlinking = false;
      lastBlink = millis();
    }
  }
  else
  {
    servoMovePos[EYE_LIDS] = 0; // fecha
    isBlinking = false;
  }
}

int nextEyesMove = EYES_MOVE - random(EYES_RANGE);
void moveEyes()
{
  if(isAwake)
  {
    unsigned long currEyesMove = millis();
    if(currEyesMove - lastEyesMove > nextEyesMove)
    {
      lastEyesMove = millis();
      nextEyesMove = EYES_MOVE - random(EYES_RANGE);
      int dir = random(5); //[0-5[
      switch(dir)
      {
        /*
        case 0: //eyes N
          servoMovePos[EYE_LEFT_X] = 90;
          servoMovePos[EYE_LEFT_Y] = 0;
          servoMovePos[EYE_RIGHT_X] = 90;
          servoMovePos[EYE_RIGHT_Y] = 180;
          break;
        case 1: //eyes NE
          servoMovePos[EYE_LEFT_X] = 180;
          servoMovePos[EYE_LEFT_Y] = 0;
          servoMovePos[EYE_RIGHT_X] = 180;
          servoMovePos[EYE_RIGHT_Y] = 180;
          break;
        case 7: //eyes NE
          servoMovePos[EYE_LEFT_X] = 0;
          servoMovePos[EYE_LEFT_Y] = 180;
          servoMovePos[EYE_RIGHT_X] = 0;
          servoMovePos[EYE_RIGHT_Y] = 0;
          break;
        */
        case 0: //eyes E
          servoMovePos[EYE_LEFT_X] = 180;
          servoMovePos[EYE_LEFT_Y] = 90;
          servoMovePos[EYE_RIGHT_X] = 180;
          servoMovePos[EYE_RIGHT_Y] = 90;
          break;
        case 1: //eyes SE
          servoMovePos[EYE_LEFT_X] = 180;
          servoMovePos[EYE_LEFT_Y] = 0;
          servoMovePos[EYE_RIGHT_X] = 180;
          servoMovePos[EYE_RIGHT_Y] = 180;
          break;
        case 2: //eyes S
          servoMovePos[EYE_LEFT_X] = 90;
          servoMovePos[EYE_LEFT_Y] = 0;
          servoMovePos[EYE_RIGHT_X] = 90;
          servoMovePos[EYE_RIGHT_Y] = 180;
          break;
        case 3: //eyes SW
          servoMovePos[EYE_LEFT_X] = 0;
          servoMovePos[EYE_LEFT_Y] = 0;
          servoMovePos[EYE_RIGHT_X] = 0;
          servoMovePos[EYE_RIGHT_Y] = 180;
          break;
        case 4: //eyes W
          servoMovePos[EYE_LEFT_X] = 0;
          servoMovePos[EYE_LEFT_Y] = 90;
          servoMovePos[EYE_RIGHT_X] = 0;
          servoMovePos[EYE_RIGHT_Y] = 90;
          break;
      }
    }
  }
  else
  {
    servoMovePos[EYE_LEFT_X] = 90;
    servoMovePos[EYE_LEFT_Y] = 90;
    servoMovePos[EYE_RIGHT_X] = 90;
    servoMovePos[EYE_RIGHT_Y] = 90;
  }
}

void moveServos()
{
  for(int i = 0; i < NUM_SERVOS; i++)
  {
    if(servoCurrPos[i] != servoMovePos[i])
    {
      int angle = ceil(abs(servoMovePos[i] - servoCurrPos[i]) / 2);
      angle = max(angle, 1);
      angle = angle * signum(servoMovePos[i] - servoCurrPos[i]);
      angle = map(angle, 0, 180, servoLimits[i][0], servoLimits[i][1]);

      #ifdef DEBUG
        Serial.print("Curr ");
        Serial.print(servoCurrPos[i]);
        Serial.print(" Moving to ");
        Serial.print(servoMovePos[i]);
        Serial.print(" Moveu ");
        Serial.println(angle);
      #endif
      servoCurrPos[i] = servoCurrPos[i] + angle;
      writeServo(servoPort[i], servoCurrPos[i]);
    }
  }
  delay(10);
}

int signum(int num)
{
  if(num > 0) return +1;
  if(num == 0) return 0;
  if(num < 0) return -1;
}

int currSelect = digitalRead(PIN_SELECT);
int currServo = 0;
void testServos()
{
  int select = digitalRead(PIN_SELECT);
  int potenc = map(analogRead(PIN_POTENC), 0, 1023, 0, 180);
  
  if(currSelect != select){
    Serial.print("Testing: ");
    Serial.print(select);
    Serial.print(" - ");
    Serial.println(currSelect);
    if(select == 1 && currSelect == 0){
      Serial.println("Soltou!");
      currServo += 1;
      if(currServo >= NUM_SERVOS){
        currServo = 0;
      }
    }
    
    currSelect = select;
  }

  if(potenc != servoMovePos[currServo]){
    servoMovePos[currServo] = potenc;
  }
  
  displayNumberLeading(currServo * 1000 + servoCurrPos[currServo]);
}

void setEyeColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(PIN_LED_RED, red);
  analogWrite(PIN_LED_GRE, green);
  analogWrite(PIN_LED_BLU, blue);
}
