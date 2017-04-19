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

static void sleep_for(std::chrono::duration<double> dt)
{
    static constexpr duration<double> minSleepDuration(0);
    HrClock::time_point start = HrClock::now();
    while (duration<double>(HrClock::now() - start) < dt) {
        std::this_thread::sleep_for(minSleepDuration);
    }
}

void MinMaxPlayer::setMarking(Player::marking_t m) {
    Player::setMarking(m);

    std::get<TypeMax>(playerMark).marking = Marking(m);
    std::get<TypeMin>(playerMark).marking = m == Player::NAUGHT ? Marking::CROSS : Marking::NAUGHT;
}

Cell MinMaxPlayer::proposeMove(const Game & g) {
    using namespace std::chrono_literals;
    // Add some delay
    sleep_for(700ms);

    if (g.getMoveCounter() == 0) {
        // We do the first move (randomly).
        const Grid & grid = g.getGrid();
        std::uniform_int_distribution<int> uni(0, grid.size() - 1);

        return {uni(rng), uni(rng)};
    }

    return findBestMove(g);
}

Cell MinMaxPlayer::findBestMove(const Game & g) {
    rowLengthToWin = g.getRowLengthToWin();

    if (minMaxRoot) {
        // This is the move of the other player
        // which actually happened in the last round.
        walkMinMaxTree(g.getLastMove());
    }
    else {
        minMaxRoot = buildMinMaxTree<TypeMax>(g.getGrid());
    }

    return walkMinMaxTree(minMaxRoot->bestMove);
}

Cell MinMaxPlayer::walkMinMaxTree(Cell move) {
    auto moves = minMaxRoot->moves;
    auto it = std::find(moves.begin(), moves.end(), move);
    long idx = it - moves.begin();
    // First, detach node from the list of children in root
    auto newRoot = std::move(minMaxRoot->nodes[idx]);
    minMaxRoot->nodes[idx] = nullptr;
    // then overwrite the original root (deleting all nodes except the detached subtree)
    minMaxRoot = std::move(newRoot);

    return move;
}
