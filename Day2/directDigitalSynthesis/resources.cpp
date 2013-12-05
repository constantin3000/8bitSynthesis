#include "resources.h"

const prog_uint32_t lut_lfo_increments[] PROGMEM = {
          2147483,    2200392,    2254604,    2310152,    2367068,
          2425387,    2485143,    2546370,    2609107,    2673389,
          2739254,    2806743,    2875894,    2946749,    3019350,
          3093739,    3169961,    3248061,    3328085,    3410081,
          3494097,    3580183,    3668390,    3758770,    3851377,
          3946265,    4043491,    4143113,    4245189,    4349780,
          4456948,    4566756,    4679269,    4794555,    4912681,
          5033717,    5157736,    5284810,    5415014,    5548427,
          5685126,    5825194,    5968712,    6115766,    6266444,
          6420834,    6579027,    6741118,    6907202,    7077379,
          7251748,    7430413,    7613480,    7801057,    7993256,
          8190190,    8391976,    8598734,    8810585,    9027656,
          9250076,    9477975,    9711488,    9950756,   10195918,
         10447120,   10704511,   10968244,   11238474,   11515362,
         11799072,   12089772,   12387634,   12692835,   13005555,
         13325980,   13654299,   13990707,   14335404,   14688592,
         15050483,   15421290,   15801232,   16190535,   16589430,
         16998153,   17416945,   17846056,   18285738,   18736254,
         19197869,   19670857,   20155498,   20652079,   21160896,
         21682248,   22216445,   22763803,   23324647,   23899308,
         24488128,   25091455,   25709646,   26343069,   26992097,
         27657115,   28338518,   29036709,   29752102,   30485120,
         31236198,   32005780,   32794324,   33602295,   34430172,
         35278446,   36147620,   37038208,   37950738,   38885750,
         39843799,   40825452,   41831290,   42861909,   43917921,
         44999950,   46108637,   47244640,   48480732,   49749166,
         51050786,   52386461,   53757082,   55163564,   56606845,
         58087887,   59607678,   61167233,   62767592,   64409822,
         66095018,   67824306,   69598837,   71419798,   73288401,
         75205893,   77173554,   79192697,   81264667,   83390848,
         85572658,   87811552,   90109023,   92466604,   94885869,
         97368430,   99915945,  102530111,  105212674,  107965422,
        110790192,  113688869,  116663386,  119715726,  122847928,
        126062079,  129360324,  132744863,  136217954,  139781914,
        143439120,  147192012,  151043093,  154994933,  159050167,
        163211501,  167481711,  171863645,  176360226,  180974455,
        185709408,  190568245,  195554208,  200670621,  205920898,
        211308542,  216837146,  222510399,  228332085,  234306087,
        240436391,  246727086,  253182369,  259806546,  266604035,
        273579372,  280737209,  288082321,  295619608,  303354098,
        311290950,  319435460,  327793060,  336369325,  345169977,
        354200886,  363468076,  372977730,  382736192,  392749970,
        403025746,  413570373,  424390886,  435494504,  446888633,
        458580874,  470579027,  482891095,  495525292,  508490046,
        521794005,  535446044,  549455271,  563831030,  578582912,
        593720756,  609254663,  625194993,  641552380,  658337736,
        675562259,  693237438,  711375065,  729987238,  749086374,
        768685213,  788796829,  809434639,  830612409,  852344267,
        874644710,  897528615,  921011245,  945108268,  969835756,
        995210206, 1021248545, 1047968141, 1075386820, 1103522872,
       1132395066, 1162022662, 1192425425, 1223623635, 1255638104,
       1288490188
};

const prog_uint32_t lut_notes[] PROGMEM = {
912984, 967273, 1024790, 1085727, 1150288, 1218688, 1291155, 1367931, 
1449272, 1535450, 1626753, 1723485, 1825969, 1934546, 2049580, 2171455, 
2300576, 2437376, 2582310, 2735862, 2898545, 3070901, 3253506, 3446970, 
3651938, 3869093, 4099161, 4342910, 4601153, 4874752, 5164620, 5471724, 
5797090, 6141803, 6507013, 6893941, 7303876, 7738187, 8198323, 8685821, 
9202307, 9749504, 10329240, 10943449, 11594180, 12283606, 13014027, 13787882, 
14607752, 15476374, 16396647, 17371642, 18404614, 19499009, 20658481, 21886898, 
23188361, 24567212, 26028055, 27575764, 29215504, 30952748, 32793294, 34743285, 
36809228, 38998019, 41316962, 43773797, 46376722, 49134425, 52056110, 55151528, 
58431008, 61905497, 65586589, 69486571, 73618457, 77996039, 82633925, 87547594, 
92753444, 98268851, 104112221, 110303056, 116862017, 123810994, 131173179, 138973142, 
147236915, 155992078, 165267850, 175095188, 185506889, 196537703, 208224443, 220606113, 
233724035, 247621989, 262346359, 277946285, 294473831, 311984157, 330535700, 350190376, 
371013779, 393075406, 416448886, 441212226, 467448070, 495243979, 524692718, 555892571, 
588947663, 623968314, 661071401, 700380752, 742027559, 786150813, 832897773, 882424452, 
934896141, 990487958, 1049385437, 1111785143, 1177895327, 1247936628, 1322142802, 1400761504,
};

