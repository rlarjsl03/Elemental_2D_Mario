#include "Enemy.h"
#include <cstdlib>

Enemy::Enemy(const sf::Texture& tex, sf::Vector2f startPos)
    : texture(tex), position(startPos), hp(100), facingRight(rand() % 2 == 0) {
    sprite.setTexture(texture);
    sprite.setScale(1.5f, 1.5f);
    sprite.setPosition(position);
}
void Enemy::update(float deltaTime, float groundY) {
    float speed = 50.f;

    if (isDead()) {
        // 죽은 적은 일단 화면 밖으로 치움
        sprite.setPosition(-9999.f, -9999.f);

        // 죽은 후 일정 거리 지나면 리스폰
        position.x += (facingRight ? speed : -speed) * deltaTime;

        if (position.x < -100.f || position.x > 2000.f) {
            hp = 100;
            position.x = 1920.f + static_cast<float>(rand() % 500);
            facingRight = rand() % 2 == 0;
        }
        return;
    }
    // 살아있는 적의 움직임
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
    if (!isDead())
        window.draw(sprite);
}

void Enemy::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) {
        hp = 0; // HP가 음수가 되지 않도록 보장
        sprite.setPosition(-9999.f, -9999.f);  // 죽었을 때 센서 완전 제거
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
    bounds.left += 10.f; // 좌우 여백 제거
    bounds.width -= 20.f;
    bounds.top += 10.f;  // 위쪽 여백 제거
    bounds.height -= 15.f;
    return bounds;
}

