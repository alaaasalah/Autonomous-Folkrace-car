#include <BluetoothSerial.h>
#include <NewPing.h>

// ----------- Bluetooth -----------
BluetoothSerial folkrace_bluetooth;

// ----------- Motor Pins -----------
#define ENA 15   // Right Motor PWM (Motor A speed)
#define IN1 4    // Right Motor direction 1
#define IN2 16   // Right Motor direction 2
#define ENB 2    // Left Motor PWM (Motor B speed)
#define IN3 17   // Left Motor direction 1
#define IN4 5    // Left Motor direction 2

// ----------- Ultrasonic Sensors -----------
#define TRIG_RIGHT 26
#define ECHO_RIGHT 25
#define TRIG_FRONT 32
#define ECHO_FRONT 33
#define TRIG_LEFT 13
#define ECHO_LEFT 12

#define MAX_DISTANCE 200  // cm

NewPing ultrasonicRight(TRIG_RIGHT, ECHO_RIGHT, MAX_DISTANCE);
NewPing ultrasonicFront(TRIG_FRONT, ECHO_FRONT, MAX_DISTANCE);
NewPing ultrasonicLeft(TRIG_LEFT, ECHO_LEFT, MAX_DISTANCE);

// ----------- Parameters -----------
const int MIN_Speed = 50;
const int MAX_Speed = 150;
const int MIN_DIST = 2;
const int MAX_DIST = 100;
const float SIDE_OBSTACLE_THRESHOLD = 10.0;
const int STEERING_GAIN = 1.5;

// ----------- Global State -----------
int semaphore_flag = 0;
bool action_ready = false;
unsigned long start_time = 0;
String planned_action = "forward";

float distRight = 0, distFront = 0, distLeft = 0;

void setup() {
  Serial.begin(115200);
  folkrace_bluetooth.begin("ESP_Bluetooth");

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  stopMotors();
}

void loop() {
  handleBluetooth();

  if (semaphore_flag == 1) {
    if (!action_ready && millis() - start_time <= 5000) {
      planAction();
    }

    if (millis() - start_time > 5000 && !action_ready) {
      action_ready = true;
    }

    if (action_ready) {
      getDistances();
      int Speed = calculateSpeed(distFront);

      if (planned_action == "slow" || planned_action == "left" || planned_action == "right") {
        applyPlannedAction(Speed);
      } else {
        steerByCurve(Speed);  // smooth curve-following
      }

      delay(80);
    }
  }
}

// ----------- Control Logic -----------

void handleBluetooth() {
  if (/*folkrace_bluetooth.available()*/ true) {
    char cmd = '1' /*folkrace_bluetooth.read()*/;
    if (cmd == '1' && semaphore_flag == 0) {
      semaphore_flag = 1;
      start_time = millis();
      action_ready = false;
    } else if (cmd == '0') {
      semaphore_flag = 0;
      stopMotors();
    }
  }
}

void planAction() {
  getDistances();

  if (distFront < 10) {
    planned_action = "slow";
  } else if (distRight < distLeft) {
    planned_action = "left";
  } else if (distLeft < distRight) {
    planned_action = "right";
  } else if (distFront < 5) {
    planned_action = "Back";
    if (distRight < 10) {
      planned_action = "left";
    }
  } else if (distFront < 5) {
    planned_action = "Back";
    if (distLeft < 10) {
      planned_action = "right";
    }
  } else {
    planned_action = "forward";
  }
}

void getDistances() {
  distRight = ultrasonicRight.ping_cm();
  delay(10);
  distFront = ultrasonicFront.ping_cm();
  delay(10);
  distLeft = ultrasonicLeft.ping_cm();
  delay(10);

  if (distRight == 0) distRight = MAX_DISTANCE;
  if (distFront == 0) distFront = MAX_DISTANCE;
  if (distLeft == 0) distLeft = MAX_DISTANCE;
}

int calculateSpeed(float distF) {
  int spd = map((int)distF, MIN_DIST, MAX_DIST, MIN_Speed, MAX_Speed);
  return constrain(spd, MIN_Speed, MAX_Speed);
}

void steerByCurve(int baseSpeed) {
  float diff = distLeft - distRight;
  int adjust = (int)(diff * STEERING_GAIN);

  int leftSpeed = constrain(baseSpeed - adjust, MIN_Speed, MAX_Speed);
  int rightSpeed = constrain(baseSpeed + adjust, MIN_Speed, MAX_Speed);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  // Right Motor
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  // Left Motor

  analogWrite(ENA, rightSpeed);
  analogWrite(ENB, leftSpeed);
}

void applyPlannedAction(int Speed) {
  if (planned_action == "slow") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, MIN_Speed);
    analogWrite(ENB, MIN_Speed);
  } else if (planned_action == "left") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, MIN_Speed);  // slow right
    analogWrite(ENB, Speed);      // full left
  } else if (planned_action == "right") {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, Speed);      // full right
    analogWrite(ENB, MIN_Speed);  // slow left
  } else if (planned_action == "back") {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, MIN_Speed);  // full right
    analogWrite(ENB, MIN_Speed);  // slow left
    
  } 
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}
