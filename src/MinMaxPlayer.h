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
#include <queue>

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

    enum class NodeType {
        MIN, MAX
    };

    struct MinMaxTree {
        static constexpr unsigned maxNodeCnt = 150000;
        Grid grid;
        std::vector<std::unique_ptr<MinMaxTree>> nodes;
        std::vector<Cell> moves;
        score_t bestScore;
        Cell bestMove;
        Marking marking;
        NodeType type;

        MinMaxTree(const Grid & g, Marking m, NodeType t) : grid(g), marking(m), type(t) {}
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

    };

    struct TypeMin : public Type {
        typedef TypeMax swap;
        static bool betterThan(score_t a, score_t b) {
            return a < b;
        }
    };

    static score_t score(unsigned val, NodeType t) {
        switch (t) {
            case NodeType::MIN:
                return -val;
            case NodeType::MAX:
                return val;
        }
    }

    std::unique_ptr<MinMaxTree> buildMinMaxTree(const Grid & grid, unsigned & nodeCnt);

    std::unique_ptr<MinMaxTree> constructMinMaxTreeNodes(const Grid & grid, unsigned & nodeCnt, Marking m);

    template <typename Type>
    void computeMinMaxOnTree(MinMaxTree & tree) {
        bool firstAvailable = true;

        unsigned i = 0;
        for (auto & nodeNext : tree.nodes) {
            Cell c = tree.moves[i++];

            computeMinMaxOnTree<typename Type::swap>(*nodeNext);

            if (firstAvailable || Type::betterThan(nodeNext->bestScore, tree.bestScore)) {
                tree.bestScore = nodeNext->bestScore;
                tree.bestMove = c;
                firstAvailable = false;
            }
        }
    }

//    Cell walkMinMaxTree(Cell move);

    std::unique_ptr<MinMaxTree> minMaxRoot;
    std::random_device rd;
    std::mt19937 rng;
    std::tuple<TypeMin, TypeMax> playerMark;
    unsigned rowLengthToWin;
};

#endif //PISKVORKY2017_MINMAXPLAYER_H
