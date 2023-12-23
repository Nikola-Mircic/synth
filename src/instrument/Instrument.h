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

#include "../player/Player.h"

struct Envelope{
    double attackTime;
    double decayTime;
    double sustainLvl;
    double releaseTime;

    double amplitude;
    double releaseAmpl;

    double timeOn;
    double timeOff;

    double getAmplitude(double time){
        if(timeOff != 0.0){
            double timePlaying = time-timeOff;

            if(timePlaying >= releaseTime)
                return 0.0f;
            else
                return releaseAmpl * (1-timePlaying/releaseTime);
        }else if(timeOn != 0.0){
            double timePlaying = time-timeOn;

            if(timePlaying <= attackTime)
                releaseAmpl = amplitude * (timePlaying/attackTime);
            else if(timePlaying <= attackTime+decayTime)
                releaseAmpl = amplitude - (amplitude-sustainLvl)*(timePlaying-attackTime)/decayTime;
            else
                releaseAmpl = sustainLvl;

            return releaseAmpl;
        }

        return 0.0;
    }
};

class Instrument {
public:
    Player* nm;

    Instrument();
    explicit Instrument(Player* nm);
    ~Instrument();

    void BindFreq(char target, float freq, float amp);
    void RemoveFreq(char target);

    void Play(char note);
    void Release(char note);

    static float waveFunc(double time);
private:
    static float* frequencies;
    static float* amplitudes;
    static bool* playing;
    static Envelope* envelopes;

};


#endif //SYNTH_INSTRUMENT_H
