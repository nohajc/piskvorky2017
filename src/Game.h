//
// Created by nohajc on 14.4.17.
//

#ifndef PISKVORKY2017_GAME_H
#define PISKVORKY2017_GAME_H

#include <memory>
#include <functional>
#include <utility>
#include <string>

#include <value.hpp>

#include "Grid.h"

class Game {
public:
    typedef std::function<void(Cell, Player::marking_t, const std::string &)> UpdateHandlerFunc;

    Game(grid_size_t n, unsigned rowLength, std::unique_ptr<Player> p1, std::unique_ptr<Player> p2);

    void play();
    void setUpdateHandler(UpdateHandlerFunc update);
    Cell getWinningMove() const;

private:
    std::unique_ptr<Player> player1, player2;
    std::reference_wrapper<Player> currentPlayer, theOtherPlayer;
    Grid grid;
    unsigned rowLengthToWin;
    bool gameOver;
    Cell winningMove;
    std::string gameStatus;
    unsigned moveCounter;
    unsigned movesMax;

    static const std::string STATUS_OVER;
    static const std::string STATUS_IN_PROGRESS;
    static const std::string STATUS_DRAW;

    UpdateHandlerFunc updateHandler;

    // Attempt to do a next move
    // Returns true if the move was made (that means it must also have been valid),
    // returns false if the move was invalid.
    bool nextMove(Player & currentPlayer);
    // Update callback (for notifying UI of the last move)
    void update(Cell c) const {
        if (updateHandler) {
            updateHandler(c, Player::marking_t(grid[c]), gameStatus);
        }
    }

    bool checkVictory(Cell c) const;

    template<typename I>
    bool gridCheckForRow(I from, I to, Marking marking) const {
        unsigned rowLength = 0;

        for (auto it = from; it <= to; it++) {
            if (*it == marking) {
                if ((++rowLength) >= rowLengthToWin) {
                    return true;
                }
            }
            else {
                rowLength = 0;
            }
        }
        return false;
    }
};


#endif //PISKVORKY2017_GAME_H
