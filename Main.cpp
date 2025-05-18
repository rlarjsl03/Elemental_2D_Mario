#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Player.h"
//#inlcude "Enemy.h"

using namespace sf;

int main() {
   RenderWindow window(VideoMode(800, 600), "Player Control");
   window.setFramerateLimit(60);

   Player player;
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

       window.clear(Color::White);
       player.draw(window);            // 플레이어 그리기
       window.display();
   }

   return 0;
}