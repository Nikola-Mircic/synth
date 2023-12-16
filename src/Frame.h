//
// Created by nikola on 6.12.23..
//

#ifndef SYNTH_FRAME_H
#define SYNTH_FRAME_H

#include "wx/wx.h"
#include "instrument/Instrument.h"

class Frame : public wxFrame
{
public:
    Frame();

private:
    Instrument* instrument;

    void KeyDown(wxKeyEvent& event);
    void OnExit(wxCommandEvent& event);
    void KeyUp(wxKeyEvent& event);
};
#endif //SYNTH_FRAME_H
