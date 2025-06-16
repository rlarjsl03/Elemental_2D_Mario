#include <SFML/Graphics.hpp>
#include "Player.h"
#include <stdexcept> // std::runtime_error를 위해 포함
#include <cmath>     // std::abs를 위해 포함
#include <iostream>  // 디버깅용

using namespace sf;

Player::Player()
    : velocity(0.f, 0.f), speed(500.f), gravity(800.f),
    groundY(920.f), jumpPower(-700.f), isOnGround(false),
    currentFrame(100), frameTime(0.1f), frameTimer(0.f), frameCount(6),
    frameWidth(100), frameHeight(106), facingRight(true),
    score(0), drawSprite(true), isDead(false), life(1), isBig(false), // 추가된 변수 초기화
    isInvincible(false), invincibilityTimer(0.f), flickerTimer(0.f) // 무적 관련 변수 초기화
    ,col(0), row(0)
{
    if (!texture.loadFromFile("Mario_SpraySheet_padded_top.png")) {
        throw std::runtime_error("이미지를 불러올 수 없습니다: Mario_SpraySheet_padded_top.png");
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
    sprite.setScale(-1.5f, 1.5f); // 오른쪽을 바라보도록 설정
    sprite.setOrigin(frameWidth / 2.f, 0.f); // 스프라이트의 중심을 왼쪽 아래로 설정(좌우 반전 시 대칭 맞추기)
}

// 키 입력 처리
void Player::handleInput(float deltaTime) {
    Vector2f direction(0.f, 0.f);
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        direction.x -= 1.f;
        facingRight = false; // 왼쪽 보기
    }
    else if (Keyboard::isKeyPressed(Keyboard::Right)) {
        direction.x += 1.f;
        facingRight = true; // 오른쪽 보기
    }

    velocity.x = direction.x * speed; // 플레이어의 x 속도 업데이트
    sprite.move(direction * speed * deltaTime);

    // 방향에 따라 스프라이트 반전
    if (facingRight)
        sprite.setScale(-1.5f, 1.5f); // 오른쪽 보기 (원본 텍스처가 왼쪽을 보게 되어있다면 -1.5f, 1.5f가 오른쪽)
    else
        sprite.setScale(1.5f, 1.5f); // 왼쪽 보기 (좌우 반전)

    if (Keyboard::isKeyPressed(Keyboard::Space) && isOnGround) {
        velocity.y = jumpPower;
        isOnGround = false;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        exit(0); // 게임 종료
    }
}

void Player::setFrame(int frameX, int frameY) {
    int srcX = frameX * frameWidth;
    int srcY = frameY * frameHeight + yOffset;
    sprite.setTextureRect(IntRect(srcX, srcY, frameWidth, visibleHeight));
}

void Player::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;

    // 움직이고 있는지 판단 (속도나 키 입력으로)
    bool isMoving = std::abs(velocity.x) > 1.0f || Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Right);
    int frameY = isBig ? 2 : 0;  // 큰 마리오면 2행, 아니면 0행 (스프라이트 시트 기준)

    if (!isMoving) {
        // 멈춰 있을 때 → (4,0) 또는 (4,2)
        setFrame(4, frameY);
    }
    else {
        // 움직일 때 → (4,0) ↔ (1,0) 또는 (4,2) ↔ (1,2)
        if (animationTimer >= frameDuration) {
            animationTimer = 0.f; // 타이머 리셋

            if (currentFrameIndex == 0) {
                setFrame(4, frameY);
                currentFrameIndex = 1;
            }
            else {
                setFrame(1, frameY);
                currentFrameIndex = 0;
            }
        }
    }
}

