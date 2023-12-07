//
// Created by nikola on 6.12.23..
//

#ifndef SYNTH_APP_H
#define SYNTH_APP_H

#include <wx/wx.h>

#include "player/NoiseMaker.h"

class App : public wxApp
{
public:
    virtual bool OnInit() override;
private:
    NoiseMaker* nm;
};


#endif //SYNTH_APP_H
