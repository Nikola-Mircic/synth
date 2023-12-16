//
// Created by nikola on 6.12.23..
//

#ifndef SYNTH_PLAYER_H
#define SYNTH_PLAYER_H

#include <cstdio>
#include <cmath>
#include <iostream>

#include "portaudio.h"

#define SAMPLE_RATE 44100

#define RUN_COMMAND(x) err = (x);               \
                       if( err != paNoError )   \
                            printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

class Player{
private:
    Player();
    ~Player();

    static Player* instance;

    PaStream* stream;
public:
    static Player* getInstance();

    static int CallbackFunc(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *phase );

    void useWaveFunc(float (*waveFunc)(double time));
    float (*waveFunc)(double time);

    void Init();
    void Start();
    void Pause();
    void Stop();

    Player(const Player& nm) = delete;
};

#endif //SYNTH_PLAYER_H