const prog_uint8_t lut_tri[] PROGMEM = {
         0,   2,   4,   6,   8,  10,  12,  14,  16,  18,  20,  22,  24,
        26,  28,  30,  32,  34,  36,  38,  40,  42,  44,  46,  48,  50,
        52,  54,  56,  58,  60,  62,  64,  66,  68,  70,  72,  74,  76,
        78,  80,  82,  84,  86,  88,  90,  92,  94,  96,  98, 100, 102,
       104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126, 128,
       130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154,
       156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180,
       182, 184, 186, 188, 190, 192, 194, 196, 198, 200, 202, 204, 206,
       208, 210, 212, 214, 216, 218, 220, 222, 224, 226, 228, 230, 232,
       234, 236, 238, 240, 242, 244, 246, 248, 250, 252, 255, 255, 252,
       250, 248, 246, 244, 242, 240, 238, 236, 234, 232, 230, 228, 226,
       224, 222, 220, 218, 216, 214, 212, 210, 208, 206, 204, 202, 200,
       198, 196, 194, 192, 190, 188, 186, 184, 182, 180, 178, 176, 174,
       172, 170, 168, 166, 164, 162, 160, 158, 156, 154, 152, 150, 148,
       146, 144, 142, 140, 138, 136, 134, 132, 130, 128, 126, 124, 122,
       120, 118, 116, 114, 112, 110, 108, 106, 104, 102, 100,  98,  96,
        94,  92,  90,  88,  86,  84,  82,  80,  78,  76,  74,  72,  70,
        68,  66,  64,  62,  60,  58,  56,  54,  52,  50,  48,  46,  44,
        42,  40,  38,  36,  34,  32,  30,  28,  26,  24,  22,  20,  18,
        16,  14,  12,  10,   8,   6,   4,   2,   0
};

const prog_uint8_t lut_ramp[] PROGMEM = {
         0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,
        13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,
        26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
        39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,
        52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,
        65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,
        78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,
        91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103,
       104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
       117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
       130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
       143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
       156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168,
       169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
       182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
       195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
       208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220,
       221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
       234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246,
       247, 248, 249, 250, 251, 252, 253, 254, 255
};

const prog_uint8_t lut_sin[] PROGMEM = {
       128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 161, 164,
       167, 170, 173, 176, 179, 182, 185, 187, 190, 193, 195, 198, 201,
       203, 206, 208, 210, 213, 215, 217, 219, 222, 224, 226, 228, 230,
       231, 233, 235, 236, 238, 240, 241, 242, 244, 245, 246, 247, 248,
       249, 250, 251, 251, 252, 253, 253, 254, 254, 254, 254, 254, 255,
       254, 254, 254, 254, 254, 253, 253, 252, 251, 251, 250, 249, 248,
       247, 246, 245, 244, 242, 241, 240, 238, 236, 235, 233, 231, 230,
       228, 226, 224, 222, 219, 217, 215, 213, 210, 208, 206, 203, 201,
       198, 195, 193, 190, 187, 185, 182, 179, 176, 173, 170, 167, 164,
       161, 158, 155, 152, 149, 146, 143, 140, 137, 134, 131, 128, 124,
       121, 118, 115, 112, 109, 106, 103, 100,  97,  94,  91,  88,  85,
        82,  79,  76,  73,  70,  68,  65,  62,  60,  57,  54,  52,  49,
        47,  45,  42,  40,  38,  36,  33,  31,  29,  27,  25,  24,  22,
        20,  19,  17,  15,  14,  13,  11,  10,   9,   8,   7,   6,   5,
         4,   4,   3,   2,   2,   1,   1,   1,   1,   1,   1,   1,   1,
         1,   1,   1,   2,   2,   3,   4,   4,   5,   6,   7,   8,   9,
        10,  11,  13,  14,  15,  17,  19,  20,  22,  24,  25,  27,  29,
        31,  33,  36,  38,  40,  42,  45,  47,  49,  52,  54,  57,  60,
        62,  65,  68,  70,  73,  76,  79,  82,  85,  88,  91,  94,  97,
       100, 103, 106, 109, 112, 115, 118, 121, 124, 127
};
