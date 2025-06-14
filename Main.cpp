#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "Player.h"
#include "Enemy.h"
#include "Background.h"
#include "Item.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));  // ���� �ʱ�ȭ

    RenderWindow window(VideoMode(1920, 1080), "Player Control");
    window.setFramerateLimit(60);
    //Item coin("coin.png", Vector2f(500.f, 600.f));   // ������ ��ü ���� 06/05 �߰�

    // ���� ����
    float groundY = 800.f;

    // �÷��̾� �ʱ� ��ġ ����
    Player player;
    player.setPosition(200.0f, groundY - player.getSprite().getGlobalBounds().height);

    // Background ��ü ���� (�÷��̾� �ʱ� ��ġ�� ����� �ʱ� X ��ġ�� ���)
    // �̷��� �ϸ� ���� ���� �� ����� �÷��̾� ��ġ�� ������ ���Դϴ�.
    // �÷��̾��� ���� X ��ġ�� ��� ���� getSprite().getPosition().x ���
    Background gameBackground("Background.png", player.getSprite().getPosition().x, 0.0f); // Y�� 0.0f���� ���� ����, ���� ���ӿ� ���� ����
    gameBackground.setScale(1.f, 1.f);


    // �ؽ�ó �̸� �ε�
    Texture cupa, mushroom4, turtle3;
    cupa.loadFromFile("cupa2.png");
    mushroom4.loadFromFile("mushroom4.png");
    turtle3.loadFromFile("turtle3.png");

    std::vector<std::unique_ptr<Enemy>> enemies;
    enemies.push_back(std::make_unique<Enemy>(cupa, Vector2f(300.f, 0.f)));
    enemies.push_back(std::make_unique<Enemy>(mushroom4, Vector2f(700.f, 0.f)));
    enemies.push_back(std::make_unique<Enemy>(turtle3, Vector2f(1100.f, 0.f)));

    Clock clock;

    // ������ ��ü ����
    std::vector<std::unique_ptr<Item>> items;
    items.push_back(std::make_unique<CoinItem>("Coin.png", Vector2f(900.0f, 700.0f))); // ����
    items.push_back(std::make_unique<MushroomItem>("Mushroom.png", Vector2f(800.0f, 720.0f))); // ����

    // --- sf::View (ī�޶�) ���� ---
    View gameView(FloatRect(0, 0, 1920, 1080)); // ������� ������ ũ���� �� ����

    // ���� ��� ����: �÷��̾ "��� �� �� �ְ�" �Ϸ��� MAP_WIDTH�� �ſ� ũ�� ����
    // �̷��� �ϸ� �䰡 �� ���� �����Ͽ� ���ߴ� ���� ���� ���� ���Դϴ�.
    const float MAP_WIDTH = 100000.f; // �ſ� ū ������ ����
    const float VIEW_WIDTH = gameView.getSize().x;
    const float VIEW_HEIGHT = gameView.getSize().y;

    // ���� �ʱ� ��ġ�� �÷��̾�� ����ϴ�.
    // ���� ���ۺ��� ȭ���� �÷��̾ ���󰡵��� �մϴ�.
    // �÷��̾��� ���� X ��ġ�� ��� ���� getSprite().getPosition().x ���
    gameView.setCenter(player.getSprite().getPosition().x, VIEW_HEIGHT / 2.f);
    window.setView(gameView);



    while (window.isOpen()) {
        Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // --- �Է� �� ������Ʈ ---
        player.handleInput(deltaTime);
        player.update(deltaTime);
        enemy.update(deltaTime, groundY);


        // --- View (ī�޶�) ������Ʈ �� ��� ���� ---
        // ���� ��ǥ �߽� X ��ǥ�� �÷��̾��� X ��ġ�� ����
        // �÷��̾��� ���� X ��ġ�� ��� ���� getSprite().getPosition().x ���
        float targetViewCenterX = player.getSprite().getPosition().x;
        float targetViewCenterY = VIEW_HEIGHT / 2.f;

        // ���� X ��ġ�� �� ��� ���� ����
        // �� ������ `MAP_WIDTH`�� �ſ� ũ���� �����Ͽ�,
        // Ȥ�� �� �ε� �Ҽ��� ������ ���� ��Ȳ���� �䰡 �� ������ ������ ���� �����մϴ�.
        float minViewCenterX = VIEW_WIDTH / 2.f;
        float maxViewCenterX = MAP_WIDTH - (VIEW_WIDTH / 2.f);

        if (targetViewCenterX < minViewCenterX) {
            targetViewCenterX = minViewCenterX;
        }
        else if (targetViewCenterX > maxViewCenterX) {
            targetViewCenterX = maxViewCenterX;
        }

        gameView.setCenter(targetViewCenterX, targetViewCenterY);
        window.setView(gameView);

        // ��� ������Ʈ: ���� �߽ɰ� ���� ����� ����
        // 0.5f�� ���� ���(Parallax Factor)�Դϴ�. 0.1f ~ 0.8f ������ ���� ��õ�մϴ�.
        // �� ���� �����Ͽ� ����� ���� �����ӿ� �󸶳� �������� �����մϴ�.
        gameBackground.update(deltaTime, gameView.getCenter(), 0.5f);



        // �浹 üũ
        sf::FloatRect playerBounds = player.getSprite().getGlobalBounds();

        for (auto& enemy : enemies) {
            enemy->update(deltaTime, 800.f);    // groundY = 500

            sf::FloatRect enemyBounds = enemy->getSprite().getGlobalBounds();

            if (playerBounds.intersects(enemyBounds)) {
                float playerBottom = playerBounds.top + playerBounds.height;
                float enemyTop = enemyBounds.top;

                if (player.getVelocity().y > 0.f && playerBottom < enemyTop + 10.f) {
                    enemy->takeDamage(100); // �� ����
                    player.bounceJump();    // ƨ���
                }
                else {
                    player.takeDamage(10);
                }
            }
        }

        window.clear(sf::Color::White);
        gameBackground.draw(window); // ��� �׸���
        player.draw(window);    // �÷��̾� �׸���
        // �� �׸��� �� ������Ʈ
        for (auto& enemy : enemies)
            enemy->draw(window);

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
