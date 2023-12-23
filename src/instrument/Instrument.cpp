//
// Created by nikola on 16.12.23..
//

#include "Instrument.h"

using namespace std::chrono;

void BindKeys(Instrument* instrument){
    std::string chars("asdfghjk");

    double noteStep = std::pow(2.0, 1.0/chars.size());

    for(int i=0; i<chars.size(); ++i){
        double step = std::pow(noteStep, i);
        instrument->BindFreq(chars[i], 339.29f * step, 0.3f/step);
    }
}

void FreeKeys(Instrument* instrument){
    std::string chars("asdfghjk");

    for(int i=0; i<chars.size(); ++i){
        instrument->RemoveFreq(chars[i]);
    }
}

Instrument::Instrument(){
    frequencies = new std::vector<float>();
    envelopes = new std::vector<Envelope>();
    playing = new bool[50];

    for(int i=0;i<50;++i) frequencies->push_back(0.0f);
    for(int i=0;i<50;++i) playing[i] = false;

    for(int i=0;i<50;++i){
        envelopes->emplace_back(0.2,2.0,0.0,1.5);

        envelopes->at(i).amplitude   = 0.3;
        envelopes->at(i).releaseAmpl = 0.0;
    }

    BindKeys(this);

    std::cout << "Frequencies: \n";
    for(float f : *frequencies){
        std::cout << f << " ";
    }
    std::cout << std::endl;
}

Instrument::~Instrument() {
    FreeKeys(this);

    delete frequencies;
    delete envelopes;
    delete playing;
}

void Instrument::Play(char note) {
    int idx = (int) (note - 65);

    if(playing[idx]) return;

    printf(" %c : %.2f - %.2f \n", note, frequencies->at(idx), envelopes->at(idx).amplitude);

    playing[idx] = true;

    microseconds us = duration_cast< microseconds >(
            system_clock::now().time_since_epoch()
    );

    envelopes->at(idx).releaseAmpl = 0.0;
    envelopes->at(idx).timeOn = us.count()/1000000.0;
    envelopes->at(idx).timeOff = 0.0;
}

void Instrument::Release(char note) {
    int idx = (int) (note - 65);
    playing[idx] = false;

    microseconds us = duration_cast< microseconds >(
            system_clock::now().time_since_epoch()
    );

    envelopes->at(idx).timeOn = 0.0;
    envelopes->at(idx).timeOff = us.count()/1000000.0;
}

void Instrument::BindFreq(char target, float freq, float amp) {
    int idx = (int) (target - 'a');

    std::cout << idx << std::endl;

    if(idx>=0 && idx<frequencies->size())
        frequencies->at(idx) = freq;
    if(idx>=0 && idx<envelopes->size())
        envelopes->at(idx).amplitude = amp;
}

void Instrument::RemoveFreq(char target) {
    int idx = (int) (target - 65);

    if(idx>=0 && idx<frequencies->size())
        frequencies->at(idx) = 0.0f;
}

float Instrument::waveFunc(double time) {
    float sample = 0.0f;
    double freq;
    float amp = 0.0f;

    for(int f=0;f<26;++f){

        freq = w(frequencies->at(f)) * time;

        amp = (float) envelopes->at(f).getAmplitude(time);

        sample += std::sin(freq) * amp;
    }

    return sample;
}
