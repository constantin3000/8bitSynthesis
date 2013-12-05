word phase;
word phase_increment;
word value;

void setup(){
  // set PWM Frequency on Pin 3/11 to 62,5kHz
  #if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__)
  // set Fast PWM Mode on Pin 3 and 11
  TCCR2A = (TCCR2A & 0xfc) | 0x03;
  // set prescaler to 1
  TCCR2B = (TCCR2B & 0xf8) | 0x01;
  #endif
  // our phase counter
  phase = 0;
  // has a useful resolution of 1..32768
  // to calculate phase increment: freq * max_phase/samplerate
  phase_increment = 3000; 
  pinMode(11, OUTPUT);
}

void loop(){
  digitalWrite(13, HIGH);
  // increment phase by phase_increment
  phase += phase_increment;
  // get the most significant 8 bits
  value = (phase>>8);
  analogWrite(11, value);
  digitalWrite(13, LOW);
  delayMicroseconds(100);
}
