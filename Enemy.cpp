#include <SFML/Graphics.hpp>
#include <stdexcept>
#include "Enemy.h"

using namespace sf;

Enemy::Enemy(const std::string& textureFile, Vector2f startPos)
    : position(startPos), hp(100), facingRight(true), sprite()
    {
        if (!texture.loadFromFile("cupa.png")) {
            throw std::runtime_error("�̹����� �ҷ��� �� �����ϴ�: cupa.png");
        }
        sprite.setTexture(texture);
        sprite.setScale(1.5f, 1.5f);
        sprite.setPosition(position);
    }

void Enemy::update(float deltaTime, float groundY) {
    float speed = 50.f;
    if (facingRight) {
        position.x += speed * deltaTime;
        if (position.x > 500) facingRight = false;
    }
    else {
        position.x -= speed * deltaTime;
        if (position.x < 100) facingRight = true;
    }
    position.y = groundY - sprite.getGlobalBounds().height;  // �ٴڿ� ����
    sprite.setPosition(position);
}

void Enemy::draw(RenderWindow& window) {
    window.draw(sprite);
}

void Enemy::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) hp = 0;
}

int Enemy::getHp() const {
    return hp;
}

bool Enemy::isDead() const {
    return hp <= 0;
}

const Sprite& Enemy::getSprite() const {  
   return sprite;
}