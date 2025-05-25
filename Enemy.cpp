#include <SFML/Graphics.hpp>
#include <stdexcept>
#include "Enemy.h"

using namespace sf;

//class Player {
//private:
//    Texture texture;
//    Sprite sprite;
//    Vector2f velocity;
//    float speed;
//    float gravity;
//    float groundY;
//    float jumpPower;
//    bool isOnGround;
//
//public:
//    Player()
//        : velocity(0.f, 0.f), speed(200.f), gravity(500.f),
//        groundY(500.f), jumpPower(-300.f), isOnGround(false)
//    {
//        if (!texture.loadFromFile("cupa.png")) {
//            throw std::runtime_error("이미지를 불러올 수 없습니다: cupa.png");
//        }
//        sprite.setTexture(texture);
//        sprite.setScale(0.1f, 0.1f);
//        sprite.setPosition(200.f, 100.f);
//    }
//
//    void handleInput(float deltaTime) {
//        Vector2f direction(0.f, 0.f);
//        if (Keyboard::isKeyPressed(Keyboard::Left))
//            direction.x -= 1.f;
//        else if (Keyboard::isKeyPressed(Keyboard::Right))
//            direction.x += 1.f;
//
//        sprite.move(direction * speed * deltaTime);
//
//        if (Keyboard::isKeyPressed(Keyboard::Space) && isOnGround) {
//            velocity.y = jumpPower;
//            isOnGround = false;
//        }
//        else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
//            exit(0);
//        }
//    }
//
//    void update(float deltaTime) {
//        velocity.y += gravity * deltaTime;
//        sprite.move(0.f, velocity.y * deltaTime);
//
//        float bottom = sprite.getPosition().y + sprite.getGlobalBounds().height;
//        if (bottom >= groundY) {
//            sprite.setPosition(sprite.getPosition().x, groundY - sprite.getGlobalBounds().height);
//            velocity.y = 0;
//            isOnGround = true;
//        }
//        else {
//            isOnGround = false;
//        }
//    }
//
//    void draw(RenderWindow& window) {
//        window.draw(sprite);
//    }
//
//    const Sprite& getSprite() const {
//        return sprite;
//    }
//};



Enemy::Enemy(const std::string& textureFile, Vector2f startPos)
        : position(startPos), hp(100), facingRight(true)
    {
        if (!texture.loadFromFile("cupa.png")) {
            throw std::runtime_error("이미지를 불러올 수 없습니다: cupa.png");
        }
        sprite.setTexture(texture);
        sprite.setScale(0.1f, 0.1f);
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
    position.y = groundY - sprite.getGlobalBounds().height;  // 바닥에 고정
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

const Sprite& getSprite() const {
    return sprite;
}