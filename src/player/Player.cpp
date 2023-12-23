//
// Created by nikola on 6.12.23..
//

#include "Player.h"

Player* Player::instance = nullptr;
Instrument* Player::instrument = nullptr;

Player::Player() {
    Init();
}

Player::~Player() {
    Stop();
}

void Player::Init() {
    PaError err;
    RUN_COMMAND(Pa_Initialize())

    /* Open an audio I/O stream. */
    RUN_COMMAND(Pa_OpenDefaultStream(&stream,
                                     0,          /* no input channels */
                                     2,          /* stereo output */
                                     paFloat32,  /* 32 bit floating point output */
                                     SAMPLE_RATE,
                                     paFramesPerBufferUnspecified,        /* frames per buffer, i.e. the number
                                                   of sample frames that PortAudio will
                                                   request from the callback. Many apps
                                                   may want to use
                                                   paFramesPerBufferUnspecified, which
                                                   tells PortAudio to pick the best,
                                                   possibly changing, buffer size.*/
                                     Player::CallbackFunc, /* this is your callback function */
                                     nullptr) /*This is a pointer that will be passed to
                                                   your callback*/
    )
}

void Player::Start() {
    PaError err;
    RUN_COMMAND(Pa_StartStream(stream))
}

void Player::Pause() {
    PaError err;
    RUN_COMMAND(Pa_StopStream(stream))
}

void Player::Stop() {
    PaError err;
    RUN_COMMAND(Pa_StopStream(stream))

    RUN_COMMAND(Pa_CloseStream(stream))
    RUN_COMMAND(Pa_Terminate())
}

/* This routine will be called by the PortAudio engine when audio is needed.
 * It may called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
*/

int Player::CallbackFunc(const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *data )
{

    double callTime = timeInfo->outputBufferDacTime;
    double timeStep = (1.0f)/SAMPLE_RATE;

    float *out = (float*)outputBuffer;

    (void) inputBuffer; /* Prevent unused variable warning. */

    for(unsigned int i=0; i<framesPerBuffer; ++i )
    {
        callTime += timeStep;

        if(!instrument) {
            *(out++) = 0;
            *(out++) = 0;
            continue;
        }

        float sample = instrument->waveFunc(callTime);
        *(out++) = sample;
        *(out++) = sample;

    }
    return 0;
}

Player *Player::getInstance(){
    if(Player::instance == nullptr){
        Player::instance = new Player();
    }

    return instance;
}

void Player::useInstrument(Instrument *i) {
    Player::instrument = i;
}

void Player::Play(char note) {
    instrument->Play(note);
}

void Player::Release(char note) {
    instrument->Release(note);
}

