//
// Created by nohajc on 14.4.17.
//

#ifndef PISKVORKY2017_HUMANPLAYER_H
#define PISKVORKY2017_HUMANPLAYER_H


#include "Player.h"
#include "Frame.h"

class HumanPlayer : public Player {
public:
    virtual Cell proposeMove(const Game & g) const override;
};


#endif //PISKVORKY2017_HUMANPLAYER_H
