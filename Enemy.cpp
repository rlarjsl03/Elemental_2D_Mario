#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <stdexcept>

using namespace sf;

enum class EnemyType {
    Regular,
    Random
};

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
        if (!texture.loadFromFile("mario-2d.png")) {
            throw std::runtime_error("이미지를 불러올 수 없습니다: mario-2d.png");
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
        std::cout << "플레이어가 " << amount << " 피해를 입었습니다!\n";
    }

    void bounceJump() {
        velocity.y = jumpPower;
        isOnGround = false;
    }

    const Sprite& getSprite() const {
        return sprite;
    }

    Vector2f getVelocity() const {
        return velocity;
    }
};

class Enemy {
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;
    int hp;
    bool facingRight;
    EnemyType type;
    float randomTimer;

public:
    Enemy(const std::string& textureFile, Vector2f startPos, EnemyType type)
        : position(startPos), hp(100), facingRight(true), type(type), randomTimer(0.f)
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

        if (type == EnemyType::Regular) {
            if (facingRight) {
                position.x += speed * deltaTime;
                if (position.x > 700) facingRight = false;
            }
            else {
                position.x -= speed * deltaTime;
                if (position.x < 100) facingRight = true;
            }
        }
        else if (type == EnemyType::Random) {
            randomTimer -= deltaTime;
            if (randomTimer <= 0.f) {
                int action = rand() % 3;
                switch (action) {
                case 0: break;
                case 1: position.x -= speed * deltaTime * (rand() % 3 + 1); break;
                case 2: position.x += speed * deltaTime * (rand() % 3 + 1); break;
                }
                randomTimer = static_cast<float>(rand() % 2 + 1);
            }
        }

        position.y = groundY - sprite.getGlobalBounds().height;
        sprite.setPosition(position);
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }

    void takeDamage(int amount) {
        hp -= amount;
        if (hp < 0) hp = 0;
    }

    bool isDead() const {
        return hp <= 0;
    }

    const Sprite& getSprite() const {
        return sprite;
    }

    int getHp() const {
        return hp;
    }
};

struct Particle {
    Sprite sprite;
    float lifetime;

    Particle(Texture& texture, Vector2f pos)
        : lifetime(0.5f)
    {
        sprite.setTexture(texture);
        sprite.setPosition(pos);
        sprite.setScale(0.1f, 0.1f);
    }

    void update(float deltaTime) {
        lifetime -= deltaTime;
        float alphaRatio = std::max(lifetime / 0.5f, 0.f);
        Uint8 alpha = static_cast<Uint8>(255 * alphaRatio);
        sprite.setColor(Color(255, 255, 255, alpha));
    }

    bool isAlive() const {
        return lifetime > 0;
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    RenderWindow window(VideoMode(800, 600), "원소술사 마리오 - 적 랜덤 스폰 & 파티클");

    Player player;

    std::vector<Enemy> enemies;
    std::vector<Particle> particles;

    float spawnTimer = 0.f;
    float spawnInterval = 2.f;

    Texture smokeTexture;
    if (!smokeTexture.loadFromFile("smoke.png")) {
        throw std::runtime_error("smoke.png 이미지를 불러올 수 없습니다.");
    }

    Clock clock;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();
        spawnTimer += deltaTime;

        player.handleInput(deltaTime);
        player.update(deltaTime);

        if (spawnTimer >= spawnInterval) {
            spawnTimer = 0.f;
            float x = static_cast<float>(rand() % 700 + 50);
            EnemyType type = (rand() % 2 == 0) ? EnemyType::Regular : EnemyType::Random;
            std::string image = (rand() % 2 == 0) ? "cupa.png" : "rocket.png";
            enemies.emplace_back(image, Vector2f(x, 0.f), type);
        }

        for (auto& enemy : enemies)
            enemy.update(deltaTime, 500.f);

        FloatRect playerBounds = player.getSprite().getGlobalBounds();
        for (auto& enemy : enemies) {
            if (enemy.isDead()) continue;

            FloatRect enemyBounds = enemy.getSprite().getGlobalBounds();
            if (playerBounds.intersects(enemyBounds)) {
                float playerBottom = playerBounds.top + playerBounds.height;
                float enemyTop = enemyBounds.top;

                if (playerBottom < enemyTop + 10.f && player.getVelocity().y > 0) {
                    enemy.takeDamage(enemy.getHp());
                    player.bounceJump();

                    particles.emplace_back(smokeTexture, enemy.getSprite().getPosition());
                }
                else {
                    player.takeDamage(10);
                }
            }
        }

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& e) { return e.isDead(); }), enemies.end());

        for (auto& p : particles)
            p.update(deltaTime);

        particles.erase(std::remove_if(particles.begin(), particles.end(),
            [](const Particle& p) { return !p.isAlive(); }), particles.end());

        window.clear(Color::Black);

        player.draw(window);

        for (auto& enemy : enemies)
            enemy.draw(window);

        for (auto& p : particles)
            p.draw(window);

        window.display();
    }

    return 0;
}
