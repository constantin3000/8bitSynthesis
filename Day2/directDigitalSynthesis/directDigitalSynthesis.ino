#include "osc.h"
byte flag = 4;
word scale = 36<<4; //start at c3

Osc osc;

void sampleInterrupt(){
  digitalWrite(flag, HIGH);
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
  digitalWrite(flag, LOW);     
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
  delay(10);  
}

// a simple Frequency sweep
void NoteTask(){
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
  scale = 36<<4; //start at c3
  }
}

