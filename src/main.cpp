#include <cstdio>
#include <cmath>
#include <ncurses.h>

#include "portaudio.h"

#include "App.h"

#define SAMPLE_RATE 44100

#define RUN_COMMAND(x) err = (x);               \
                       if( err != paNoError )   \
                            printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );

static bool keyPressed[3] = {false};
static float phases[3] = {0.0f};
static float phaseSteps[3] = {2500.0f/SAMPLE_RATE, 3000.0f/SAMPLE_RATE, 3500.0f/SAMPLE_RATE};


/* This routine will be called by the PortAudio engine when audio is needed.
 * It may called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
*/
static int callbackFunc(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *phase )
{
    float* phasef = (float* ) phase;

    float *out = (float*)outputBuffer;

    (void) inputBuffer; /* Prevent unused variable warning. */

    for(unsigned int i=0; i<framesPerBuffer; ++i )
    {
        *(out) = 0; /* left */
        *(out+1) = 0;  /* right */
        for(int k=0; k<3; ++k){
            if(keyPressed[k]){
                phasef[k] += phaseSteps[k];
                *(out) += std::sin(phasef[k]) * (1.0f/(std::pow(2,k+1)));  /* left */
                *(out+1) += std::sin(phasef[k]) * (1.0f/(std::pow(2,k+1)));  /* right */
            }
        }
        out += 2;

    }
    return 0;
}

wxIMPLEMENT_APP(App);