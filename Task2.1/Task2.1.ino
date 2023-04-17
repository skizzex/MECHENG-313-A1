#include <avr/interrupt.h>

//define global variables

//timer ISR to update every second

//ADC ISR to record ADC value


//blink the LED a number of times
//input: number of blinks
void blink(uint8_t blinks) {

}

int main(void) {
  sei();  //enable global interrupts

  //set pin functions(LED, ADC) (N/A?)

  DDRB |= (1 << DDB5); //set built-in LED pin to output

  //testing - turn LED on
  PORTB |= (1 << PORTB5);

  //configure ADC

  //configure timer 1 (16 bit timer at 1MHz) to CTC mode to read ADC every second



  while (1) {

  }
}