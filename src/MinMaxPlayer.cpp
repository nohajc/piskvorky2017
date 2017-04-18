//
// Created by nohajc on 17.4.17.
//

#include "MinMaxPlayer.h"
#include "Game.h"

#include <chrono>
#include <thread>

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


Cell MinMaxPlayer::proposeMove(const Game & g) {
    using namespace std::chrono_literals;

    const Grid & grid = g.getGrid();
    std::uniform_int_distribution<int> uni(0, grid.size() - 1);

    // Add some delay
    sleep_for(700ms);
    return {uni(rng), uni(rng)};
}
