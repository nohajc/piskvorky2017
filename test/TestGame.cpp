//
// Created by nohajc on 16.4.17.
//

#include <vector>

#include <Player.h>
#include <Game.h>

#include "TestGame.h"

// TODO: move to a separate compilation target

#define TEST_ASSERT(idx, cond) if (cond) { \
    printf("Test %d OK.\n", (idx)); \
} \
else { \
    printf("Test %d FAILED.\n", (idx)); \
}

// Player that plays a predefined game
class TestPlayer : public Player {
    std::vector<Cell> recordedMoves;
    std::vector<Cell>::iterator move;
public:
    TestPlayer(const std::vector<Cell> & moves) {
        recordedMoves = moves;
        move = recordedMoves.begin();
    }

    virtual Cell proposeMove(const Game &) override {
        if (move == recordedMoves.end()) return {};
        return *move++;
    };
};

void playGame(int testIdx,
              const std::vector<Cell> & moves1,
              const std::vector<Cell> & moves2,
              Cell expectedWinningMove) {

    std::unique_ptr<Player> p1 = std::make_unique<TestPlayer>(moves1);
    std::unique_ptr<Player> p2 = std::make_unique<TestPlayer>(moves2);

    auto game = std::make_unique<Game>(5, 3, std::move(p1), std::move(p2));
    game->play();
    TEST_ASSERT(testIdx, game->getWinningMove() == expectedWinningMove);
}

void testEntryPoint() {
    playGame(1,
             {{0, 2}, {0, 1}, {1, 0}},
             {{1, 2}, {2, 1}, {3, 0}},
             {3, 0});
}
