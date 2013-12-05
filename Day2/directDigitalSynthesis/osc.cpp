#include "Osc.h"

void Osc::Init(){
  phase.value = 0;
  phase_increment.value = 0;
  parameter = 0;
  wave = 0;
  note = 69;
  detune = 0;
  Update();
}

void Osc::note_on(uint8_t _note){
  if(_note > 127){ 
    note = 127;
  } else {
    note = _note;
  }
}

void Osc::note_off(){
  note = 128;
  phase_increment.value = 0;
  sync();
}

void Osc::sync(){
  phase.value = 0;
}

void Osc::set_detune(int16_t _detune){
  // limit detune to +-12 semitones, one semitone is divided into 24 steps
  // range is +-288
  if(_detune > kDetuneAmount) {
    detune = kDetuneAmount;
  } else if(detune <= ~kDetuneAmount) {
    detune = ~kDetuneAmount + 1;
  } else {
    detune = _detune;
  }
}

//void Osc::pitch(uint16_t _pitch){
//  if(_pitch > kHighestNote){
//    pitch = kHighestNote;
//  } else if(_pitch < kLowestNote){
//    pitch = kLowestNote;
//  } else {
//    pitch = _pitch;
//  }
//  note = (pitch>>7) & 0x7f;
//  detune = pitch & 0x7f;
//}

void Osc::set_parameter(uint8_t _parameter){
  parameter = _parameter;
}

void Osc::set_wave(uint16_t _wave){
  uint8_t section = _wave>>2;   // 1024/128
  if(section <= 256){
    wave = RAMP;
  } else {
    wave = TRIANGLE;
  }
}
