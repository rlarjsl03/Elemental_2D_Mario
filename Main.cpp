#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "Player.h"
#include "Enemy.h"
#include "BackGround.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));  // 랜덤 초기화

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Player Control");
    window.setFramerateLimit(60);

    Background gameBackground("Background.jpg");
    Player player;

    // 텍스처 미리 로드
    sf::Texture tex1, tex2, tex3;
    tex1.loadFromFile("cupa2.png");
    tex2.loadFromFile("mushroom4.png");
    tex3.loadFromFile("turtle3.png");

    std::vector<std::unique_ptr<Enemy>> enemies;
    enemies.push_back(std::make_unique<Enemy>(tex1, sf::Vector2f(300.f, 0.f)));
    enemies.push_back(std::make_unique<Enemy>(tex2, sf::Vector2f(700.f, 0.f)));
    enemies.push_back(std::make_unique<Enemy>(tex3, sf::Vector2f(1100.f, 0.f)));

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

        for (auto& enemy : enemies)
            enemy->update(deltaTime, 800.f);

        // 충돌 체크
        sf::FloatRect playerBounds = player.getSprite().getGlobalBounds();

        for (auto& enemy : enemies) {
            sf::FloatRect enemyBounds = enemy->getSprite().getGlobalBounds();

            if (playerBounds.intersects(enemyBounds)) {
                float playerBottom = playerBounds.top + playerBounds.height;
                float enemyTop = enemyBounds.top;

                if (player.getVelocity().y > 0.f && playerBottom < enemyTop + 10.f) {
                    enemy->takeDamage(100); // 적 제거
                    player.bounceJump();    // 튕기기
                }
                else {
                    player.takeDamage(10);
                }
            }
        }

        window.clear(sf::Color::White);
        gameBackground.draw(window);
        player.draw(window);
        for (auto& enemy : enemies)
            enemy->draw(window);
        window.display();
    }

    return 0;
}
