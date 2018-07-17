/*
  Ultrasonic Sensor HC-SR04

  Garage Wall defender.
*/
//#include "LowPower.h"

// define pin numbers
const int RED_LIGHT    = 11;
const int YELLOW_LIGHT = 10;
const int GREEN_LIGHT  = 9;
const int trigPin = 5; // was 9
const int echoPin = 6; // was 10

long duration;
int distance;
int startDistance = -1;


int getDistance() {
      // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
  
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    return duration * 12 * 0.0133/2;  
}


void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication

  // Clear LEDs
  digitalWrite(RED_LIGHT, LOW);
  digitalWrite(YELLOW_LIGHT, LOW);
  digitalWrite(GREEN_LIGHT, LOW);
  // Test Each LED
  digitalWrite(RED_LIGHT, HIGH);
  delay(500);
  digitalWrite(RED_LIGHT, LOW);
  digitalWrite(YELLOW_LIGHT, HIGH);
  delay(500);
  digitalWrite(YELLOW_LIGHT, LOW);
  digitalWrite(GREEN_LIGHT, HIGH);
  delay(500);
  digitalWrite(GREEN_LIGHT, LOW);
  startDistance = getDistance() + 5;
}

//    # wait for car, not wasting energy - can measure to garage door, if < garage door start sampling
//    # car is present, up sampling
//    # car is parked, go back to sleep.

void loop() {
    distance = getDistance();
    // Serial.print("Distance: ");
    // Serial.println(distance);
  
    digitalWrite(RED_LIGHT, LOW);
    digitalWrite(YELLOW_LIGHT, LOW);
    digitalWrite(GREEN_LIGHT, LOW);

    Serial.print(distance);
    Serial.print("\n");
    if(distance <= startDistance)
    {
      digitalWrite(RED_LIGHT, HIGH);
    } else if(distance < startDistance + 200)
    {
      digitalWrite(YELLOW_LIGHT, HIGH);
    } else
    {
      digitalWrite(GREEN_LIGHT, HIGH);
    }

//    LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
}
