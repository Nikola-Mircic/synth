//
// Created by nikola on 16.12.23..
//

#include "Instrument.h"

#define w(x) ((x)*2*M_PI)

using namespace std::chrono;

float* Instrument::frequencies = new float[50];
float* Instrument::amplitudes = new float[50];
bool* Instrument::playing = new bool[50];
Envelope* Instrument::envelopes = new Envelope[50];

Instrument::Instrument(){
    for(int i=0;i<50;++i) frequencies[i] = 0.0f;
    for(int i=0;i<50;++i) amplitudes[i] = 0.0f;
    for(int i=0;i<50;++i) playing[i] = false;

    for(int i=0;i<50;++i){
        envelopes[i].attackTime  = 0.1;
        envelopes[i].decayTime   = 1.5;
        envelopes[i].sustainLvl  = 0.0;
        envelopes[i].releaseTime = 0.75;
        envelopes[i].timeOn      = 0.0;
        envelopes[i].timeOff     = 0.0;

        envelopes[i].amplitude   = 0.3;
        envelopes[i].releaseAmpl = 0.0;
    }

    nm = Player::getInstance();

    nm->useWaveFunc(waveFunc);

    std::string chars("asdfghjkl;'");


    double noteStep = std::pow(4, 1.0/chars.size());

    for(int i=0; i<chars.size(); ++i){
        BindFreq(chars[i], 311.127f * std::pow(noteStep, i), 0.15f);
    }

    nm->Start();
}

Instrument::Instrument(Player* nm): nm(nm) {
    for(int i=0;i<50;++i) frequencies[i] = 0.0f;
    for(int i=0;i<50;++i) amplitudes[i] = 0.0f;
    for(int i=0;i<50;++i) playing[i] = false;

    nm->Start();
}

Instrument::~Instrument() {
    nm->Stop();
}

float Instrument::waveFunc(double time) {
    float sample = 0.0f;
    double freq;
    float amp = 0.0f;

    for(int f=0;f<26;++f){

        freq = w(frequencies[f])*time + 0.001*w(frequencies[f])*std::sin(w(2.0)*time);

        amp = (float) envelopes[f].getAmplitude(time);

        sample += std::sin(freq) * amp;
    }

    return sample;
}

void Instrument::Play(char note) {
    int idx = (int) (note - 65);

    if(playing[idx]) return;

    printf(" %c : %.2f - %.2f \n", note, frequencies[idx], amplitudes[idx]);

    playing[idx] = true;

    microseconds us = duration_cast< microseconds >(
            system_clock::now().time_since_epoch()
    );

    envelopes[idx].amplitude = amplitudes[idx];
    envelopes[idx].releaseAmpl = 0.0;
    envelopes[idx].timeOn = us.count()/1000000.0;
    envelopes[idx].timeOff = 0.0;
}

void Instrument::Release(char note) {
    int idx = (int) (note - 65);
    playing[idx] = false;

    microseconds us = duration_cast< microseconds >(
            system_clock::now().time_since_epoch()
    );

    envelopes[idx].timeOn = 0.0;
    envelopes[idx].timeOff = us.count()/1000000.0;
}

void Instrument::BindFreq(char target, float freq, float amp) {
    int idx = (int) (target - 'a');

    std::cout << idx << std::endl;

    frequencies[idx] = freq;
    amplitudes[idx] = amp;
    envelopes[idx].amplitude = amp;
}

void Instrument::RemoveFreq(char target) {
    int idx = (int) (target - 65);

    frequencies[idx] = 0.0f;
    amplitudes[idx] = 0.0f;
}


