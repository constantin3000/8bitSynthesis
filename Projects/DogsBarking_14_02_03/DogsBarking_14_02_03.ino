#include "samples.h"
#include "base.h"
// our sample is decoded as 8bit unsigned 8kHz
// whoever finds the reference, gets something from the vending machine!
// desired samplerate 16MHz/2000 ~ 8kHz

static const uint16_t kTimerPrescaler = 64;
static const uint16_t kSamplePrescaler = 2000;
static const uint8_t  kOCRRegister = (kSamplePrescaler/kTimerPrescaler) - 1;

word pointer = 0;
uint8_t sample_increment = 1;
uint8_t bad_sample_select = 0;
uint16_t bad_sample_length = 0;
uint8_t bad_mod = 0;
uint8_t barking_mod = 0;
uint16_t grain_block = 256;
uint16_t grain_begin = 0;
uint16_t grain_end = grain_block;
uint8_t grain_overflow = 0;

const uint8_t num_bad_samples = 3;

const uint16_t bad_samples_length[] = {
  LUT_BARKING_01_SIZE,
  LUT_BARKING_02_SIZE,
  LUT_BARKING_03_SIZE
};


void sampleInterrupt(){
  uint8_t current_sample, second_sample;
//  const prog_uint8_t* sample_table = bad_samples[bad_sample_select];
//  current_sample = pgm_read_byte(&sample_table + pointer);
 
  // pgm_read_byte reads a value from the program memory
  switch(bad_sample_select){
    case 0:
      current_sample = pgm_read_byte(lut_barking_01 + pointer);
    break;
    case 1:
      current_sample = pgm_read_byte(lut_barking_02 + pointer);
    break;
    case 2:
      current_sample = pgm_read_byte(lut_barking_03 + pointer);
    break;
    case 3:
      current_sample = pgm_read_byte(lut_barking_04 + pointer);
    break;
    
  }
 
  pointer += sample_increment;
  if (pointer >= bad_sample_length){
    pointer = 0;
  }
//                // wrap pointer
//                if(pointer >= grain_end) {
//                  grain_overflow++; 
//                  if(grain_overflow >= bad_mod){
//                    grain_begin = grain_end;
//                    grain_end += grain_block;
//                    grain_overflow = 0;
//                  }
//                  pointer = grain_begin;   
//                 }
//                 
//                 if(grain_end >= bad_sample_length) {
//                   grain_overflow = 0;
//                   grain_begin = 0;
//                   grain_end = grain_block;
//                   pointer = grain_begin;
//                 }
  //current_sample = U8Mix(current_sample, other_sample, balance);
   // second_sample = map(sin(millis()*100), -1, 1, 0, 127);// to slow function
   // current_sample = U8Mix(current_sample, second_sample, barking_mod);
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
  uint16_t pot_a, pot_b, sample_rate;
  pot_a = analogRead(A0);
  pot_b = analogRead(A1);
  
  bad_sample_select = pot_a>>8;//bis 3
  bad_mod = pot_b>>3;// bis 127
  barking_mod = pot_b>>2;//bis 255
  // change samplerate, value should be between 16 or 32 and 255, 32 = 8kHz, 64=4kHz, 128=2kHz, 256=1kHz
  sample_rate = pot_b>>2;
   if(sample_rate < kOCRRegister) {
     sample_rate = kOCRRegister;
   } 
  OCR2A = sample_rate; 
  
  bad_sample_length = bad_samples_length[bad_sample_select];  
}
