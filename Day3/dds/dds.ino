#include "osc.h"
byte flag = 4;
word scale = 36 * kDetunePerSemitone; //start at c3
uint8_t rungler;
uint8_t sub_clock;

Osc osc, the_osc;

void sampleInterrupt(){
  uint8_t clock, data;
  uint8_t sample, the_sample;
  sample = osc.Render();
  the_sample = the_osc.Render();
  
  clock = osc.gimme_overflow();
  sub_clock += clock;
  data = the_sample > 127;
  if(sub_clock == 31){
    sub_clock = 0;
    rungler = (rungler<<clock) | data;
  }
  sample = (sample>>1) + (the_sample>>1);
//  analogWrite causes audible glitches
//  analogWrite(9, osc.Render()>>8);
  // set pwm duty on Pin 9
  OCR1A = sample; 
  // set pwm duty on Pin 10
  OCR1B = 255-sample; 
  
//  digitalWrite(flag, osc.gimme_overflow());
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
  the_osc.Init();
  setupSampleTimer();
  setupPwmTimer();
  
  NoteTask();
}

void loop(){
  osc.Update();
  the_osc.Update();
  RunglerTask();
//  CounterTask();
  PotTask();
  delay(10);  
}

void RunglerTask(){
  uint8_t note = 60;
  // a 3 bit value 0..7
  note += ((rungler>>5))*4;
  osc.set_note(note);
  
  note = 13;
  note += (rungler>>5)*2;
  the_osc.set_note(note);
  the_osc.set_detune(8);
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
  the_osc.set_note(76);
}

void PotTask(){
  uint16_t pot_wave;
  uint8_t bank, wave;
  pot_wave = analogRead(A0);
  
  // bank 0 is sine harmonics
  bank = 0;
  // morph trough wavetables
  wave = pot_wave>>3; // divided by 8
  
  // get highest 3 bit, a value between 0..7
  bank = (pot_wave >> 7); // divided by 2**7 128
  // use lower 7 bit (0..127) as our wave selector/interpolator; 
  // jitter in analog reads, causes audible glitches
  wave = (pot_wave & 0x7f); // pot_wave & b00 0111 1111, 0..127
  osc.set_wave(bank, wave);
  the_osc.set_wave(bank, wave);
}

