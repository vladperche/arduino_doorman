#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define FRQ_SRV 60 // Defines Servo's working frequence
#define SERVOMIN  205 // VALOR PARA UM PULSO MAIOR QUE 1 mS
#define SERVOMAX  409 // VALOR PARA UM PULSO MENOR QUE 2 mS

const int SERVO_LIMITS[6][2] = {
  { 330, 459 },//right eye, y axis
  { 225, 369 },//right eye, x axis
  { 180, 459 },//eye lids
  { 205, 409 },//não utilizado
  { 225, 369 },//left eye, x axis
  { 170, 289 } //left eye, y axis
};

// Servo's controller
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void initServos()
{
  Serial.println("Initializing 16 Channel Servo Test...");
  pwm.begin();
  pwm.setPWMFreq(FRQ_SRV);
  Serial.println("Done!");
}

void writeServo(int nServo, int posicao) {
#if defined(DEBUG)
  Serial.print("Moving servo nr ");
  Serial.print(nServo);
  Serial.print(" to position ");
  Serial.println(posicao);
#endif

  int pos = map ( posicao , 0 , 180 , SERVO_LIMITS[nServo][0], SERVO_LIMITS[nServo][1]);
  pwm.setPWM(nServo, 0, pos);
  delay(5);
}
