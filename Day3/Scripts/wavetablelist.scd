/*
This scripts generates our wavettable.h/.cc files used by the osc class

The oscillator uses a specific wavetable format:
- one wavetable is 257 values long
- the wavetables are arranged in banks, each containig 8 waves
- a total number of 64 waves organised in 8 banks
*/

(
q = q ? ();
q.tables = ();
q.functions = ();
q.buffers = ();
q.temp = ();
q.temp.waves = List.new();

q.samplerate = 16e6/512;
q.tableSize = 257;
);

//sine harmonics
(
q.temp.waves.add(Array.fill(8, { |i| Signal.sineFill(q.tableSize, 1.0/(1..(i*3)+1).scramble) }).linlin(-1,1,0,255).asInteger.flat);
);

q.temp.waves.asArray.plot

(
/*
Single cycle waveform wav files
http://www.adventurekid.se/akrt/waveforms/adventure-kid-waveforms/

download this zip and unpack it into the same directory as this script
http://www.adventurekid.se/AKRTfiles/AKWF/AKWF.zip
a visual guide to the included waveforms
http://www.adventurekid.se/AKRTfiles/AKWF/view/waveforms_index.html

.resize depends on wslib Quark
*/

// convert a complete folder to our wavetable format
// check waves/AKWFWaves.txt, to see which wavetables have been used
(
// read our folders and get filenames
q.buffers.path = PathName.new("".resolveRelative ++ "waves");
q.buffers.files = q.buffers.path.folders.collect{|folder| folder.files.postln}.flat;

q.buffers.akwf = q.buffers.files.collect({ |file| Buffer.read(s, file.fullPath)});
q.buffers.arrays = List.new;
q.buffers.akwf.do({|buffer| buffer.loadToFloatArray(0,-1, { |array| q.buffers.arrays.add(array); })});

// resize wavetables
q.buffers.arrays = q.buffers.arrays.collect({ |array| array.linlin(-1,1, 0,255).resize(q.tableSize).asInteger});

q.temp.waves.add(q.buffers.arrays.flat);

// don't forget to free the buffers
q.buffers.akwf.do{|buf| buf.free};
q.buffers.clear;
);

// our wavetable
q.temp.waves.flat.asArray.size;
q.temp.waves.flat.asArray.plot;
/*
wavetable C++ variable definitions
*/
(
q.tables.waves = [
	"lut_waves",
	"prog_uint8_t",
	q.temp.waves.flat
]
);

/*
Create header and source files
*/
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

q.functions.createSource = { |tables, filename = "resources"|
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

// create header and source files
q.functions[\createResources].value(q.tables, "wavetables");