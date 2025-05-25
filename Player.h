#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
private:
	// �ִϸ��̼� ���� ����
    int frameCount;         // �� ������ ��
    int currentFrame;       // ���� ������ �ε���
    float frameTime;        // ������ �� �ð� ���� (��)
    float frameTimer;       // ��� �ð� ������
    int frameWidth;
    int frameHeight;
    int col;
    int row;
    std::vector<sf::Vector2i> frameSequence;  // �ִϸ��̼� ������ (col, row)
    int currentFrameIndex = 0;                // ���� ������ �ε���
    float animationTimer = 0.f;               // Ÿ�̸� ������
    float frameDuration = 0.15f;              // ������ �� �ð� ����
    bool facingRight = true;

    // �÷��̾� ���� ����
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
