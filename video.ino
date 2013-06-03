#include <Wire.h>
#include <TVout.h>

#include "fplib.h"
#include "quaternionlib.h"

#define W 128
#define H 128

TVout tv;

bool debug=true;

quaternion imu_q=ident;
fixed imu_z=0;
fixed wall=one>>2;

void setup()  {
  pinMode(13, OUTPUT);
  Serial.begin(57600);
  Serial2.begin(115200);
  Serial.println("\nInitializing... ");
  Wire.begin();
  sonar_init();
  tv.begin(PAL, W, H);
  initOverlay();
  initInputProcessing();

  tv.fill(0);
  Serial.println("OK");
  //sonar_address_change();  //only enable it once and with one sensor connected
}

void initOverlay() {
  TCCR1A = 0;
  // Enable timer1.  ICES0 is set to 0 for falling edge detection on input capture pin.
  TCCR1B = _BV(CS10);

  // Enable input capture interrupt
  TIMSK1 |= _BV(ICIE1);

  // Enable external interrupt INT3 on pin 18 with falling edge.
//  EIMSK = _BV(INT3);
//  EICRA = _BV(ISC31);
  attachInterrupt(5, Reset_scanLine, FALLING);
}

void initInputProcessing() {
  // Analog Comparator setup
  ADCSRA &= ~_BV(ADEN); // disable ADC
  ADCSRB |= _BV(ACME); // enable ADC multiplexer
  ADMUX &= ~_BV(MUX0);  // select A2 for use as AIN1 (negative voltage of comparator)
  ADMUX |= _BV(MUX1);
  ADMUX &= ~_BV(MUX2);
  ACSR &= ~_BV(ACIE);  // disable analog comparator interrupts
  ACSR &= ~_BV(ACIC);  // disable analog comparator input capture
}

//ISR(INT3_vect) {
//  display.scanLine = 0;
//}

void Reset_scanLine(void) {
  display.scanLine = 0;
}


void loop() {

//  cmd_rangea();
//  cmd_rangeb();
  DetectLandingPad();

  tv.delay_frame(5);

}

void error (const char *msg)
{
  Serial.print("Error: ");
  Serial.println(msg);
}

void print(fixed val)
{
  int v;
  if(val==one)
    Serial.print(" 1.00 ( one )");
  else
  {
    if(val==-one)
      Serial.print("-1.00 (");
    else
    {
      if(val>=0)
        Serial.print(" 0.");
      else
        Serial.print("-0.");
      v=abs(val.value)/0x20C49C;
      if(v<10)
        Serial.print("00");
      else if(v<100)
        Serial.print("0");
      Serial.print(v);
    }
    Serial.print(" (");
    if(val>=0) Serial.print(" ");
    Serial.print(val.value);
    Serial.print(" )");
  }
}

void print(const char *name, fixed val)
{
  Serial.print(name);
  Serial.print(" =");
  print(val);
  Serial.println("");
}

void print(const char *name, quaternion val)
{
  Serial.print(name);
  Serial.print(" = [");
  print(val.w);
  Serial.print(",");
  print(val.x);
  Serial.print(",");
  print(val.y);
  Serial.print(",");
  print(val.z);
  Serial.println("]");
}
