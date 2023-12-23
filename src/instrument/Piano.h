//
// Created by nikola on 23.12.23..
//

#ifndef SYNTH_PIANO_H
#define SYNTH_PIANO_H

#include "Instrument.h"

class Piano : public Instrument{
public:
    Piano();
    ~Piano();

    float waveFunc(double time) override;
};

#endif //SYNTH_PIANO_H