void Player::update(float deltaTime) {
    // 중력 적용
    velocity.y += gravity * deltaTime;
    sprite.move(0.f, velocity.y * deltaTime);

    // 무적 상태 확인
    if (isInvincible) {
        invincibilityTimer += deltaTime;
        flickerTimer += deltaTime;

        if (flickerTimer >= flickerInterval) {
            drawSprite = !drawSprite; // 스프라이트 그릴지 말지 토글
            flickerTimer -= flickerInterval; // 타이머 감소 (잔여 시간으로 만듦)
        }

        if (invincibilityTimer >= invincibilityDuration) {
            isInvincible = false;
            invincibilityTimer = 0.f; // 타이머를 0으로 초기화
            drawSprite = true; // 무적 끝나면 다시 보이게
            flickerTimer = 0.f; // 깜빡임 타이머도 초기화
        }
    }
    else
		setInvincible(false);   // 무적 상태가 아니면 무적 타이머 초기화

    // 바닥 충돌 처리
    float bottom = sprite.getPosition().y + sprite.getGlobalBounds().height;
    if (bottom >= groundY) {
        sprite.setPosition(sprite.getPosition().x, groundY - sprite.getGlobalBounds().height);
        velocity.y = 0;
        isOnGround = true;
    }
    else {
        isOnGround = false;
    }

    updateAnimation(deltaTime);
}

void Player::takeDamage(int amount) {
    // 플레이어 데미지 처리 (필요시 구현)
}

void Player::bounceJump() {
    velocity.y = jumpPower;
    isOnGround = false;
}

void Player::draw(RenderWindow& window) {
    if (isInvincible) {
        // 일정 주기마다 깜빡임: 0.1초 간격
        if (static_cast<int>(invincibilityTimer * 10) % 2 == 0) {
            window.draw(sprite); // 깜빡임 효과
        }
    }
    else {
        window.draw(sprite);
    }
}
const Sprite& Player::getSprite() const {
    return sprite;
}

void Player::setScale(float scale) {
    sprite.setScale((facingRight ? -1 : 1) * scale, scale);
}

void Player::addScore(int amount) {
    score += amount;
}
int Player::getScore() const {
    return score;
}
// --- 새로 추가된 함수들 구현 ---
// 플레이어 위치 설정 함수 구현
void Player::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}
// 플레이어 현재 속도 반환 함수 구현
Vector2f Player::getVelocity() const {
    return velocity; // m_velocity 반환
}

// 플레이어의 현재 위치를 반환하는 함수 추가
sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

// 플레이어의 전역 바운딩 박스를 반환하는 함수 추가
sf::FloatRect Player::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

void Player::die() {
    isDead = true;
    // 죽었을 때 추가로 사운드 재생, 애니메이션 등도 여기에 구현 가능
}
bool Player::isAlive() const {
    return !isDead;
}
void Player::increaseLife(int amount) {
    life += amount;
}
void Player::loseLife(int amount) {
    life -= amount;
    if (life < 0) {
        life = 0; // 생명이 0 이하로 떨어지지 않도록 보장
    }
    getLife();
}
int Player::getLife() const {
    return life;
}
bool Player::getisWin() {
    return isWin;
}
void Player::setisWin() {
    isWin = true;
}
bool Player::getIsBig() const { return isBig; }
void Player::setIsBig(bool big) { 
    isBig = big; 
    getIsBig(); 
}
void Player::setInvincible(bool value) {
    isInvincible = value;
    if (!value) {
        invincibilityTimer = 0.f;
        flickerTimer = 0.f; // 깜빡임도 초기화
    }
}
bool Player::getisInvincible() const {
    return isInvincible;
}

// setVelocityY 함수 구현 (player.cpp 에 추가)
void Player::setVelocityY(float y) {
    velocity.y = y;
}

FloatRect Player::getHitBox() const {
    FloatRect bounds = sprite.getGlobalBounds();
    bounds.left += 20.f; // 좌우 여백 제거
    bounds.width -= 20.f;
    bounds.top += 10.f;  // 위쪽 여백 제거
    bounds.height -= 12.f;
    return bounds;
}

