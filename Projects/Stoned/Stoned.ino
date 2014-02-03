#include "osc.h"

byte amplitude;
Osc osc, the_osc;

void sampleInterrupt(){
  uint8_t sample, the_sample;
  sample = osc.Render();
  the_sample = the_osc.Render();
//  sample = (sample>>1) + (the_sample>>1);
//  analogWrite causes audible glitches
//  analogWrite(9, sample);
  // set pwm duty on Pin 9
  OCR1A = U8Mix(127, sample, amplitude); 
  // set pwm duty on Pin 10
  OCR1B = 255-sample; 
  
//  digitalWrite(flag, osc.gimme_overflow());
  #if defined(OCR1C)
  // set pwm duty on Pin 11 on Leonardo, Out2 on Esplora
  OCR1C = sample; 
  #endif   
}

void setup(){
  // our audio outs, PWM @ 62.5kHz
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT); 
  pinMode(11, OUTPUT); // OUT2 on Esplora 
  osc.Init();
  the_osc.Init();
  
  // internal reference 1.1V
  analogReference(INTERNAL);
  
  setupSampleTimer();
  setupPwmTimer();
}

void loop(){
  osc.Update();
  the_osc.Update();
  
  PotTask();
  delay(10);  
}

void PotTask(){
  uint16_t pot_wave, pot_pitch;
  uint8_t bank, wave;
  uint8_t note, detune;
  pot_pitch = analogRead(A0);
  pot_wave = analogRead(A1);
  pot_amp = analogRead(A1);
  
  // bank 0 is sine harmonics
  bank = 0;
  // morph trough wavetables
  wave = map(pot_amp, 0, 1023, 0, 127);
  // bank range 0..7, wave range 0..127
  osc.set_wave(bank, wave);
  
  amplitude = map(pot_amp, 0, 1023, 0, 255);
//  the_osc.set_wave(bank, wave);
  note = map(pot_pitch, 0, 1023, 32, 100);
//  osc.set_note(note);
  osc.set_note(60);
  osc.set_detune((pot_pitch&0x07)<<1);
}


