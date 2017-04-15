//
// Created by nohajc on 14.4.17.
//

#ifndef PISKVORKY2017_GAME_H
#define PISKVORKY2017_GAME_H

#include <memory>
#include <vector>
#include <functional>

#include "Player.h"

typedef unsigned long grid_size_t;

class Game {
    enum class Marking {EMPTY = 0, NAUGHT = Player::NAUGHT, CROSS = Player::CROSS};

    class Grid {
        std::vector<std::vector<Marking>> data;
    public:
        Grid(grid_size_t n) : data(n, std::vector<Marking>(n)) {}
        // No bounds checking!
        Marking & operator[](Cell c) {
            return data[c.y][c.x];
        }
        // No bounds checking!
        const Marking & operator[](Cell c) const {
            return data[c.y][c.x];
        }

        grid_size_t size() const {
            return data.size();
        }
    };

    std::unique_ptr<Player> player1, player2;
    Grid grid;
    bool gameOver;

    typedef std::function<void(Cell, Player::marking_t)> UpdateCallBackFunc;
    UpdateCallBackFunc updateCallback;

    // Attempt to do a next move
    // Returns true if the move was made (that means it must also have been valid),
    // returns false if the move was invalid.
    bool nextMove(const Player & currentPlayer);
    // Update callback (for notifying UI of the last move)
    void update(Cell c) {
        if (updateCallback) {
            updateCallback(c, Player::marking_t(grid[c]));
        }
    }
public:
    Game(grid_size_t n, std::unique_ptr<Player> p1, std::unique_ptr<Player> p2);
    // TODO: give Game access to UI (some wrappers to specific frame::call_function instances)
    // Then we also want to provide means for HumanPlayer to handle onclick events.
    // AIPlayer, on the other hand, will need access to Grid cells in order to play.

    grid_size_t getGridSize() const;
    void play();
    void setUpdateCallback(UpdateCallBackFunc update);
};


#endif //PISKVORKY2017_GAME_H
