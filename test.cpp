#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Background.h"
// #include "GameObject.h" // GameObject�� ���� ������� ������ �ּ� ó���ϰų� �����ص� �˴ϴ�.

using namespace sf;

int main() {
    RenderWindow window(VideoMode(1920, 1080), "Mario 2D Game");
    window.setFramerateLimit(60);

    // ���� ����
    float groundY = 800.f;

    Background gameBackground("Background.png");
    gameBackground.setScale(1.f, 1.f);

    Player player;
    player.setPosition(200.0f, groundY - player.getSprite().getGlobalBounds().height);

    Enemy enemy("cupa2.png", Vector2f(300.f, groundY));

    // --- sf::View (ī�޶�) ���� ---
    View gameView(FloatRect(0, 0, 1920, 1080)); // ������� ������ ũ���� �� ����

    // ���� ��� ���� (�ӽ� ��, �ʿ信 ���� ����)
    // �Ϲ������δ� ���� �����Ϳ��� ����������, ���⼭�� �ӽ÷� ���� ���̳� ��� �ʺ�� ����
    const float MAP_WIDTH = 5000.f; // ����: ���� ���� ��ü �ʺ� (����� �ݺ��ǹǷ� ����� ũ�� ����)
    const float VIEW_WIDTH = gameView.getSize().x;
    const float VIEW_HEIGHT = gameView.getSize().y;

    Clock clock;

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
        float targetViewCenterX = player.getSprite().getPosition().x;

        // ���� Y�� �߽��� ���� (�ٴ��� ȭ�� �ϴܿ� ����������)
        float targetViewCenterY = VIEW_HEIGHT / 2.f; // Ȥ�� groundY - (VIEW_HEIGHT / 2.f)�� ����� ���� �ֽ��ϴ�.

        // ���� X ��ġ�� �� ��� ���� ����
        // ���� ���� �� (targetViewCenterX - VIEW_WIDTH / 2)�� 0���� �۾����� �ʵ���
        // ���� ������ �� (targetViewCenterX + VIEW_WIDTH / 2)�� MAP_WIDTH�� ���� �ʵ���
        float minViewCenterX = VIEW_WIDTH / 2.f;
        float maxViewCenterX = MAP_WIDTH - (VIEW_WIDTH / 2.f);

        // ���� X �߽��� ��� ���� �ֵ��� Ŭ����
        if (targetViewCenterX < minViewCenterX) {
            targetViewCenterX = minViewCenterX;
        }
        else if (targetViewCenterX > maxViewCenterX) {
            targetViewCenterX = maxViewCenterX;
        }

        gameView.setCenter(targetViewCenterX, targetViewCenterY);
        window.setView(gameView);

        // ��� ������Ʈ (��� ��ũ�� �ӵ��� �÷��̾� �ӵ��� ����)
        // �÷��̾��� x �ӵ��� ������ ��������, ����� ���������� �̵��մϴ�.
        // ����� �÷��̾��� �̵� ����� �ݴ�� ��ũ�ѵǾ�� �ڿ��������ϴ�.
        // ���� ���, �÷��̾ ���������� ���� ����� �������� �̵��ؾ� �մϴ�.
        // ���� player.getVelocity().x ���� ��ȣ�� �������� ��濡 �����մϴ�.
        // ��� ��ũ�� �ӵ��� ���� '���� ȿ��'�� �ֱ� ���� ����� ���� �� �ֽ��ϴ�.
        gameBackground.update(deltaTime, gameView.getCenter(), 0.5f);



        // --- �浹 ó�� ---
        FloatRect playerBounds = player.getSprite().getGlobalBounds();
        FloatRect enemyBounds = enemy.getSprite().getGlobalBounds();

        if (playerBounds.intersects(enemyBounds)) {
            float playerBottom = playerBounds.top + playerBounds.height;
            float enemyTop = enemyBounds.top;

            if (player.getVelocity().y > 0.f && playerBottom < enemyTop + 10.f) {
                enemy.takeDamage(enemy.getHp());
                player.bounceJump();
            }
            else {
                player.takeDamage(10);
            }
        }

        // --- �׸��� ---
        window.clear(Color::White);

        gameBackground.draw(window);

        player.draw(window);
        if (!enemy.isDead())
            enemy.draw(window);

        window.display();
    }

    return 0;
}