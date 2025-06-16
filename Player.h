#pragma once
#include <SFML/Graphics.hpp>
#include <vector> // std::vector�� ����ϹǷ� �ʿ��մϴ�.

using namespace sf;

class Player {
private:
    // �ִϸ��̼� ���� ����
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

    // �÷��̾� ���� ����
    Texture texture;
    Sprite sprite;
    Vector2f velocity; // <-- ���⿡ velocity�� ���ǵǾ� �ֽ��ϴ�.
    float speed;
    float gravity;
    float groundY;
    float jumpPower;
    bool isOnGround;
    bool isDead = false;
    int life = 1; // �ʱ� ���� �� ����
    bool isBig = false;

	// �浹 ���� ����
    bool isInvincible = false;
    float invincibilityTimer = 0.f;
    const float invincibilityDuration = 2.f; // ���� �ð� 2��
    float flickerTimer = 0.f;
    float flickerInterval = 0.1f; // ������ ���� (0.1�� ����)

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
    // --- ���� �߰��� �Լ��� ---
    void setPosition(float x, float y); // �÷��̾� ��ġ ���� �Լ� �߰�};
    Vector2f getVelocity() const;
    void die();
    bool isAlive() const;
    void increaseLife(int amount); // ���� ���� �Լ�
    void loseLife(int amount);
    int getLife() const;               // ���� �� Ȯ�� �Լ�
    bool getIsBig() const;
    void setIsBig(bool big);
    void setInvincible(bool value);
    bool getisInvincible() const;
    FloatRect getHitBox() const;
};