b = Buffer.read(s, "".resolveRelative ++"selected/barking_01.aiff");
c = Buffer.read(s, "".resolveRelative ++"selected/barking_02.aiff");
d = Buffer.read(s, "".resolveRelative ++"selected/barking_03.aiff");
e = Buffer.read(s, "".resolveRelative ++"selected/barking_04.aiff");

// print samplerate of Buffer
b.sampleRate

f = Buffer.alloc(s, 256);
g = Buffer.alloc(s, 256);
f.sendCollection(Signal.sineFill(256, 1/[1]));
// no change to signal;
f.sendCollection(Signal.chebyFill(256, [1]));
// differetn overtone spectrum

// mix fixed value
z = [-0.9, 0.4, -0.3, 1.0, 0.8].scramble;
// random values
z = {rrand(-1.0, 1.0)}!8;
f.sendCollection(Signal.chebyFill(256, z));

f.sendCollection(Signal.chebyFill(256, [0,1,0,1]));
f.plot;

s.freqscope

f.plot;
f.free

(
Ndef(\Playback, { |select = 0, rate = 0.1667, amp = 0.1|
	var phase, trig, buffers, bufFrames, bufNums, snd;

	buffers = [b,c,d,e];
	bufFrames = buffers.collect{|buf| buf.numFrames};
	bufNums = buffers.collect{|buf| buf.bufnum};

	// phase = Phasor.ar(0, BufRateScale.kr(b) * rate, 0, bufFrames);
	// buffers = BufRd.ar(1, bufNums, phase);

	trig = Impulse.kr(0.5);
	buffers = PlayBuf.ar(1, bufNums, BufRateScale.kr(b) * rate, trig);
	// plays sample 0 then 1, then 2, then 3
	// select = Demand.kr(trig, 0, Dseq([0,1,2,3], inf));
	// randomly choose a sample, infinite repeats
	select = Demand.kr(trig, 0, Dxrand([0,1,2,3], inf));
	snd = SelectX.ar(select, buffers);

	// simulation of Arduino output, 8kHz, 8bit
	snd = Decimator.ar(snd, 8000, 8);
	snd!2 * amp;
});

Ndef(\Playback).addSpec(\rate, [0.1, 10, \exp]);
Ndef(\Playback).addSpec(\select, [0, 1]);

)

(
Ndef(\Wave, { |amp = 0.1|
	var in, phase;
	in = Ndef(\Playback).ar(1);
	phase = in.linlin(-1, 1, 0, 256);
	BufRd.ar(1, f.bufnum, phase)!2 * amp;
});
);

NdefMixer(s);

{rrand(0,10)}!8
