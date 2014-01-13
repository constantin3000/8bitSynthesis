#ifndef OSC_H
#define OSC_H

#include "base.h"
#include "resources.h"
#include "wavetables.h"

  const uint8_t kDetunePerSemitone = 16; 
  const uint16_t kLowestNote       = 0 * 16;
  const uint16_t kHighestNote      = 128 * 16;
  const uint16_t kPitchTableStart  = 116 * 16;
  const int16_t kDetuneAmount      = 12 * 16;
  const uint16_t kOctave           = 12 * 16;
  
  const uint8_t kWavesBanks        = 8;
  const uint8_t kWavesPerBank      = 8;
  const uint16_t kWavetableLength  = 257;


class Osc{
  
  public:
    Osc(){};
    
    void Init();
    
    inline uint8_t Render(){
      uint8_t value_a, value_b, sample; 
      
      const prog_uint8_t* wave_1 = lut_waves + wave_index;
      const prog_uint8_t* wave_2 = lut_waves + wave_index + kWavetableLength;
  
      phase.value += phase_increment.value;
      the_overflow = (phase.value < phase_increment.value);
      sample = InterpolateTwoTables(wave_1, wave_2, phase.words[INTEGRAL], wave_balance);           
      return sample;
    }
    
    inline void Update(){
      uint8_t octave_shift = 0;
      int16_t pitch, pitch_index;
      LongWord increment, increment_next;
      
      pitch = note<<4;
      pitch += detune;
      
      while(pitch >= kHighestNote){
        pitch -= kOctave;
      }
      
      pitch_index = pitch;
      pitch_index -= kPitchTableStart;
      
      while(pitch_index < 0){
        pitch_index += kOctave;
        ++octave_shift;
      }
      
      increment.words[INTEGRAL] = pgm_read_word(lut_pitch_increments + pitch_index);

      while(octave_shift--){
        increment.value = increment.value>>1;
      } 
      
      phase_increment.value = increment.value;
    }
    
    void sync();
    
    void set_note(uint8_t _note); // 0..127
    void set_detune(int16_t _detune);
    void set_wave(uint8_t _bank, uint8_t _wave);
    uint8_t gimme_overflow() {
      if(the_overflow == 1) {
        the_overflow = 0;
        return 1;
      } else {
        return 0;
      }
    }
    
  private:
    LongWord phase;
    LongWord phase_increment;
    uint8_t wave_balance;
    uint16_t wave_index;
    uint8_t note;
    int16_t detune;
    uint8_t the_overflow; 
};

#endif
