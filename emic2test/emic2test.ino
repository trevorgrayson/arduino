#include <SoftwareSerial.h>

#define rxPin 2
#define txPin 3
#define ledPin 13

SoftwareSerial emicSerial = SoftwareSerial(rxPin, txPin);

void setup() {
  // define pins:
  pinMode(ledPin, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  emicSerial.begin(9600);

  digitalWrite(ledPin, LOW);
  emicSerial.print('\n');
  while (emicSerial.read() != ':');
  delay(10);
  emicSerial.flush();
}

void loop() {
  // put your main code here, to run repeatedly:
  emicSerial.print('S');
  emicSerial.print("would you like to play a game?");
  emicSerial.print('\n');
  digitalWrite(ledPin, HIGH);
  while(emicSerial.read() != ':');
  digitalWrite(ledPin, LOW);

  delay(1000);
}
