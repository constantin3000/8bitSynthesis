byte counter; // 8bit, 0..255, integer
byte freq, mod, signal;

void setup(){
  counter = 0;
  pinMode(11, OUTPUT);
}

void loop(){
  //increase counter by 1 in each cycle
  counter++;
  //read second bit of counter, should give us a frequency of 1/4 of our samplerate
  freq = (counter>>1)&1;
  //read 7bit of counter, divide sample freq by 64 for amplitude modulation
  mod = (counter>>6)&1;
  //mix both signals together
  signal = freq & mod;
  //write signal (0 or 1) to pin 11
  digitalWrite(11, signal);
  delayMicroseconds(1000);
}
