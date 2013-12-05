(
q = q ? ();
q.tables = ();
q.functions = ();
q.buffers = ();

q.samplerate = 16e6/416;
);

(
q.functions.createHeader = { |tables, filename = "resources"|
	var file;

	file = File.new("".resolveRelative ++ filename ++".h", "w");
	file.putString("#include <avr/pgmspace.h>\n\n");

	tables.do{ |table|
		var type, name, values;
		name = table[0];
		type = table[1];
		values = table[2];

		file.putString(format("extern const % %[] PROGMEM;\n", type, name));
	};
	file.close;
};

q.functions.createSource = { |tables, filename = "resources.cpp"|
	var file;

	file = File.new("".resolveRelative ++ filename ++".cpp", "w");
	file.putString(format("#include \"%.h\"\n\n", filename));

	tables.do{ |table|
		var type, name, values;
		name = table[0];
		type = table[1];
		values = table[2];

		file.putString(format("const % %[] PROGMEM = {\n", type, name));
		values.clump(8).collect{ |row|
			row.do{ |elem|
				file.putString(elem.asString.padLeft(6) ++ ", ");
			};
			file.putString("\n");
		};
		file.putString("};\n\n");
	};
	file.close;
};

q.functions.createResources = { |tables, filename = "resources"|
	q.functions[\createHeader].value(tables, filename);
	q.functions[\createSource].value(tables, filename);
};
);

/*
Our wave and utility tables
*/
(
// Sine wavetable
q.tables.sin = [
	"lut_sin",
	"prog_uint8_t",
	Signal.sineFill(256, [1]).linlin(-1,1,0,255).asInteger
];
/*
q.tables.sin[2].plot
*/

// Harmonic sine
q.tables.rsin = [
	"lut_rsin",
	"prog_uint8_t",
	Signal.sineFill(256, 1.0/[1,2,3,4].scramble).linlin(-1,1,0,255).asInteger
];
/*
q.tables.rsin[2].plot
*/

// Triangle
q.tables.tri = [
	"lut_tri",
	"prog_uint8_t",
	Signal.newClear(256).waveFill({ |x, i| x.fold(-1, 1) }, 2, -2).linlin(-1,1,0,255).asInteger
];
/*
q.tables.tri[2].plot
*/
);

(
// to calculate phase increment: freq * max_phase/samplerate
q.tables.notes = [
	"lut_notes",
	"prog_uint32_t",
	Array.fill(128, { |note| note.midicps * 65536.0/q.samplerate * 65536.0 }).asInteger
];
);
(
/*
Process single cycle waveform wav files
"http://www.adventurekid.se/akrt/waveforms/adventure-kid-waveforms/"
*/
(
SynthDef(\sampleConversion, { |in, out|
	var phasor, input;
	phasor = Line.ar(0, 1, 0.1, doneAction: 2);
	input = BufRd.ar(1, in, phasor * BufFrames.kr(in), loop: 0, interpolation: 4);
	BufWr.ar(input, out, phasor * BufFrames.kr(out), loop:0);
}).add;
);

q.buffers.organ = Buffer.read(s, "".resolveRelative ++ "AKWF/AKWF_eorgan/AKWF_eorgan_0002.wav");
q.buffers.organ256 = Buffer.alloc(s, 256, 1);

x = Synth(\sampleConversion, [\in, q.buffers.organ.bufnum, \out, q.buffers.organ256.bufnum]);

q.buffers.organ256.loadToFloatArray(0, -1, {|array| q.buffers.temp = array});
q.buffers.temp[0] = 0;
q.buffers.temp[-1] = 0;
/*
q.buffers.temp.plot;
*/
q.tables.wave0 = [
	"lut_wave0",
	"prog_uint8_t",
	q.buffers.temp.linlin(-1,1,0,255).asInteger
];
q.buffers.temp.size
/*
q.tables.wave0[2].plot;
*/

/*
// Listen to single cycle waveform
(
Ndef(\Wavetable, { |freq = 440, amp = 0.1|
    var phasor;
phasor = Phasor.ar(0, freq/SampleRate.ir, 0, BufFrames.kr(b.bufnum));
	BufRd.ar(1, b.bufnum, phasor)!2;
}).play;
NdefMixer(s);
);
*/
);


/*
Create header and source files
*/
q.functions[\createResources].value(q.tables);
69.midicps