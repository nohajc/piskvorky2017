//
// Created by nohajc on 14.4.17.
//

#include <vector>
#include <algorithm>
#include <cstdio>

#include "Game.h"
#include "Grid.h"

// TODO: add bounds checking
// n >= 3, rowLength >= 3 and rowLength <= n
Game::Game(grid_size_t n,
           unsigned rowLength,
           std::unique_ptr<Player> p1,
           std::unique_ptr<Player> p2)
        : player1(std::move(p1)), player2(std::move(p2)),
          currentPlayer(*player1), theOtherPlayer(*player2), grid(n) {
    gameOver = false;
    gameStatus = STATUS_IN_PROGRESS;
    rowLengthToWin = rowLength;
    moveCounter = 0;
    movesMax = n * n;

    player1->setMarking(Player::CROSS);
    player2->setMarking(Player::NAUGHT);
}

void Game::play() {
    while (moveCounter < movesMax) {
        while (!nextMove(currentPlayer));
        if (gameOver) break;
        std::swap(currentPlayer, theOtherPlayer);
    }
}

bool Game::nextMove(Player & currPlayer) {
    Cell c = currPlayer.proposeMove(*this);
    if (grid[c] == Marking::EMPTY) {
        grid[c] = Marking(currPlayer.getMarking());
        lastMove = c;
        moveCounter++;

        gameOver = checkVictory(c);
        if (gameOver) {
            winningMove = c;
            gameStatus = STATUS_OVER;
        }
        else if (moveCounter == movesMax) {
            gameStatus = STATUS_DRAW;
        }
        update(c);

        return true;
    }
    return false;
}

void Game::setUpdateHandler(UpdateHandlerFunc update) {
    updateHandler = update;
}

bool Game::checkVictory(Cell c) const {
    unsigned longestRow = grid.longestRowAround(c, rowLengthToWin - 1);
    return longestRow >= rowLengthToWin;
}

Cell Game::getWinningMove() const {
    return winningMove;
}

const std::string Game::STATUS_OVER = "OVER";
const std::string Game::STATUS_IN_PROGRESS = "IN_PROGRESS";
const std::string Game::STATUS_DRAW = "DRAW";

const Grid & Game::getGrid() const {
    return grid;
}

Cell Game::getLastMove() const {
    return lastMove;
}

unsigned Game::getMoveCounter() const {
    return moveCounter;
}
