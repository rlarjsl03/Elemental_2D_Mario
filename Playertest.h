#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
private:
    // 애니메이션 관련 변수 (기존과 동일)
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

    // 플레이어 관련 변수 (기존과 동일)
    Texture texture;
    Sprite sprite;
    Vector2f velocity;
    float speed;
    float gravity;
    float groundY;
    float jumpPower;
    bool isOnGround;

public:
    Player();
    Vector2f getVelocity() const;
    void handleInput(float deltaTime);
    void update(float deltaTime);
    void draw(RenderWindow& window);
    void updateAnimation(float deltaTime);
    void setFrame(int frameX, int frameY);
    void takeDamage(int amount);
    void bounceJump();
    const Sprite& getSprite() const;

    // --- 새로 추가된 함수들 ---
    void setPosition(float x, float y);     // 플레이어 위치를 직접 설정
    void setVelocityY(float y);             // 플레이어 수직 속도를 직접 설정
    void setOnGround(bool onGround);        // 플레이어가 땅에 닿았는지 여부 설정
};