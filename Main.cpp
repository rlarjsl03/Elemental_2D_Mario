#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Player.h"
#include "Enemy.h"

using namespace sf;

int main() {
   RenderWindow window(VideoMode(800, 600), "Player Control");
   window.setFramerateLimit(60);

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
       player.update(deltaTime); // �߷� ����
       enemy.update(deltaTime, 500.f);  // groundY = 500

       // �浹 üũ
       FloatRect playerBounds = player.getSprite().getGlobalBounds();
       FloatRect enemyBounds = enemy.getSprite().getGlobalBounds();

       if (playerBounds.intersects(enemyBounds)) {
           float playerBottom = playerBounds.top + playerBounds.height;
           float enemyTop = enemyBounds.top;

           if (playerBottom < enemyTop + 10.f) {
               // �÷��̾ �� ����
               enemy.takeDamage(enemy.getHp());
               player.bounceJump();
           }
           else {
               // �÷��̾ ���̳� �Ʒ��� ���� (������ ó�� ��)
               player.takeDamage(10);
           }
       }

       window.clear(Color::White);
       player.draw(window);            // �÷��̾� �׸���
       if (!enemy.isDead())
           enemy.draw(window);
       window.display();
   }

   return 0;
}