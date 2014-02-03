(
q = q ? ();
q.tables = ();
q.functions = ();
q.buffers = ();
q.temp = ();

q.samplerate = 16e6/2000;
q.tableSize = 257;
q.lutSize = 256;
);


/*
Our wave and utility tables
*/
(
// Sine wavetable
q.tables.sin = [
	"lut_sin",
	"prog_uint8_t",
	Signal.sineFill(q.tableSize, [1]).linlin(-1,1,0,255).asInteger
];
/*
q.tables.sin[2].plot
*/

// Triangle
q.tables.tri = [
	"lut_tri",
	"prog_uint8_t",
	Signal.newClear(q.tableSize).waveFill({ |x, i| x.fold(-1, 1) }, 2, -2).linlin(-1,1,0,255).asInteger
];
/*
q.tables.tri[2].plot
*/

q.tables.ramp = [
	"lut_ramp",
	"prog_uint8_t",
	Signal.newClear(q.tableSize).waveFill({ |x| x }, 0, 1.0).linlin(0,1,0,256).asInteger
];
/*
q.tables.tri[2].plot
*/
);

//sine harmonics
(
q.tables.sineHarm = [
	"lut_sineHarm",
	"prog_uint8_t",
	Array.fill(8, { |i| Signal.sineFill(q.tableSize, 1.0/(1..(i*3)+1).scramble) }).linlin(-1,1,0,255).asInteger.flat.plot;
];
);



/*
Impulserate lookup table
0.2 Hz to 2 Hz
*/
(
q.temp.lowRate = 0.2;
q.temp.midRate = 1.0;
q.temp.highRate = 2.0;


q.temp.lowerRate = Array.fill(q.lutSize/2, { |i|
	i.linlin(0, q.lutSize/2, log(q.temp.lowRate), log(q.temp.midRate)).exp
});

q.temp.upperRate = Array.fill(q.lutSize/2, { |i|
	i.linlin(0, q.lutSize/2, log(q.temp.midRate), log(q.temp.highRate)).exp
});

q.temp.impulseRate = [q.temp.lowerRate, q.temp.upperRate.keep(-255)].flat;
q.temp.impulseIncrements = q.temp.impulseRate * 65536.0/q.samplerate * 65536.0;

// midi notes to phase increment
q.tables.rates = [
	"lut_rate_increments",
	"prog_uint32_t",
	q.temp.impulseIncrements.asInteger
];
)

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

// create header and source files
q.functions[\createResources].value(q.tables, "resources");