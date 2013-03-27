#include <Wire.h>
#include <TVout.h>

#include "fplib.h"
#include "quaternionlib.h"

#define W 128
#define H 128

TVout tv;

bool debug=true;

void setup()  {
  Serial.begin(115200);
  clear_cmdBuf();
  Wire.begin();
  tv.begin(PAL, W, H);
  initOverlay();
  initInputProcessing();

//  tv.select_font(font4x6);
  tv.fill(0);
}

void initOverlay() {
  TCCR1A = 0;
  // Enable timer1.  ICES0 is set to 0 for falling edge detection on input capture pin.
  TCCR1B = _BV(CS10);

  // Enable input capture interrupt
  TIMSK1 |= _BV(ICIE1);

  // Enable external interrupt INT0 on pin 2 with falling edge.
  EIMSK = _BV(INT0);
  EICRA = _BV(ISC01);
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

ISR(INT0_vect) {
  display.scanLine = 0;
}

void loop() {

  cmd_cross();

  tv.delay_frame(5);
}

void error (const char *msg)
{
  Serial.print("Error: ");
  Serial.println(msg);
}
