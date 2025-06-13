#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <random>
#include "Player.h"
#include "Enemy.h"
#include "BackGround.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Player Control");
    window.setFramerateLimit(60);

    Background gameBackground("Background.jpg");
    Player player;

    // 텍스처 로드 및 shared_ptr 생성
    std::shared_ptr<sf::Texture> tex1 = std::make_shared<sf::Texture>();
    std::shared_ptr<sf::Texture> tex2 = std::make_shared<sf::Texture>();
    std::shared_ptr<sf::Texture> tex3 = std::make_shared<sf::Texture>();

    if (!tex1->loadFromFile("cupa2.png") ||
        !tex2->loadFromFile("turtle2.png") ||
        !tex3->loadFromFile("mushroom3.png")) {
        throw std::runtime_error("텍스처 로드 실패");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(100.f, 1500.f);

    std::vector<std::unique_ptr<Enemy>> enemies;
    enemies.push_back(std::make_unique<Enemy>(tex1, sf::Vector2f(distX(gen), 0.f)));
    enemies.push_back(std::make_unique<Enemy>(tex2, sf::Vector2f(distX(gen), 0.f)));
    enemies.push_back(std::make_unique<Enemy>(tex3, sf::Vector2f(distX(gen), 0.f)));

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.handleInput(deltaTime);
        gameBackground.update(deltaTime, 100.0f);
        player.update(deltaTime);

        for (auto& enemy : enemies) {
            enemy->update(deltaTime, 800.f);

            if (enemy->isActive()) {
                if (player.getSprite().getGlobalBounds().intersects(enemy->getSprite().getGlobalBounds())) {
                    float playerBottom = player.getSprite().getGlobalBounds().top + player.getSprite().getGlobalBounds().height;
                    float enemyTop = enemy->getSprite().getGlobalBounds().top;

                    if (player.getVelocity().y > 0.f && playerBottom < enemyTop + 10.f) {
                        enemy->takeDamage(enemy->getHp());
                        player.bounceJump();
                    }
                    else {
                        player.takeDamage(10);
                    }
                }
            }
        }

        window.clear(sf::Color::White);
        gameBackground.draw(window);
        player.draw(window);
        for (auto& enemy : enemies) {
            enemy->draw(window);
        }
        window.display();
    }

    return 0;
}
