//
// Created by nikola on 6.12.23..
//

#include "NoiseMaker.h"

float* NoiseMaker::frequencies = new float[26];
float* NoiseMaker::amplitudes = new float[26];
bool* NoiseMaker::playing = new bool[26];

NoiseMaker::NoiseMaker() {
    for(int i=0;i<26;++i) frequencies[i] = 0.0f;
    for(int i=0;i<26;++i) amplitudes[i] = 0.0f;
    for(int i=0;i<26;++i) playing[i] = false;

    Init();
}

NoiseMaker::~NoiseMaker() {
    Stop();
}

void NoiseMaker::Init() {
    PaError err;
    RUN_COMMAND(Pa_Initialize())

    /* Open an audio I/O stream. */
    RUN_COMMAND(Pa_OpenDefaultStream(&stream,
                                     0,          /* no input channels */
                                     2,          /* stereo output */
                                     paFloat32,  /* 32 bit floating point output */
                                     SAMPLE_RATE,
                                     256,        /* frames per buffer, i.e. the number
                                                   of sample frames that PortAudio will
                                                   request from the callback. Many apps
                                                   may want to use
                                                   paFramesPerBufferUnspecified, which
                                                   tells PortAudio to pick the best,
                                                   possibly changing, buffer size.*/
                                     NoiseMaker::CallbackFunc, /* this is your callback function */
                                     nullptr) /*This is a pointer that will be passed to
                                                   your callback*/
    )
}

void NoiseMaker::Start() {
    PaError err;
    RUN_COMMAND(Pa_StartStream(stream))
}

void NoiseMaker::Pause() {
    PaError err;
    RUN_COMMAND(Pa_StopStream(stream))
}

void NoiseMaker::Stop() {
    PaError err;
    RUN_COMMAND(Pa_StopStream(stream))

    RUN_COMMAND(Pa_CloseStream(stream))
    RUN_COMMAND(Pa_Terminate())
}

/* This routine will be called by the PortAudio engine when audio is needed.
 * It may called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
*/

static float phase = 0.0f;
static float phaseStep = 500.0f*2*M_PI/SAMPLE_RATE;

int NoiseMaker::CallbackFunc(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *data )
{
    //float* phasef = (float* ) phase;
    double callTime = timeInfo->outputBufferDacTime;
    double timeStep = (1.0f)/SAMPLE_RATE;

    float *out = (float*)outputBuffer;

    (void) inputBuffer; /* Prevent unused variable warning. */

    for(unsigned int i=0; i<framesPerBuffer; ++i )
    {
         callTime += timeStep;
        *(out) = 0;
        *(out+1) = 0;
        for(int f=0;f<26;++f){
            if(!playing[f]) continue;

            *(out) += std::sin(frequencies[f]*2*M_PI*callTime) * amplitudes[f];
            *(out+1) += std::sin(frequencies[f]*2*M_PI*callTime) * amplitudes[f];
        }
        out += 2;
    }
    return 0;
}

void NoiseMaker::BindFreq(char target, float freq, float amp) {
    int idx = (int) (target - 'a');

    std::cout << idx << std::endl;

    frequencies[idx] = freq;
    amplitudes[idx] = amp;
}

void NoiseMaker::RemoveFreq(char target) {
    int idx = (int) (target - 65);

    frequencies[idx] = 0.0f;
    amplitudes[idx] = 0.0f;
}

void NoiseMaker::Play(char c) {
    int idx = (int) (c - 65);

    playing[idx] = true;
}

void NoiseMaker::Release(char c) {
    int idx = (int) (c - 65);
    playing[idx] = false;
}

