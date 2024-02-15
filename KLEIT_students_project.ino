#include <TimerOne.h>

// Define PWM pin and relay pins
constexpr uint8_t relayPin1 = 3;
constexpr uint8_t relayPin2 = 4;
constexpr uint8_t pwmPin = 9;
constexpr uint8_t bluetoothRx = 0; // Using hardware UART RX pin

char receivedChar;
int Speed = 0;

enum class motorStates : char {
  FRONT = 'F',
  BACK = 'B',
  SPEED0 = '0',
  SPEED1 = '1',
  SPEED2 = '2',
  SPEED3 = '3',
  SPEED4 = '4',
  SPEED5 = '5',
  SPEED6 = '6',
  SPEED7 = '7',
  SPEED8 = '8',
  SPEED9 = '9',
  SPEED10 = 'q',
};
motorStates motorStatus = motorStates::FRONT;

void setup() {
  Serial.begin(9600); // Initialize Serial for debugging

  pinMode(pwmPin, OUTPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);

  // Set up Timer1 to generate a 2kHz PWM signal on pin 5
  Timer1.initialize(0.1);  // 500 microseconds (2kHz)
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
    case '0': return motorStates::SPEED0;
    case '1': return motorStates::SPEED1;
    case '2': return motorStates::SPEED2;
    case '3': return motorStates::SPEED3;
    case '4': return motorStates::SPEED4;
    case '5': return motorStates::SPEED5;
    case '6': return motorStates::SPEED6;
    case '7': return motorStates::SPEED7;
    case '8': return motorStates::SPEED8;
    case '9': return motorStates::SPEED9;
    case 'q': return motorStates::SPEED10;
  }
}

void loop() {

  if (Serial.available() > 0) 
  {
    receivedChar = Serial.read();
    motorStatus = charToMotorStates(receivedChar);
  }

  switch (motorStatus) 
  {
    case motorStates::FRONT:
      front();
      break;
    case motorStates::BACK:
      back();
      break;
    case motorStates::SPEED0:
      Speed = 0;
      break;
    case motorStates::SPEED1:
      Speed = 22;
      break;
    case motorStates::SPEED2:
      Speed = 44;
      break;
    case motorStates::SPEED3:
      Speed = 66;
      break;
    case motorStates::SPEED4:
      Speed = 88;
      break;
    case motorStates::SPEED5:
      Speed = 110;
      break;
    case motorStates::SPEED6:
      Speed = 132;
      break;
    case motorStates::SPEED7:
      Speed = 154;
      break;
    case motorStates::SPEED8:
      Speed = 176;
      break;
    case motorStates::SPEED9:
      Speed = 198;
      break;
    case motorStates::SPEED10:
      Speed = 218;
      break;
  }
  Timer1.pwm(pwmPin, Speed);
  Serial.println(Speed);
}
