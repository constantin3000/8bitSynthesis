#include "osc.h"

// desired samplerate 16MHz/416 => 38461Hz
static const uint16_t kTimerPrescaler = 32;
static const uint16_t kSamplePrescaler = 416;
// range of the sample rate is between (16MHz/(32*kOCRRegister)) => 50000Hz - 2440Hz
static const uint8_t  kOCRRegister = (kSamplePrescaler/kTimerPrescaler) - 1;

uint8_t flag = 4;

Osc osc;

ISR(TIMER4_COMPA_vect){
  uint8_t sample;
  digitalWrite(flag, HIGH);
  sample = osc.Render();
  OCR1A = sample; // set pwm duty Pin 9
//  OCR1B = sample; // set pwm duty Pin 10
//  OCR1C = sample; // setpwm duty on Out2 on Esplora

//  analogWrite causes audible glitches
//  analogWrite(9, osc.Render()>>8); 
  digitalWrite(flag, LOW);     
}

void setup(){
  pinMode(flag, OUTPUT);
  // our audio outs, PWM @ 62.5kHz
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  osc.Init();
  setupTimers();
}

static void NoteTask(){
osc.set_note(69);
}

void loop(){
NoteTask();
}

void setupTimers(){
  // Timer1 for our PWM output
  // clear Timer1 Modes and Prescaler
  TCCR1A = 0;
  TCCR1B = 0;
  // set prescaler of Timer1 to 1, => PWM Frequency on Pin 9/10 is 62500Hz (16MHz/256)
  TCCR1B = (TCCR1B & 0xf8) | 0x01;  
  // enable PWM on Pin 9 (COM1A1) and Pin 10 (COM1B1)
  TCCR1A |= (1<<COM1A1) | (1<<COM1B1);
  // for atmega32u4, 
  #if defined(COM1C1)
  TCCR1A |= (1<<COM1C1);
  #endif
  // Fast PWM 8bit, see p. 138
  TCCR1A |= (1<<WGM10);
  TCCR1B |= (1<<WGM12);
  
  // for Arduino Uno, Nano, Duemilanove
  #if defined(__AVR_ATmega328P__)
  // Timer2 for samplerate
  // set mode of Timer 2 to CTC Capture
  TCCR2A = (TCCR2A & 0xfc) | 0x02;  
  // set prescaler of Timer2 to 32
  TCCR2B = (TCCR2B & 0xf8) | 0x03;  
  // set Compare Match A value, can be altered for Samplerate reduction. Hooray Aliasing!
  OCR2A = kOCRRegister;      
  // enable Timer/Counter2 Output Compare Match A Interrupt Enable
  // triggers ISR(TIMER2_COMPA_vect) routine when TCNT2 = OCR2A, (TCNT2 = timer value) 
  TIMSK2 |= (1<<OCIE2A);
  // for Arduino Leonardo, Esplora
  #elif (__AVR_ATmega32U4__)
  // Fast PWM on Pin 6
//  TCCR4B |= (1<<PWM4X);
//  TCCR4D &= ~(1<<WGM40);
//  TCCR4D &= ~(1<<WGM41);
//  TCCR4A &= ~(1<<COM4A1);
//  TCCR4A &= ~(1<<COM4A0);
//  TCCR4A &= ~(1<<PWM4A);
  TCCR4A = 0;
  // set prescaler of Timer4 to 32
  TCCR4B = 0;
  TCCR4B = (TCCR4B & 0xf0) | 0x05;
//  TCCR4C = 0;
  // set Compare Match A value, can be altered for Samplerate reduction. Hooray Aliasing!
  OCR4A = kOCRRegister;    
  // enable Timer/Counter4 Output Compare Match A Interrupt Enable
  // triggers ISR(TIMER4_COMPA_vect) routine when TCNT2 = OCR4A, (TCNT2 = timer value) 
  TIMSK4 |= (1<<OCIE4A);
  #endif  
}


