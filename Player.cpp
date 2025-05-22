#include <SFML/Graphics.hpp>
#include "Player.h"

using namespace sf;

Player::Player()
    : velocity(0.f, 0.f), speed(200.f), gravity(500.f),
    groundY(500.f), jumpPower(-300.f), isOnGround(false) {
    if (!texture.loadFromFile("mario-2d.png")) {
        throw std::runtime_error("이미지를 불러올 수 없습니다: mario-2d.png");
    }
    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f);
    sprite.setPosition(200.0f, 100.0f);
}
    // 키 입력 처리
void Player::handleInput(float deltaTime) {
    Vector2f direction(0.f, 0.f);
    if (Keyboard::isKeyPressed(Keyboard::Left))
        direction.x -= 1.f;
    else if (Keyboard::isKeyPressed(Keyboard::Right))
        direction.x += 1.f;

    sprite.move(direction * speed * deltaTime);

    if (Keyboard::isKeyPressed(Keyboard::Space) && isOnGround) {
        velocity.y = jumpPower;
        isOnGround = false;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        // 게임 종료
        exit(0);
    }
}

    // 중력 적용
    void Player::update(float deltaTime) {
        velocity.y += gravity * deltaTime;
        sprite.move(0.f, velocity.y * deltaTime);

        // 바닥 충돌 처리
        float bottom = sprite.getPosition().y + sprite.getGlobalBounds().height;
        if (bottom >= groundY) {
            sprite.setPosition(sprite.getPosition().x, groundY - sprite.getGlobalBounds().height);
            velocity.y = 0;
            isOnGround = true;
        }
        else
            isOnGround = false;
    }

void Player::draw(RenderWindow& window) {
    window.draw(sprite);
}