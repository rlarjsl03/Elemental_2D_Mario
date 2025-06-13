#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Player.h"
#include "Enemy.h"
#include "BackGround.h"
#include "Item.h"

using namespace sf;

int main() {
    RenderWindow window(VideoMode(1920, 1080), "Player Control");
    window.setFramerateLimit(60);
    //Item coin("coin.png", Vector2f(500.f, 600.f));   // ������ ��ü ���� 06/05 �߰�

    Background gameBackground("Background.jpg");
    Player player;
    Enemy enemy("enemy.png", Vector2f(300.f, 0.f));

    Clock clock;

    // ������ ��ü ����
    std::vector<std::unique_ptr<Item>> items;
    items.push_back(std::make_unique<CoinItem>("Coin.png", Vector2f(900.0f, 700.0f))); // ����
    items.push_back(std::make_unique<MushroomItem>("Mushroom.png", Vector2f(800.0f, 720.0f))); // ����

    while (window.isOpen()) {
        Time dt = clock.restart();
        float deltaTime = dt.asSeconds();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        player.handleInput(deltaTime);  // Ű �Է� ó��
        // ��� ������Ʈ (��: 100 �ȼ�/�� �ӵ��� ��ũ��)
            // �� scrollSpeed�� ���� ĳ������ �̵� �ӵ��� ���� �������� ������ �� �ֽ��ϴ�.
        gameBackground.update(deltaTime, 100.0f);
        player.update(deltaTime); // �߷� ����
        enemy.update(deltaTime, 800.f);  // groundY = 500
        gameBackground.draw(window); // ��� �׸���

        // �浹 üũ
        FloatRect playerBounds = player.getSprite().getGlobalBounds();
        FloatRect enemyBounds = enemy.getSprite().getGlobalBounds();

        // ���� ����ִ� ��쿡�� �浹 ����
        if (enemy.isActive()) {
            FloatRect playerBounds = player.getSprite().getGlobalBounds();
            FloatRect enemyBounds = enemy.getSprite().getGlobalBounds();

            if (playerBounds.intersects(enemyBounds)) {
                float playerBottom = playerBounds.top + playerBounds.height;
                float enemyTop = enemyBounds.top;

                // 1. �������� �ִ� ������ Ȯ�� (velocity.y > 0)
                // 2. �÷��̾ �� ������ ������ ���
                if (player.getVelocity().y > 0.f && playerBottom < enemyTop + 10.f) {
                    enemy.takeDamage(enemy.getHp());
                    player.bounceJump(); // ƨ�� ���� 06/05 �߰�
                }
                else {
                    player.takeDamage(10);
                }
            }
            window.clear(Color::White);
            gameBackground.draw(window); // ��� �׸���

            player.draw(window);            // �÷��̾� �׸���

            // �� �׸��� �� ������Ʈ
            if (!enemy.isDead())
                enemy.draw(window);

            // ������ ������Ʈ(�׸���)  �� �浹 ó��
            for (auto& item : items) {
                item->update(deltaTime);
                if (!item->isCollected() && item->checkCollision(player.getSprite())) {
                    item->applyEffect(player);
                }
                item->draw(window);
            }
            window.display();
        }
        return 0;
    }
}
