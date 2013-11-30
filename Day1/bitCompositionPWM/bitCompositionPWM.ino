unsigned long counter;
word value;
unsigned long t;

void setup(){
  counter = 0;
  // changes the PWM frequency to 62.5kHz to be above our hearing threshold
  // currently only works for Arduino Uno/Duemilanove/Nano
  setupFastPwm();
  pinMode(11, OUTPUT);
}

void loop(){
  // now uses a function with our counter as argument to generate each sample value
  value = generate(counter++);
  // write our current sample to PWM Pin 11
  analogWrite(11, value);
  delayMicroseconds(100);
}

// this is our composition function, the counter is passed as argument and the current value is returned by the function
// unquote for different compositions
static unsigned long generate(unsigned long t){
//  return  t * ((t>>12|t>>8)&63&t>>4);
//  return t*((t>>9|t>>13)&25&t>>6);
//  return t*((t>>3|t>>7)&3&t>>12);
  return t*((t>>12|t>>13)&127&t>>8);
//  return (t>>6|t|t>>(t>>16))*10+((t>>11)&7);
//  return (t*(t>>8*(t>>15|t>>8)&(20|(t>>19)*5>>t|t>>3)));
}

void setupFastPwm(){
  #if defined (__AVR_ATmega168__) || defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__) || defined (__AVR_ATmega328P__) || (__AVR_ATmega1280__)
  // set Fast PWM Mode on Pin 3 and 11
  TCCR2A = (TCCR2A & 0xfc) | 0x03;
  // set prescaler to 1
  TCCR2B = (TCCR2B & 0xf8) | 0x01;
  #endif
}
