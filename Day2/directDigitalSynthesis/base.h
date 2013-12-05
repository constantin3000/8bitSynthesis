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
