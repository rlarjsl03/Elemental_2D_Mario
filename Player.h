#pragma once
#include <SFML/Graphics.hpp>
#include <vector> // std::vector를 사용하므로 필요합니다.
#include <memory> // std::unique_ptr를 사용하기 위해 추가

// GameObject 클래스 전방 선언 (순환 참조 방지)
class GameObject;

using namespace sf;

class Player {
private:
    // 애니메이션 관련 변수
    int frameCount;
    int currentFrame;
    float frameTime;
    float frameTimer;
    int frameWidth;
    int frameHeight;
    int col;
    int row;
    std::vector<sf::Vector2i> frameSequence;
    int currentFrameIndex = 0;
    float animationTimer = 0.f;
    float frameDuration = 0.15f;
    bool facingRight = true;
    int visibleHeight = 100;
    int yOffset = -20;

    // 플레이어 관련 변수
    Texture texture;
    Sprite sprite;
    // private 접근 제어자를 위해 m_ 접두사 추가
    Vector2f m_velocity;
    float speed;
    float gravity;
    float groundY;
    float jumpPower;
    bool m_isOnGround; // private 접근 제어자를 위해 m_ 접두사 추가
    bool isDead = false;
    int life = 1; // 초기 생명 수 설정
    bool isBig = false;
    int score = 0; // 점수 변수 추가

    // 충돌 관련 변수
    bool isInvincible = false;
    float invincibilityTimer = 0.f;
    const float invincibilityDuration = 2.f; // 무적 시간 2초
    float flickerTimer = 0.f;
    float flickerInterval = 0.1f; // 깜빡임 간격 (0.1초 간격)
    bool drawSprite = true; // 깜빡임 효과를 위한 변수

public:
    Player();
    void handleInput(float deltaTime);
    void update(float deltaTime);
    void draw(RenderWindow& window);
    void updateAnimation(float deltaTime);
    void setFrame(int frameX, int frameY);
    void takeDamage(int amount);
    void bounceJump();
    const Sprite& getSprite() const;
    void setScale(float scale);
    void addScore(int amount);
    // --- 새로 추가할 함수들 ---
    void setPosition(float x, float y); // 플레이어 위치 설정 함수 추가
    Vector2f getVelocity() const;
    void setVelocityY(float y); // m_velocity.y 값을 설정하는 함수 추가 (public에 선언)

    sf::Vector2f getPosition() const; // 플레이어 현재 위치 반환 함수 추가
    sf::FloatRect getGlobalBounds() const; // 플레이어의 전역 바운딩 박스 반환 함수 추가

    void die();
    bool isAlive() const;
    void increaseLife(int amount); // 생명 증가 함수
    void loseLife(int amount);
    int getLife() const;        // 생명 수 확인 함수
    bool getIsBig() const;
    void setIsBig(bool big);
    void setInvincible(bool value);
    bool getisInvincible() const;
    FloatRect getHitBox() const;

    // Getter/Setter for m_isOnGround (main.cpp에서 접근하기 위함)
    bool getIsOnGround() const { return m_isOnGround; }
    void setIsOnGround(bool onGround) { m_isOnGround = onGround; }

    // --- 새로 추가된 함수: 게임 오브젝트와의 충돌 처리 ---
    // 플레이어가 플랫폼, 파이프 등 GameObject 위로 착지하는 것을 처리합니다.
    void checkPlatformCollision(float deltaTime, const std::vector<std::unique_ptr<GameObject>>& gameObjects);
};