#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Player.h"
#include "BackGround.h"
//#inlcude "Enemy.h"

using namespace sf;

int main() {
   RenderWindow window(VideoMode(800, 600), "Player Control");
   //window.setVerticalSyncEnabled(true);
   window.setFramerateLimit(120);

   Background gameBackground("background1.jpg"); //백그라운드 객체 생성


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

       // 배경 업데이트 (예: 100 픽셀/초 속도로 스크롤)
        // 이 scrollSpeed는 게임 캐릭터의 이동 속도에 따라 동적으로 조절할 수 있습니다.
       gameBackground.update(deltaTime, 100.0f);
       window.clear(Color::White);
       gameBackground.draw(window); // 배경 그리기
       // 여기에 마리오, 블록, 적 등의 다른 게임 오브젝트 그리기 코드
       window.display(); // 화면에 최종 이미지 표시

       player.handleInput(deltaTime);  // 키 입력 처리
       player.update(deltaTime); // 중력 적용
       window.clear(Color::White);
       player.draw(window);            // 플레이어 그리기       window.display();

   }

   return 0;
}