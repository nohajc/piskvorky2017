//
// Created by nohajc on 14.4.17.
//

#include <thread>
#include <utility>

#include "Frame.h"
#include "HumanPlayer.h"

static RECT defaultRect = { 100, 100, 400, 400 };

Frame::Frame() : window( SW_MAIN | SW_TITLEBAR | SW_CONTROLS | SW_ENABLE_DEBUG, defaultRect) {}

void Frame::gameLoop() {
    std::function<bool()> job;

    while (true) {
        std::unique_lock<std::mutex> uLock(gameMutex);
        gameCondVar.wait(uLock, [this] { return jobAvailable; });

        job = std::move(gameJob);
        gameJob = nullptr;
        jobAvailable = false;

        uLock.unlock();

        if (!job()) {
            return;
        }
    }
}

bool Frame::startGameLoop() {
    jobAvailable = false;
    gameLoopThread = std::thread([this] { gameLoop(); });
    return true;
}

// TODO: add parameters to configure the game,
// make available to UI script.
sciter::value Frame::startGame(sciter::value n, sciter::value k) {
    auto addOneClickHnd = [this](HumanPlayer::ClickCallbackFunc callback) {
        call_function("addOneClickHandler", sciter::vfunc(callback));
    };

    std::unique_ptr<Player> p1 = std::make_unique<HumanPlayer>(addOneClickHnd);
    std::unique_ptr<Player> p2 = std::make_unique<HumanPlayer>(addOneClickHnd);

    game = std::make_unique<Game>(n.get<int>(), k.get<int>(), std::move(p1), std::move(p2));

    game->setUpdateHandler([this](Cell c, Player::marking_t mark, const std::string & gameStatus) {
        call_function("updateGrid", int(c.x), int(c.y), mark, gameStatus);
    });

    scheduleGameJob([this] {
        gameInProgress = true;
        game->play();
        gameInProgress = false;
        return false;
    });
}

bool Frame::endGameLoop() {
    scheduleGameJob([] { return false; });
    if (gameInProgress) {
        gameLoopThread.detach();
    }
    else {
        gameLoopThread.join();
    }
    return true;
}
