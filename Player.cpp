#include <SFML/Graphics.hpp>
#include "Player.h"
#include <stdexcept> // std::runtime_error�� ���� ����
#include <cmath>     // std::abs�� ���� ����
#include <iostream>  // ������

// GameObject.h�� ���⿡ �����Ͽ� GameObject Ŭ������ ���Ǹ� �����ɴϴ�.
// Item.h�� �ʿ��� �� �����Ƿ� �����մϴ�.
#include "GameObject.h"
#include "Item.h" // CoinItem, MushroomItem ���� ���Ǹ� ���� �ʿ��� �� �ֽ��ϴ�.

using namespace sf;

Player::Player()
// velocity�� m_velocity��, isOnGround�� m_isOnGround�� ���� (��� ���� �̸� ����)
    : m_velocity(0.f, 0.f), speed(500.f), gravity(700.f),
    groundY(920.f), jumpPower(-600.f), m_isOnGround(false), // m_isOnGround �ʱ�ȭ
    currentFrame(100), frameTime(0.1f), frameTimer(0.f), frameCount(6),
    frameWidth(100), frameHeight(106), facingRight(true),
    score(0), drawSprite(true), isDead(false), life(1), isBig(false), // �߰��� ���� �ʱ�ȭ
    isInvincible(false), invincibilityTimer(0.f), flickerTimer(0.f) // ���� ���� ���� �ʱ�ȭ
    , col(0), row(0)
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

    m_velocity.x = direction.x * speed; // �÷��̾��� x �ӵ� ������Ʈ
    // X�� �̵��� ���⼭ �ٷ� �����Ͽ� �ﰢ���� ������ �ݴϴ�.
    sprite.move(m_velocity.x * deltaTime, 0.f);

    // ���⿡ ���� ��������Ʈ ����
    if (facingRight)
        sprite.setScale(-1.5f, 1.5f); // ������ ���� (���� �ؽ�ó�� ������ ���� �Ǿ��ִٸ� -1.5f, 1.5f�� ������)
    else
        sprite.setScale(1.5f, 1.5f); // ���� ���� (�¿� ����)

    if (Keyboard::isKeyPressed(Keyboard::Space) && m_isOnGround) { // m_isOnGround ���
        m_velocity.y = jumpPower; // m_velocity ���
        m_isOnGround = false; // �����ϸ� �ٷ� ���� ���� ����
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
    bool isMoving = std::abs(m_velocity.x) > 1.0f || Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Right); // m_velocity ���
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
    m_velocity.y += gravity * deltaTime; // m_velocity ���
    sprite.move(0.f, m_velocity.y * deltaTime); // m_velocity ���

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

    // --- ���� ���� �浹 ó�� ���� ---
    // ���� �� �κ��� checkPlatformCollision �Լ����� ó���մϴ�.
    // float bottom = sprite.getPosition().y + sprite.getGlobalBounds().height;
    // if (bottom >= groundY) {
    //     sprite.setPosition(sprite.getPosition().x, groundY - sprite.getGlobalBounds().height);
    //     m_velocity.y = 0; // m_velocity ���
    //     m_isOnGround = true; // m_isOnGround ���
    // }

    updateAnimation(deltaTime);
}

// --- ���� �߰��� checkPlatformCollision �Լ� ���� ---
void Player::checkPlatformCollision(float deltaTime, const std::vector<std::unique_ptr<GameObject>>& gameObjects) {
    sf::FloatRect playerBounds = getGlobalBounds(); // �÷��̾��� ���� �ٿ�� �ڽ�

    // �켱 �÷��̾ ���߿� �ִٰ� �����ϰ�, �浹 �߻� �� ���� ���·� ����
    bool foundGround = false;

    // ���� ����(groundY)���� �浹�� Ȯ���մϴ�.
    float playerBottom = playerBounds.top + playerBounds.height;
    if (playerBottom >= groundY) {
        setPosition(getPosition().x, groundY - playerBounds.height); // ���鿡 ��Ȯ�� ����
        m_velocity.y = 0; // ���� �ӵ��� 0���� ����
        foundGround = true; // ���鿡 ������� ǥ��
    }

    // �� ����, �� ���� ������Ʈ���� �浹�� Ȯ���մϴ�.
    for (auto& obj : gameObjects) {
        sf::FloatRect objBounds = obj->getGlobalBounds(); // ������Ʈ�� �ٿ�� �ڽ�

        // �÷��̾ �Ʒ��� �������� ���̰�, ������Ʈ�� ��ĥ ��
        if (m_velocity.y >= 0 && playerBounds.intersects(objBounds)) {
            // �÷��̾��� �ٴ��� ������Ʈ�� ��ܿ� ��Ҵ��� Ȯ�� (��Ȯ�� ���� ����)
            // ���� �����ӿ��� �÷��̾��� �ٴ��� ������Ʈ ��� ���� �־���,
            // ���� �����ӿ��� ������Ʈ ����� �Ѿ�� ��츦 �����մϴ�.
            if ((playerBounds.top + playerBounds.height - (m_velocity.y * deltaTime)) <= objBounds.top && // ���� ��ġ�� ������Ʈ ��
                playerBounds.top + playerBounds.height >= objBounds.top) // ���� ��ġ�� ������Ʈ�� ��ħ
            {
                // �÷��̾��� Y ��ġ�� ������Ʈ ��ܿ� ��Ȯ�� ����ϴ�.
                setPosition(getPosition().x, objBounds.top - playerBounds.height);
                m_velocity.y = 0; // ���� �ӵ� 0���� ����
                foundGround = true; // ���� ������� ǥ��
                break; // �ϳ��� �÷����� ���������� �� �̻� �ٸ� ������Ʈ�� Ȯ���� �ʿ� ����
            }
        }
    }
    // ���������� m_isOnGround ���¸� ������Ʈ�մϴ�.
    m_isOnGround = foundGround;
}


