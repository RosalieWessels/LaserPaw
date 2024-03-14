#include <Servo.h> // include the Servo library
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

//create the 2 servo objects
Servo myservo1;
Servo myservo2;
int buttonPin = 2;
int trigPin = 9;    // TRIG pin
int echoPin = 11;    // ECHO pin
int laserPin = 7;
unsigned long previousMillis;
bool buttonStop;
bool paused;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  srand(time(NULL));

  myservo1.attach(4); 
  myservo2.attach(5);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, HIGH);
  paused = false;
}

void loop() {
  // put your main code here, to run repeatedly:
   unsigned long currentMillis = millis();

   int distance_cm = measureDistance();

   int currState = digitalRead(buttonPin);

   if (currState == HIGH || distance_cm < 2) {
    if (currState == HIGH) {
      buttonStop = true;
    } else {
      buttonStop = false;
    }
    paused = true;
    delay(500);
    while (paused) {
      myservo1.write(90);
      digitalWrite(laserPin, LOW);
      if (buttonStop == true) {
        currState = digitalRead(buttonPin);
        delay(200);
        Serial.println(currState);
        if (currState == HIGH) {
          digitalWrite(laserPin, HIGH);
          paused = false;
        }
      }
      else {
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
  
  if ((currentMillis - previousMillis) > 1000) {
    previousMillis = currentMillis;
    int len = 0 + rand() % (500 - 0);
    int direction = 0 + rand() % (2 - 0);
    int deg2 = 0 + rand() % (180 - 0);

    if (direction == 0) {
      myservo1.write(180);
    } else {
      myservo1.write(0);
    }
    myservo2.write(deg2);
  }
  if ((currentMillis - previousMillis) > 500) {
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

  
