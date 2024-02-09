#include <TimerOne.h>

// Define PWM pin and relay pins
constexpr uint8_t relayPin1 = 5;
constexpr uint8_t relayPin2 = 6;
constexpr uint8_t pwmPin = 9;
constexpr uint8_t buzzPin = 2;
constexpr uint8_t redLed = 4;
constexpr uint8_t blueLed = 3;
constexpr uint8_t potPin = A1;

int Speed = 0;

enum class motorStates : char {
  FRONT = 'F',
  BACK = 'B'
};
motorStates motorStatus = motorStates::BACK;

void setup() {
  Serial.begin(9600);

  pinMode(pwmPin, OUTPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(potPin, INPUT);

  digitalWrite(buzzPin, LOW);
  digitalWrite(redLed, LOW);
  digitalWrite(blueLed, LOW);

  // Set up Timer1 to generate a 2kHz PWM signal on pin 5
  Timer1.initialize(500);  // 500 microseconds (2kHz)
  Timer1.pwm(pwmPin, 0);
}

void front() {
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
  Serial.print("Front: ");
}

void back() {
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, HIGH);
  Serial.print("Back: ");
}

motorStates charToMotorStates(char state) {
  switch (state) {
    case 'F': return motorStates::FRONT;
    case 'f': return motorStates::FRONT;
    case 'B': return motorStates::BACK;
    case 'b': return motorStates::BACK;
  }
}

void loop() {

  Speed = analogRead(potPin);
  
  Speed = map(Speed,0,1024,0,200);

  Timer1.pwm(pwmPin, Speed);
  // analogWrite(redLed, Speed);
  analogWrite(blueLed, Speed);


  switch (motorStatus) {
    case motorStates::FRONT:
      front();
      break;
    case motorStates::BACK:
      back();
      break;
  }
  Serial.println(Speed);
}
