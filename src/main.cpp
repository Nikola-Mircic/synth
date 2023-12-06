#include <cstdio>
#include <cmath>
#include <ncurses.h>

#include "portaudio.h"

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

int main(){
    initscr();			/* Start curses mode 		*/
    raw();				/* Line buffering disabled	*/
    noecho();			/* Don't echo() while we do getch */

    PaError err;// = Pa_Initialize();

    RUN_COMMAND(Pa_Initialize())

    PaStream *stream;

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
                                     callbackFunc, /* this is your callback function */
                                     phases) /*This is a pointer that will be passed to
                                                   your callback*/
                                )

    RUN_COMMAND(Pa_StartStream(stream))

    bool running = true;

    int ch;

    while(running){
        ch = getch();

        if(ch == 'Q') {
            running = false;
        }else{
            if(ch == 'a') keyPressed[0] = true;
            else keyPressed[0] = false;

            if(ch == 's') keyPressed[1] = true;
            else keyPressed[1] = false;

            if(ch == 'd') keyPressed[2] = true;
            else keyPressed[2] = false;
        }
    }

    echo();
    endwin();			/* End curses mode		  */

    RUN_COMMAND(Pa_StopStream(stream))

    RUN_COMMAND(Pa_CloseStream(stream))
    RUN_COMMAND(Pa_Terminate())

    return 0;
}