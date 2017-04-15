//
// Created by nohajc on 14.4.17.
//

#include "Player.h"

void Player::setMarking(Player::marking_t m) {
    marking = m;
}

Player::marking_t Player::getMarking() const {
    return marking;
}
