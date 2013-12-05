#include "Osc.h"

void Osc::Init(){
  phase.value = 0;
  phase_increment.value = 0;
  pulse_width = 0;
  parameter = 0;
  wave = 0;
}

void Osc::set_rate(uint16_t _rate){
  rate = _rate>>2;
  phase_increment.value = pgm_read_dword(lut_lfo_increments + rate);
}

void Osc::set_note(uint8_t _note){
  if(_note > 127){ _note = 127;}
  phase_increment.value = pgm_read_dword(lut_notes + _note);
}

void Osc::set_parameter(uint16_t _parameter){
  parameter = _parameter>>2;
  if(parameter < 4) {pulse_width = 4;}
  if(parameter > 251) {pulse_width = 251;}
}

void Osc::set_wave(uint16_t _wave){
  uint8_t section = _wave>>2;   // 1024/128
  if(section <= 256){
    wave = RAMP;
  } else {
    wave = TRIANGLE;
  }
}
