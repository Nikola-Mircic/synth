//
// Created by nikola on 23.12.23..
//
#include "Piano.h"

float Piano::waveFunc(double time) {
    float sample = 0.0f;
    double freq;
    float amp = 0.0f;

    for(int f=0;f<26;++f){

        freq = w(frequencies->at(f)) * time;

        amp = (float) envelopes->at(f).getAmplitude(time);

        sample += std::sin(freq) * amp;
        sample += std::sin(2*freq) * amp/2;

        sample += sample * sample * sample * amp;
    }

    return sample;
}

Piano::Piano():Instrument() {
    for(int i=0;i<50;++i){
        envelopes->at(i).attackTime = 0.1;
        envelopes->at(i).decayTime = 1.0;
        envelopes->at(i).sustainLvl = 0.0;
        envelopes->at(i).releaseTime = 0.5;

        envelopes->at(i).releaseAmpl = 0.0;
    }


}

