//
// Created by nohajc on 14.4.17.
//

#ifndef PISKVORKY2017_HUMANPLAYER_H
#define PISKVORKY2017_HUMANPLAYER_H

#include <functional>

#include "Player.h"
#include "Frame.h"

class HumanPlayer : public Player {
public:
    // The return value is ignored, we need something else thant 'void' for sciter compatibility
    typedef std::function<bool(int, int)> ClickCallbackFunc;
    typedef std::function<void(ClickCallbackFunc)> AddOneClickHandlerFunc;

    HumanPlayer(AddOneClickHandlerFunc clickHnd);
    virtual Cell proposeMove(const Game &) override;

private:
    AddOneClickHandlerFunc addOneClickHandler;
};


#endif //PISKVORKY2017_HUMANPLAYER_H
