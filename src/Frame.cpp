//
// Created by nohajc on 14.4.17.
//

#include "Frame.h"
#include "HumanPlayer.h"

static RECT defaultRect = { 100, 100, 800, 800 };

Frame::Frame() : window( SW_MAIN | SW_TITLEBAR | SW_CONTROLS | SW_ENABLE_DEBUG, defaultRect) {
    // TODO: bind this action to an UI event (main menu click or something)
    auto addOneClickHnd = [this](HumanPlayer::ClickCallbackFunc callback) {
        call_function("addOneClickHandler", sciter::vfunc(callback));
    };

    std::unique_ptr<Player> p1 = std::make_unique<HumanPlayer>(addOneClickHnd);
    std::unique_ptr<Player> p2 = std::make_unique<HumanPlayer>(addOneClickHnd);
    
    game = std::make_unique<Game>(5, std::move(p1), std::move(p2));
}

int Frame::gridSize() {
    return (int)(game ? game->getGridSize() : 0);
}

void Frame::startGame() {
    game->setUpdateHandler([this](Cell c, Player::marking_t mark) {
        call_function("updateGrid", int(c.x), int(c.y), mark);
    });

    game->play();
}
