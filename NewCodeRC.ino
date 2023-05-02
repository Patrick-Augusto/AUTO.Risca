void Controlado() {
  int forward = PS4.R2Value();
  int steering = PS4.LStickX();
  int backward = PS4.L2Value();

  if (abs(steering) < 10) {
    steering = 0;
  }

  if (forward < 10) {
    forward = 0;
  }

  if (PS4.Cross()) {
    forward = PS4.Square() ? 180 : 63;
  } else if (PS4.Circle()) {
    forward = -63;
  }

  int leftMotorOutput = constrain(map(forward, 0, 255, 90, 150) - map(backward, 0, 255, 0, 60) - map(steering, -127, 127, -20, 20), 30, 150);
  int rightMotorOutput = constrain(map(forward, 0, 255, 90, 150) - map(backward, 0, 255, 0, 60) + map(steering, -127, 127, -20, 20), 30, 150);

  MotorEsquerdo.write(leftMotorOutput);
  MotorDireito.write(rightMotorOutput);

#ifdef DEBUG
    Serial.print(rightMotorOutput);
    Serial.print(" ");
    Serial.println(leftMotorOutput);
#endif
}