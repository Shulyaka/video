#include <TVout.h>
//#include <fontALL.h>
#define W 128
#define H 128

TVout tv;
//unsigned char x,y;
char s[32];


void setup()  {
  Serial.begin(115200);
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
  EICRA = _BV(ISC11);
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
  bool matchfound=false;
  unsigned char x, y;
  tv.capture();
  tv.fill(INVERT);
  
  //FilterNoise();
  
  for(y=0; y<H; y++)
  {
    for(x=0; x<W; x++)
      if(tv.get_pixel(x,y))
      {
        if(!MatchX(x,y))
          {ClearArea(x,y);
          }
        else
          {matchfound=true;
          //ClearArea(x,y);
          break;
          }
      }
      if(matchfound)
        break;
  }
  
  if(matchfound)
  {
    Serial.print("Object detectad at ");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.println(".");
    tv.draw_rect(x-5, y-5, 10, 10, 1);
  }
  else
    Serial.println("Object not detected");

  //tv.fill(INVERT);
  tv.resume();
  tv.delay_frame(5);
}

