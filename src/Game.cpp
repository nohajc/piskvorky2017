//
// Created by nohajc on 14.4.17.
//

#include <vector>
#include <algorithm>
#include <cstdio>

#include "Game.h"

// TODO: add bounds checking
// n >= 3, rowLength >= 3 and rowLength <= n
Game::Game(grid_size_t n,
           unsigned rowLength,
           std::unique_ptr<Player> p1,
           std::unique_ptr<Player> p2)
        : player1(std::move(p1)), player2(std::move(p2)), grid(n) {
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
        while (!nextMove(*player1));
        if (gameOver) break;
        while (!nextMove(*player2));
        if (gameOver) break;
    }
}

bool Game::nextMove(Player & currentPlayer) {
    Cell c = currentPlayer.proposeMove(*this);
    if (grid[c] == Marking::EMPTY) {
        grid[c] = Marking(currentPlayer.getMarking());
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
    // Do four sweeps around the newly filled cell
    // (horizontal, vertical and two diagonal)
    // and search for a row of crosses or naughts
    Marking marking = grid[c];
    int radius = rowLengthToWin - 1;

//    printf("c.x = %u, c.y = %u, leftX = %d, rightX = %d, topY = %d, bottomY = %d\n",
//           c.x, c.y, leftX, rightX, topY, bottomY);

    // TODO: move bound checking to Grid, unify interface (row_at, col_at, ...)
    auto left = grid.row_at(c.y, grid.boundCheck(c.x - radius));
    auto right = grid.row_at(c.y, grid.boundCheck(c.x + radius));

    auto top = grid.col_at(grid.boundCheck(c.y - radius), c.x);
    auto bottom = grid.col_at(grid.boundCheck(c.y + radius), c.x);

    Cell chkTopLeft = grid.boundCheckMainDiag(c, -radius);
    auto topLeft = grid.main_diag_at(chkTopLeft.y, chkTopLeft.x);
    Cell chkBottomRight = grid.boundCheckMainDiag(c, radius);
    auto bottomRight = grid.main_diag_at(chkBottomRight.y, chkBottomRight.x);

    Cell chkTopRight = grid.boundCheckAntiDiag(c, -radius);
    auto topRight = grid.anti_diag_at(chkTopRight.y, chkTopRight.x);
    Cell chkBottomLeft = grid.boundCheckAntiDiag(c, radius);
    auto bottomLeft = grid.anti_diag_at(chkBottomLeft.y, chkBottomLeft.x);

    return gridCheckForRow(left, right, marking)
            || gridCheckForRow(top, bottom, marking)
            || gridCheckForRow(topLeft, bottomRight, marking)
            || gridCheckForRow(topRight, bottomLeft, marking);
}

Cell Game::getWinningMove() const {
    return winningMove;
}

const std::string Game::STATUS_OVER = "OVER";
const std::string Game::STATUS_IN_PROGRESS = "IN_PROGRESS";
const std::string Game::STATUS_DRAW = "DRAW";