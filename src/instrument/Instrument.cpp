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
        envelopes[i].attackTime  = 0.5;
        envelopes[i].decayTime   = 1.0;
        envelopes[i].sustainLvl  = 0.0;
        envelopes[i].releaseTime = 0.5;
        envelopes[i].timeOn      = 0.0;
        envelopes[i].timeOff     = 0.0;
    }

    nm = Player::getInstance();

    nm->useWaveFunc(waveFunc);

    std::string chars("asdfghjkl;'");

    for(int i=0; i<chars.size(); ++i){
        BindFreq(chars[i], 250.0f * std::pow(1.15, i), 0.3f);
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

        freq = w(frequencies[f])*time;

        amp = (float) envelopes[f].getAmplitude(amplitudes[f], time);

        sample += std::sin(freq) * amp
                       +  std::sin(freq*2.0) * amp/2
                       +  std::sin(freq*4.0) * amp/4;
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
}

void Instrument::RemoveFreq(char target) {
    int idx = (int) (target - 65);

    frequencies[idx] = 0.0f;
    amplitudes[idx] = 0.0f;
}


