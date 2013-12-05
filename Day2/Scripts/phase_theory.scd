
// bit toggles with increasing counter
65.collect{|counter| [(counter>>0)&1, (counter>>1)&1, (counter>>2)&1, (counter>>3)&1, (counter>>4)&1, (counter>>5)&1]}.flop.plot.plotMode = \steps
// value wrapping with increasing counter, in this case like modulo %
65.collect{|counter| [counter&1, counter&3, counter&7, counter&15, counter&31, counter&63]}.flop.plot.plotMode = \steps

/*
top value is variable as our increment stays constant
Let's try:
top values stay constant, increment variable
*/

(
var increments = 64 * [1/2, 1/4, 1/8, 1/16, 1/32, 1/64];
var phases = 0.dup(6);
256.collect{|counter| phases = (phases + increments)%64}.flop.plot;
)





// deviation of frequency with 8kHz Samplerate, 16bit phase
a = (8000.0/65535) * (440.0*65535/8000).asInteger
// in cents
1200 * log(a/440.0)

// deviation of frequency with 40kHz Samplerate, 16bit phase
a = (40000/65535)*(440.0*65535/40000).asInteger
// in cents
1200 * log(a/440.0)
