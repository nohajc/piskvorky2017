//
// Created by nohajc on 14.4.17.
//

#ifndef PISKVORKY2017_FRAME_H
#define PISKVORKY2017_FRAME_H

#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <sciter-x-window.hpp>

#include "Game.h"


class Frame: public sciter::window {
    std::unique_ptr<Game> game;
    std::thread gameLoopThread;
    std::mutex gameMutex;
    std::condition_variable gameCondVar;
    bool jobAvailable;
    std::function<bool(void)> gameJob;
    std::atomic<bool> gameInProgress;

    void gameLoop();

    template<typename F>
    void scheduleGameJob(F job) {
        {
            std::lock_guard<std::mutex> lockG(gameMutex);

            gameJob = job;
            jobAvailable = true;
        }
        gameCondVar.notify_one();
    }
public:
    Frame();
    void startGameLoop();
    sciter::value startGame(sciter::value n, sciter::value k);
    bool endGameLoop();

    BEGIN_FUNCTION_MAP
    FUNCTION_2("startGame", startGame);
    FUNCTION_0("endGameLoop", endGameLoop);
    END_FUNCTION_MAP
};

#endif //PISKVORKY2017_FRAME_H
