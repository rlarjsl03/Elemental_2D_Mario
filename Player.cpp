#include <SFML/Graphics.hpp>
#include "Player.h"
#include <stdexcept> // std::runtime_error를 위해 포함
#include <cmath>     // std::abs를 위해 포함
#include <iostream>  // 디버깅용

// GameObject.h를 여기에 포함하여 GameObject 클래스의 정의를 가져옵니다.
// Item.h도 필요할 수 있으므로 포함합니다.
#include "GameObject.h"
#include "Item.h" // CoinItem, MushroomItem 등의 정의를 위해 필요할 수 있습니다.

using namespace sf;

Player::Player()
// velocity를 m_velocity로, isOnGround를 m_isOnGround로 변경 (멤버 변수 이름 변경)
    : m_velocity(0.f, 0.f), speed(500.f), gravity(700.f),
    groundY(920.f), jumpPower(-600.f), m_isOnGround(false), // m_isOnGround 초기화
    currentFrame(100), frameTime(0.1f), frameTimer(0.f), frameCount(6),
    frameWidth(100), frameHeight(106), facingRight(true),
    score(0), drawSprite(true), isDead(false), life(1), isBig(false), // 추가된 변수 초기화
    isInvincible(false), invincibilityTimer(0.f), flickerTimer(0.f) // 무적 관련 변수 초기화
    , col(0), row(0)
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

    m_velocity.x = direction.x * speed; // 플레이어의 x 속도 업데이트
    // X축 이동은 여기서 바로 적용하여 즉각적인 반응을 줍니다.
    sprite.move(m_velocity.x * deltaTime, 0.f);

    // 방향에 따라 스프라이트 반전
    if (facingRight)
        sprite.setScale(-1.5f, 1.5f); // 오른쪽 보기 (원본 텍스처가 왼쪽을 보게 되어있다면 -1.5f, 1.5f가 오른쪽)
    else
        sprite.setScale(1.5f, 1.5f); // 왼쪽 보기 (좌우 반전)

    if (Keyboard::isKeyPressed(Keyboard::Space) && m_isOnGround) { // m_isOnGround 사용
        m_velocity.y = jumpPower; // m_velocity 사용
        m_isOnGround = false; // 점프하면 바로 지상 상태 해제
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
    bool isMoving = std::abs(m_velocity.x) > 1.0f || Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Right); // m_velocity 사용
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
    m_velocity.y += gravity * deltaTime; // m_velocity 사용
    sprite.move(0.f, m_velocity.y * deltaTime); // m_velocity 사용

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

    // --- 기존 지면 충돌 처리 제거 ---
    // 이제 이 부분은 checkPlatformCollision 함수에서 처리합니다.
    // float bottom = sprite.getPosition().y + sprite.getGlobalBounds().height;
    // if (bottom >= groundY) {
    //     sprite.setPosition(sprite.getPosition().x, groundY - sprite.getGlobalBounds().height);
    //     m_velocity.y = 0; // m_velocity 사용
    //     m_isOnGround = true; // m_isOnGround 사용
    // }

    updateAnimation(deltaTime);
}

// --- 새로 추가된 checkPlatformCollision 함수 구현 ---
void Player::checkPlatformCollision(float deltaTime, const std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    sf::FloatRect playerBounds = getGlobalBounds(); // 플레이어의 현재 바운딩 박스

    // 우선 플레이어가 공중에 있다고 가정하고, 충돌 발생 시 지상 상태로 변경
    bool foundGround = false;

    // 먼저 지면(groundY)과의 충돌을 확인합니다.
    float playerBottom = playerBounds.top + playerBounds.height;
    if (playerBottom >= groundY) {
        setPosition(getPosition().x, groundY - playerBounds.height); // 지면에 정확히 맞춤
        m_velocity.y = 0; // 수직 속도를 0으로 설정
        foundGround = true; // 지면에 닿았음을 표시
    }

    // 그 다음, 각 게임 오브젝트와의 충돌을 확인합니다.
    for (auto& obj : gameObjects) {
        sf::FloatRect objBounds = obj->getGlobalBounds(); // 오브젝트의 바운딩 박스

        // 플레이어가 아래로 떨어지는 중이고, 오브젝트와 겹칠 때
        if (m_velocity.y >= 0 && playerBounds.intersects(objBounds)) {
            // 플레이어의 바닥이 오브젝트의 상단에 닿았는지 확인 (정확한 착지 판정)
            // 이전 프레임에는 플레이어의 바닥이 오브젝트 상단 위에 있었고,
            // 현재 프레임에는 오브젝트 상단을 넘어섰을 경우를 감지합니다.
            if ((playerBounds.top + playerBounds.height - (m_velocity.y * deltaTime)) <= objBounds.top && // 이전 위치가 오브젝트 위
                playerBounds.top + playerBounds.height >= objBounds.top) // 현재 위치가 오브젝트와 겹침
            {
                // 플레이어의 Y 위치를 오브젝트 상단에 정확히 맞춥니다.
                setPosition(getPosition().x, objBounds.top - playerBounds.height);
                m_velocity.y = 0; // 수직 속도 0으로 설정
                foundGround = true; // 지상에 닿았음을 표시
                break; // 하나의 플랫폼에 착지했으면 더 이상 다른 오브젝트를 확인할 필요 없음
            }
        }
    }
    // 최종적으로 m_isOnGround 상태를 업데이트합니다.
    m_isOnGround = foundGround;
}


