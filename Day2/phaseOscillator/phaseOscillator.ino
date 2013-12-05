#include "resources.h"

word phase;
word phase_increment;
word value;

void setup(){
  setupPwmTimer();
  // our phase counter
  phase = 0;
  // has a useful resolution of 1..32768
  // to calculate phase increment: freq * max_phase/samplerate
  phase_increment = 3000; 
  pinMode(10, OUTPUT);
  // OUT2 on Esplora  
  pinMode(11, OUTPUT);
}

void loop(){
  digitalWrite(13, HIGH);
  // increment phase by phase_increment  
  phase += phase_increment;
  // get the most significant 8 bits
  value = (phase>>8);
  value = pgm_read_byte(lut_sin + value);
  analogWrite(10, value);
//  analogWrite(11, value); // OUT2 on Esplora 
  digitalWrite(13, LOW);
  delayMicroseconds(100);
}

void sampleInterrupt(){}

