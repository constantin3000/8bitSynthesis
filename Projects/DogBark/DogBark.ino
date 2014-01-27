#include "samples.h"
// our sample is decoded as 8bit unsigned 8kHz
// whoever finds the reference, gets something from the vending machine!

word pointer = 0;
uint8_t sample_increment = 1;
uint8_t bad_sample_select = 0;
uint16_t bad_sample_length = 0;
uint8_t bad_mod = 0;
uint16_t grain_block = 256;
uint16_t grain_begin = 0;
uint16_t grain_end = grain_block;
uint8_t grain_overflow = 0;

const uint8_t num_bad_samples = 4;

const uint16_t bad_samples_length[] = {
  LUT_BAD_01_SIZE,
  LUT_BAD_02_SIZE,
  LUT_BAD_03_SIZE,
  LUT_BAD_04_SIZE
};


void sampleInterrupt(){
  uint8_t current_sample;
//  const prog_uint8_t* sample_table = bad_samples[bad_sample_select];
//  current_sample = pgm_read_byte(&sample_table + pointer);
  
  // pgm_read_byte reads a value from the program memory
  switch(bad_sample_select){
    case 0:
      current_sample = pgm_read_byte(lut_bad_01 + pointer);
    break;
    case 1:
      current_sample = pgm_read_byte(lut_bad_02 + pointer);
    break;
    case 2:
      current_sample = pgm_read_byte(lut_bad_03 + pointer);
    break;
    case 3:
      current_sample = pgm_read_byte(lut_bad_04 + pointer);
    break;
  }
  pointer += sample_increment;
  // wrap pointer
  if(pointer >= grain_end) {
    grain_overflow++; 
    if(grain_overflow >= bad_mod){
      grain_begin = grain_end;
      grain_end += grain_block;
      grain_overflow = 0;
    }
    pointer = grain_begin;   
   }
   
   if(grain_end >= bad_sample_length) {
     grain_overflow = 0;
     grain_begin = 0;
     grain_end = grain_block;
     pointer = grain_begin;
   }
  
  // set pwm duty on Pin 9
  OCR1A = current_sample; 
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
  PotTask();
}

inline void PotTask(){
  uint16_t pot_a, pot_b;
  pot_a = analogRead(A0);
  pot_b = analogRead(A1);
  
  bad_sample_select = pot_a>>8;
  bad_mod = pot_b>>6;
  bad_sample_length = bad_samples_length[bad_sample_select];  
}
