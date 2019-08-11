#include <QTRSensors.h>
#define PWMB 3
#define BIN2 4
#define BIN1 5
#define STBY 6
#define AIN1 7
#define AIN2 8
#define PWMA 9
#define start 2
#define NUM_SENSORS             6  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
float kd = 0.01;
float kp = 0.05;
int Speed = 100;
int max_speed = 255;
int min_speed = 0;

int Move = 0;
int L_med = (max_speed + min_speed) / 2;
int R_med = (max_speed + min_speed) / 2;
int d = 0;
int mode = 0;
int pos, opt, last_error, error, R, L = 0;
QTRSensorsAnalog qtra((unsigned char[]) {
  0, 1, 2, 3, 4, 5
},
NUM_SENSORS, NUM_SAMPLES_PER_SENSOR);
unsigned int sensorValues[NUM_SENSORS];

int frequency = 64;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("ON UART");
  pinMode(10, OUTPUT);

  analogWrite(10, 3);    // turn on Arduino's LED to indicate we are in calibration
  for (int i = 0; i < 127; i++)  // make the calibration take about 10 seconds
  {
    qtra.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)

  }


  analogWrite(10, 0);     // turn off Arduino's LED to indicate we are through with calibration

  pinMode(start, INPUT_PULLUP);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
    setPwmFrequency(PWMA, frequency);
    setPwmFrequency(PWMB, frequency);
  while (digitalRead(2) != 0);
  digitalWrite(STBY, 1);
analogWrite(10,255);
delay(100);
analogWrite(10,0);
delay(50);
analogWrite(10,255);
delay(100);
analogWrite(10,0);
}

void loop() {
  //  if (mode == 0) {
  line();
  //  } else {
 //    car();
  //  }

}
void line() {
  unsigned int position = qtra.readLine(sensorValues,true,true);
  error = (position - 2500);
//  Serial.println(error);
    opt = kp * error + kd * (last_error - error);
    last_error = error;
  
    L = L_med - opt;
    R = R_med + opt;
  
    if (L > max_speed) {
      L = max_speed;
    }
    if (R > max_speed) {
      R = max_speed;
    }
    if (L < min_speed) {
      L = L;
    }
    if (R < min_speed) {
      R = R;
  
    }
  
    Motor(L, R);
}

void Motor(int zuun, int baruun)
{

  analogWrite(PWMA, abs(zuun));
  analogWrite(PWMB, abs(baruun));
  if (zuun < 0)
  {
    digitalWrite(AIN1, 0);
    digitalWrite(AIN2, 1);
  }
  else if (zuun > 0)
  {
    digitalWrite(AIN1, 1);
    digitalWrite(AIN2, 0);

  }
  else
  {
    digitalWrite(AIN1, 1);
    digitalWrite(AIN2, 1);

  }
  if (baruun > 0)
  {
    digitalWrite(BIN1, 0);
    digitalWrite(BIN2, 1);
  }
  else if (baruun < 0)
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


void car() {
  if (Serial.available() > 0) {
    Move = Serial.read();
    Motor(0, 0);
    switch (Move) {

      case 'F':
        Motor(Speed, Speed);
        break;
      case 'B':
        Motor(-Speed, -Speed);
        break;
      case 'L':
        Motor(Speed, 50);
        break;
      case 'R':
        Motor(50, Speed );
        break;
      case 'G':
        Motor(Speed, 0);
        break;
      case 'H':
        Motor( 0, -Speed);
        break;
      case 'J':
        Motor(-Speed, 0);
        break;

      case 'I':
        Motor(0, Speed);
        break;
      case '0':
        Speed = 100;
        break;
      case '1':
        Speed = 140;
        break;
      case '2':
        Speed = 153;
        break;
      case '3':
        Speed = 165;
        break;
      case '4':
        Speed = 178;
        break;
      case '5':
        Speed = 191;
        break;
      case '6':
        Speed = 204;
        break;
      case '7':
        Speed = 216;
        break;
      case '8':
        Speed = 229;
        break;
      case '9':
        Speed = 242;
        break;
      case 'q':
        Speed = 255;
        break;
      case 'v':
        analogWrite(10, 0);
        break;
      case 'V':
        analogWrite(10, 3);
        break;
      case 'x':
        Speed = 204;
        break;
      case 'X':
        Speed = 0;
        break;
    }
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
