// Define constants for remote control codes
const int CODE_STOP = 10;
const int CODE_READY = 810;
const int CODE_RUNNING = 410;
// Define constantes para os pinos dos sensores
const int RIGHT_SENSOR_PIN = 2;
const int LEFT_SENSOR_PIN = 3;
const int MIDDLE_SENSOR_PIN = 4;
const int LEFT_INFRARED_SENSOR_PIN = 5;
const int RIGHT_INFRARED_SENSOR_PIN = 6;

// Define constantes para valores de velocidade do motor
const int MOTOR_SPEED_FORWARD = 110;
const int MOTOR_SPEED_LEFT_TURN = 100;
const int MOTOR_SPEED_RIGHT_TURN = 95;
const int MOTOR_SPEED_BACKWARD = 80;

// Define constantes para referências de sensor
const int LEFT_SENSOR_THRESHOLD = 500;
const int RIGHT_SENSOR_THRESHOLD = 500;
// Define a tolerância dos sensores
const int SENSOR_TOLERANCE = 10;

void IRRead() {
    // Declare value as an integer
    int value;
    if (irrecv.decode(&results)) {
        value = results.value;
        Serial.println(value);
        irrecv.resume();
    }
    // Use constants instead of hard-coded strings
    if (value == CODE_STOP) {
        if (autoState == STOPPED) {
            autoState = READY;
            MotorWrite(90, 90);
            CalibrateSensors();
        }
    } else if (value == CODE_READY) {
        if (autoState == READY) {
            autoState = RUNNING;
            PS4.setLed(0, 100, 0);
            PS4.sendToController();
        }
    } else if (value == CODE_RUNNING) {
        if (autoState == RUNNING || autoState == READY) {
            autoState = STOPPED;
            MotorWrite(90, 90);
        }
    }
}


void Star() {
  int rightSensorValue = analogRead(RIGHT_SENSOR_PIN);
  int leftSensorValue = analogRead(LEFT_SENSOR_PIN);

  if (digitalRead(MIDDLE_SENSOR_PIN)) {
    MotorWrite(MOTOR_SPEED_FORWARD, MOTOR_SPEED_FORWARD);
  } else if (digitalRead(LEFT_INFRARED_SENSOR_PIN)) {
    MotorWrite(MOTOR_SPEED_LEFT_TURN, MOTOR_SPEED_RIGHT_TURN);
  } else if (digitalRead(RIGHT_INFRARED_SENSOR_PIN)) {
    MotorWrite(MOTOR_SPEED_RIGHT_TURN, MOTOR_SPEED_LEFT_TURN);
  }

  if (leftSensorValue < LEFT_SENSOR_THRESHOLD && rightSensorValue < RIGHT_SENSOR_THRESHOLD) {
    MotorWrite(MOTOR_SPEED_BACKWARD, MOTOR_SPEED_BACKWARD);
    delay(200);
    MotorWrite(MOTOR_SPEED_RIGHT_TURN, MOTOR_SPEED_LEFT_TURN);
    delay(200);
  } else if (leftSensorValue < LEFT_SENSOR_THRESHOLD) {
    MotorWrite(MOTOR_SPEED_FORWARD, MOTOR_SPEED_LEFT_TURN);
  } else if (rightSensorValue < RIGHT_SENSOR_THRESHOLD) {
    MotorWrite(MOTOR_SPEED_RIGHT_TURN, MOTOR_SPEED_FORWARD);
  } else {
    MotorWrite(MOTOR_SPEED_FORWARD, MOTOR_SPEED_FORWARD);
  }
}

void moveForward(int leftSpeed, int rightSpeed, unsigned int duration) {
    unsigned int timerStart = millis() + duration;
    while (millis() < timerStart) {
        MotorWrite(leftSpeed, rightSpeed);
    }
}

void avoidObstacle() {
    if (right) {
        MotorWrite(80, 110);
    } else {
        MotorWrite(110, 80);
    }
}

void Radar() {
    while (autoState == RUNNING) {
        IRRead();

        // Verifica se há obstáculos à frente
        if (!digitalRead(middleInfSensor)) {
            // Evita obstáculo
            avoidObstacle();
            // Move para frente por 300ms
            moveForward(120, 80, 300);
        } else {
            // Move para frente
            MotorWrite(150, 150);
        }

        // Alterna o lado de desvio de obstáculos
        right = !right;

        // Espera até não haver obstáculos à frente
        while (digitalRead(middleInfSensor) && autoState == RUNNING) {
            IRRead();
            MotorWrite(150, 150);
        }
    }
}

void Fradar() {
    if (!desempate) {
        elapsedMillis timer;
        while (timer < 300) {
            MotorWrite(120, 80);
        }
    }

    MotorWrite(100, 100);
    delayMicroseconds(50);

    bool right = false;
    while (autoState == RUNNING) {
        IRRead();

        if (!digitalRead(middleInfSensor)) {
            if (right) {
                MotorWrite(80, 110);
            } else {
                MotorWrite(110, 80);
            }
            while (!digitalRead(middleInfSensor)) {
                IRRead();
            }
            right = !right;
        } else {
            MotorWrite(150, 150);
        }
    }
}


void Auto() {
  IRRead();

  switch (PS4.getButtonPressed()) {
    case CIRCLE:
      Serial.println("RadarMode");
      tactic = RADAR;
      break;
    case SQUARE:
      Serial.println("ForwardRadarMode");
      tactic = FRADAR;
      break;
    case TRIANGLE:
      Serial.println("StarMode");
      tactic = STAR;
      break;
  }

  if (PS4.Right()) {
    Serial.println("Right");
    right = true;
  } else if (PS4.Left()) {
    Serial.println("Left");
    right = false;
  }

  if (PS4.Up()) {
    Serial.println("Up");
    desempate = false;
  } else if (PS4.Down()) {
    Serial.println("Down");
    desempate = true;
  }

  if (autoState == RUNNING) {
    switch (tactic) {
      case STAR:
        Star();
        break;
      case RADAR:
        Radar();
        break;
    }
  } else if (autoState == READY || autoState == STOPPED) {
    MotorWrite(90, 90);
    if (blinkTimer < millis()) {
      if (autoState == READY) {
        ledOn ? PS4.setLed(0, 0, 0) : PS4.setLed(0, 100, 0);
      } else {
        ledOn ? PS4.setLed(0, ledIntensity++, 0) : PS4.setLed(0, ledIntensity--, 0);
        if (ledIntensity == 0 || ledIntensity == 100) ledOn = !ledOn;
      }
      PS4.sendToController();
      blinkTimer = millis() + BLINK_INTERVAL_MS;
    }
  }
}

void CalibrateSensor(int sensorPin, int& sensorRef) {
  // Verifica se o sensor está funcionando corretamente
  int sensorValue = analogRead(sensorPin);
  if (sensorValue < 0 || sensorValue > 1023) {
    Serial.println("Erro: sensor não está funcionando corretamente");
    return;
  }
  
  // Calibra o sensor
  sensorRef = sensorValue - SENSOR_TOLERANCE;
}

void CalibrateSensors() {
  // Calibra o sensor esquerdo
  CalibrateSensor(leftSensorPin, leftSensorRef);
  
  // Calibra o sensor direito
  CalibrateSensor(rightSensorPin, rightSensorRef);
}








