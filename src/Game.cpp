//
// Created by nohajc on 14.4.17.
//

#include <vector>
#include <cstdio>

#include "Game.h"

Game::Game(grid_size_t n,
           std::unique_ptr<Player> p1,
           std::unique_ptr<Player> p2)
        : player1(std::move(p1)), player2(std::move(p2)), grid(n){
    gameOver = false;

    player1->setMarking(Player::CROSS);
    player2->setMarking(Player::NAUGHT);
}

grid_size_t Game::getGridSize() const {
    return grid.size();
}

void Game::play() {
    while (true) {
        while (!nextMove(*player1));
        if (gameOver) break;
        while (!nextMove(*player2));
        if (gameOver) break;
    }
}

bool Game::nextMove(const Player & currentPlayer) {
    Cell c = currentPlayer.proposeMove(*this);
    if (grid[c] == Marking::EMPTY) {
        grid[c] = Marking(currentPlayer.getMarking());
        update(c);
        return true;
    }
    return false;
}

void Game::setUpdateCallback(UpdateCallBackFunc update) {
    updateCallback = update;
}
