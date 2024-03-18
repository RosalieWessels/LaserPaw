#include <Servo.h> // include the Servo library
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

//create the 2 servo objects, set up pins and variables
Servo myservo1;
Servo myservo2;
int buttonPin = 2;
int trigPin = 9;    
int echoPin = 11;   
int laserPin = 7;
unsigned long previousMillis;
bool buttonStop;
bool paused;
int prevDirection = 0;

void setup() {
  //setup the servos, ultrasonic sensor, button, and laser
  srand(time(NULL));
  myservo1.attach(4); 
  myservo2.attach(5);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, HIGH);
  //not paused in the beginning
  paused = false;
}

void loop() {
  //get distance and read button
  unsigned long currentMillis = millis();
  int distance_cm = measureDistance();
  int currState = digitalRead(buttonPin);

  //see if it needs to be paused  
  if (currState == HIGH || distance_cm < 2) {
    if (currState == HIGH) {
      buttonStop = true;
    } else {
      buttonStop = false;
    }
    paused = true;
    delay(500);
    //uses while loop to pause until pressed again or distance increases
    while (paused) {
      //stops continuous servo
      myservo1.write(90);
      digitalWrite(laserPin, LOW);
      if (buttonStop == true) {
        //check if button is pressed again
        currState = digitalRead(buttonPin);
        delay(200);
        if (currState == HIGH) {
          digitalWrite(laserPin, HIGH);
          paused = false;
        }
      }
      else {
        //check if distance increases
        distance_cm = measureDistance();
        Serial.println(distance_cm);
        if (distance_cm > 20){
          digitalWrite(laserPin, HIGH);
          paused = false;
        }
      }
      delay(100);
    }
   }
  
  if ((currentMillis - previousMillis) > 2000) {
    previousMillis = currentMillis;
    int deg2 = 0 + rand() % (180 - 0);

    if (prevDirection == 0) {
      myservo1.write(180);
      prevDirection = 1;
    } else {
      myservo1.write(0);
      prevDirection = 0;
    }
    myservo2.write(deg2);
  }
  if ((currentMillis - previousMillis) > 200) {
    myservo1.write(90);
  }
  
}

int measureDistance() {
  Serial.println("MEASURE DISTANCE");
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // measure duration of pulse from ECHO pin
  int duration_us = pulseIn(echoPin, HIGH);
  
  // calculate the distance
  Serial.println(0.017 * duration_us);
  return 0.017 * duration_us;
}

  
