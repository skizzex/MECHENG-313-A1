#include <avr/interrupt.h>

#define TOGGLELED() PORTB ^= (1 << PORTB5)
#define ENABLELED() PORTB |= (1 << PORTB5)
#define DISABLELED() PORTB &= ~(1 << PORTB5)

//define global variables
static volatile uint8_t toggleCount;
static volatile uint8_t toggleMax;
static volatile uint16_t cycleCount; //for counting cycles with 16MHz clock

//timer 0 ISR to toggle LED
ISR(TIMER0_COMPA_vect) {
  if (!(++cycleCount % 16)) { //toggle LED only at multiples of 16 cycles so period is 0.2 secs
    TOGGLELED();

      //check if LED cycles reached to stop blinking
    if (++toggleCount == toggleMax) {
      TIMSK0 &= ~(1 << OCIE0A);//disable comp A interrupt
    }
  }
}

//timer 1 ISR 
ISR(TIMER1_OVF_vect) {
  TOGGLELED();
}

//ADC ISR to record ADC value


//initialise built-in LED
void LED_init() {
  DDRB |= (1 << DDB5); //set built-in LED pin to output
}

//initialise timer 0 for blinking the LED
void timer0_init() {
  TCCR0B &= ~(CS02 | CS01 | CS00); //stop timer by setting no clock source

  TCCR0A |= (1 << WGM01); //set timer 0 to CTC mode
  TCCR0B |= (1 << CS02) | (1 << CS00); //start timer and set timer 0 clock prescaler to 1024

  OCR0A = 194;  //set compare register A value to 194 for LED toggle period of about 0.2s (see notes for calculation)
}

//initialise timer 1 for ADC sampling every second
void timer1_init() {
  TCCR1B &= ~(CS12 | CS11 | CS10); //stop timer by setting no clock source

  TCCR1A = 0; //TESTING - timer 1 in normal mode
  TCCR1B |= (1 << CS10); //TESTING - no prescaler

  TIMSK1 |= (1 << TOIE1); //TESTING - overflow interrupt enable
}

//initialise ADC
void ADC_init() {

}

//blink the LED a number of times
//input: number of blinks
void blinkLED(uint8_t blinks) {
  ENABLELED();  //turn LED on 

  toggleCount = 0;  //initialise counter for number of toggles for LED
  toggleMax = 2*blinks - 1; //set required number of toggles given number of blinks

  TCNT0 = 0;  //reset timer 0 counter value to begin counting
  TIFR0 &= ~(1 << OCF0A);  //reset OCR0A interrupt flag (as it might be high after starting timer)
  TIMSK0 |= (1 << OCIE0A);  //enable comp A interrupt 
}

int main(void) {
  sei();  //enable global interrupts

  LED_init();
  timer0_init();  //configure timer 0 to toggle LED every 0.2 seconds (CLOCK IS AT 16MHz)
  // timer1_init();  //configure timer 1 (16 bit timer at 16MHz) to CTC mode to read ADC every second
    //configure ADC

  blinkLED(2);

  while (1) {
    //begin ADC 

  }
}