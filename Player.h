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
    void handleInput(float deltaTime);
    void update(float deltaTime);
    void draw(RenderWindow& window);
    void updateAnimation(float deltaTime);
    const Sprite& getSprite() const;
};
