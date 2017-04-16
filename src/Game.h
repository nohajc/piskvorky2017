//
// Created by nohajc on 14.4.17.
//

#ifndef PISKVORKY2017_GAME_H
#define PISKVORKY2017_GAME_H

#include <memory>
#include <functional>
#include <utility>

#include "Grid.h"

class Game {
public:
    typedef std::function<void(Cell, Player::marking_t)> UpdateHandlerFunc;

    Game(grid_size_t n, std::unique_ptr<Player> p1, std::unique_ptr<Player> p2, unsigned rowLength = 3);

    grid_size_t getGridSize() const;
    void play();
    void setUpdateHandler(UpdateHandlerFunc update);

private:
    std::unique_ptr<Player> player1, player2;
    Grid grid;
    unsigned rowLengthToWin;
    bool gameOver;

    UpdateHandlerFunc updateHandler;

    // Attempt to do a next move
    // Returns true if the move was made (that means it must also have been valid),
    // returns false if the move was invalid.
    bool nextMove(const Player & currentPlayer);
    // Update callback (for notifying UI of the last move)
    void update(Cell c) {
        if (updateHandler) {
            updateHandler(c, Player::marking_t(grid[c]));
        }
    }

    bool checkVictory(Cell c);
    grid_size_t boundCheck(int x);
};


#endif //PISKVORKY2017_GAME_H
