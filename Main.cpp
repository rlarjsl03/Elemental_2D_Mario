#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Player.h"
#include "Enemy.h"
#include "BackGround.h"

using namespace sf;

int main() {
    RenderWindow window(VideoMode(1920, 1080), "Player Control");
    window.setFramerateLimit(60);

    Background gameBackground("Background.jpg");
    Player player;
    Enemy enemy("enemy.png", Vector2f(300.f, 0.f));

    Clock clock;

    while (window.isOpen()) {
        Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        player.handleInput(deltaTime);
        gameBackground.update(deltaTime, 100.0f);
        player.update(deltaTime);
        enemy.update(deltaTime, 800.f);  // groundY = 800

        // 적이 살아있는 경우에만 충돌 판정
        if (enemy.isActive()) {
            FloatRect playerBounds = player.getSprite().getGlobalBounds();
            FloatRect enemyBounds = enemy.getSprite().getGlobalBounds();

            if (playerBounds.intersects(enemyBounds)) {
                float playerBottom = playerBounds.top + playerBounds.height;
                float enemyTop = enemyBounds.top;

                if (player.getVelocity().y > 0.f && playerBottom < enemyTop + 10.f) {
                    enemy.takeDamage(enemy.getHp());  // 한 번에 제거
                    player.bounceJump();              // 점프 반동
                }
                else {
                    player.takeDamage(10);
                }
            }
        }

        window.clear(Color::White);
        gameBackground.draw(window);
        player.draw(window);
        enemy.draw(window);  // 내부에서 isDead() 체크함
        window.display();
    }

    return 0;
}
