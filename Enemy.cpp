#include "Enemy.h"
#include <random>

Enemy::Enemy(std::shared_ptr<sf::Texture> texturePtr, sf::Vector2f startPos)
    : texture(texturePtr), position(startPos), hp(100)
{
    sprite.setTexture(*texture);
    sprite.setScale(1.5f, 1.5f);
    sprite.setPosition(position);

    // 랜덤 엔진과 분포(속도, 방향)
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> speedDist(30.f, 100.f);
    std::bernoulli_distribution dirDist(0.5);

    speed = speedDist(gen);
    facingRight = dirDist(gen);
}

void Enemy::update(float deltaTime, float groundY) {
    if (isDead()) return;

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

void Enemy::draw(sf::RenderWindow& window) {
    if (isDead()) return;
    window.draw(sprite);
}

void Enemy::takeDamage(int amount) {
    hp -= amount;
    if (hp <= 0) {
        hp = 0;
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

const sf::Sprite& Enemy::getSprite() const {
    return sprite;
}
