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
        // ���� ���� �ϴ� ȭ�� ������ ġ��
        sprite.setPosition(-9999.f, -9999.f);

        // ���� �� ���� �Ÿ� ������ ������
        position.x += (facingRight ? speed : -speed) * deltaTime;

        if (position.x < -100.f || position.x > 2000.f) {
            hp = 100;
            position.x = 1920.f + static_cast<float>(rand() % 500);
            facingRight = rand() % 2 == 0;
        }
        return;
    }
    // ����ִ� ���� ������
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
        hp = 0; // HP�� ������ ���� �ʵ��� ����
        sprite.setPosition(-9999.f, -9999.f);  // �׾��� �� ���� ���� ����
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
    bounds.left += 10.f; // �¿� ���� ����
    bounds.width -= 20.f;
    bounds.top += 10.f;  // ���� ���� ����
    bounds.height -= 15.f;
    return bounds;
}

