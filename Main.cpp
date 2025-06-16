// main.cpp
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>    // std::unique_ptr 사용
#include <cstdlib>    // std::srand, rand 사용
#include <ctime>      // std::time 사용
#include <iostream>  // std::cerr, std::cout 사용
#include <algorithm> // std::remove_if 를 위해 추가

// 커스텀 클래스 헤더 파일 포함
#include "Player.h"
#include "Enemy.h"
#include "Background.h"
#include "Item.h"
#include "GameObject.h" // 새로 추가된 게임 오브젝트 헤더

// --- 헬퍼 함수: 적들 간의 최소 거리 유지를 위한 함수 ---
// 새로운 적이 생성될 위치가 다른 적들과 너무 가까운지 확인합니다.
bool isTooCloseToOtherEnemies(const sf::Vector2f& pos, const std::vector<std::unique_ptr<Enemy>>& enemies, float minDistance) {
    for (const auto& other : enemies) {
        if (!other->isDead()) { // 죽지 않은 적들만 고려
            float dist = std::abs(other->getSprite().getPosition().x - pos.x); // X 좌표 간 거리 계산
            if (dist < minDistance)
                return true; // 너무 가까우면 true 반환
        }
    }
    return false; // 충분히 멀면 false 반환
}

// --- 메인 게임 함수 ---
int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr))); // 난수 생성기 초기화 (적 리스폰 등에 사용)

    // 윈도우 생성 (해상도 1920x1080, 제목 "Player Control")
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "2D Mario Game");
    window.setFramerateLimit(60); // 프레임 속도를 60FPS로 제한

    // --- 게임 기본 변수 ---
    float groundY = 920.f; // 게임 내 지면의 Y 좌표
    // 마리오의 최대 점프 높이를 고려하여, 점프 가능한 최대 Y 좌표 (더 낮을수록 위쪽)
    const float MARIO_MAX_JUMP_HEIGHT = 600.f; // 마리오가 지면으로부터 최대로 점프할 수 있는 높이
    const float MARIO_REACHABLE_MIN_Y = groundY - MARIO_MAX_JUMP_HEIGHT; // 마리오가 도달할 수 있는 가장 높은 지점의 Y 좌표

    // --- 플레이어 초기화 ---
    Player player;
    player.setPosition(200.0f, groundY); // 플레이어 초기 위치 설정 (X: 200, Y: 지면)

    // --- 배경 초기화 ---
    // 배경 이미지를 로드하고 플레이어 초기 위치에 맞춰 정렬합니다.
    Background gameBackground("Background.png", player.getSprite().getPosition().x, 0.0f);
    gameBackground.setScale(1.f, 1.f); // 배경 스케일 설정

    // --- 적 텍스처 미리 로드 ---
    sf::Texture cupaTexture, goombaTexture, turtleTexture;
    if (!cupaTexture.loadFromFile("cupa.png")) { // 이미지 로드 실패 시 오류 출력
        std::cerr << "Error loading cupa.png" << std::endl;
        return 1; // 프로그램 종료
    }
    if (!goombaTexture.loadFromFile("Goomba.png")) {
        std::cerr << "Error loading Goomba.png" << std::endl;
        return 1;
    }
    if (!turtleTexture.loadFromFile("Turtle.png")) {
        std::cerr << "Error loading Turtle.png" << std::endl;
        return 1;
    }

    // --- 적 객체 생성 및 벡터에 추가 ---
    std::vector<std::unique_ptr<Enemy>> enemies;
    enemies.push_back(std::make_unique<Enemy>(cupaTexture, sf::Vector2f(9500.f, groundY)));
    enemies.push_back(std::make_unique<Enemy>(goombaTexture, sf::Vector2f(3000.f, groundY)));
    enemies.push_back(std::make_unique<Enemy>(turtleTexture, sf::Vector2f(4000.f, groundY)));

    // --- 게임 타이밍 관리 ---
    sf::Clock clock; // 게임 시간 측정을 위한 클록

    // --- 아이템 객체 생성 및 벡터에 추가 ---
    std::vector<std::unique_ptr<Item>> items;
    // 참고: 미스터리 블록에서 스폰되는 아이템은 이 벡터에 동적으로 추가됩니다.

    // GameObject들을 담을 벡터 선언
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    // --- 중요: 텍스처의 원본 크기 정의 ---
    // 이 값들은 사용하는 이미지 파일의 실제 픽셀 크기에 맞춰야 합니다.
    // 이미지 뷰어 등으로 각 이미지의 실제 픽셀 크기를 확인하여 정확히 입력해주세요.
    // 이 값을 정확히 입력해야 setPosition 계산이 올바르게 됩니다.
    const float ORIGINAL_PIPE_HEIGHT = 720.f;     // "pipe.png"의 실제 픽셀 높이 (예: 160)
    const float ORIGINAL_PIPE_WIDTH = 564.f;      // "pipe.png"의 실제 픽셀 너비 (예: 112)
    const float ORIGINAL_BLOCK_HEIGHT = 336.f;    // "mysteryblock.png", "deadblock.png"의 실제 픽셀 높이 (예: 70)
    const float ORIGINAL_BLOCK_WIDTH = 336.f;     // "mysteryblock.png"의 실제 픽셀 너비 (예: 70)
    const float ORIGINAL_PLATFORM_HEIGHT = 336.f; // "platform.png"의 실제 픽셀 높이 (예: 70)
    const float ORIGINAL_PLATFORM_WIDTH = 336.f;  // "platform.png"의 실제 픽셀 너비 (예: 70)


    // --- 맵의 경계 설정: 플레이어가 계속 갈 수 있게 매우 크게 설정
    const float MAP_WIDTH = 10000.f; // 매우 큰 맵 너비

    // --- 맵 끝까지 파이프, 플랫폼, 미스터리 블록 추가 ---
    // 시작 X 위치 설정 (기존 오브젝트 이후부터 시작)
    float currentX = 1000.f; // 첫 파이프 시작 위치부터 이어서 배치

    // 마리오가 점프하여 도달할 수 있는 오브젝트의 최소 높이 (지면으로부터)
    // 오브젝트의 아랫면이 이 값보다 아래(Y값이 클수록) 있어야 마리오가 밟을 수 있습니다.
    // 마리오 스프라이트의 높이도 고려해야 점프해서 올라갔을 때 겹치지 않고 잘 서 있을 수 있습니다.
    // 플레이어의 스프라이트 높이에 따라 이 값을 조정해야 할 수 있습니다.
    // 여기서는 일단 마리오 머리 위 공간을 충분히 확보하는 정도로 설정합니다.
    const float MARIO_PLAYER_HEIGHT_OFFSET = 100.f; // 마리오의 대략적인 높이 (픽셀) 또는 여유 공간
    const float MAX_OBJECT_TOP_Y = groundY - (MARIO_MAX_JUMP_HEIGHT - MARIO_PLAYER_HEIGHT_OFFSET); // 오브젝트의 상단이 이 Y 좌표보다 아래에 있어야 함 (Y값이 클수록 아래)

    while (currentX < MAP_WIDTH - 500.f) { // 맵 끝에서 500px 전에 멈추도록 설정
        int objectType = rand() % 3; // 0: 파이프, 1: 플랫폼, 2: 미스터리 블록 (혹은 조합)

        if (objectType == 0) { // 파이프
            float pipeScaleY = 0.3f + static_cast<float>(rand() % 20) / 100.f; // 0.3 ~ 0.49 사이 랜덤 스케일
            float pipeScaleX = 0.25f + static_cast<float>(rand() % 10) / 100.f; // 0.25 ~ 0.34 사이 랜덤 스케일
            float pipeY = groundY - (ORIGINAL_PIPE_HEIGHT * pipeScaleY); // 파이프는 항상 지면에 고정

            // 파이프 상단이 마리오가 점프해서 도달 가능한 범위를 넘지 않도록 제한
            // 파이프 상단 Y = pipeY
            if (pipeY < MARIO_REACHABLE_MIN_Y) { // 만약 파이프가 너무 높다면
                pipeScaleY = (groundY - MARIO_REACHABLE_MIN_Y) / ORIGINAL_PIPE_HEIGHT; // 점프 가능 높이에 맞춰 스케일 재조정
                pipeY = groundY - (ORIGINAL_PIPE_HEIGHT * pipeScaleY); // Y 위치도 재계산
            }

            auto pipe = std::make_unique<Pipe>("Pipe.png", sf::Vector2f(currentX, pipeY));
            pipe->getSprite().setScale(pipeScaleX, pipeScaleY);
            gameObjects.push_back(std::move(pipe));
            currentX += (ORIGINAL_PIPE_WIDTH * pipeScaleX) + (rand() % 300 + 100.f); // 파이프 너비 + 랜덤 간격 추가
        }
        else if (objectType == 1) { // 플랫폼 또는 플랫폼-블록 조합
            float platformScaleX = 0.3f;
            float platformScaleY = 0.3f;
            // 플랫폼의 Y 위치는 groundY - (마리오가 점프할 수 있는 최대 높이 범위 내)로 설정
            // platformY가 작을수록 위에 위치합니다.
            // groundY - 200.f (최소 높이) ~ MAX_OBJECT_TOP_Y (최대 높이) 범위에서 랜덤
            float platformY = groundY - (200.f + static_cast<float>(rand() % static_cast<int>(MARIO_MAX_JUMP_HEIGHT - 200.f - MARIO_PLAYER_HEIGHT_OFFSET)));
            if (platformY < MARIO_REACHABLE_MIN_Y + ORIGINAL_PLATFORM_HEIGHT * platformScaleY) { // 너무 높으면 조정 (플랫폼 바닥 기준)
                platformY = MARIO_REACHABLE_MIN_Y + ORIGINAL_PLATFORM_HEIGHT * platformScaleY;
            }
            if (platformY > groundY - (ORIGINAL_PLATFORM_HEIGHT * platformScaleY)) { // 지면에 너무 가까우면 조정
                platformY = groundY - (ORIGINAL_PLATFORM_HEIGHT * platformScaleY);
            }

            auto platform = std::make_unique<Platform>("platform.png", sf::Vector2f(currentX, platformY));
            platform->getSprite().setScale(platformScaleX, platformScaleY);
            gameObjects.push_back(std::move(platform));
            currentX += (ORIGINAL_PLATFORM_WIDTH * platformScaleX); // 플랫폼 너비만큼 이동

            // 50% 확률로 플랫폼 옆에 미스터리 블록 추가
            if (rand() % 2 == 0) {
                float mysteryBlockScaleX = 0.3f;
                float mysteryBlockScaleY = 0.3f;
                float mysteryBlockY = platformY; // 플랫폼과 같은 높이에 배치

                // 어떤 아이템을 스폰할지 랜덤으로 결정
                std::unique_ptr<Item> itemToSpawn;
                int itemType = rand() % 3; // 0: 코인, 1: 버섯, 2: 녹색 버섯
                if (itemType == 0) {
                    itemToSpawn = std::make_unique<CoinItem>("Coin.png", sf::Vector2f(0, 0));
                }
                else if (itemType == 1) {
                    itemToSpawn = std::make_unique<MushroomItem>("Mushroom.png", sf::Vector2f(0, 0));
                }
                else {
                    itemToSpawn = std::make_unique<GreenMushroomItem>("GreenMushroom.png", sf::Vector2f(0, 0));
                }

                auto mysteryBlock = std::make_unique<MysteryBlock>(
                    "mysteryblock.png",
                    "deadblock.png",
                    sf::Vector2f(currentX, mysteryBlockY),
                    std::move(itemToSpawn)
                );
                mysteryBlock->getSprite().setScale(mysteryBlockScaleX, mysteryBlockScaleY);
                gameObjects.push_back(std::move(mysteryBlock));
                currentX += (ORIGINAL_BLOCK_WIDTH * mysteryBlockScaleX); // 블록 너비만큼 이동
            }
            currentX += (rand() % 400 + 150.f); // 플랫폼/블록 조합 이후 랜덤 간격 추가
        }
        else { // 미스터리 블록 단독 또는 연속
            float mysteryBlockScaleX = 0.3f;
            float mysteryBlockScaleY = 0.3f;
            // 미스터리 블록의 Y 위치도 마리오 점프 높이 범위 내로 설정
            float mysteryBlockY = groundY - (200.f + static_cast<float>(rand() % static_cast<int>(MARIO_MAX_JUMP_HEIGHT - 200.f - MARIO_PLAYER_HEIGHT_OFFSET)));
            if (mysteryBlockY < MARIO_REACHABLE_MIN_Y + ORIGINAL_BLOCK_HEIGHT * mysteryBlockScaleY) { // 너무 높으면 조정 (블록 바닥 기준)
                mysteryBlockY = MARIO_REACHABLE_MIN_Y + ORIGINAL_BLOCK_HEIGHT * mysteryBlockScaleY;
            }
            if (mysteryBlockY > groundY - (ORIGINAL_BLOCK_HEIGHT * mysteryBlockScaleY)) { // 지면에 너무 가까우면 조정
                mysteryBlockY = groundY - (ORIGINAL_BLOCK_HEIGHT * mysteryBlockScaleY);
            }

            // 어떤 아이템을 스폰할지 랜덤으로 결정
            std::unique_ptr<Item> itemToSpawn;
            int itemType = rand() % 3; // 0: 코인, 1: 버섯, 2: 녹색 버섯
            if (itemType == 0) {
                itemToSpawn = std::make_unique<CoinItem>("Coin.png", sf::Vector2f(0, 0));
            }
            else if (itemType == 1) {
                itemToSpawn = std::make_unique<MushroomItem>("Mushroom.png", sf::Vector2f(0, 0));
            }
            else {
                itemToSpawn = std::make_unique<GreenMushroomItem>("GreenMushroom.png", sf::Vector2f(0, 0));
            }

            auto mysteryBlock = std::make_unique<MysteryBlock>(
                "mysteryblock.png",
                "deadblock.png",
                sf::Vector2f(currentX, mysteryBlockY),
                std::move(itemToSpawn)
            );
            mysteryBlock->getSprite().setScale(mysteryBlockScaleX, mysteryBlockScaleY);
            gameObjects.push_back(std::move(mysteryBlock));
            currentX += (ORIGINAL_BLOCK_WIDTH * mysteryBlockScaleX) + (rand() % 200 + 50.f); // 블록 너비 + 랜덤 간격 추가

            // 30% 확률로 블록 하나 더 추가 (연속 블록처럼 보이게)
            if (rand() % 10 < 3) {
                // 어떤 아이템을 스폰할지 랜덤으로 결정 (두 번째 블록용)
                std::unique_ptr<Item> itemToSpawn2;
                int itemType2 = rand() % 3; // 0: 코인, 1: 버섯, 2: 녹색 버섯
                if (itemType2 == 0) {
                    itemToSpawn2 = std::make_unique<CoinItem>("Coin.png", sf::Vector2f(0, 0));
                }
                else if (itemType2 == 1) {
                    itemToSpawn2 = std::make_unique<MushroomItem>("Mushroom.png", sf::Vector2f(0, 0));
                }
                else {
                    itemToSpawn2 = std::make_unique<GreenMushroomItem>("GreenMushroom.png", sf::Vector2f(0, 0));
                }
                auto mysteryBlock2 = std::make_unique<MysteryBlock>(
                    "mysteryblock.png",
                    "deadblock.png",
                    sf::Vector2f(currentX, mysteryBlockY), // 같은 Y 높이
                    std::move(itemToSpawn2)
                );
                mysteryBlock2->getSprite().setScale(mysteryBlockScaleX, mysteryBlockScaleY);
                gameObjects.push_back(std::move(mysteryBlock2));
                currentX += (ORIGINAL_BLOCK_WIDTH * mysteryBlockScaleX) + (rand() % 100 + 20.f); // 블록 너비 + 작은 간격
            }
        }
    }

    // --- SFML View (카메라) 설정 ---
    sf::View gameView(sf::FloatRect(0, 0, 1920, 1080)); // 윈도우와 동일한 크기의 뷰 생성

    // 폰트 생성 (Game Over 메시지 및 UI용)
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) { // "arial.ttf" 폰트 파일이 프로젝트에 있어야 함
        std::cerr << "Error loading arial.ttf" << std::endl;
        return 1;
    }

    const float VIEW_WIDTH = gameView.getSize().x;
    const float VIEW_HEIGHT = gameView.getSize().y;

    // 뷰의 초기 위치를 플레이어에게 맞춥니다.
    gameView.setCenter(player.getSprite().getPosition().x, VIEW_HEIGHT / 2.f);
    window.setView(gameView); // 윈도우에 뷰 적용

    // --- 게임 루프 시작 ---
    while (window.isOpen()) {
        sf::Time dt = clock.restart(); // 이전 프레임으로부터 경과 시간 측정
        float deltaTime = dt.asSeconds(); // 시간을 초 단위로 변환

        // --- 이벤트 처리 ---
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) // 윈도우 닫기 버튼 클릭 시
                window.close(); // 윈도우 닫기
        }

        // --- 플레이어 입력 및 업데이트 ---
        player.handleInput(deltaTime);
        player.update(deltaTime);

        // --- 플레이어와 게임 오브젝트(파이프, 플랫폼, 미스터리 블록) 충돌 처리 ---
        // 이 부분은 Player::checkPlatformCollision() 함수로 이동되었습니다.
        player.checkPlatformCollision(deltaTime, gameObjects); // 플레이어의 지면 충돌 로직 호출

        // --- View (카메라) 업데이트 및 경계 제한 ---
        // 뷰의 목표 중심 X 좌표를 플레이어의 X 위치에 400px 오프셋을 더한 값으로 설정
        float targetViewCenterX = player.getSprite().getPosition().x + 400.f;
        float targetViewCenterY = VIEW_HEIGHT / 2.f;

        // 뷰의 X 위치를 맵 경계 내로 제한
        float minViewCenterX = VIEW_WIDTH / 2.f;
        float maxViewCenterX = MAP_WIDTH - (VIEW_WIDTH / 2.f);

        if (targetViewCenterX < minViewCenterX) {
            targetViewCenterX = minViewCenterX;
        }
        else if (targetViewCenterX > maxViewCenterX) {
            targetViewCenterX = maxViewCenterX;
        }

        gameView.setCenter(targetViewCenterX, targetViewCenterY); // 뷰 중심 설정
        window.setView(gameView); // 윈도우에 뷰 다시 적용

        // --- 배경 업데이트 ---
        // 뷰의 중심과 시차 계수(0.5f)를 전달하여 배경을 업데이트합니다.
        gameBackground.update(deltaTime, gameView.getCenter(), 0.5f);

        // --- 플레이어와 적 충돌 처리 및 적 업데이트 ---
        sf::FloatRect playerHitBox = player.getHitBox(); // 플레이어의 히트박스 가져오기

        for (auto& enemy : enemies) {
            enemy->update(deltaTime, groundY, player.getSprite().getPosition()); // 적 업데이트

            sf::FloatRect enemyHitBox = enemy->getHitBox(); // 적의 히트박스 가져오기

            // 플레이어가 무적 상태가 아닐 때만 적과 충돌 처리
            if (!player.getisInvincible() && playerHitBox.intersects(enemyHitBox)) {
                float playerBottom = playerHitBox.top + playerHitBox.height;
                float enemyTop = enemyHitBox.top;

                // "적 위에서 밟은 경우"를 좀 더 엄격하게 체크
                // 플레이어가 아래로 이동 중이고(점프 후 착지 중), 플레이어 바닥이 적 머리보다 약간 위에 있을 때
                if (player.getVelocity().y > 0.f && playerBottom <= enemyTop + 5.f) {
                    enemy->takeDamage(100); // 적에게 데미지
                    player.bounceJump();    // 플레이어는 튕겨오름
                }
                else { // 적에게 옆/아래에서 부딪힌 경우 (데미지 받음)
                    if (player.getIsBig()) { // 플레이어가 큰 상태이면
                        player.setIsBig(false); // 작은 상태로 돌아가고
                        player.setInvincible(true); // 무적 상태 시작
                        printf("Player is now small.\n"); // 디버깅용 출력
                    }
                    else { // 플레이어가 작은 상태이면
                        if (player.getLife() > 1) { // 생명이 1보다 많으면
                            player.loseLife(1); // 생명 감소
                            player.setPosition(200.0f, groundY); // 플레이어 위치 초기화
                            gameView.setCenter(player.getSprite().getPosition().x, gameView.getCenter().y); // 뷰 위치 초기화
                            player.setInvincible(true); // 무적 상태 시작
                        }
                        else { // 생명이 1밖에 없으면
                            player.die(); // 죽음 처리
                        }
                        printf("Player hit.\n"); // 디버깅용 출력
                    }
                }
            }
            // 적 리스폰 로직
            if (enemy->isDead() && enemy->canRespawn()) {
                float offsetX = 500.f + static_cast<float>(rand() % 400); // 플레이어 앞쪽으로 랜덤 오프셋
                float spawnX = player.getSprite().getPosition().x + offsetX;
                sf::Vector2f proposedPos(spawnX, groundY);

                // 다른 적과 너무 가깝지 않으면 리스폰
                if (!isTooCloseToOtherEnemies(proposedPos, enemies, 100.f)) {
                    enemy->respawnAt(proposedPos);
                }
            }
        }

        // --- 물음표 블록을 아래에서 머리로 치는 경우 (main.cpp에 그대로 유지) ---
        // 이 로직은 아이템을 생성하고 items 벡터에 추가해야 하므로 Player 클래스 내부에 두기 어렵습니다.
        for (auto& obj : gameObjects) {
            if (MysteryBlock* mysteryBlock = dynamic_cast<MysteryBlock*>(obj.get())) {
                sf::FloatRect objBounds = obj->getGlobalBounds();
                sf::FloatRect playerHitBox = player.getHitBox(); // 플레이어의 히트박스 가져오기

                if (mysteryBlock->isActive() &&
                    player.getVelocity().y < 0 && // 플레이어가 위로 점프하는 중이어야 함
                    playerHitBox.intersects(objBounds) &&
                    // 플레이어의 머리가 블록의 바닥 부분에 닿았는지 확인 (약간의 오차 허용)
                    playerHitBox.top <= objBounds.top + objBounds.height &&
                    playerHitBox.top + playerHitBox.height > objBounds.top + objBounds.height - 10.f) {

                    mysteryBlock->hit();
                    std::unique_ptr<Item> spawnedItem = mysteryBlock->spawnItem();
                    if (spawnedItem) {
                        items.push_back(std::move(spawnedItem));
                    }
                    player.setVelocityY(0); // 블록을 치면 플레이어의 수직 속도를 0으로 만들어 더 이상 위로 올라가지 않게 합니다.
                }
            }
        }

        // --- 마리오 사망 시 Game Over 처리 ---
        if (!player.isAlive()) { // 플레이어가 죽었으면
            // 윈도우를 다시 그리기 전에 모든 것을 지웁니다.
            window.clear(sf::Color::Black); // 검은색 배경으로 설정

            // 간단한 Game Over 메시지 표시
            sf::Text gameOverText("Game Over", font, 100); // 폰트와 크기 설정
            gameOverText.setFillColor(sf::Color::Red);         // 텍스트 색상 빨간색
            // 뷰의 중앙에 메시지 위치 설정
            gameOverText.setPosition(gameView.getCenter().x - gameOverText.getGlobalBounds().width / 2.f,
                gameView.getCenter().y - gameOverText.getGlobalBounds().height / 2.f);
            window.draw(gameOverText); // 텍스트 그리기

            window.display(); // 화면 업데이트
            sf::sleep(sf::seconds(3)); // 3초 대기
            window.close(); // 윈도우 닫기 (게임 종료)
            break; // 게임 루프 종료
        }

        // --- 렌더링 (그리기) ---
        window.clear(sf::Color::White); // 배경을 흰색으로 지웁니다. (사실상 배경 이미지로 덮힘)
        gameBackground.draw(window);        // 배경을 그립니다.

        // 게임 오브젝트(파이프, 플랫폼, 미스터리 블록) 그리기
        // 플레이어보다 먼저 그려야 플레이어가 오브젝트 위에 올라선 것처럼 보입니다.
        for (auto& obj : gameObjects) {
            obj->draw(window);
        }

        player.draw(window); // 플레이어를 그립니다.

        // 적 그리기
        for (auto& enemy : enemies) {
            enemy->draw(window);
        }

        // 플레이어 생명 표시 UI
        sf::Text lifeText;
        lifeText.setFont(font);
        lifeText.setCharacterSize(30);
        lifeText.setString("Life: " + std::to_string(player.getLife()));
        lifeText.setFillColor(sf::Color::Black);
        // 뷰의 왼쪽 상단에 생명 UI 위치 설정
        lifeText.setPosition(gameView.getCenter().x - VIEW_WIDTH / 2.f + 10.f, gameView.getCenter().y - VIEW_HEIGHT / 2.f + 10.f);
        window.draw(lifeText);

        // 아이템 업데이트 및 그리기
        // 수집된 아이템은 벡터에서 제거합니다.
        items.erase(std::remove_if(items.begin(), items.end(),
            [&](std::unique_ptr<Item>& item) {
                item->update(deltaTime);
                if (!item->isCollected() && item->checkCollision(player.getSprite())) {
                    item->applyEffect(player);
                    std::cout << "Player is big: " << (player.getIsBig() ? "true" : "false") << ", Life: " << player.getLife() << std::endl; // 디버깅 출력
                    return true; // 아이템을 수집했으면 벡터에서 제거
                }
                item->draw(window);
                return false; // 수집되지 않았으면 유지
            }),
            items.end());

        window.display(); // 최종 렌더링된 내용을 화면에 표시
    }

    return 0; // 프로그램 정상 종료
}