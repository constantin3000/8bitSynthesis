/*
asembler implementations of math functions from avrlib op.h, Oliver Gillet
https://github.com/pichenettes/avril
*/

#include <avr/pgmspace.h>
#include <inttypes.h>

enum PhaseWords {
  FRACTIONAL,
  INTEGRAL
};

enum PhaseBytes {
  LOW_FRACTIONAL,
  HIGH_FRACTIONAL,
  LOW_INTEGRAL,
  HIGH_INTEGRAL
};

typedef union {
  uint16_t value;
  uint8_t bytes[2];
} Word;

typedef union {
  uint32_t value;
  uint16_t words[2];
  uint8_t bytes[4];
} LongWord;

/*
static inline uint8_t U8Mix(uint8_t a, uint8_t b, uint8_t balance) {
  return a * (255 - balance) + b * balance >> 8;
}
*/
static inline uint8_t U8Mix(uint8_t a, uint8_t b, uint8_t balance) {
  Word sum;
  asm(
    "mul %3, %2"      "\n\t"  // b * balance
    "movw %A0, r0"    "\n\t"  // to sum
    "com %2"          "\n\t"  // 255 - balance
    "mul %1, %2"      "\n\t"  // a * (255 - balance)
    "com %2"          "\n\t"  // reset balance to its previous value
    "add %A0, r0"     "\n\t"  // add to sum L
    "adc %B0, r1"     "\n\t"  // add to sum H
    "eor r1, r1"      "\n\t"  // reset r1 after multiplication
    : "&=r" (sum)
    : "a" (a), "a" (balance), "a" (b)
    );
  return sum.bytes[1];
}

/*
static inline uint8_t U8Mix(uint8_t a, uint8_t b, uint8_t gain_a, uint8_t gain_b) {
  return a * gain_a + b * gain_b >> 8;
}
*/
static inline uint8_t U8Mix(
    uint8_t a, uint8_t b,
    uint8_t gain_a, uint8_t gain_b) {
  Word sum;
  asm(
    "mul %3, %4"      "\n\t"  // b * gain_b
    "movw %A0, r0"    "\n\t"  // to sum
    "mul %1, %2"      "\n\t"  // a * gain_a
    "add %A0, r0"     "\n\t"  // add to sum L
    "adc %B0, r1"     "\n\t"  // add to sum H
    "eor r1, r1"      "\n\t"  // reset r1 after multiplication
    : "&=r" (sum)
    : "a" (a), "a" (gain_a), "a" (b), "a" (gain_b)
    );
  return sum.bytes[1];
}

/*
static inline uint16_t U8MixU16(uint8_t a, uint8_t b, uint8_t balance) {
  return a * (255 - balance) + b * balance;
}
*/
static inline uint16_t U8MixU16(uint8_t a, uint8_t b, uint8_t balance) {
  Word sum;
  asm(
    "mul %3, %2"      "\n\t"  // b * balance
    "movw %A0, r0"    "\n\t"  // to sum
    "com %2"          "\n\t"  // 255 - balance
    "mul %1, %2"      "\n\t"  // a * (255 - balance)
    "com %2"          "\n\t"  // reset balance to its previous value
    "add %A0, r0"     "\n\t"  // add to sum L
    "adc %B0, r1"     "\n\t"  // add to sum H
    "eor r1, r1"      "\n\t"  // reset r1 after multiplication
    : "&=r" (sum)
    : "a" (a), "a" (balance), "a" (b)
    );
  return sum.value;
}

/*
static inline uint8_t InterpolateSample(
    const prog_uint8_t* table,
    uint16_t phase) {
  return U8Mix(
      pgm_read_byte(table + (phase >> 8)),
      pgm_read_byte(1 + table + (phase >> 8)),
      phase & 0xff);
}
*/

static inline uint8_t InterpolateSample(
    const prog_uint8_t* table,
    uint16_t phase) {
  uint8_t result;
  uint8_t work;
  asm(
    "movw r30, %A2"           "\n\t"  // copy base address to r30:r31
    "add r30, %B3"            "\n\t"  // increment table address by phaseH
    "adc r31, r1"             "\n\t"  // just carry
    "mov %1, %A3"             "\n\t"  // move phaseL to working register
    "lpm %0, z+"              "\n\t"  // load sample[n]
    "lpm r1, z+"              "\n\t"  // load sample[n+1]
    "mul %1, r1"              "\n\t"  // multiply second sample by phaseL
    "movw r30, r0"            "\n\t"  // result to accumulator
    "com %1"                  "\n\t"  // 255 - phaseL -> phaseL
    "mul %1, %0"              "\n\t"  // multiply first sample by phaseL
    "add r30, r0"             "\n\t"  // accumulate L
    "adc r31, r1"             "\n\t"  // accumulate H
    "eor r1, r1"              "\n\t"  // reset r1 after multiplication
    "mov %0, r31"             "\n\t"  // use sum H as output
    : "=r" (result), "=r" (work)
    : "r" (table), "r" (phase)
    : "r30", "r31"
  );
  return result;
}

static inline uint8_t InterpolateTwoTables(
    const prog_uint8_t* table_a, const prog_uint8_t* table_b,
    uint16_t phase, uint8_t balance) {
  return U8Mix(
      InterpolateSample(table_a, phase),
      InterpolateSample(table_b, phase),
      balance);
}
