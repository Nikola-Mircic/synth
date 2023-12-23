//
// Created by nikola on 16.12.23..
//

#ifndef SYNTH_INSTRUMENT_H
#define SYNTH_INSTRUMENT_H

#include <cstdio>
#include <cmath>
#include <iostream>
#include <chrono>
#include <ctime>
#include <vector>

#include "Envelope.h"

class Instrument {
public:
    Instrument();
    ~Instrument();

    void BindFreq(char target, float freq, float amp);
    void RemoveFreq(char target);

    void Play(char note);
    void Release(char note);

    virtual float waveFunc(double time);
private:
    std::vector<float>* frequencies;
    bool* playing;
    std::vector<Envelope>* envelopes;

};


#endif //SYNTH_INSTRUMENT_H
