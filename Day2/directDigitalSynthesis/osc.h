#ifndef OSC_H
#define OSC_H

#include "base.h"
#include "resources.h"

class Osc{
  
  enum WaveShapes {
    RAMP,
    TRIANGLE
  };

  public:
    Osc(){};
    
    void Init();
    
    inline uint8_t Render(){
      uint8_t value_a, value_b, value;
      
      phase.value += phase_increment.value;

    /*
      switch(wave){
        case(RAMP):
          value_a = pgm_read_byte(lut_ramp + phase.bytes[HIGH_INTEGRAL]);
          value_b = pgm_read_byte(lut_ramp + phase.bytes[HIGH_INTEGRAL] + 1);
          value = U8MixU16(value_a, value_b, phase.bytes[HIGH_INTEGRAL]);
        break;
        case(TRIANGLE):
          value_a = pgm_read_byte(lut_tri + phase.bytes[HIGH_INTEGRAL]);
          value_b = pgm_read_byte(lut_tri + phase.bytes[HIGH_INTEGRAL] + 1);
          value = U8MixU16(value_a, value_b, phase.bytes[HIGH_INTEGRAL]);
        break;
      }
     
      */
      value_a = pgm_read_byte(lut_sin + phase.bytes[HIGH_INTEGRAL]);
      value_b = pgm_read_byte(lut_sin + phase.bytes[HIGH_INTEGRAL] + 1);
      value = U8Mix(value_a, value_b, phase.bytes[LOW_INTEGRAL]);
        
      return value;
    }
    
    void set_parameter(uint16_t _pw);
    void set_rate(uint16_t _rate);
    void set_note(uint8_t _note);
    void set_wave(uint16_t _wave);
  private:
    LongWord phase;
    LongWord phase_increment;
    uint8_t pulse_width;
    uint8_t parameter;
    uint8_t rate;
    uint8_t wave;
    uint8_t overflow;
};

#endif
