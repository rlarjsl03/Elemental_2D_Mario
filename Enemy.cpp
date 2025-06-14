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
        // 죽은 적은 일단 화면 밖으로 치움
        sprite.setPosition(-9999.f, -9999.f);

        // 죽은 후 일정 거리 지나면 리스폰
        position.x += (facingRight ? speed : -speed) * deltaTime;

        if (position.x < -100.f || position.x > 2000.f) {
            hp = 100;
            position.x = 1920.f + static_cast<float>(rand() % 500);
            facingRight = rand() % 2 == 0;
        }
        return;
    }
    // 살아있는 적의 움직임
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
        hp = 0; // HP가 음수가 되지 않도록 보장
        sprite.setPosition(-9999.f, -9999.f);  // 죽었을 때 센서 완전 제거
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
//            throw std::runtime_error("이미지를 불러올 수 없습니다: cupa.png");
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
// main.cpp 에 올릴거
//if (playerBottom < enemyTop + 10.f) {
//    // 플레이어가 적 밟음
//    enemy.takeDamage(enemy.getHp());
//    player.bounceJump();
//}
//else {
//    // 플레이어가 옆이나 아래서 닿음 (데미지 처리 등)
//    player.takeDamage(10);
//}





const Sprite& Enemy::getSprite() const {  
   return sprite;
}//    const Sprite& getSprite() const {
//        return sprite;
//    }
//};
// main.cpp 에 올릴거
//if (playerBottom < enemyTop + 10.f) {
//    // 플레이어가 적 밟음
//    enemy.takeDamage(enemy.getHp());
//    player.bounceJump();
//}
//else {
//    // 플레이어가 옆이나 아래서 닿음 (데미지 처리 등)
//    player.takeDamage(10);
//}
        if (position.x < -100.f || position.x > 2000.f) {
            hp = 100;
            position.x = 1920.f + static_cast<float>(rand() % 500);
            facingRight = rand() % 2 == 0;
        }

        return;
    }

    // 살아있는 적의 움직임
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
		hp = 0; // HP가 음수가 되지 않도록 보장
        sprite.setPosition(-9999.f, -9999.f);  // 죽었을 때 센서 완전 제거
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
