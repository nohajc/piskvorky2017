//
// Created by nohajc on 14.4.17.
//

#ifndef PISKVORKY2017_FRAME_H
#define PISKVORKY2017_FRAME_H

#include <sciter-x-window.hpp>

#include "Game.h"


class Frame: public sciter::window {
    std::unique_ptr<Game> game;
public:
    Frame();
    void startGame();

    BEGIN_FUNCTION_MAP
    FUNCTION_0("gridSize", gridSize);
    END_FUNCTION_MAP

    int gridSize();
};

#endif //PISKVORKY2017_FRAME_H
