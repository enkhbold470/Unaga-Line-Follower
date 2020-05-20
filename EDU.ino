#define PWMB 9
#define BIN2 8
#define BIN1 7
#define STBY 6
#define AIN1 5
#define AIN2 4
#define PWMA 3
#define NOTE_CS4  277

float kd = 0.5;
float kp = 0.05;
int Speed = 100;
int max_speed = 255;
int min_speed = 0;
int s_max[6]; int s_min[6]; int s_threshold[6];
int sd[6]; int s[6];
int Move = 0;
int L_med = (max_speed + min_speed) / 2 +30;
int R_med = (max_speed + min_speed) / 2 +30;
int data = 0;
int mode = 0;
int pos, opt, last_error, error, R, L = 0;
int count = 4;
int frequency = 64;
void setup() {

  Serial.begin(9600);
  Serial.println("ON UART");
  pinMode(10, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
//  tone(10, NOTE_C4,50);  
//  tone(10, NOTE_C4,50); 
  buzzer(3,100);
do{
  digitalWrite(STBY, 1);
car();

}
  while (digitalRead(2) != 0);
  digitalWrite(STBY, 0);
  buzzer(1);
  for (int i = 0; i < 6; i++) {
    s_max[i] = analogRead(i);
    Serial.print("sensor_max"); Serial.print(i); Serial.print("=");
    Serial.print(s_max[i]);
  }
  Serial.println(" ");
  Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

  delay(300);
  while (digitalRead(2) != 0);
  buzzer(1);
  for (int i = 0; i < 6; i++) {
    s_min[i] = analogRead(i);
    Serial.print("sensor_min"); Serial.print(i); Serial.print("=");
    Serial.print(s_min[i]);
  }
  Serial.println(" ");
  Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

  for (int i = 0; i < 6; i++) {
    s_threshold[i] = (s_max[i] + s_min[i]) / 2;

    Serial.print("sensor_th"); Serial.print(i); Serial.print("=");
    Serial.print(s_threshold[i]);
  }
  Serial.println(" ");
  Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

  delay(300);
 
  while (digitalRead(2) != 0 );
  buzzer(2);
  digitalWrite(STBY, 1);

}
void loop() {
  
 line(false);//true robot will follow the white line
             //false robot will follow the black line

 

}
void buzzer(int times,int del){
for(int i=0;i<times;i++){
  tone(10, NOTE_CS4,100);
  delay(del);
  noTone(10);
  delay(del);
}
  
}
void line(bool color) {
  if (color == true) {
    for (int i = 0; i < 6; i++) {
      s[i] = analogRead(i);
      if (s[i] > s_threshold[i]) {
        sd[i] = 1;
      } else {
        sd[i] = 0;
      }
    }
  }
  if (color == false) {
    for (int i = 0; i < 6; i++) {
      s[i] = analogRead(i);
      if (s[i] > s_threshold[i]) {
        sd[i] = 0;
      } else {
        sd[i] = 1;
      }
    }
  }

   if (sd[0] == 0 && sd[1] == 0 && sd[2] == 0 && sd[3] == 0 && sd[4] == 0 && sd[5] == 0) {
    error = 2500;
  } else {
  pos = (0 * sd[0] + 1000 * sd[1] + 2000 * sd[2] + 3000 * sd[3] + 4000 * sd[4] + 5000 * sd[5]) / (sd[0] + sd[1] + sd[2] + sd[3] + sd[4] + sd[5]);
  }
  error = pos - 2500;
  //  Serial.println(error);
  opt = kp * error + kd * (last_error - error);
  last_error = error;
  L = L_med - opt;
  R = R_med + opt;
  if (L > max_speed) L = max_speed;
  if (R > max_speed) R = max_speed;
  if (L < min_speed) L = L;
  if (R < min_speed) R = R;
  Motor(L, R);
}
void car() {
  if (Serial.available() > 0) {
    Move = Serial.read();
    Motor(0, 0);
    analogWrite(10, 0);
    Speed=255;
    switch (Move) {

      case 'F':
        Motor(Speed, Speed);
        break;
      case 'B':
        Motor(-Speed, -Speed);
        break;
      case 'L':
        Motor(Speed, 0);
        break;
      case 'R':
        Motor(0, Speed );
        break;
      case 'V':
        analogWrite(10, 255);
        break;
      case 'X':
        Speed = 0;
        break;
    }
  }
}

void Motor(int zuun, int baruun)
{

  analogWrite(PWMA, abs(baruun));
  analogWrite(PWMB, abs(zuun));
  if (baruun < 0)
  {
    digitalWrite(AIN1, 0);
    digitalWrite(AIN2, 1);
  }
  else if (baruun > 0)
  {
    digitalWrite(AIN1, 1);
    digitalWrite(AIN2, 0);

  }
  else
  {
    digitalWrite(AIN1, 1);
    digitalWrite(AIN2, 1);

  }
  if (zuun > 0)
  {
    digitalWrite(BIN1, 0);
    digitalWrite(BIN2, 1);
  }
  else if (zuun < 0)
  {
    digitalWrite(BIN1, 1);
    digitalWrite(BIN2, 0);

  }
  else
  {
    digitalWrite(BIN1, 1);
    digitalWrite(BIN2, 1);

  }
}
void buzzer(int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(10, 1);
    delay(50);
    digitalWrite(10, 0);
    delay(50);

  }

}
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if (pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if (pin == 3 || pin == 11) {
    switch (divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
