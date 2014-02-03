#include "Osc.h"

void Osc::Init(){
  phase.value = 0;
  phase_increment.value = 0;
  wave_index = 0;
  wave_balance = 0;
  note = 69;
  detune = 0;
}

// set oscillation frequency to a midinote (0..127);
void Osc::set_note(uint8_t _note){
  if(_note > 127){ 
    note = 127;
  } else {
    note = _note;
  }
}

// hard sync the oscillator
void Osc::sync(){
  phase.value = 0;
}

// detune the current note in 1/16 steps per semitone, range is +-192
void Osc::set_detune(int16_t _detune){
  // limit detune to +-12 semitones, one semitone is divided into 16 steps
  // range is +-16
  if(_detune > kDetuneAmount) {
    detune = kDetuneAmount;
  } else if(_detune <= ~kDetuneAmount) {
    detune = ~kDetuneAmount + 1;
  } else {
    detune = _detune;
  }
}

// set wavetable by bank and interpolated wave, bank 0..7, wave 0..127 (uses linear interpolation)
void Osc::set_wave(uint8_t _bank, uint8_t _wave){
  uint8_t wave_select;
  
  if(_bank > kWavesBanks) {_bank = kWavesBanks;};
  if(_wave > 127) {_wave = 127;};
  
  wave_select = (_wave>>4);
  wave_balance = (_wave&0x0f)<<4; 
  wave_index = ((_bank * kWavesPerBank) + wave_select) * kWavetableLength;
}
