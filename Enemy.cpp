#include "Enemy.h"
#include <cstdlib>

Enemy::Enemy(const sf::Texture& tex, sf::Vector2f startPos)
    : texture(tex), position(startPos), hp(100), facingRight(rand() % 2 == 0) {
    sprite.setTexture(texture);
    sprite.setScale(1.5f, 1.5f);
    sprite.setPosition(position);
}
void Enemy::update(float deltaTime, float groundY) {
    float speed = 50.f;

    if (isDead()) {
        // ���� ���� �ϴ� ȭ�� ������ ġ��
        sprite.setPosition(-9999.f, -9999.f);

        // ���� �� ���� �Ÿ� ������ ������
        position.x += (facingRight ? speed : -speed) * deltaTime;

        if (position.x < -100.f || position.x > 2000.f) {
            hp = 100;
            position.x = 1920.f + static_cast<float>(rand() % 500);
            facingRight = rand() % 2 == 0;
        }
        return;
    }
    // ����ִ� ���� ������
    if (facingRight) {
        position.x += speed * deltaTime;
        if (position.x > 500) facingRight = false;
    }
    else {
        position.x -= speed * deltaTime;
        if (position.x < 100) facingRight = true;
    }

    position.y = groundY - sprite.getGlobalBounds().height;
    sprite.setPosition(position);
}

void Enemy::draw(sf::RenderWindow& window) {
    if (!isDead())
        window.draw(sprite);
}

void Enemy::takeDamage(int amount) {
    hp -= amount;
    if (hp < 0) {
        hp = 0; // HP�� ������ ���� �ʵ��� ����
        sprite.setPosition(-9999.f, -9999.f);  // �׾��� �� ���� ���� ����
    }
}

int Enemy::getHp() const {
    return hp;
}

bool Enemy::isDead() const {
    return hp <= 0;
}


bool Enemy::isActive() const {
    return !isDead();
}

const sf::Sprite& Enemy::getSprite() const {
    return sprite;
}
//        : velocity(0.f, 0.f), speed(200.f), gravity(500.f),
//        groundY(500.f), jumpPower(-300.f), isOnGround(false)
//    {
//        if (!texture.loadFromFile("cupa.png")) {
//            throw std::runtime_error("�̹����� �ҷ��� �� �����ϴ�: cupa.png");
//        }
//        sprite.setTexture(texture);
//        sprite.setScale(0.1f, 0.1f);
//        sprite.setPosition(200.f, 100.f);
//    }
//
//    void handleInput(float deltaTime) {
//        Vector2f direction(0.f, 0.f);
//        if (Keyboard::isKeyPressed(Keyboard::Left))
//            direction.x -= 1.f;
//        else if (Keyboard::isKeyPressed(Keyboard::Right))
//            direction.x += 1.f;
//
//        sprite.move(direction * speed * deltaTime);
//
//        if (Keyboard::isKeyPressed(Keyboard::Space) && isOnGround) {
//            velocity.y = jumpPower;
//            isOnGround = false;
//        }
//        else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
//            exit(0);
//        }
//    }
//
//    void update(float deltaTime) {
//        velocity.y += gravity * deltaTime;
//        sprite.move(0.f, velocity.y * deltaTime);
//
//        float bottom = sprite.getPosition().y + sprite.getGlobalBounds().height;
//        if (bottom >= groundY) {
//            sprite.setPosition(sprite.getPosition().x, groundY - sprite.getGlobalBounds().height);
//            velocity.y = 0;
//            isOnGround = true;
//        }
//        else {
//            isOnGround = false;
//        }
//    }
//
//    void draw(RenderWindow& window) {
//        window.draw(sprite);
//    }
//
//    const Sprite& getSprite() const {
//        return sprite;
//    }
//};
// main.cpp �� �ø���
//if (playerBottom < enemyTop + 10.f) {
//    // �÷��̾ �� ����
//    enemy.takeDamage(enemy.getHp());
//    player.bounceJump();
//}
//else {
//    // �÷��̾ ���̳� �Ʒ��� ���� (������ ó�� ��)
//    player.takeDamage(10);
//}





const Sprite& Enemy::getSprite() const {  
   return sprite;
}//    const Sprite& getSprite() const {
//        return sprite;
//    }
//};
// main.cpp �� �ø���
//if (playerBottom < enemyTop + 10.f) {
//    // �÷��̾ �� ����
//    enemy.takeDamage(enemy.getHp());
//    player.bounceJump();
//}
//else {
//    // �÷��̾ ���̳� �Ʒ��� ���� (������ ó�� ��)
//    player.takeDamage(10);
//}
        if (position.x < -100.f || position.x > 2000.f) {
            hp = 100;
            position.x = 1920.f + static_cast<float>(rand() % 500);
            facingRight = rand() % 2 == 0;
        }

        return;
    }

    // ����ִ� ���� ������
    if (facingRight) {
        position.x += speed * deltaTime;
        if (position.x > 500) facingRight = false;
    }
    else {
        position.x -= speed * deltaTime;
        if (position.x < 100) facingRight = true;
    }

    position.y = groundY - sprite.getGlobalBounds().height;
    sprite.setPosition(position);
}

void Enemy::draw(sf::RenderWindow& window) {
    if (!isDead())
        window.draw(sprite);
}

void Enemy::takeDamage(int amount) {
    hp -= amount;
	if (hp < 0) {
		hp = 0; // HP�� ������ ���� �ʵ��� ����
        sprite.setPosition(-9999.f, -9999.f);  // �׾��� �� ���� ���� ����
    }
}

int Enemy::getHp() const {
    return hp;
}

bool Enemy::isDead() const {
    return hp <= 0;
}

bool Enemy::isActive() const {
    return !isDead();
}

const sf::Sprite& Enemy::getSprite() const {
    return sprite;
}
