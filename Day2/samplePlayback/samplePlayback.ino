#include "sample.h"
// our sample is decoded as 8bit unsigned 8kHz
// whoever finds the reference, gets something from the vending machine!
// desired samplerate 16MHz/2000 ~ 8kHz
static const uint16_t kTimerPrescaler = 64;
static const uint16_t kSamplePrescaler = 2000;
static const uint8_t  kOCRRegister = (kSamplePrescaler/kTimerPrescaler) - 1;

static uint16_t pointer = 0;

ISR(TIMER0_COMPA_vect){
  uint8_t current_sample;
  // pgm_read_byte reads a value from the program memory
  current_sample = pgm_read_byte(sample_data + pointer++);
  
  // set pwm duty on Pin 9
  OCR1A = current_sample; 
  
  #if defined(OCR1C)
  // set pwm duty on Pin 11 on Leonardo, Out2 on Esplora
  OCR1C = current_sample; 
  #endif
  
  // wrap pointer
  if(pointer == sample_length) {
    pointer = 0;
  }
}

void setup(){
  // our audio outs, PWM @ 62.5kHz
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  // OUT2 on Esplora  
  pinMode(11, OUTPUT);
  
  setupSampleTimer();
  setupPwmTimer();
}

void loop(){
}

void setupSampleTimer(){
  // Timer0 for samplerate
  // prescaler of timer is set to 64 by wiring.c
  // set mode of Timer 2 to CTC Capture
  TCCR0A = (TCCR0A & 0xfc) | 0x02;  
  // set Compare Match A value, can be altered for Samplerate reduction. Hooray Aliasing!
  OCR0A = kOCRRegister;      
  // enable Timer/Counter2 Output Compare Match A Interrupt Enable
  // triggers ISR(TIMER0_COMPA_vect) routine when TCNT0 = OCR0A, (TCNT0 = timer value) 
  TIMSK0 |= (1<<OCIE0A);
}

void setupPwmTimer(){
  // Timer1 for our PWM output
  // clear Timer1 Modes and Prescaler
  TCCR1A = 0;
  TCCR1B = 0;
  // set prescaler of Timer1 to 1, => PWM Frequency on Pin 9/10 is 62500Hz (16MHz/256)
  TCCR1B = (TCCR1B & 0xf8) | 0x01;  
  // enable PWM on Pin 9 (COM1A1) and Pin 10 (COM1B1)
  TCCR1A |= (1<<COM1A1) | (1<<COM1B1);

  #if defined(COM1C1)
  // for atmega32u4, Esplora OUT2
  TCCR1A |= (1<<COM1C1);
  #endif

  // Fast PWM 8bit, see p. 138 atmega328 datasheet
  TCCR1A |= (1<<WGM10);
  TCCR1B |= (1<<WGM12);  
}



