#include <SFML/Graphics.hpp>
#include <stdexcept>

using namespace sf;

class Player {
private:
    Texture texture;
    Sprite sprite;
    Vector2f velocity;
    float speed;
    float gravity;
    float groundY;
    float jumpPower;
    bool isOnGround;

public:
    Player()
        : velocity(0.f, 0.f), speed(200.f), gravity(500.f),
        groundY(500.f), jumpPower(-300.f), isOnGround(false)
    {
        if (!texture.loadFromFile("cupa.png")) {
            throw std::runtime_error("이미지를 불러올 수 없습니다: cupa.png");
        }
        sprite.setTexture(texture);
        sprite.setScale(0.1f, 0.1f);
        sprite.setPosition(200.f, 100.f);
    }

    void handleInput(float deltaTime) {
        Vector2f direction(0.f, 0.f);
        if (Keyboard::isKeyPressed(Keyboard::Left))
            direction.x -= 1.f;
        else if (Keyboard::isKeyPressed(Keyboard::Right))
            direction.x += 1.f;

        sprite.move(direction * speed * deltaTime);

        if (Keyboard::isKeyPressed(Keyboard::Space) && isOnGround) {
            velocity.y = jumpPower;
            isOnGround = false;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            exit(0);
        }
    }

    void update(float deltaTime) {
        velocity.y += gravity * deltaTime;
        sprite.move(0.f, velocity.y * deltaTime);

        float bottom = sprite.getPosition().y + sprite.getGlobalBounds().height;
        if (bottom >= groundY) {
            sprite.setPosition(sprite.getPosition().x, groundY - sprite.getGlobalBounds().height);
            velocity.y = 0;
            isOnGround = true;
        }
        else {
            isOnGround = false;
        }
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    void takeDamage(int amount) {
        // 플레이어 데미지 처리 (필요시 구현)
    }

    void bounceJump() {
        velocity.y = jumpPower;
        isOnGround = false;
    }

    const Sprite& getSprite() const {
        return sprite;
    }
};


class Enemy {
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;
    int hp;
    bool facingRight;

public:
    Enemy(const std::string& textureFile, Vector2f startPos)
        : position(startPos), hp(100), facingRight(true)
    {
        if (!texture.loadFromFile(textureFile)) {
            throw std::runtime_error("이미지를 불러올 수 없습니다: " + textureFile);
        }
        sprite.setTexture(texture);
        sprite.setScale(0.1f, 0.1f);
        sprite.setPosition(position);
    }

    void update(float deltaTime, float groundY) {
        float speed = 50.f;
        if (facingRight) {
            position.x += speed * deltaTime;
            if (position.x > 500) facingRight = false;
        }
        else {
            position.x -= speed * deltaTime;
            if (position.x < 100) facingRight = true;
        }
        position.y = groundY - sprite.getGlobalBounds().height;  // 바닥에 고정
        sprite.setPosition(position);
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    void takeDamage(int amount) {
        hp -= amount;
        if (hp < 0) hp = 0;
    }

    int getHp() const {
        return hp;
    }

    bool isDead() const {
        return hp <= 0;
    }

    const Sprite& getSprite() const {
        return sprite;
    }
};


int main() {

    RenderWindow window(VideoMode(800, 600), "원소술사 마리오 - 적 충돌 예시");

    Player player;
    Enemy enemy("enemy.png", Vector2f(300.f, 0.f));  // y는 update에서 바닥 고정됨

    Clock clock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();

        player.handleInput(deltaTime);
        player.update(deltaTime);
        enemy.update(deltaTime, 500.f);  // groundY = 500

        // 충돌 체크
        FloatRect playerBounds = player.getSprite().getGlobalBounds();
        FloatRect enemyBounds = enemy.getSprite().getGlobalBounds();

        if (playerBounds.intersects(enemyBounds)) {
            float playerBottom = playerBounds.top + playerBounds.height;
            float enemyTop = enemyBounds.top;

            if (playerBottom < enemyTop + 10.f) {
                // 플레이어가 적 밟음
                enemy.takeDamage(enemy.getHp());
                player.bounceJump();
            }
            else {
                // 플레이어가 옆이나 아래서 닿음 (데미지 처리 등)
                player.takeDamage(10);
            }
        }

        window.clear(Color::Black);

        player.draw(window);
        if (!enemy.isDead())
            enemy.draw(window);

        window.display();
    }

    return 0;
}
