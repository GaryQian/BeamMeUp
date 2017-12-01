#include <Servo.h>

Servo l;
Servo r;

int lservopin = 5;
int rservopin = 6;
float lstartang = 180;
float rstartang = 180;

int lsensorpin = 1;

float midLen = 35.0;

float targ = 0;

float lang = 180;
float rang = 180;

float prevPos = 9999;
float prevPrevPos = 9999;

float zeroThresh = 0.1;

float t = 0;
float del = 1000;

void setup() {
  l.attach(lservopin);
  r.attach(rservopin);
  t = micros();
}

void loop() {
  t = micros();

  float pos = getPosition();
  float err = targ - pos;

  float velocityTarg = err * 0.01;
  float velocity = ((prevPos - prevPrevPos) + (pos - prevPos)) / 2.0 / (del * 1000000);
  float velocityErr = velocityTarg - velocity;
  
  if (prevPrevPos != 9999) {
    //Update servos
    adjAngle = velocityErr * 10;
    l.write(lstartang + adjAng);
    r.write(rstartang + adgAng);

    if (err < zeroThresh) {
      float adjust = velocityErr * 0.1;
      lstartang += adjust;
      rstartang += adjust;
    }
  }

  prevPrevPos = prevPos;
  prevPos = pos;

  while (micros() < t + del) {}
  
}

float getPosition() {
  
  float ld = constrain(getDistance(lsensorpin), 0, midLen);
  float rd = constrain(getDistance(rsensorpin), 0 , midLen);

  if (ld >= midLen) {
    //Use right distance
    return midLen - rd;
  }
  else if (rd >= midLen) {
    //Use left distance
    return ld - midLen;
  }
  else {
    //Use avg of both distances
    return ld - rd / 2.0;
  }
}


float getDistance(int pin) {

  float mx = 9999999;
  float mn = 0;
  float sum = 0;
  int samples = 5;
  int d = 5;
  for (int i = 0; i < d; ++i) {
    float val = analogRead(pin);
    if (val > mx) mx = val;
    if (val < mn) mn = val;
    sum += val;
    delayMicroseconds(d);
  }
  sum -= mx;
  sum -= mn;
  sum /= (samples - 2.0)

  return (1.0/(sum / 1024.0)) * 6.5;
  
}

