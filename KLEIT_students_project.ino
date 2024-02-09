#include <TimerOne.h>

// Define PWM pin and relay pins
constexpr uint8_t relayPin1 = 3;
constexpr uint8_t relayPin2 = 4;
constexpr uint8_t pwmPin = 9;

static const uint8_t buf_len = 5;
char receivedChar;
char buf[buf_len];
uint8_t idx = 0;
int Speed = 0;
double volts = 0.00;

enum class motorStates : char {
  FRONT = 'F',
  BACK = 'B'
};
motorStates motorStatus = motorStates::FRONT;

void setup() {
  Serial.begin(9600);

  pinMode(pwmPin, OUTPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);

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

void clearBuffer() {
  for (int i = 0; i < buf_len; i++) {
    buf[i] = 0;
  }
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
  while ( {
    receivedChar = Serial.read();

    if (receivedChar == 'F' || receivedChar == 'f' || receivedChar == 'B' || receivedChar == 'b') {
      motorStatus = charToMotorStates(receivedChar);
    } else if (receivedChar == '\n') {
      Speed = atoi(buf);

      if (idx > 4 || Speed > 1024) {
        Serial.println("Invalid input, \nPlease enter values between 0 to 1024\nF or f for Front \nB or b for back.");
        Speed = 0;
      } else {
        Speed = map(Speed, 0, 1024, 0, 76);  //Convert the voltage from 0v to 3.3v pwm signals
        volts = map(Speed, 0, 76, 0, 3.3);
        Timer1.pwm(pwmPin, Speed);  // PWM duty cycle (0 to 1023, where 0 is 0% and 1023 is 100%)
      }
      idx = 0;
      clearBuffer();
    } else {
      // Only append if index is not over message limit
      if (idx < buf_len - 1) {
        buf[idx] = receivedChar;
        idx++;
      }
    }
  }

  switch (motorStatus) {
    case motorStates::FRONT:
      front();
      break;
    case motorStates::BACK:
      back();
      break;
  }

  // Serial.println(Speed);
  Serial.println(volts);
}
