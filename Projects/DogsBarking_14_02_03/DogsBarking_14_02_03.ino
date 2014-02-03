#include "samples.h"
#include "resources.h"
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
// control playback sample
uint8_t play_sample = 1;
uint8_t amplitude = 0;

LongWord sample_phase;
LongWord sample_phase_increment;

LongWord trigger_phase;
LongWord trigger_phase_increment;

const uint8_t num_bad_samples = 4;

const uint16_t bad_samples_length[] = {
  LUT_BARKING_01_SIZE,
  LUT_BARKING_02_SIZE,
  LUT_BARKING_03_SIZE,
  LUT_BARKING_04_SIZE
};

uint8_t pattern_3[] = {1,2,2, 2,2,3, 2,2,1};
uint8_t pattern_2[] = {1,2, 3,2, 0,1};

uint8_t pattern_3_index = 0;
uint8_t pattern_2_index = 0;

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
  
  if(trigger_phase.value < trigger_phase_increment.value){
    play_sample = 1;
  }
 
  pointer = sample_phase.words[INTEGRAL];
  trigger_phase.value += trigger_phase_increment.value;
  if(play_sample){
    sample_phase.value += sample_phase_increment.value;
  }
  
  
  if (pointer >= bad_sample_length){
    play_sample = 0;
    sample_phase.value = 0;
    pattern_3_index++;
    pattern_2_index++;
    
    if(pattern_3_index == 3) {
      pattern_3_index = 0;
    }
    if(pattern_2_index == 2) {
      pattern_2_index = 0;
    }
  } 
  
  if(play_sample){ 
   // set pwm duty on Pin 9
    OCR1A = U8Mix(127, current_sample, amplitude); 
  } else {
    OCR1A = 127;
  }
  
}

void setup(){
  // our audio outs, PWM @ 62.5kHz
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);  // OUT2 on Esplora  
  
  trigger_phase.value = 0;
  trigger_phase_increment.value = pgm_read_dword(lut_rate_increments);
  
  sample_phase.value = 0;
  // original playback rate
  sample_phase_increment.value = 32767;
  
  Serial.begin(57600);
  
  setupSampleTimer();
  setupPwmTimer();
}

void loop(){
  PotTask();
  PatternTask();
  delay(10);
}

inline void PotTask(){
  uint16_t pot_a, pot_b, sample_rate;
  pot_a = analogRead(A0);
  pot_b = analogRead(A1);
  
  // select new phase increment via pot, range 0..255
  trigger_phase_increment.value = pgm_read_dword(lut_rate_increments + (pot_a>>2));
  // change samplerate, value should be between 16 or 32 and 255, 32 = 8kHz, 64=4kHz, 128=2kHz, 256=1kHz
  sample_rate = map(pot_b, 0, 1023, 32, 1023);
  // 32767 is original rate, 65535 doubled rate, 16384 half rate
  sample_phase_increment.value = (sample_rate*2*32); 
}

void PatternTask(){
  uint16_t pot_pattern;
  uint8_t section = 0;
  // pot value divided by num of sections
  const uint8_t pot_section_size = (1024/6); 
  
  pot_pattern = analogRead(A2);
//  Serial.println(pot_pattern);
  if(pot_pattern > 140) {
    
    while(pot_pattern > pot_section_size){
      pot_pattern -= pot_section_size;
      section++;
    }
    
    switch(section){
      //which pattern to choose from
      case 0:
        bad_sample_select = pattern_3[pattern_3_index];
      break;
      case 1:
        bad_sample_select = pattern_3[3 + pattern_3_index];
      break;
      case 2:
        bad_sample_select = pattern_3[6 + pattern_3_index];
      break;
      case 3:
        bad_sample_select = pattern_2[pattern_2_index];
      break;
      case 4:
        bad_sample_select = pattern_2[2 + pattern_2_index];
      break;
      case 5:
        bad_sample_select = pattern_2[4 + pattern_2_index];
      break;
    }
    
    // selects sample
    bad_sample_length = bad_samples_length[bad_sample_select]; 
    amplitude = 255;
  } else {
    amplitude = 0;
  }
}
