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

        // ���� ����ִ� ��쿡�� �浹 ����
        if (enemy.isActive()) {
            FloatRect playerBounds = player.getSprite().getGlobalBounds();
            FloatRect enemyBounds = enemy.getSprite().getGlobalBounds();

            if (playerBounds.intersects(enemyBounds)) {
                float playerBottom = playerBounds.top + playerBounds.height;
                float enemyTop = enemyBounds.top;

                if (player.getVelocity().y > 0.f && playerBottom < enemyTop + 10.f) {
                    enemy.takeDamage(enemy.getHp());  // �� ���� ����
                    player.bounceJump();              // ���� �ݵ�
                }
                else {
                    player.takeDamage(10);
                }
            }
        }

        window.clear(Color::White);
        gameBackground.draw(window);
        player.draw(window);
        enemy.draw(window);  // ���ο��� isDead() üũ��
        window.display();
    }

    return 0;
}
