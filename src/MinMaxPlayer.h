//
// Created by nohajc on 17.4.17.
//

#ifndef PISKVORKY2017_MINMAXPLAYER_H
#define PISKVORKY2017_MINMAXPLAYER_H

#include <memory>
#include <vector>
#include <random>
#include <utility>
#include <tuple>

#include "Player.h"
#include "Grid.h"

// This is the AI player
class MinMaxPlayer : public Player {
public:
    MinMaxPlayer() : rng(rd()) {

    }
    virtual Cell proposeMove(const Game & g) override;
    virtual void setMarking(Player::marking_t m) override;

private:
    typedef int score_t;

    struct MinMaxTree {
        Grid grid;
        std::vector<std::unique_ptr<MinMaxTree>> nodes;
        std::vector<Cell> moves; // TODO: make this a map?
        score_t bestScore;
        Cell bestMove;

        MinMaxTree(const Grid & g) : grid(g) {}
    };

    Cell findBestMove(const Game & g);

    struct Type {
        Marking marking;
    };

    struct TypeMin;

    struct TypeMax : public Type {
        typedef TypeMin swap;
        static bool betterThan(score_t a, score_t b) {
            return a > b;
        }
        static score_t score(unsigned val) {
            return val;
        }
    };

    struct TypeMin : public Type {
        typedef TypeMax swap;
        static bool betterThan(score_t a, score_t b) {
            return a < b;
        }
        static score_t score(unsigned val) {
            return -val;
        }
    };

    template <typename Type>
    std::unique_ptr<MinMaxTree> buildMinMaxTree(const Grid & grid) {
        auto tree = std::make_unique<MinMaxTree>(grid);
        grid_size_t gridSize = grid.size();
        bool firstAvailable = true;
        bool boardFull = true;

        for (int y = 0; y < gridSize; y++) {
            for (int x = 0; x < gridSize; x++) {
                Cell c = {x, y};
                if (grid[c] == Marking::EMPTY) {
                    boardFull = false;
                    Grid gridNext = grid;
                    gridNext[c] = std::get<Type>(playerMark).marking;

                    std::unique_ptr<MinMaxTree> nodeNext;
                    if (gridNext.longestRowAround(c, rowLengthToWin - 1) >= rowLengthToWin) {
                        nodeNext = std::make_unique<MinMaxTree>(gridNext); // Leaf
                        nodeNext->bestScore = Type::score(rowLengthToWin);
                    }
                    else {
                        nodeNext = buildMinMaxTree<typename Type::swap>(gridNext);
                    }

                    if (firstAvailable || Type::betterThan(nodeNext->bestScore, tree->bestScore)) {
                        tree->bestScore = nodeNext->bestScore;
                        tree->bestMove = c;
                        firstAvailable = false;
                    }
                    tree->moves.push_back(c);
                    tree->nodes.push_back(std::move(nodeNext));
                }
            }
        }

        if (boardFull) { // Draw
            tree->bestScore = Type::score(0);
        }

        return tree;
    }

    Cell walkMinMaxTree(Cell move);

    std::unique_ptr<MinMaxTree> minMaxRoot;
    std::random_device rd;
    std::mt19937 rng;
    std::tuple<TypeMin, TypeMax> playerMark;
    unsigned rowLengthToWin;
};

#endif //PISKVORKY2017_MINMAXPLAYER_H
