// arduino uno code
#include <Time.h>
#include <SoftwareSerial.h>

SoftwareSerial xbee(0,1);

int pwmPin = 9;
int revolutions = 0;
const float pi = 3.14;
unsigned long t1, t2, delta_t = 0, time;
float R = 4.7;
float circumference = R*pi; // R in this case equals 2 Centimeters,    = 12
float rcSpeed = 0;
int speeder = 200;//this is for the car, change it to change car speed

int talk_flag = 0;
int speed_test;
int i = 0;
float speed_ave[5];
float speed_curr, dif, dif2;

void setup()
{
  pinMode(pwmPin, OUTPUT);
  attachInterrupt(0, indicate, FALLING); // Pin 2 Is used for INT 0
  t1 = millis();
  xbee.begin(9600)
}

void loop()
{
  xbee.listen();
  int comm = xbee.read();
  if (comm == 0)
  {
    analogWrite(pwmPin, 0);
  }
  else if(comm == 1)
    {
     analogWrite(pwmPin, speeder);
     
    }
    
    else if(comm == 5)
    {
      int i = Serial.read();
      while(i == 5)
      {
        i = Serial.read();
      }
      speeder = i;
      analogWrite(pwmPin, speeder);
    }
    
    else if (comm == 2)
     {
       talk_flag = 1;
      }
      
      if (comm == -1)
      {
      if (talk_flag == 1)
      {
        speed_test = (int) rcSpeed;
        xbee.write(speed_test);
        talk_flag = 0;
      }
      }
   
}
void indicate() // ISR
{
  t2 = millis();
  delta_t = (t2 - t1);
  revolutions = revolutions + 1;
  rcSpeed = ((revolutions*circumference)/(delta_t))*1000; 
  t1 = t2; revolutions = 0;
  revolutions = 0;
  
}
