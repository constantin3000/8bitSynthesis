#include "resources.h"

word phase;
word phase_increment;
word sample;

void setup(){
  // initializes and enables our sample timer interrupt
  setupSampleTimer();
  // fast PWM Mode on Timer1, 62.5kHz
  // DAC Digital Analog Converter, 1 bit DAC
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
}

void sampleInterrupt(){
  digitalWrite(13, HIGH);
  // increment phase by phase_increment  
  phase += phase_increment;
  // get the most significant 8 bits
  sample = (phase>>8);
  // read out value from our sine look up table
  // lut_sin acts as a pointer to the position in the program memory
  // with + sample we get the nth value from that position on 
  sample = pgm_read_byte(lut_sin + sample);
  analogWrite(10, sample);
  // uncomment on Esplora, directly writes the value to the PWM Register
  // circumvents the analogWrite functionOCR1C = sample;  
  //  OCR1C = sample;  // OUT2 on Esplora 
  digitalWrite(13, LOW);
}

