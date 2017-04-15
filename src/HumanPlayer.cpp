//
// Created by nohajc on 14.4.17.
//

#include <future>
#include <utility>

#include "HumanPlayer.h"

Cell HumanPlayer::proposeMove(const Game & g) const {
    std::promise<Cell> promiseCell;
    std::future<Cell> futureCell = promiseCell.get_future();

    evtReg.addOnClickHandler([&promiseCell](int x, int y) {
        promiseCell.set_value({x, y});
        return true;
    });
    Cell result = futureCell.get();

    evtReg.removeOnClickHandler();
    return result;
}

HumanPlayer::HumanPlayer(EventHandlerRegistration eHndReg) : evtReg(eHndReg) {}

