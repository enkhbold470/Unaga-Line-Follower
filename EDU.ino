/*
Зохиогч : TTF Robotics 

Сайн байна уу? Унага роботыг сонгосон танд баярлалаа. ^^

Та доорх холбоосоор орж роботоо програмчлах зааврыг үзээрэй. ^^
https://www.youtube.com/watch?v=OtFdYViO9gM

Та доорх холбоосоор орж роботоо угсарах зааврыг үзээрэй. ^^
https://www.youtube.com/watch?v=3mSFMrgr_0Y&t=30s

© Оюуны өмчийг дээдэлсэн танд баярлалаа.
*/
#define PWMB 9
#define BIN2 8
#define BIN1 7
#define STBY 6
#define AIN1 5
#define AIN2 4
#define PWMA 3

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

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

int melody[] = {NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4};

int durations[] = {4, 4, 4, 5, 16, 4, 5, 16, 2, 4, 4, 4, 5, 16, 4, 5, 16, 2};

int tempo = 200; 

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
  
playTune(melody, durations, tempo);


do{
  digitalWrite(STBY, 1);
car();

}
  while (digitalRead(2) != 0);
  digitalWrite(STBY, 0);
  buzzer(1,100);
  for (int i = 0; i < 6; i++) {
    s_max[i] = analogRead(i);
    Serial.print("sensor_max"); Serial.print(i); Serial.print("=");
    Serial.print(s_max[i]);
  }
  Serial.println(" ");
  Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");

  delay(300);
  while (digitalRead(2) != 0);
  buzzer(1,100);
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
  buzzer(2,100);
  digitalWrite(STBY, 1);

}
void loop() {
  
 line(false);//true robot will follow the white line
             //false robot will follow the black line

 

}
void playTune(int notes[], int durations[], int BPM)
{
  int tuneSize = sizeof(melody) / sizeof(int);
  for (int thisNote = 0; thisNote < tuneSize; thisNote++) {   
    int noteDuration = (int)((1000 * (60 * 4 / BPM)) / durations[thisNote] + 0.);
    tone(10, notes[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.20;
    delay(pauseBetweenNotes);
    noTone(10);
  }
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
    buzzer(2,100);
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
