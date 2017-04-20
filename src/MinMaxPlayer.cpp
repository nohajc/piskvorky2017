//
// Created by nohajc on 17.4.17.
//

#include "MinMaxPlayer.h"
#include "Game.h"

#include <chrono>
#include <thread>
#include <algorithm>

typedef std::chrono::high_resolution_clock HrClock;
template <typename T>
using duration = std::chrono::duration<T>;

static void sleep_for_from(std::chrono::duration<double> dt, HrClock::time_point from)
{
    static constexpr duration<double> minSleepDuration(0);
    while (duration<double>(HrClock::now() - from) < dt) {
        std::this_thread::sleep_for(minSleepDuration);
    }
}

using namespace std::chrono_literals;

void MinMaxPlayer::setMarking(Player::marking_t m) {
    Player::setMarking(m);

    std::get<TypeMax>(playerMark).marking = Marking(m);
    std::get<TypeMin>(playerMark).marking = m == Player::NAUGHT ? Marking::CROSS : Marking::NAUGHT;
}

Cell MinMaxPlayer::proposeMove(const Game & g) {
    Cell res;
    HrClock::time_point start = HrClock::now();

    if (g.getMoveCounter() == 0) {
        // We do the first move (randomly).
        const Grid & grid = g.getGrid();
        std::uniform_int_distribution<int> uni(0, grid.size() - 1);

        res = {uni(rng), uni(rng)};
    }
    else {
        res = findBestMove(g);
    }

    sleep_for_from(700ms, start);

    return res;
}

Cell MinMaxPlayer::findBestMove(const Game & g) {
    rowLengthToWin = g.getRowLengthToWin();

//    if (minMaxRoot) {
//        // This is the move of the other player
//        // which actually happened in the last round.
//        walkMinMaxTree(g.getLastMove());
//    }

    unsigned nodeCnt = 0;
    HrClock::time_point start = HrClock::now();
    minMaxRoot = buildMinMaxTree(g.getGrid(), nodeCnt);
    auto d = duration<double>(HrClock::now() - start);

//    printf("Created %u game tree nodes in %f.\n", nodeCnt, d.count());

//    return walkMinMaxTree(minMaxRoot->bestMove);
    return minMaxRoot->bestMove;
}

std::unique_ptr<MinMaxPlayer::MinMaxTree> MinMaxPlayer::buildMinMaxTree(const Grid &grid, unsigned &nodeCnt) {
    // 1st phase: build (incomplete) game tree in BFS order, assign scores to leaf nodes
    auto tree = constructMinMaxTreeNodes(grid, nodeCnt, std::get<TypeMax>(playerMark).marking);
    // 2nd phase: compute the best move for each inner node based on the leaf score
    computeMinMaxOnTree<TypeMax>(*tree);
    return tree;
}

std::unique_ptr<MinMaxPlayer::MinMaxTree>
MinMaxPlayer::constructMinMaxTreeNodes(const Grid &grid, unsigned &nodeCnt, Marking m) {
    auto tree = std::make_unique<MinMaxTree>(grid, m, NodeType::MAX);
    nodeCnt++;

    grid_size_t gridSize = grid.size();
    bool tooManyNodes = false;

    std::queue<MinMaxTree*> q;
    q.push(tree.get());

    while (!q.empty()) {
        MinMaxTree * node = q.front();
        q.pop();

        bool boardFull = true;
        Grid & nodeGrid = node->grid;
        Marking nodeMarking = node->marking;
        NodeType nodeType = node->type;

        for (int y = 0; y < gridSize; y++) {
            for (int x = 0; x < gridSize; x++) {
                Cell c = {x, y};
                if (nodeGrid[c] == Marking::EMPTY) {
                    boardFull = false;
                    Grid gridNext = nodeGrid;
                    gridNext[c] = nodeMarking;

                    std::unique_ptr<MinMaxTree> nodeNext;
                    unsigned longestRow = gridNext.longestRowAround(c, rowLengthToWin - 1);

                    tooManyNodes = nodeCnt >= MinMaxTree::maxNodeCnt - 1;

                    // Allocate new node
                    nodeNext = std::make_unique<MinMaxTree>(
                            gridNext,
                            nodeMarking == Marking::NAUGHT ? Marking::CROSS : Marking::NAUGHT,
                            nodeType == NodeType::MAX ? NodeType::MIN : NodeType::MAX
                    );
                    nodeCnt++;

                    if (longestRow >= rowLengthToWin || tooManyNodes) {
                        // Leaf
                        nodeNext->bestScore = score(longestRow, nodeType);
                    } else {
                        // Inner node
                        q.push(nodeNext.get());
                    }

                    node->moves.push_back(c);
                    node->nodes.push_back(std::move(nodeNext));

                    if (tooManyNodes) {
                        return tree;
                    }
                }
            }
        }

        if (boardFull) { // Draw
            node->bestScore = 0;
        }
    }

    return tree;
}

//Cell MinMaxPlayer::walkMinMaxTree(Cell move) {
//    auto moves = minMaxRoot->moves;
//    auto it = std::find(moves.begin(), moves.end(), move);
//    if (it == moves.end()) {
//        printf("No such move in the tree\n.");
//        throw 0;
//    }
//    long idx = it - moves.begin();
//    // First, detach node from the list of children in root
//    auto newRoot = std::move(minMaxRoot->nodes[idx]);
//    minMaxRoot->nodes[idx] = nullptr;
//    // then overwrite the original root (deleting all nodes except the detached subtree)
//    minMaxRoot = std::move(newRoot);
//
//    return move;
//}
