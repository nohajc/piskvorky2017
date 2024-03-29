//
// Created by nohajc on 14.4.17.
//

#ifndef PISKVORKY2017_PLAYER_H
#define PISKVORKY2017_PLAYER_H

struct Cell {
    unsigned x, y;

    Cell(int cx, int cy) {
        x = (unsigned)cx;
        y = (unsigned)cy;
    }

    Cell(unsigned cx, unsigned cy) {
        x = cx;
        y = cy;
    }

    Cell() {
        x = 0;
        y = 0;
    }

    bool operator==(const Cell & other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Cell & other) const {
        return !(*this == other);
    }
};

class Game;

class Player {
public:
    enum marking_t {NAUGHT = 1, CROSS = 2};

    virtual void setMarking(marking_t m);
    marking_t getMarking() const;
    virtual Cell proposeMove(const Game &) = 0;
    virtual ~Player() = default;

private:
    marking_t marking;
};


#endif //PISKVORKY2017_PLAYER_H
