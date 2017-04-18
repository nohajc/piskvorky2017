//
// Created by nohajc on 17.4.17.
//

#ifndef PISKVORKY2017_MINMAXPLAYER_H
#define PISKVORKY2017_MINMAXPLAYER_H

#include <memory>
#include <vector>
#include <random>

#include "Player.h"
#include "Grid.h"

// This is the AI player
class MinMaxPlayer : public Player {
    struct MinMaxTree {
        Grid grid;
        std::vector<std::unique_ptr<MinMaxTree>> nodes;
    };

    std::unique_ptr<MinMaxTree> minMaxRoot;

    std::random_device rd;
    std::mt19937 rng;
public:
    MinMaxPlayer() : rng(rd()) {}
    virtual Cell proposeMove(const Game & g) override;
};


#endif //PISKVORKY2017_MINMAXPLAYER_H
