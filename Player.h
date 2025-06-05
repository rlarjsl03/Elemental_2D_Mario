#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
private:
	// 애니메이션 관련 변수
    int frameCount;         // 총 프레임 수
    int currentFrame;       // 현재 프레임 인덱스
    float frameTime;        // 프레임 간 시간 간격 (초)
    float frameTimer;       // 경과 시간 누적용
    int frameWidth;
    int frameHeight;
    int col;
    int row;
    std::vector<sf::Vector2i> frameSequence;  // 애니메이션 프레임 (col, row)
    int currentFrameIndex = 0;                // 현재 프레임 인덱스
    float animationTimer = 0.f;               // 타이머 누적값
    float frameDuration = 0.15f;              // 프레임 간 시간 간격
    bool facingRight = true;
    int visibleHeight = 100;  // 실제 이미지 높이
    int yOffset = -20;        // 아래 공백 제거용 오프셋

    // 플레이어 관련 변수
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
};
