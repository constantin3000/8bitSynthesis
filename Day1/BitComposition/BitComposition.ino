unsigned long counter; // 32bit, 0..4294967296, integer
byte signal;
  
void setup(){
  counter = 0;
  pinMode(11, OUTPUT); // setup Pin 11 as Output
}

void loop(){
  //increase counter by 1 in each cycle
  counter++;
  //more complex bit operations for tone and amp modulation
  signal = counter*((counter>>12|counter>>8)&63&counter>>4);
  //select bit to play
  signal = (signal>>4)&1;
  //write signal (0 or 1) to pin 11
  digitalWrite(11, signal);
  //higher Samplerate ~8kHz
  delayMicroseconds(100);
}



