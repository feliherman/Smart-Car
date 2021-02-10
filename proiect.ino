#include <Servo.h>


  // Pinii motor 1 - FORWARD 0 ca sa mearga inainte
#define mpin00 5
#define mpin01 6
  // Pinii motor 2 - FORWARD 1 ca sa mearga inainte
#define mpin10 3
#define mpin11 11
#define pinServo 8

#define trigPin 9
#define echoPin 10 
int unghiInainte = 100;
int unghiStanga = 160;
int unghiDreapta = 0;
int unghiMijlocParcare = 80;
long duration;
int distance;


char c;
Servo srv;


void setup() {
 // configurarea pinilor motor ca iesire, initial valoare 0

   digitalWrite(mpin00, 0);
   digitalWrite(mpin01, 0);
   digitalWrite(mpin10, 0);
   digitalWrite(mpin11, 0);
   
   pinMode (mpin00, OUTPUT);
   pinMode (mpin01, OUTPUT);
   pinMode (mpin10, OUTPUT);
   pinMode (mpin11, OUTPUT);

   pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
   pinMode(echoPin, INPUT); 

   Serial.begin(9600); // Interfata Serial 0, pentru PC
   Serial1.begin(9600); // Interfata Serial1, Pentru BT!!
   
  
}
// Funcție pentru controlul unui motor
// Intrare: pinii m1 și m2, direcția și viteza


void loop() {
  // put your main code here, to run repeatedly:
  // Acum se pornesc motoarele DC
   //StartMotor (mpin00, mpin01, 0, 128);
   //StartMotor (mpin10, mpin11, 0, 128);
  
   


   // Pornirea motorului Servo
  //playWithServo(pinServo,110);
  distance = readDistanceSensor();
  //Serial.print("Distance: ");
  //Serial.println(distance);

   if (Serial1.available()) // Citire de pe Bluetooth, trimite la PC
    {
      c=Serial1.read();
      //Serial.write(c);
    }
        
   /*if (Serial.available()) // Citire de la PC, trimite la Bluetooth
        Serial1.write(Serial.read());*/
   //Serial.write(c);
   moveManagement(c, mpin00, mpin01, mpin10, mpin11, 150,distance);
   
} 



void StartMotor (int m1, int m2, int forward, int speed)
{

   if (speed==0) // oprire
   {
     digitalWrite(m1, 0);
     digitalWrite(m2, 0);
   }
   else
   {
      if(forward)
      {
        digitalWrite(m2, 0);
        analogWrite(m1, speed); // folosire PWM
      }
     else
      {
        digitalWrite(m1, 0);
        analogWrite(m2, speed);
      }
  }
}

void delayStopped(int ms)
{
   StartMotor (mpin00, mpin01, 0, 0);
   StartMotor (mpin10, mpin11, 0, 0);
   delay(ms);
}

void moveManagement(char c, int m1, int m2,int m3, int m4, int speed,int limit){
      if(c == 'F'){
          
          if(limit > 30){
            playWithServo(pinServo, unghiInainte);
            StartMotor (m1, m2, 0, speed);
            StartMotor (m3, m4, 1, speed);
            //delay(500);
            //delayStopped(500);
            //Serial.write("MERGEM INAINTE\n");
          }
          else{
            delayStopped(1500);
            playWithServo(pinServo, unghiDreapta);
            limit = readDistanceSensor();
            delay(500);
            
          }
      }
      if(c == 'D'){          
          StartMotor (m1, m2, 1, speed);
          StartMotor (m3, m4, 0, speed);
          //delay(500);
          //delayStopped(500);
          //Serial.write("MERGEM INAPOI\n");       
      }
  
      if(c == 'R'){
          if(limit > 30){
            playWithServo(pinServo, unghiDreapta);
            StartMotor (m1, m2, 0, speed);
            StartMotor (m3, m4, 0, speed/2);
            //delay(500);
            //delayStopped(500);
            //Serial.write("MERGEM DREAPTA\n");
          }
          else{
            delayStopped(1500);
            playWithServo(pinServo, unghiStanga);
            limit = readDistanceSensor();
            //delay(500);
          }
      }
  
      if(c == 'L'){
          if(limit > 30){
            playWithServo(pinServo, unghiStanga);
            StartMotor (m1, m2, 1, speed/2);
            StartMotor (m3, m4, 1, speed);
            //delay(500);
            //delayStopped(500);
            //Serial.write("MERGEM STANGA\n");
          }
          else{
            delayStopped(1500);
            playWithServo(pinServo, unghiInainte);
            limit = readDistanceSensor();
            //delay(500);
          }
      }
      if(c == 'P'){
          parkRight(m1,m2,m3,m4,speed);
      }
      if(c == 'S') {
        delayStopped(1000);
      }
}

void playWithServo(int pin, int unghi)
{
 srv.attach(pin);
 srv.write(unghi);
 delay(500);
 /*srv.write(180);
 delay(1000);
 srv.write(90);
 delay(1000);*/
 srv.detach();
}

int readDistanceSensor(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  return distance;
}

void parkRight(int m1, int m2,int m3, int m4,int speed){
  int distance1 = 0;
  int distance2 = 0;
  int auxDistance1=0;
  int auxDistance2 = 0;
  int auxDistance3 = 0;
  
  playWithServo(pinServo, unghiDreapta);
  distance1 = readDistanceSensor();
  playWithServo(pinServo, unghiMijlocParcare-20);
  auxDistance1 = readDistanceSensor();
  playWithServo(pinServo, unghiMijlocParcare);
  auxDistance2 = readDistanceSensor();
  playWithServo(pinServo, unghiMijlocParcare+20);
  auxDistance3 = readDistanceSensor();
  distance2  = auxDistance2;
  
  if(auxDistance1 <= auxDistance2)
    if(auxDistance1 <= auxDistance3)
      distance2 = auxDistance1;
    else
      distance2 = auxDistance3;
  else
      if(auxDistance2 <= auxDistance3)
        distance2 = auxDistance2;
      else
        distance2 = auxDistance3;
        
  Serial.print("Distance1: ");
  Serial.println(distance1);

  Serial.print("Distance2: ");
  Serial.println(distance2);
  if((distance2 - distance1) > 35 && (distance2 - distance1) < 90){
    
      StartMotor (m1, m2, 0, speed);
      StartMotor (m3, m4, 1, speed);
      
      delay(1200);
      
      StartMotor (m1, m2, 0, speed);
      StartMotor (m3, m4, 0, speed/2);
      
      delay(800);
      
      StartMotor (m1, m2, 0, speed);
      StartMotor (m3, m4, 1, speed);
      
      delay(700);
      
      StartMotor (m1, m2, 1, speed);
      StartMotor (m3, m4, 1, speed/2);

      delay(800);

      delayStopped(1000);
  }
  else{
    delayStopped(1000);
    
  }
  
}
