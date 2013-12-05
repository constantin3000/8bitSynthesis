#include "sample.h"
// our sample is decoded as 8bit unsigned 8kHz
// whoever finds the reference, gets something from the vending machine!

word pointer = 0;

void sampleInterrupt(){
  uint8_t current_sample;
  // pgm_read_byte reads a value from the program memory
  current_sample = pgm_read_byte(sample_data + pointer++);
  
  // set pwm duty on Pin 9
  OCR1A = current_sample; 
  // set pwm duty on Pin 11 on Leonardo, Out2 on Esplora
  #if defined(OCR1C)
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
  pinMode(11, OUTPUT);  // OUT2 on Esplora  
  
  setupSampleTimer();
  setupPwmTimer();
}

void loop(){
}

