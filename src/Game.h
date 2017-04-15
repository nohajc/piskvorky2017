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
public:
    typedef std::function<void(Cell, Player::marking_t)> UpdateHandlerFunc;

private:
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
public:
    Game(grid_size_t n, std::unique_ptr<Player> p1, std::unique_ptr<Player> p2, unsigned rowLength = 3);

    grid_size_t getGridSize() const;
    void play();
    void setUpdateHandler(UpdateHandlerFunc update);
};


#endif //PISKVORKY2017_GAME_H
