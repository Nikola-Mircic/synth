//
// Created by nikola on 6.12.23..
//

#ifndef SYNTH_FRAME_H
#define SYNTH_FRAME_H

#include "wx/wx.h"
#include "player/NoiseMaker.h"

class Frame : public wxFrame
{
public:
    Frame();

private:
    NoiseMaker* nm;

    void KeyDown(wxKeyEvent& event);
    void OnExit(wxCommandEvent& event);
    void KeyUp(wxKeyEvent& event);
};
#endif //SYNTH_FRAME_H
