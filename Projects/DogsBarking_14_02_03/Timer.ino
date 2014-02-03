

#if defined(TCCR2A)
ISR(TIMER2_COMPA_vect){
  (*sampleInterrupt)();
}
#elif defined(TCCR3A)
ISR(TIMER3_COMPA_vect){
  (*sampleInterrupt)();     
}
#endif

void setupSampleTimer(){
  #if defined(TCCR2A)
  // Arduino Uno, Nano
  // Timer2 for samplerate
  // prescaler of timer is set to 64 by wiring.c
  TCCR2B = (TCCR2B & 0xf8) | 0x03;
  // set mode of Timer 2 to CTC Capture
  TCCR2A = (TCCR2A & 0xfc) | 0x02;  
  // set Compare Match A value, can be altered for Samplerate reduction. Hooray Aliasing!
  OCR2A = kOCRRegister;      
  // enable Timer/Counter2 Output Compare Match A Interrupt Enable
  // triggers ISR(TIMER0_COMPA_vect) routine when TCNT0 = OCR0A, (TCNT0 = timer value) 
  TIMSK2 |= (1<<OCIE2A);
  #elif defined(TCCR3A)
  // Arduino Leonardo, Esplora
  // disbale PWM, enable CTC Mode, see datasheet p. 138
    TCCR3A = 0;
    TCCR3B = 0;
    TCCR3B |= (1<<WGM12);
    // prescaler 64 62.5kHz p. 139 
    TCCR3B = (TCCR3B & 0xf8) | 0x03;
    // set Compare Match A value, can be altered for Samplerate reduction. Hooray Aliasing!
    OCR3A = kOCRRegister; 
    // enable Timer/Counter3 Output Compare Match A Interrupt Enable
    TIMSK3 |= (1<<OCIE3A); 
  #endif
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
