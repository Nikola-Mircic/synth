//
// Created by nikola on 6.12.23..
//
#include "Frame.h"

#include <iostream>

#define FRAME_ID wxID_ANY
#define FRAME_TITLE "Synth by Nikola Mircic"
#define FRAME_POS wxPoint(100, 100)
#define FRAME_SIZE wxSize(400, 400)
#define FRAME_STYLE (wxDEFAULT_FRAME_STYLE | wxWANTS_CHARS)

bool pressed[26] = {false};

Frame::Frame()
        : wxFrame(nullptr, FRAME_ID, FRAME_TITLE, FRAME_POS, FRAME_SIZE, FRAME_STYLE, "")
{
    //Bind(wxEVT_KEY_DOWN, &Frame::KeyDown, this);
    wxPanel* mainPane = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);
    mainPane->Bind(wxEVT_KEY_DOWN, &Frame::KeyDown, this);
    mainPane->Bind(wxEVT_KEY_UP, &Frame::KeyUp, this);

    nm = new NoiseMaker();

    std::string chars("asdfghjkl");

    for(int i=0; i<chars.size(); ++i){
        nm->BindFreq(chars[i], 120.0f*std::pow(1.5f, i), 0.3f);
    }

    nm->Start();
}

void Frame::KeyDown(wxKeyEvent& event) {
    nm->Play(event.GetKeyCode());

    event.Skip();
}

void Frame::KeyUp(wxKeyEvent& event) {
    nm->Release(event.GetKeyCode());

    event.Skip();
}

void Frame::OnExit(wxCommandEvent& event)
{
    Close(true);
}