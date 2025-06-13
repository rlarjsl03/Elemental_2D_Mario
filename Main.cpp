#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Background.h"

using namespace sf;

int main() {
    RenderWindow window(VideoMode(1920, 1080), "Mario 2D Game");
    window.setFramerateLimit(60);

    // 게임 변수
    float groundY = 800.f;

    // 플레이어 초기 위치 설정
    Player player;
    player.setPosition(200.0f, groundY - player.getSprite().getGlobalBounds().height);

    // Background 객체 생성 (플레이어 초기 위치를 배경의 초기 X 위치로 사용)
    // 이렇게 하면 게임 시작 시 배경이 플레이어 위치에 맞춰져 보입니다.
    // 플레이어의 실제 X 위치를 얻기 위해 getSprite().getPosition().x 사용
    Background gameBackground("Background.png", player.getSprite().getPosition().x, 0.0f); // Y는 0.0f으로 임의 설정, 실제 게임에 따라 조절
    gameBackground.setScale(1.f, 1.f);


    Enemy enemy("cupa2.png", Vector2f(300.f, groundY));

    // --- sf::View (카메라) 설정 ---
    View gameView(FloatRect(0, 0, 1920, 1080)); // 윈도우와 동일한 크기의 뷰 생성

    // 맵의 경계 설정: 플레이어가 "계속 갈 수 있게" 하려면 MAP_WIDTH를 매우 크게 설정
    // 이렇게 하면 뷰가 맵 끝에 도달하여 멈추는 일은 거의 없을 것입니다.
    const float MAP_WIDTH = 100000.f; // 매우 큰 값으로 설정
    const float VIEW_WIDTH = gameView.getSize().x;
    const float VIEW_HEIGHT = gameView.getSize().y;

    // 뷰의 초기 위치를 플레이어에게 맞춥니다.
    // 게임 시작부터 화면이 플레이어를 따라가도록 합니다.
    // 플레이어의 실제 X 위치를 얻기 위해 getSprite().getPosition().x 사용
    gameView.setCenter(player.getSprite().getPosition().x, VIEW_HEIGHT / 2.f);
    window.setView(gameView);


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
        // 플레이어의 실제 X 위치를 얻기 위해 getSprite().getPosition().x 사용
        float targetViewCenterX = player.getSprite().getPosition().x;
        float targetViewCenterY = VIEW_HEIGHT / 2.f;

        // 뷰의 X 위치를 맵 경계 내로 제한
        // 이 로직은 `MAP_WIDTH`가 매우 크더라도 유지하여,
        // 혹시 모를 부동 소수점 오차나 극한 상황에서 뷰가 맵 밖으로 나가는 것을 방지합니다.
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

        // 배경 업데이트: 뷰의 중심과 시차 계수를 전달
        // 0.5f는 시차 계수(Parallax Factor)입니다. 0.1f ~ 0.8f 사이의 값을 추천합니다.
        // 이 값을 조절하여 배경이 뷰의 움직임에 얼마나 반응할지 제어합니다.
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