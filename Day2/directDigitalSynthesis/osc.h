#ifndef OSC_H
#define OSC_H

#include "base.h"
#include "resources.h"

  const uint8_t kStepsPerSemitone = 16;
  const uint8_t kLowestNote = 0*kStepsPerSemitone;
  const uint8_t kHighestNote = 128*kStepsPerSemitone;
  const uint8_t kPitchTableStart = 116*kStepsPerSemitone;
  const uint8_t kDetuneAmount = 12*kStepsPerSemitone;
  const uint8_t kOctave = 12*kStepsPerSemitone;


class Osc{
  
  enum WaveShapes {
    RAMP,
    TRIANGLE,
    SINE,
    
  };
  
  public:
    Osc(){};
    
    void Init();
    
    inline uint8_t Render(){
      uint8_t value_a, value_b, value;
      
      phase.value += phase_increment.value;

      switch(wave){
        case(RAMP):
          value_a = pgm_read_byte(lut_ramp + phase.bytes[HIGH_INTEGRAL]);
          value_b = pgm_read_byte(lut_ramp + phase.bytes[HIGH_INTEGRAL] + 1);
          value = U8Mix(value_a, value_b, phase.bytes[LOW_INTEGRAL]);
        break;
        case(TRIANGLE):
          value_a = pgm_read_byte(lut_tri + phase.bytes[HIGH_INTEGRAL]);
          value_b = pgm_read_byte(lut_tri + phase.bytes[HIGH_INTEGRAL] + 1);
          value = U8Mix(value_a, value_b, phase.bytes[LOW_INTEGRAL]);
        break;
        case(SINE):
          value_a = pgm_read_byte(lut_tri + phase.bytes[HIGH_INTEGRAL]);
          value_b = pgm_read_byte(lut_tri + phase.bytes[HIGH_INTEGRAL] + 1);
          value = U8Mix(value_a, value_b, phase.bytes[LOW_INTEGRAL]);
        break;
      }
      
      return value;
    }
    
    inline void Update(){
      uint8_t octave_shift = 0, pitch_index;
      int16_t pitch;
      LongWord increment, increment_next;
      
      pitch = (note<<4);
      pitch += detune;
      
      while(pitch >= kHighestNote){
        pitch -= kOctave
      }
      
      pitch_index = pitch;
      pitch_index -= kPitchTableStart;
      
      while(pitch < 0){
        pitch_index += kOctave;
        ++octave_shift;
      }
      
      increment.words[INTEGRAL] = pgm_read_word(lut_pitch_increments + pitch_index);
      increment_next.words[INTEGRAL] = pgm_read_word(lut_pitch_increments + pitch_index + 1);
      
      while(octave_shift--){
        increment.value = increment.value>>1;
      } 
      
      if(note<128){
        phase_increment.value = increment.value;
      }
    }
    
    void note_on(uint8_t _note);
    void note_off();
    void sync();
    void set_detune(int16_t _detune);
    void set_pitch(uint16_t _pitch);
    void set_parameter(uint8_t _parameter);
    void set_wave(uint16_t _wave);
    
  private:
    LongWord phase;
    LongWord phase_increment;
    uint8_t parameter;
    uint8_t wave;
    uint8_t note;
    int16_t detune;
};

#endif
