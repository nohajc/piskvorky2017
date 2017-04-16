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
};

class Game;

class Player {
public:
    enum marking_t {NAUGHT = 1, CROSS = 2};

    void setMarking(marking_t m);
    marking_t getMarking() const;
    virtual Cell proposeMove(const Game & g) const = 0;

private:
    marking_t marking;
};


#endif //PISKVORKY2017_PLAYER_H
