#include "../include/project/Player.hpp"
#include <iostream>
#include <cmath>

Player::Player() { }

int Player::getLife() const { return m_life; }
int Player::getPoints() const { return m_points; }
int Player::getMaxLives() const { return m_maxLives; }

void Player::setLife(int life) { m_life = life; }
void Player::setPoints(int points) { m_points = points; }

void Player::initialization() {

    this->setLife(3);
    this->setPoints(0);
}

void Player::gainPoints(unsigned int points) {
    if (m_points + points >= 10000*(ceil(m_points/10000)+1)) gainLife();
    m_points += points;
}

void Player::losePoints(int points) {
    m_points += points;
}

void Player::gainLife() {
    if (m_life < m_maxLives) m_life++;
}

void Player::loseLife() {
    if (m_life > 0) m_life --;
}
