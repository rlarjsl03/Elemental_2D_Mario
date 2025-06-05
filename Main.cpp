#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Player.h"
#include "Enemy.h"
#include "BackGround.h"

using namespace sf;

int main() {
   RenderWindow window(VideoMode(1920, 1080), "Player Control");
   window.setFramerateLimit(60);

   Background gameBackground("Background.jpg"); //��׶��� ��ü ����
   Player player;
   Enemy enemy("enemy.png", Vector2f(300.f, 0.f));  // y�� update���� �ٴ� ������

   Clock clock;

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
       if (!enemy.isDead())
           enemy.draw(window);
       window.display();
   }

   return 0;
}