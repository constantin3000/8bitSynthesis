#include "osc.h"

// desired samplerate 16MHz/512 => 31250Hz
static const uint16_t kTimerPrescaler = 64;
static const uint16_t kSamplePrescaler = 512;
// range of the sample rate is between (16MHz/(kTimerPrescaler*kOCRRegister)) => 250000Hz - 980Hz
static const uint8_t  kOCRRegister = (kSamplePrescaler/kTimerPrescaler) - 1;

uint8_t flag = 4;

Osc osc;

ISR(TIMER0_COMPA_vect){
  uint8_t sample;
  digitalWrite(flag, HIGH);
  sample = osc.Render();
  // set pwm duty on Pin 9
  OCR1A = sample; 
  // set pwm duty on Pin 10
  OCR1B = 255-sample; 
  #if defined(OCR1C)
  // set pwm duty on Pin 11 on Leonardo, Out2 on Esplora
  OCR1C = sample; 
  #endif
//  analogWrite causes audible glitches
//  analogWrite(9, osc.Render()>>8); 
  digitalWrite(flag, LOW);     
}

void setup(){
  pinMode(flag, OUTPUT);
  // our audio outs, PWM @ 62.5kHz
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  // OUT2 on Esplora  
  pinMode(11, OUTPUT);
  osc.Init();
  setupSampleTimer();
  setupPwmTimer();
}

static void NoteTask(){
  int detune = analogRead(0)>>2;
  osc.set_detune(detune);
}

void loop(){
  osc.Update();
  NoteTask();
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


