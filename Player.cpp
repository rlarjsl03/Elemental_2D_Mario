#include <SFML/Graphics.hpp>
#include "Player.h"
#include <stdexcept> // std::runtime_error�� ���� ����
#include <cmath>     // std::abs�� ���� ����
#include <iostream>  // ������

using namespace sf;

Player::Player()
    : velocity(0.f, 0.f), speed(500.f), gravity(800.f),
    groundY(920.f), jumpPower(-700.f), isOnGround(false),
    currentFrame(100), frameTime(0.1f), frameTimer(0.f), frameCount(6),
    frameWidth(100), frameHeight(106), facingRight(true),
    score(0), drawSprite(true), isDead(false), life(1), isBig(false), // �߰��� ���� �ʱ�ȭ
    isInvincible(false), invincibilityTimer(0.f), flickerTimer(0.f) // ���� ���� ���� �ʱ�ȭ
    ,col(0), row(0)
{
    if (!texture.loadFromFile("Mario_SpraySheet_padded_top.png")) {
        throw std::runtime_error("�̹����� �ҷ��� �� �����ϴ�: Mario_SpraySheet_padded_top.png");
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
    sprite.setScale(-1.5f, 1.5f); // �������� �ٶ󺸵��� ����
    sprite.setOrigin(frameWidth / 2.f, 0.f); // ��������Ʈ�� �߽��� ���� �Ʒ��� ����(�¿� ���� �� ��Ī ���߱�)
}

// Ű �Է� ó��
void Player::handleInput(float deltaTime) {
    Vector2f direction(0.f, 0.f);
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        direction.x -= 1.f;
        facingRight = false; // ���� ����
    }
    else if (Keyboard::isKeyPressed(Keyboard::Right)) {
        direction.x += 1.f;
        facingRight = true; // ������ ����
    }

    velocity.x = direction.x * speed; // �÷��̾��� x �ӵ� ������Ʈ
    sprite.move(direction * speed * deltaTime);

    // ���⿡ ���� ��������Ʈ ����
    if (facingRight)
        sprite.setScale(-1.5f, 1.5f); // ������ ���� (���� �ؽ�ó�� ������ ���� �Ǿ��ִٸ� -1.5f, 1.5f�� ������)
    else
        sprite.setScale(1.5f, 1.5f); // ���� ���� (�¿� ����)

    if (Keyboard::isKeyPressed(Keyboard::Space) && isOnGround) {
        velocity.y = jumpPower;
        isOnGround = false;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        exit(0); // ���� ����
    }
}

void Player::setFrame(int frameX, int frameY) {
    int srcX = frameX * frameWidth;
    int srcY = frameY * frameHeight + yOffset;
    sprite.setTextureRect(IntRect(srcX, srcY, frameWidth, visibleHeight));
}

void Player::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;

    // �����̰� �ִ��� �Ǵ� (�ӵ��� Ű �Է�����)
    bool isMoving = std::abs(velocity.x) > 1.0f || Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Right);
    int frameY = isBig ? 2 : 0;  // ū �������� 2��, �ƴϸ� 0�� (��������Ʈ ��Ʈ ����)

    if (!isMoving) {
        // ���� ���� �� �� (4,0) �Ǵ� (4,2)
        setFrame(4, frameY);
    }
    else {
        // ������ �� �� (4,0) �� (1,0) �Ǵ� (4,2) �� (1,2)
        if (animationTimer >= frameDuration) {
            animationTimer = 0.f; // Ÿ�̸� ����

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
    // �߷� ����
    velocity.y += gravity * deltaTime;
    sprite.move(0.f, velocity.y * deltaTime);

    // ���� ���� Ȯ��
    if (isInvincible) {
        invincibilityTimer += deltaTime;
        flickerTimer += deltaTime;

        if (flickerTimer >= flickerInterval) {
            drawSprite = !drawSprite; // ��������Ʈ �׸��� ���� ���
            flickerTimer -= flickerInterval; // Ÿ�̸� ���� (�ܿ� �ð����� ����)
        }

        if (invincibilityTimer >= invincibilityDuration) {
            isInvincible = false;
            invincibilityTimer = 0.f; // Ÿ�̸Ӹ� 0���� �ʱ�ȭ
            drawSprite = true; // ���� ������ �ٽ� ���̰�
            flickerTimer = 0.f; // ������ Ÿ�̸ӵ� �ʱ�ȭ
        }
    }
    else
		setInvincible(false);   // ���� ���°� �ƴϸ� ���� Ÿ�̸� �ʱ�ȭ

    // �ٴ� �浹 ó��
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
    // �÷��̾� ������ ó�� (�ʿ�� ����)
}

void Player::bounceJump() {
    velocity.y = jumpPower;
    isOnGround = false;
}

void Player::draw(RenderWindow& window) {
    if (isInvincible) {
        // ���� �ֱ⸶�� ������: 0.1�� ����
        if (static_cast<int>(invincibilityTimer * 10) % 2 == 0) {
            window.draw(sprite); // ������ ȿ��
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
// --- ���� �߰��� �Լ��� ���� ---
// �÷��̾� ��ġ ���� �Լ� ����
void Player::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}
// �÷��̾� ���� �ӵ� ��ȯ �Լ� ����
Vector2f Player::getVelocity() const {
    return velocity; // m_velocity ��ȯ
}

// �÷��̾��� ���� ��ġ�� ��ȯ�ϴ� �Լ� �߰�
sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

// �÷��̾��� ���� �ٿ�� �ڽ��� ��ȯ�ϴ� �Լ� �߰�
sf::FloatRect Player::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

void Player::die() {
    isDead = true;
    // �׾��� �� �߰��� ���� ���, �ִϸ��̼� � ���⿡ ���� ����
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
        life = 0; // ������ 0 ���Ϸ� �������� �ʵ��� ����
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
        flickerTimer = 0.f; // �����ӵ� �ʱ�ȭ
    }
}
bool Player::getisInvincible() const {
    return isInvincible;
}

// setVelocityY �Լ� ���� (player.cpp �� �߰�)
void Player::setVelocityY(float y) {
    velocity.y = y;
}

FloatRect Player::getHitBox() const {
    FloatRect bounds = sprite.getGlobalBounds();
    bounds.left += 20.f; // �¿� ���� ����
    bounds.width -= 20.f;
    bounds.top += 10.f;  // ���� ���� ����
    bounds.height -= 12.f;
    return bounds;
}

