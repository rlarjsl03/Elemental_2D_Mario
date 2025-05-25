#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Player.h"
#include "Enemy.h"

using namespace sf;

int main() {
   RenderWindow window(VideoMode(800, 600), "Player Control");
   window.setFramerateLimit(60);

   Player player;
   Enemy enemy("enemy.png", Vector2f(300.f, 0.f));  // y는 update에서 바닥 고정됨

   Clock clock;

   while (window.isOpen()) {
       Time dt = clock.restart();
       float deltaTime = dt.asSeconds();

       Event event;
       while (window.pollEvent(event)) {
           if (event.type == Event::Closed)
               window.close();
       }

       player.handleInput(deltaTime);  // 키 입력 처리
       player.update(deltaTime); // 중력 적용
       enemy.update(deltaTime, 500.f);  // groundY = 500

       // 충돌 체크
       FloatRect playerBounds = player.getSprite().getGlobalBounds();
       FloatRect enemyBounds = enemy.getSprite().getGlobalBounds();

       if (playerBounds.intersects(enemyBounds)) {
           float playerBottom = playerBounds.top + playerBounds.height;
           float enemyTop = enemyBounds.top;

           if (playerBottom < enemyTop + 10.f) {
               // 플레이어가 적 밟음
               enemy.takeDamage(enemy.getHp());
               player.bounceJump();
           }
           else {
               // 플레이어가 옆이나 아래서 닿음 (데미지 처리 등)
               player.takeDamage(10);
           }
       }

       window.clear(Color::White);
       player.draw(window);            // 플레이어 그리기
       if (!enemy.isDead())
           enemy.draw(window);
       window.display();
   }

   return 0;
}