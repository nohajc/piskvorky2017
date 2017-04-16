//
// Created by nohajc on 14.4.17.
//

#include <vector>
#include <algorithm>
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

bool Game::nextMove(Player & currentPlayer) {
    Cell c = currentPlayer.proposeMove(*this);
    if (grid[c] == Marking::EMPTY) {
        grid[c] = Marking(currentPlayer.getMarking());
        update(c);
        gameOver = checkVictory(c);
        if (gameOver) {
            winningMove = c;
        }
        return true;
    }
    return false;
}

void Game::setUpdateHandler(UpdateHandlerFunc update) {
    updateHandler = update;
}

bool Game::checkVictory(Cell c) const {
    // Do four sweeps around the newly filled cell
    // (horizontal, vertical and two diagonal)
    // and search for a row of crosses or naughts
    Marking marking = grid[c];

    int leftX = c.x - rowLengthToWin + 1;
    int rightX = c.x + rowLengthToWin - 1;

    int topY = c.y - rowLengthToWin + 1;
    int bottomY = c.y + rowLengthToWin - 1;

//    printf("c.x = %u, c.y = %u, leftX = %d, rightX = %d, topY = %d, bottomY = %d\n",
//           c.x, c.y, leftX, rightX, topY, bottomY);

    // TODO: move bound checking to Grid, unify interface (row_at, col_at, ...)
    auto left = std::next(grid.row_begin(c.y), boundCheck(leftX));
    auto right = std::next(grid.row_begin(c.y), boundCheck(rightX));

    auto top = std::next(grid.col_begin(c.x), boundCheck(topY));
    auto bottom = std::next(grid.col_begin(c.x), boundCheck(bottomY));

    Cell chkTopLeft = boundCheckMainDiag(c, -rowLengthToWin + 1);
    auto topLeft = grid.main_diag_at(chkTopLeft.y, chkTopLeft.x);
    Cell chkBottomRight = boundCheckMainDiag(c, rowLengthToWin - 1);
    auto bottomRight = grid.main_diag_at(chkBottomRight.y, chkBottomRight.x);

    Cell chkTopRight = boundCheckAntiDiag(c, -rowLengthToWin + 1);
    auto topRight = grid.anti_diag_at(chkTopRight.y, chkTopRight.x);
    Cell chkBottomLeft = boundCheckAntiDiag(c, rowLengthToWin - 1);
    auto bottomLeft = grid.anti_diag_at(chkBottomLeft.y, chkBottomLeft.x);

    return gridCheckForRow(left, right, marking)
            || gridCheckForRow(top, bottom, marking)
            || gridCheckForRow(topLeft, bottomRight, marking)
            || gridCheckForRow(topRight, bottomLeft, marking);
}

grid_size_t Game::boundCheck(int x) const {
    if (x < 0) {
        return 0ul;
    }
    else if (x >= grid.size()) {
        return grid.size() - 1;
    }
    return (grid_size_t)x;
}

Cell Game::boundCheckMainDiag(Cell c, int shift) const {
    int size = (int)grid.size();
    if (shift > 0) {
        unsigned actualShift = std::min({size - 1 - c.x, size - 1 - c.y, (unsigned)shift});
        return {c.x + actualShift, c.y + actualShift};
    }

    unsigned actualShift = std::min({c.x, c.y, (unsigned)(-shift)});
    return {c.x - actualShift, c.y - actualShift};
}

Cell Game::boundCheckAntiDiag(Cell c, int shift) const {
    int size = (int)grid.size();
    if (shift > 0) {
        unsigned actualShift = std::min({c.x, size - 1 - c.y, (unsigned)shift});
        return {c.x - actualShift, c.y + actualShift};
    }

    unsigned actualShift = std::min({size - 1 - c.x, c.y, (unsigned)(-shift)});
    return {c.x + actualShift, c.y - actualShift};
}

Cell Game::getWinningMove() const {
    return winningMove;
}
