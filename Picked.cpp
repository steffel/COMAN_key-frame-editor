//
// Created by stef on 15.04.20.
//
#ifndef PROJECT_PICKED_CPP
#define PROJECT_PICKED_CPP

#include "Channel.h"
#include "FFD.h"

struct Picked {
    Picked(Channel *channel = nullptr, FFD *ffd = nullptr, int ctrlPtI = 0) :
            channel(nullptr), ffd(nullptr), controlPointIndex(0) {}
    Channel *channel;
    FFD *ffd;
    int controlPointIndex;
};

#endif