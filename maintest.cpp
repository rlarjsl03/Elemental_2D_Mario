#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector> // 여러 오브젝트를 관리하기 위해 vector 사용
#include "Playertest.h" // Player.h 대신 Playertest.h를 사용하는 것으로 보임
#include "Enemy.h"
#include "Background.h" // Background.h로 수정
#include "GameObject.h" // 새로 추가된 GameObject 헤더

using namespace sf;

int main() {
    RenderWindow window(VideoMode(1920, 1080), "Mario 2D Game");
    window.setFramerateLimit(60);

    // 게임 변수
    float groundY = 910.f; // 플레이어와 오브젝트들이 닿는 바닥 Y 좌표

    Background gameBackground("Background.png"); // 백그라운드 객체 생성
    gameBackground.setScale(1.f, 1.f); // 예시 스케일, 실제 배경 이미지 크기에 따라 조절

    Player player;
    // 플레이어 초기 위치 설정 (바닥에 맞춰)
    player.setPosition(200.0f, groundY - player.getSprite().getGlobalBounds().height);

    Enemy enemy("cupa2.png", Vector2f(1000.f, groundY)); // y는 update에서 바닥 고정됨

    // --- 게임 오브젝트들 (Pipe, Platform, MysteryBlock) ---
    // 여러 오브젝트를 관리하기 위한 벡터
    std::vector<Pipe> pipes;
    std::vector<Platform> platforms;
    std::vector<MysteryBlock> mysteryBlocks;

    // 파이프 추가 (텍스처, 초기 위치, 바닥 Y 좌표, 스케일)
    // 이제 파이프는 하나만 생성됩니다.
    pipes.emplace_back("pipe.png", Vector2f(500.f, 0.f), groundY, Vector2f(0.8f, 0.8f)); // 높이 자동 조절됨

    // 플랫폼 추가 (텍스처, 초기 위치, 스케일)
    // 이제 플랫폼은 하나만 생성됩니다.
    // 현재 스케일 Vector2f(0.2f, 0.2f)로 지정되어 있습니다.
    platforms.emplace_back("platform.png", Vector2f(700.f, groundY - 250.f), Vector2f(0.2f, 0.2f));

    // 미스터리 블록 추가 (텍스처, 초기 위치, 스케일)
    // 첫 번째 (그리고 유일한) 플랫폼 옆에 배치합니다.
    // 미스터리 블록의 스케일을 플랫폼과 동일하게 Vector2f(0.2f, 0.2f)로 설정합니다.
    mysteryBlocks.emplace_back("mysteryblock.png",
        Vector2f(platforms[0].getPosition().x + platforms[0].getGlobalBounds().width, platforms[0].getPosition().y),
        Vector2f(0.2f, 0.2f)); // 플랫폼의 스케일과 동일하게 설정

    Clock clock;

    while (window.isOpen()) {
        Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        player.handleInput(deltaTime); // 키 입력 처리
        gameBackground.update(deltaTime, 100.0f); // 배경 업데이트
        player.update(deltaTime); // 중력 적용
        enemy.update(deltaTime, groundY); // 적 업데이트 (바닥 Y 좌표 전달)

        // --- 충돌 처리 ---

        // 플레이어와 적 충돌 체크
        FloatRect playerBounds = player.getSprite().getGlobalBounds();
        FloatRect enemyBounds = enemy.getSprite().getGlobalBounds();

        if (playerBounds.intersects(enemyBounds)) {
            float playerBottom = playerBounds.top + playerBounds.height;
            float enemyTop = enemyBounds.top;

            // 1. 떨어지고 있는 중인지 확인 (velocity.y > 0)
            // 2. 플레이어가 적 위에서 떨어진 경우 (플레이어 바닥이 적 상단보다 약간 위에 있을 때)
            if (player.getVelocity().y > 0.f && playerBottom < enemyTop + 10.f) {
                enemy.takeDamage(enemy.getHp()); // 적 사망
                player.bounceJump(); // 튕겨 오름
            }
            else {
                // 적에게 옆으로 부딪히거나 아래에서 부딪힌 경우 플레이어 데미지
                player.takeDamage(10); // 플레이어 데미지 처리 (구현 필요)
            }
        }

        // 플레이어와 플랫폼 충돌 체크
        for (auto& platform : platforms) {
            FloatRect platformBounds = platform.getGlobalBounds();
            if (playerBounds.intersects(platformBounds)) {
                // 플레이어가 플랫폼 위로 착지하는 경우
                if (player.getVelocity().y > 0 && // 플레이어가 아래로 움직이고 있고
                    playerBounds.top + playerBounds.height - player.getVelocity().y * deltaTime <= platformBounds.top && // 이전 프레임에는 플랫폼 위에 있지 않았지만 현재 프레임에 닿음
                    playerBounds.top + playerBounds.height > platformBounds.top) { // 플레이어 바닥이 플랫폼 상단에 닿음

                    player.setPosition(player.getSprite().getPosition().x, platformBounds.top - playerBounds.height); // 플랫폼 위에 안착
                    player.setVelocityY(0); // 수직 속도 0으로 설정
                    player.setOnGround(true); // 땅에 닿았다고 설정 (플랫폼을 땅처럼 처리)
                }
            }
        }

        // 플레이어와 미스터리 블록 충돌 체크
        for (auto& block : mysteryBlocks) {
            FloatRect blockBounds = block.getGlobalBounds();
            if (playerBounds.intersects(blockBounds)) {
                // 플레이어가 블록 아래에서 위로 점프하여 닿는 경우
                if (player.getVelocity().y < 0 && // 플레이어가 위로 움직이고 있고
                    playerBounds.top >= blockBounds.top + blockBounds.height - 10.f) { // 플레이어 머리가 블록 하단에 닿음 (오차 범위 10px)

                    block.hit(); // 블록 닿음 처리
                    player.setVelocityY(0); // 플레이어 수직 속도 0으로 설정 (튕겨 오름 방지)
                }
            }
        }

        // --- 그리기 ---
        window.clear(Color::White); // 화면 지우기 (배경색 흰색)

        gameBackground.draw(window); // 배경 그리기

        // 파이프 그리기
        for (const auto& pipe : pipes) {
            pipe.draw(window);
        }

        // 플랫폼 그리기
        for (const auto& platform : platforms) {
            platform.draw(window);
        }

        // 미스터리 블록 그리기
        for (const auto& block : mysteryBlocks) {
            block.draw(window);
        }

        player.draw(window); // 플레이어 그리기
        if (!enemy.isDead()) // 적이 죽지 않았다면 그림
            enemy.draw(window);

        window.display(); // 화면에 그리기
    }

    return 0;
}