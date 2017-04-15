//
// Created by nohajc on 14.4.17.
//

#include "Frame.h"
#include "HumanPlayer.h"

static RECT defaultRect = { 100, 100, 800, 800 };

Frame::Frame() : window( SW_MAIN | SW_TITLEBAR | SW_CONTROLS | SW_ENABLE_DEBUG, defaultRect) {
    // TODO: bind this action to an UI event (main menu click or something)
    std::unique_ptr<Player> p1 = std::make_unique<HumanPlayer>();
    std::unique_ptr<Player> p2 = std::make_unique<HumanPlayer>();
    game = std::make_unique<Game>(5, std::move(p1), std::move(p2));
}

sciter::value Frame::uiClick(sciter::value cur_x, sciter::value cur_y) {
    int x = cur_x.get<int>();
    int y = cur_y.get<int>();

    printf("Clicked at %d, %d\n", x, y);
    return sciter::value();
}

int Frame::gridSize() {
    return (int)(game ? game->getGridSize() : 0);
}

void Frame::startGame() {
    game->setUpdateCallback([this](Cell c, Player::marking_t mark){
        call_function("updateGrid", int(c.x), int(c.y), mark);
    });

    game->play();
}