void Player::takeDamage(int amount) {
    // 플레이어 데미지 처리 (필요시 구현)
    // 이 함수는 현재 사용되지 않으며, 충돌 처리 로직에서 직접 생명력 감소 및 무적 처리가 이루어집니다.
}

void Player::bounceJump() {
    m_velocity.y = jumpPower; // m_velocity 사용
    m_isOnGround = false; // m_isOnGround 사용
}

void Player::draw(RenderWindow& window) {
    if (drawSprite) { // drawSprite 값에 따라 그릴지 말지 결정
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

// --- 새로 추가된 함수들 구현 ---
// 플레이어 위치 설정 함수 구현
void Player::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

// 플레이어 현재 속도 반환 함수 구현
Vector2f Player::getVelocity() const {
    return m_velocity; // m_velocity 반환
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
    std::cout << "Player has died!" << std::endl; // 디버깅 메시지
    // 죽었을 때 추가로 사운드 재생, 애니메이션 등도 여기에 구현 가능
}

bool Player::isAlive() const {
    return !isDead;
}
void Player::increaseLife(int amount) {
    life += amount;
    std::cout << "Life increased to: " << life << std::endl; // 디버깅 메시지
}
void Player::loseLife(int amount) {
    life -= amount;
    if (life < 0) {
        life = 0; // 생명이 0 이하로 떨어지지 않도록 보장
    }
    std::cout << "Life decreased to: " << life << std::endl; // 디버깅 메시지
}
int Player::getLife() const {
    return life;
}

bool Player::getIsBig() const { return isBig; }
void Player::setIsBig(bool big) {
    isBig = big;
    // 스프라이트 크기를 변경하여 큰 마리오/작은 마리오를 시각적으로 표현 (필요시)
    if (isBig) {
        // 큰 마리오일 때 스프라이트 시트의 다른 부분을 사용하도록 setFrame 조정하거나
        // 스케일을 변경할 수 있습니다. (현재 애니메이션 로직은 frameY로 처리)
        // sprite.setScale((facingRight ? -1.5f : 1.5f), 1.5f); // 예시
    }
    else {
        // 작은 마리오일 때 (원래 스케일)
        // sprite.setScale((facingRight ? -1.5f : 1.5f), 1.5f); // 예시
    }
    std::cout << "Player is now Big: " << (isBig ? "true" : "false") << std::endl; // 디버깅 메시지
}

void Player::setInvincible(bool value) {
    isInvincible = value;
    if (value) { // 무적 상태가 시작될 때 타이머 초기화
        invincibilityTimer = 0.f;
        flickerTimer = 0.f;
        drawSprite = true; // 무적 시작 시에는 일단 보이게
    }
    else { // 무적 상태가 끝날 때
        invincibilityTimer = 0.f;
        flickerTimer = 0.f;
        drawSprite = true; // 무적 상태가 끝나면 다시 스프라이트를 그리도록 설정
    }
    std::cout << "Player is now Invincible: " << (isInvincible ? "true" : "false") << std::endl; // 디버깅 메시지
}

bool Player::getisInvincible() const {
    return isInvincible;
}

// setVelocityY 함수 구현 (player.cpp 에 추가)
void Player::setVelocityY(float y) {
    m_velocity.y = y;
}

FloatRect Player::getHitBox() const {
    FloatRect bounds = sprite.getGlobalBounds();
    bounds.left += 20.f; // 좌우 여백 제거
    bounds.width -= 20.f;
    bounds.top += 10.f;  // 위쪽 여백 제거
    bounds.height -= 5.f;
    return bounds;
}