void Player::takeDamage(int amount) {
    // �÷��̾� ������ ó�� (�ʿ�� ����)
    // �� �Լ��� ���� ������ ������, �浹 ó�� �������� ���� ����� ���� �� ���� ó���� �̷�����ϴ�.
}

void Player::bounceJump() {
    m_velocity.y = jumpPower; // m_velocity ���
    m_isOnGround = false; // m_isOnGround ���
}

void Player::draw(RenderWindow& window) {
    if (drawSprite) { // drawSprite ���� ���� �׸��� ���� ����
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

// --- ���� �߰��� �Լ��� ���� ---
// �÷��̾� ��ġ ���� �Լ� ����
void Player::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

// �÷��̾� ���� �ӵ� ��ȯ �Լ� ����
Vector2f Player::getVelocity() const {
    return m_velocity; // m_velocity ��ȯ
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
    std::cout << "Player has died!" << std::endl; // ����� �޽���
    // �׾��� �� �߰��� ���� ���, �ִϸ��̼� � ���⿡ ���� ����
}

bool Player::isAlive() const {
    return !isDead;
}
void Player::increaseLife(int amount) {
    life += amount;
    std::cout << "Life increased to: " << life << std::endl; // ����� �޽���
}
void Player::loseLife(int amount) {
    life -= amount;
    if (life < 0) {
        life = 0; // ������ 0 ���Ϸ� �������� �ʵ��� ����
    }
    std::cout << "Life decreased to: " << life << std::endl; // ����� �޽���
}
int Player::getLife() const {
    return life;
}

bool Player::getIsBig() const { return isBig; }
void Player::setIsBig(bool big) {
    isBig = big;
    // ��������Ʈ ũ�⸦ �����Ͽ� ū ������/���� �������� �ð������� ǥ�� (�ʿ��)
    if (isBig) {
        // ū �������� �� ��������Ʈ ��Ʈ�� �ٸ� �κ��� ����ϵ��� setFrame �����ϰų�
        // �������� ������ �� �ֽ��ϴ�. (���� �ִϸ��̼� ������ frameY�� ó��)
        // sprite.setScale((facingRight ? -1.5f : 1.5f), 1.5f); // ����
    }
    else {
        // ���� �������� �� (���� ������)
        // sprite.setScale((facingRight ? -1.5f : 1.5f), 1.5f); // ����
    }
    std::cout << "Player is now Big: " << (isBig ? "true" : "false") << std::endl; // ����� �޽���
}

void Player::setInvincible(bool value) {
    isInvincible = value;
    if (value) { // ���� ���°� ���۵� �� Ÿ�̸� �ʱ�ȭ
        invincibilityTimer = 0.f;
        flickerTimer = 0.f;
        drawSprite = true; // ���� ���� �ÿ��� �ϴ� ���̰�
    }
    else { // ���� ���°� ���� ��
        invincibilityTimer = 0.f;
        flickerTimer = 0.f;
        drawSprite = true; // ���� ���°� ������ �ٽ� ��������Ʈ�� �׸����� ����
    }
    std::cout << "Player is now Invincible: " << (isInvincible ? "true" : "false") << std::endl; // ����� �޽���
}

bool Player::getisInvincible() const {
    return isInvincible;
}

// setVelocityY �Լ� ���� (player.cpp �� �߰�)
void Player::setVelocityY(float y) {
    m_velocity.y = y;
}

FloatRect Player::getHitBox() const {
    FloatRect bounds = sprite.getGlobalBounds();
    bounds.left += 20.f; // �¿� ���� ����
    bounds.width -= 20.f;
    bounds.top += 10.f;  // ���� ���� ����
    bounds.height -= 5.f;
    return bounds;
}