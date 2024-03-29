#ifndef LPFILTER_H
#define LPFILTER_H

//low pass filter definition
class LPFilter {
public:
  float lastval;
  float val;

  virtual float update(float input, float freq) {
    val = input * (1.0 - freq);
    val = val + (lastval * freq);
    lastval = val;
    return val;
  }
};

#endif