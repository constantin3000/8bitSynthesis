#include "osc.h"
byte flag = 4;
word scale = 36 * kDetunePerSemitone; //start at c3

Osc osc;

void sampleInterrupt(){
  uint8_t sample = osc.Render();
//  analogWrite causes audible glitches
//  analogWrite(9, osc.Render()>>8);
  // set pwm duty on Pin 9
  OCR1A = sample; 
  // set pwm duty on Pin 10
  OCR1B = 255-sample; 
  #if defined(OCR1C)
  // set pwm duty on Pin 11 on Leonardo, Out2 on Esplora
  OCR1C = sample; 
  #endif    
}

void setup(){
  pinMode(flag, OUTPUT);
  // our audio outs, PWM @ 62.5kHz
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT); 
  pinMode(11, OUTPUT); // OUT2 on Esplora 
  osc.Init();
  setupSampleTimer();
  setupPwmTimer();
}

void loop(){
  osc.Update();
  NoteTask();
  PotTask();
  delay(10);  
}

// a simple Frequency sweep
void SweepTask(){
  byte note;
  int detune;
  // get lower 4 bits for our detune value
  detune = scale & 0x0f;
  // get note value
  note = scale>>4;
  
  osc.set_note(note);
  osc.set_detune(detune);
  
  scale++;
  if(scale >= kHighestNote) {
    scale = 36 * kDetunePerSemitone; //start at c3, midinote multiplied by detune steps (16) per semitone 
  }
}

void NoteTask(){
  osc.set_note(60); // C5
  osc.set_detune(0);
}

void PotTask(){
  uint16_t pot_wave;
  uint8_t bank, wave;
  pot_wave = analogRead(A0);
  
  // bank 0 is sine harmonics
  bank = 0;
  // morph trough wavetables
  wave = pot_wave>>3;
  
  // get highest 3 bit, a value between 0..7
//  bank = (pot_wave >> 7);
  // use lower 7 bit (0..127) as our wave selector/interpolator; 
  // jitter in analog reads, causes audible glitches
//  wave = (pot_wave & 0x7f);
  osc.set_wave(bank, wave);
}

