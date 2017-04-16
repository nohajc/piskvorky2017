//
// Created by nohajc on 14.4.17.
//

#include <vector>
#include <cstdio>

#include "Game.h"

Game::Game(grid_size_t n,
           std::unique_ptr<Player> p1,
           std::unique_ptr<Player> p2,
           unsigned rowLength)
        : player1(std::move(p1)), player2(std::move(p2)), grid(n) {
    gameOver = false;
    rowLengthToWin = rowLength;

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
    printf("GAME OVER\n");
}

bool Game::nextMove(const Player & currentPlayer) {
    Cell c = currentPlayer.proposeMove(*this);
    if (grid[c] == Marking::EMPTY) {
        grid[c] = Marking(currentPlayer.getMarking());
        update(c);
        gameOver = checkVictory(c);
        return true;
    }
    return false;
}

void Game::setUpdateHandler(UpdateHandlerFunc update) {
    updateHandler = update;
}

template<typename I>
static bool gridCheckForRow(I from, I to, unsigned rowLengthToWin, Marking marking) {
    unsigned rowLength = 0;

    for (auto it = from; it != to; it++) {
        if (*it == marking) {
            if (++rowLength >= rowLengthToWin) {
                return true;
            }
        }
        else {
            rowLength = 0;
        }
    }
    return false;
}

bool Game::checkVictory(Cell c) {
    // Do four sweeps around the newly filled cell
    // (horizontal, vertical and two diagonal)
    // and search for a row of crosses or naughts
    Marking marking = grid[c];

    auto left = std::next(grid.row_begin(c.y), boundCheck(c.x - rowLengthToWin + 1));
    auto right = std::next(grid.row_begin(c.y), boundCheck(c.x + rowLengthToWin - 1));

    auto top = std::next(grid.col_begin(c.x), boundCheck(c.y - rowLengthToWin + 1));
    auto bottom = std::next(grid.col_begin(c.x), boundCheck(c.y + rowLengthToWin - 1));

    return gridCheckForRow(left, right, rowLengthToWin, marking)
            || gridCheckForRow(top, bottom, rowLengthToWin, marking);
}

grid_size_t Game::boundCheck(int x) {
    if (x < 0) {
        return 0ul;
    }
    else if (x >= grid.size()) {
        return grid.size() - 1;
    }
    return (grid_size_t)x;
}
