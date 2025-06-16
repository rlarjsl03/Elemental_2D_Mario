#include <SFML/Graphics.hpp>
#include "Player.h"
#include <stdexcept> // std::runtime_error를 위해 포함
#include <cmath>     // std::abs를 위해 포함

using namespace sf;

Player::Player()
    : velocity(0.f, 0.f), speed(2000.f), gravity(700.f),
    groundY(920.f), jumpPower(-400.f), isOnGround(false),
    currentFrame(100), frameTime(0.1f), frameTimer(0.f), frameCount(6),
    frameWidth(100), frameHeight(106), facingRight(true) {
    if (!texture.loadFromFile("Mario_SpraySheet_padded_top.png")) {
        throw std::runtime_error("이미지를 불러올 수 없습니다: Mario_SpraySheet_padded_top.png");
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
    sprite.setScale(-1.5f, 1.5f); // 오른쪽을 바라보도록 설정
    sprite.setOrigin(frameWidth / 2.f, 0.f); // 스프라이트의 중심을 왼쪽 아래로 설정(좌우 반전 시 대칭 맞추기)
    //sprite.setPosition(200.0f, 400.0f);
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
	//int frameX = isBig ? 4 : 1; // 큰 마리오면 4, 아니면 1
    int frameY = isBig ? 2 : 0;  // 큰 마리오면 1행, 아니면 0행

    if (!isMoving) {
        // 멈춰 있을 때 → (4,0)
        setFrame(4, frameY);
    }
    else {
        // 움직일 때 → (4,0) ↔ (1,0)
        if (animationTimer >= frameDuration) {
            animationTimer = 0.f; // 타이머 리셋, 0.05f가 아니라 0.f로 해야 정확한 시간 간격이 됩니다.

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
        if (invincibilityTimer >= invincibilityDuration) {
            isInvincible = false;
            invincibilityTimer = 2.f;
        }
    }
    // 바닥 충돌 처리
    // sprite.getScale().y는 스케일 비율입니다. 실제 높이를 얻으려면 텍스처의 원본 높이에 스케일을 곱해야 합니다.
    // sprite.getGlobalBounds().height를 사용하는 것이 더 정확합니다.
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
        flickerTimer += 0.016f; // 대충 프레임당 0.016초(60FPS 기준)
        if (static_cast<int>(flickerTimer / flickerInterval) % 2 == 0) {
            window.draw(sprite); // 보임
        }
        // else일 땐 안 그림 (깜빡임 효과)
    }
    else {
        window.draw(sprite); // 무적이 아니면 항상 그림
    }
}

const Sprite& Player::getSprite() const {
    return sprite;
}
// 플레이어의 스프라이트를 반환하는 함수
void Player::setScale(float scale) {
    sprite.setScale((facingRight ? -1 : 1) * scale, scale);
}

void Player::addScore(int amount) {
    // 점수 변수 만들고 += amount 처리
}

// --- 새로 추가된 함수들 구현 ---
// 플레이어 위치 설정 함수 구현
void Player::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

// 플레이어 현재 속도 반환 함수 구현
Vector2f Player::getVelocity() const {
    return velocity;
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
void Player::grow() {
    isBig = true;
    setScale(2.0f);  // 기존과 동일하게 크기도 조절
}
bool Player::getIsBig() const { return isBig; }
void Player::setIsBig(bool big) { isBig = big; }

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

FloatRect Player::getHitBox() const {
    FloatRect bounds = sprite.getGlobalBounds();
    bounds.left += 10.f; // 좌우 여백 제거
    bounds.width -= 20.f;
    bounds.top += 10.f;  // 위쪽 여백 제거
    bounds.height -= 15.f;
    return bounds;
}