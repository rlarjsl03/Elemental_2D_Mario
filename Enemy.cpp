#include "Enemy.h"
#include <stdexcept>

using namespace sf;

Enemy::Enemy(const std::string& textureFile, Vector2f startPos)
    : position(startPos), hp(100), facingRight(true), sprite()
{
    if (!texture.loadFromFile("cupa2.png")) {
        throw std::runtime_error("이미지를 불러올 수 없습니다: " + textureFile);
    }
    sprite.setTexture(texture);
    sprite.setScale(1.5f, 1.5f);
    sprite.setPosition(position);
}

void Enemy::update(float deltaTime, float groundY) {
    if (isDead()) return;

    float speed = 50.f;
    if (facingRight) {
        position.x += speed * deltaTime;
        if (position.x > 500) facingRight = false;
    }
    else {
        position.x -= speed * deltaTime;
        if (position.x < 100) facingRight = true;
    }

    position.y = groundY - sprite.getGlobalBounds().height;
    sprite.setPosition(position);
}

void Enemy::draw(RenderWindow& window) {
    if (isDead()) return;
    window.draw(sprite);
}

void Enemy::takeDamage(int amount) {
    hp -= amount;
    if (hp <= 0) {
        hp = 0;
        // 히트박스 제거 및 위치 이동
        sprite.setScale(0.f, 0.f);
        sprite.setPosition(-9999.f, -9999.f);
    }
}

int Enemy::getHp() const {
    return hp;
}

bool Enemy::isDead() const {
    return hp <= 0;
}

bool Enemy::isActive() const {
    return !isDead();
}

const Sprite& Enemy::getSprite() const {
    return sprite;
}
