/*
This scripts generates our sample.h/.cc files used by the sample class
*/

(
q = q ? ();
q.tables = ();
q.functions = ();
q.buffers = ();
q.temp = ();
q.temp.samples = List.new();

q.samplerate = 16e6/(8*250); // 8kHz
);

/*
30720 Bytes available memory on the Arduino
we should leave around 4kb for the code
26kb for sampledata, 3.25s @ 8kHz
*/

/*
.resize depends on wslib Quark
*/

// convert a complete folder to our sample format
(
// read our folders and get filenames
q.buffers.path = PathName.new("".resolveRelative ++ "selectedbad");
q.buffers.files = q.buffers.path.files;

q.buffers.samples = q.buffers.files.collect({ |file| Buffer.read(s, file.fullPath)});
// q.buffers.samples[1].plot
q.buffers.arrays = List.new;
q.buffers.samples.do({|buffer| buffer.loadToFloatArray(0,-1, { |array| q.buffers.arrays.add(array); })});
q.buffers.arrays = q.buffers.arrays.collect({ |array| array.normalize(0,255).asInteger});
// q.buffers.arrays[0].plot

// don't forget to free the buffers
q.buffers.samples.do{|buf| buf.free};
// q.buffers.clear;
);

/*
wavetable C++ variable definitions
*/
(
q.tables.sample0 = [
	"lut_bad_01",
	"prog_uint8_t",
	q.buffers.arrays[0]
];
q.tables.sample1 = [
	"lut_bad_02",
	"prog_uint8_t",
	q.buffers.arrays[1]
];
q.tables.sample2 = [
	"lut_bad_03",
	"prog_uint8_t",
	q.buffers.arrays[2]
];
q.tables.sample3 = [
	"lut_bad_04",
	"prog_uint8_t",
	q.buffers.arrays[3]
];
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
		file.putString(format("#define % %\n",name.toUpper ++ "_SIZE", values.size));
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
q.functions[\createResources].value(q.tables, "samples");