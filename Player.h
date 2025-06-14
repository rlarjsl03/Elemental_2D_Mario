#pragma once
#include <SFML/Graphics.hpp>
#include <vector> // std::vector를 사용하므로 필요합니다.

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
    Vector2f velocity; // <-- 여기에 velocity가 정의되어 있습니다.
    float speed;
    float gravity;
    float groundY;
    float jumpPower;
    bool isOnGround;

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
    void setPosition(float x, float y); // 플레이어 위치 설정 함수 추가};
    Vector2f getVelocity() const;
};