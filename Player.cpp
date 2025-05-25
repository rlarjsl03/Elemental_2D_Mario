#include <SFML/Graphics.hpp>
#include "Player.h"

using namespace sf;

Player::Player()    //player.h�� �ִ� player Ŭ������ ������
    : velocity(0.f, 0.f), speed(200.f), gravity(500.f),
    groundY(650.f), jumpPower(-500.f), isOnGround(false),
    currentFrame(100), frameTime(0.1f), frameTimer(0.f), frameCount(6),
    frameWidth(100), frameHeight(100), facingRight(true) {
	if (!texture.loadFromFile("Mario_SpraySheet_padded_top.png")) { //Mario_SpraySheet.png
        throw std::runtime_error("�̹����� �ҷ��� �� �����ϴ�: mario-2d.png");
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
    sprite.setScale(-1.5f, 1.5f);   // �������� �ٶ󺸵��� ����
	sprite.setOrigin(frameWidth / 2.f, 0.f);    // ��������Ʈ�� �߽��� ���� �Ʒ��� ����(�¿� ���� �� ��Ī ���߱�)
    sprite.setPosition(200.0f, 400.0f);
}


    // Ű �Է� ó��
void Player::handleInput(float deltaTime) {
    Vector2f direction(0.f, 0.f);
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        direction.x -= 1.f;
        facingRight = false;    // ���� ����
    }
    else if (Keyboard::isKeyPressed(Keyboard::Right)) {
        direction.x += 1.f;
		facingRight = true;   // ������ ����
    }

    velocity.x = direction.x * speed;
    sprite.move(direction * speed * deltaTime);

    // ���⿡ ���� ��������Ʈ ����
    if (facingRight)
        sprite.setScale(-1.5f, 1.5f);   // ������ ����
    else
        sprite.setScale(1.5f, 1.5f);  // ���� ���� (�¿� ����)

    if (Keyboard::isKeyPressed(Keyboard::Space) && isOnGround) {
        velocity.y = jumpPower;
        isOnGround = false;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        // ���� ����
        exit(0);
    }
}

void Player::setFrame(int frameX, int frameY) {
    int srcX = frameX * frameWidth;
    int srcY = frameY * frameHeight + yOffset;
    sprite.setTextureRect(IntRect(srcX, srcY, frameWidth, visibleHeight));
}

void Player::updateAnimation(float deltaTime) {
    animationTimer += deltaTime;

    // �����̰� �ִ��� �Ǵ�
    bool isMoving = std::abs(velocity.x) > 1.0f || Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Right);

    if (!isMoving) {
        // ���� ���� �� �� (4,0)
		setFrame(4, 0);
    }
    else {
        // ������ �� �� (4,0) �� (1,0)
        if (animationTimer >= frameDuration) {
            animationTimer = 0.05f;

            if (currentFrameIndex == 0) {
                setFrame(4, 0);
                currentFrameIndex = 1;
            }
            else {
                setFrame(1, 0);
                currentFrameIndex = 0;
            }
        }
    }
}
void Player::update(float deltaTime) {
    // �߷� ����
    velocity.y += gravity * deltaTime;
    sprite.move(0.f, velocity.y * deltaTime);

    // �ٴ� �浹 ó��
    float bottom = sprite.getPosition().y + sprite.getScale().y;
    if (bottom >= groundY) {
        sprite.setPosition(sprite.getPosition().x, groundY - sprite.getScale().y);
        velocity.y = 0;
        isOnGround = true;
    }
    else
        isOnGround = false;

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
    window.draw(sprite);
}

const Sprite& Player::getSprite() const {
    return sprite;
}