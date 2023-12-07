//
// Created by nikola on 6.12.23..
//

#ifndef SYNTH_NOISEMAKER_H
#define SYNTH_NOISEMAKER_H

#include <cstdio>
#include <cmath>
#include <iostream>

#include "portaudio.h"

#define SAMPLE_RATE 44100

#define RUN_COMMAND(x) err = (x);               \
                       if( err != paNoError )   \
                            printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

class NoiseMaker{
public:
    NoiseMaker();
    ~NoiseMaker();
    static int CallbackFunc(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *phase );

    static float* frequencies;
    static float* amplitudes;
    static bool* playing;

    void Init();
    void Start();
    void Pause();
    void Stop();

    void BindFreq(char target, float freq, float amp);
    void RemoveFreq(char target);

    void Play(char c);
    void Release(char c);
private:
    PaStream* stream;
};

#endif //SYNTH_NOISEMAKER_H
