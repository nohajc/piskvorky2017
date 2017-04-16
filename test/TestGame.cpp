//
// Created by nohajc on 16.4.17.
//

#include <vector>

#include <Player.h>
#include <Game.h>

#include "TestGame.h"

#define TEST_ASSERT(cond) if (cond) { \
    printf("Test OK.\n"); \
} \
else { \
    printf("Test FAILED.\\n"); \
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

    virtual Cell proposeMove(const Game & g) override {
        if (move == recordedMoves.end()) return {};
        return *move++;
    };
};

void playGame(const std::vector<Cell> & moves1, const std::vector<Cell> & moves2, Cell expectedWinningMove) {
    std::unique_ptr<Player> p1 = std::make_unique<TestPlayer>(moves1);
    std::unique_ptr<Player> p2 = std::make_unique<TestPlayer>(moves2);

    auto game = std::make_unique<Game>(5, std::move(p1), std::move(p2));
    game->play();
    TEST_ASSERT(moves2.back() == expectedWinningMove);
}

void testEntryPoint() {
    playGame({{0, 2}, {0, 1}, {1, 0}},
             {{1, 2}, {2, 1}, {3, 0}},
             {3, 0});
}
