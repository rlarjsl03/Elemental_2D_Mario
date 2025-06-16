#include "Enemy.h"
#include <cstdlib>

Enemy::Enemy(const sf::Texture& tex, sf::Vector2f startPos)
    : texture(tex), position(startPos), hp(100), facingRight(rand() % 2 == 0) {
    sprite.setTexture(texture);
    sprite.setScale(1.5f, 1.5f);
    sprite.setPosition(position);
}

void Enemy::update(float deltaTime, float groundY, const sf::Vector2f& playerPos) {
    float speed = 50.f;

    if (isDead()) {
        // 죽은 적은 화면 밖으로 이동 (충돌 비활성화)
        sprite.setPosition(-9999.f, -9999.f);
        return;
    }

    // 이동
    if (facingRight) {
        position.x += speed * deltaTime;
        if (position.x > playerPos.x + 300) facingRight = false;
    }
    else {
        position.x -= speed * deltaTime;
        if (position.x < playerPos.x - 300) facingRight = true;
    }

    position.y = groundY - sprite.getGlobalBounds().height;
    sprite.setPosition(position);
}

void Enemy::draw(sf::RenderWindow& window) {
    if (!isDead())
        window.draw(sprite);
}

void Enemy::takeDamage(int amount) {
    hp -= amount;
    if (hp <= 0) {
        hp = 0;
        sprite.setPosition(-9999.f, -9999.f);  // 충돌 제거
        respawnTimer.restart();
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
sf::FloatRect Enemy::getHitBox() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    bounds.left += 20.f; // 좌우 여백 제거
    bounds.width -= 20.f;
    //bounds.top += 10.f;  // 위쪽 여백 제거
    //bounds.height -= 15.f;
    return bounds;
}

bool Enemy::canRespawn() const {
    return respawnTimer.getElapsedTime().asSeconds() >= respawnDelay;
}

void Enemy::respawnAt(sf::Vector2f newPos) {
    hp = 100;
    position = newPos;
    facingRight = rand() % 2 == 0;
    sprite.setPosition(position);
    respawnTimer.restart();
}
