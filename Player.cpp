#include <SFML/Graphics.hpp>
#include "Player.h"

using namespace sf;

Player::Player()    //player.h에 있는 player 클래스의 생성자
    : velocity(0.f, 0.f), speed(200.f), gravity(500.f),
    groundY(500.f), jumpPower(-300.f), isOnGround(false),
    currentFrame(100), frameTime(0.1f), frameTimer(0.f), frameCount(6),
    frameWidth(100), frameHeight(95), facingRight(true) {
    if (!texture.loadFromFile("Mario_SpraySheet.png")) {
        throw std::runtime_error("이미지를 불러올 수 없습니다: mario-2d.png");
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
    sprite.setScale(-1.5f, 1.5f);   // 오른쪽을 바라보도록 설정
	sprite.setOrigin(frameWidth / 2.f, 0.f);    // 스프라이트의 중심을 왼쪽 아래로 설정(좌우 반전 시 대칭 맞추기)
    sprite.setPosition(200.0f, 100.0f);
}


    // 키 입력 처리
void Player::handleInput(float deltaTime) {
    Vector2f direction(0.f, 0.f);
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        direction.x -= 1.f;
        facingRight = false;    // 왼쪽 보기
    }
    else if (Keyboard::isKeyPressed(Keyboard::Right)) {
        direction.x += 1.f;
		facingRight = true;   // 오른쪽 보기
    }

    velocity.x = direction.x * speed;
    sprite.move(direction * speed * deltaTime);

    // 방향에 따라 스프라이트 반전
    if (facingRight)
        sprite.setScale(-1.5f, 1.5f);   // 오른쪽 보기
    else
        sprite.setScale(1.5f, 1.5f);  // 왼쪽 보기 (좌우 반전)

    if (Keyboard::isKeyPressed(Keyboard::Space) && isOnGround) {
        velocity.y = jumpPower;
        isOnGround = false;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        // 게임 종료
        exit(0);
    }
}

void Player::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;

    // 움직이고 있는지 판단
    bool isMoving = std::abs(velocity.x) > 1.0f || Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Right);

    if (!isMoving) {
        // 멈춰 있을 때 → (1,5)
        sprite.setTextureRect(IntRect(4 * frameWidth, 0 * frameHeight, frameWidth, frameHeight));
    }
    else {
        // 움직일 때 → (1,0) ↔ (1,5)
        if (animationTimer >= frameDuration) {
            animationTimer = 0.05f;

            if (currentFrameIndex == 0) {
                sprite.setTextureRect(IntRect(4 * frameWidth, 0 * frameHeight, frameWidth, frameHeight));
                currentFrameIndex = 1;
            }
            else {
                sprite.setTextureRect(IntRect(1 * frameWidth, 0 * frameHeight, frameWidth, frameHeight));
                currentFrameIndex = 0;
            }
        }
    }
}
void Player::update(float deltaTime) {
    // 중력 적용
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

    updateAnimation(deltaTime);
}

void Player::draw(RenderWindow& window) {
    window.draw(sprite);
}

const Sprite& Player::getSprite() const {
    return sprite;
}