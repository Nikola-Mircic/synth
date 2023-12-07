//
// Created by nikola on 6.12.23..
//
#include "App.h"
#include "Frame.h"

bool App::OnInit() {
    Frame *frame = new Frame();

    frame->Show();

    SetTopWindow(frame);

    return true;
}