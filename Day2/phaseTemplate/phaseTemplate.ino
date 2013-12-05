void setup(){
  // fast PWM Mode on Timer1, 62.5kHz
  setupPwmTimer();
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);  // OUT2 on Esplora  
  pinMode(13, OUTPUT);
}

void loop(){
  digitalWrite(13, HIGH);
  
  digitalWrite(13, LOW);
  delayMicroseconds(100);
}


void sampleInterrupt(){}

