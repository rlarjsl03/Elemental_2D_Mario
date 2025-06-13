#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Background.h"
// #include "GameObject.h" // GameObject를 직접 사용하지 않으면 주석 처리하거나 제거해도 됩니다.

using namespace sf;

int main() {
    RenderWindow window(VideoMode(1920, 1080), "Mario 2D Game");
    window.setFramerateLimit(60);

    // 게임 변수
    float groundY = 800.f;

    Background gameBackground("Background.png");
    gameBackground.setScale(1.f, 1.f);

    Player player;
    player.setPosition(200.0f, groundY - player.getSprite().getGlobalBounds().height);

    Enemy enemy("cupa2.png", Vector2f(300.f, groundY));

    // --- sf::View (카메라) 설정 ---
    View gameView(FloatRect(0, 0, 1920, 1080)); // 윈도우와 동일한 크기의 뷰 생성

    // 맵의 경계 설정 (임시 값, 필요에 따라 조정)
    // 일반적으로는 레벨 데이터에서 가져오지만, 여기서는 임시로 넓은 값이나 배경 너비로 설정
    const float MAP_WIDTH = 5000.f; // 예시: 게임 맵의 전체 너비 (배경이 반복되므로 충분히 크게 설정)
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

        // --- 입력 및 업데이트 ---
        player.handleInput(deltaTime);
        player.update(deltaTime);
        enemy.update(deltaTime, groundY);



        // --- View (카메라) 업데이트 및 경계 제한 ---
        // 뷰의 목표 중심 X 좌표를 플레이어의 X 위치로 설정
        float targetViewCenterX = player.getSprite().getPosition().x;

        // 뷰의 Y축 중심은 고정 (바닥이 화면 하단에 맞춰지도록)
        float targetViewCenterY = VIEW_HEIGHT / 2.f; // 혹은 groundY - (VIEW_HEIGHT / 2.f)를 고려할 수도 있습니다.

        // 뷰의 X 위치를 맵 경계 내로 제한
        // 뷰의 왼쪽 끝 (targetViewCenterX - VIEW_WIDTH / 2)이 0보다 작아지지 않도록
        // 뷰의 오른쪽 끝 (targetViewCenterX + VIEW_WIDTH / 2)이 MAP_WIDTH를 넘지 않도록
        float minViewCenterX = VIEW_WIDTH / 2.f;
        float maxViewCenterX = MAP_WIDTH - (VIEW_WIDTH / 2.f);

        // 뷰의 X 중심이 경계 내에 있도록 클램프
        if (targetViewCenterX < minViewCenterX) {
            targetViewCenterX = minViewCenterX;
        }
        else if (targetViewCenterX > maxViewCenterX) {
            targetViewCenterX = maxViewCenterX;
        }

        gameView.setCenter(targetViewCenterX, targetViewCenterY);
        window.setView(gameView);

        // 배경 업데이트 (배경 스크롤 속도를 플레이어 속도와 연동)
        // 플레이어의 x 속도가 음수면 왼쪽으로, 양수면 오른쪽으로 이동합니다.
        // 배경은 플레이어의 이동 방향과 반대로 스크롤되어야 자연스럽습니다.
        // 예를 들어, 플레이어가 오른쪽으로 가면 배경은 왼쪽으로 이동해야 합니다.
        // 따라서 player.getVelocity().x 값의 부호를 반전시켜 배경에 전달합니다.
        // 배경 스크롤 속도에 대한 '시차 효과'를 주기 위해 상수를 곱할 수 있습니다.
        gameBackground.update(deltaTime, gameView.getCenter(), 0.5f);



        // --- 충돌 처리 ---
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

        // --- 그리기 ---
        window.clear(Color::White);

        gameBackground.draw(window);

        player.draw(window);
        if (!enemy.isDead())
            enemy.draw(window);

        window.display();
    }

    return 0;
}