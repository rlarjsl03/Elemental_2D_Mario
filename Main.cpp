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
    float groundY = 920.f;

    // �÷��̾� �ʱ� ��ġ ����
    Player player;
    player.setPosition(200.0f, groundY);

    // Background ��ü ���� (�÷��̾� �ʱ� ��ġ�� ����� �ʱ� X ��ġ�� ���)
    // �̷��� �ϸ� ���� ���� �� ����� �÷��̾� ��ġ�� ������ ���Դϴ�.
    // �÷��̾��� ���� X ��ġ�� ��� ���� getSprite().getPosition().x ���
    Background gameBackground("Background.png", player.getSprite().getPosition().x, 0.0f); // Y�� 0.0f���� ���� ����, ���� ���ӿ� ���� ����
    gameBackground.setScale(1.f, 1.f);


    // �ؽ�ó �̸� �ε�
    Texture cupa, Goomba, Turtle;
    cupa.loadFromFile("cupa.png");
    Goomba.loadFromFile("Goomba.png");
    Turtle.loadFromFile("Turtle.png");

	std::vector<std::unique_ptr<Enemy>> enemies;    // �� ��ü ����
	// �� ��ü ���� �� �ʱ� ��ġ ����
    enemies.push_back(std::make_unique<Enemy>(cupa, Vector2f(9500.f, groundY)));
    enemies.push_back(std::make_unique<Enemy>(Goomba, Vector2f(3000.f, groundY)));
    enemies.push_back(std::make_unique<Enemy>(Turtle, Vector2f(4000.f, groundY)));

    Clock clock;

    // ������ ��ü ����
    std::vector<std::unique_ptr<Item>> items;
    items.push_back(std::make_unique<CoinItem>("Coin.png", Vector2f(900.0f, groundY-75.f))); // ����
    items.push_back(std::make_unique<MushroomItem>("Mushroom.png", Vector2f(800.0f, groundY-75.f))); // ����
    items.push_back(std::make_unique<MushroomItem>("Mushroom.png", Vector2f(5000.0f, groundY - 75.f))); // ����


    // --- sf::View (ī�޶�) ���� ---
    View gameView(FloatRect(0, 0, 1920, 1080)); // ������� ������ ũ���� �� ����
    
    // ��Ʈ ����
    sf::Font font;
    font.loadFromFile("arial.ttf"); // ��Ʈ ������ ������Ʈ�� �־�� ��
    // ���� ��� ����: �÷��̾ "��� �� �� �ְ�" �Ϸ��� MAP_WIDTH�� �ſ� ũ�� ����
    // �̷��� �ϸ� �䰡 �� ���� �����Ͽ� ���ߴ� ���� ���� ���� ���Դϴ�.
    const float MAP_WIDTH = 10000.f; // �ſ� ū ������ ����
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

        // --- View (ī�޶�) ������Ʈ �� ��� ���� ---
        // ���� ��ǥ �߽� X ��ǥ�� �÷��̾��� X ��ġ�� ����
        // �÷��̾��� ���� X ��ġ�� ��� ���� getSprite().getPosition().x ���
        float targetViewCenterX = player.getSprite().getPosition().x + 400.f;
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
            enemy->update(deltaTime, groundY);    // groundY = 920.f

            //sf::FloatRect enemyBounds = enemy->getSprite().getGlobalBounds();
            sf::FloatRect playerHitBox = player.getHitBox();
            sf::FloatRect enemyHitBox = enemy->getHitBox();  // �� Ŭ������ �� �Լ��� �ִٰ� ������


            if (!player.getisInvincible() && playerHitBox.intersects(enemyHitBox)) {
                float playerBottom = playerHitBox.top + playerHitBox.height;
                float enemyTop = enemyHitBox.top;

                // "�� ������ ���� ���"�� �� �� �����ϰ� üũ
                if (player.getVelocity().y > 0.f && playerBottom <= enemyTop + 5.f) {
                    enemy->takeDamage(100);
                    player.bounceJump();
                }
                else {
                    if (player.getIsBig()) {
                        player.setIsBig(false);
                        player.setInvincible(true); // ���� ����
                    }
                    else {
                        if (player.getLife() > 1) {
                            player.loseLife(1); // ���� ����
                            player.setPosition(200.0f, groundY); // �÷��̾� ��ġ �ʱ�ȭ
                            gameView.setCenter(player.getSprite().getPosition().x, gameView.getCenter().y); // �� ��ġ �ʱ�ȭ
                            player.setInvincible(true); // ���� ����
                        }
                        else
                            player.die(); // ���� ���¸� ����
                    }
                    
                }
            }
        }
        // �������� �׾��� �� Game Over â ����
        if (!player.isAlive()) {
            // �׾��� �� ó��: ���� ����, ȭ�� ����, ����, ���� ��
            //window.clear(sf::Color::Black);

            // ������ Game Over �޽��� ǥ��
            if (font.loadFromFile("arial.ttf")) {
                sf::Text gameOverText("Game Over", font, 100);
                gameOverText.setFillColor(sf::Color::Red);
                gameOverText.setPosition(gameView.getCenter().x - 300.f, gameView.getCenter().y - 100.f);
                window.draw(gameOverText);
            }
            window.display();
            sf::sleep(sf::seconds(3)); // 3�� ��� �� ����
            window.close();
            break;
        }


		// ��� �ʱ�ȭ
        window.clear(sf::Color::White);
        gameBackground.draw(window); // ��� �׸���
        player.draw(window);    // �÷��̾� �׸���
        // �� �׸��� �� ������Ʈ
        for (auto& enemy : enemies)
            enemy->draw(window);

        // �÷��̾��� ���� ǥ��
        sf::Text lifeText;
        lifeText.setFont(font);
        lifeText.setCharacterSize(30);
        lifeText.setString("Life: " + std::to_string(player.getLife()));
        lifeText.setFillColor(sf::Color::Black);
        lifeText.setPosition(gameView.getCenter().x - 900.f, gameView.getCenter().y - 500.f);
        window.draw(lifeText);

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
