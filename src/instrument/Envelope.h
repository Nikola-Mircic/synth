//
// Created by nikola on 23.12.23..
//

#ifndef SYNTH_ENVELOPE_H
#define SYNTH_ENVELOPE_H

struct Envelope{
    double attackTime;
    double decayTime;
    double sustainLvl;
    double releaseTime;

    double amplitude;
    double releaseAmpl;

    double timeOn;
    double timeOff;

    Envelope():Envelope(0,0,0,0){}

    Envelope(double attack_time, double decay_time, double sustain_level, double release_time):
            attackTime(attack_time),
            decayTime(decay_time),
            sustainLvl(sustain_level),
            releaseTime(release_time),
            amplitude(0.0),
            releaseAmpl(0.0),
            timeOn(0.0),
            timeOff(0.0){}

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

#endif //SYNTH_ENVELOPE_H
