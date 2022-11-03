#include <Servo.h>

// Arduino pin assignment

#define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3
#define PIN_IR 0 // IR at Pin A0
// Add IR Sensor Definition Here !!!
#define PIN_SERVO 10
#define PIN_LED 9
#define _EMA_ALPHA 0.5    // EMA weight of new sample (range: 0 to 1)

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 4000 // servo full counter-clockwise position (180 degree)

#define LOOP_INTERVAL 50   // Loop Interval (unit: msec)

Servo myservo;
unsigned long last_loop_time;   // unit: msec

float dist;
float dist_ema;

void setup()
{ 
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(57600);
}

void loop()
{
  unsigned long time_curr = millis();
    int a_value, duty;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  a_value = analogRead(PIN_IR);
  float dist = (6762.0 / (a_value - 9) - 4.0) * 10.0;

  if (dist_ema >= 100.0 && dist_ema <= 250.0){
    digitalWrite(PIN_LED, 0);
  }
  else{
    digitalWrite(PIN_LED, 1);
  }

  dist_ema = _EMA_ALPHA * dist + (1-_EMA_ALPHA) * dist_ema;
  duty = map(a_value, 0, 1023, _DUTY_MIN, _DUTY_MAX);

  myservo.writeMicroseconds(duty);

  Serial.print("ADC Read: "); Serial.print(a_value);
  Serial.print(" = ");
  Serial.print((a_value / 1024.0) * 5.0);
  Serial.print(" Volt => Duty : ");
  Serial.print(duty);
  Serial.println("usec");
}