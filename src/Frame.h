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
    FUNCTION_2("uiClick", uiClick);
    FUNCTION_0("gridSize", gridSize);
    END_FUNCTION_MAP

    sciter::value uiClick(sciter::value cur_x, sciter::value cur_y);
    int gridSize();
};

#endif //PISKVORKY2017_FRAME_H
