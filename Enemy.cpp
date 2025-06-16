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
        // ���� ���� ȭ�� ������ �̵� (�浹 ��Ȱ��ȭ)
        sprite.setPosition(-9999.f, -9999.f);
        return;
    }

    // �̵�
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
        sprite.setPosition(-9999.f, -9999.f);  // �浹 ����
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
    bounds.left += 20.f; // �¿� ���� ����
    bounds.width -= 20.f;
    //bounds.top += 10.f;  // ���� ���� ����
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
