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
  analogWrite(10, value);
//  analogWrite(11, value);
  digitalWrite(13, LOW);
  delayMicroseconds(100);
}

void setupPwmTimer(){
  // Timer1 for our PWM output
  // clear Timer1 Modes and Prescaler
  TCCR1A = 0;
  TCCR1B = 0;
  // set prescaler of Timer1 to 1, => PWM Frequency on Pin 9/10 is 62500Hz (16MHz/256)
  TCCR1B = (TCCR1B & 0xf8) | 0x01;  
  // enable PWM on Pin 9 (COM1A1) and Pin 10 (COM1B1)
  TCCR1A |= (1<<COM1A1) | (1<<COM1B1);

  #if defined(COM1C1)
  // for atmega32u4, Esplora OUT2
  TCCR1A |= (1<<COM1C1);
  #endif

  // Fast PWM 8bit, see p. 138 atmega328 datasheet
  TCCR1A |= (1<<WGM10);
  TCCR1B |= (1<<WGM12);  
}
