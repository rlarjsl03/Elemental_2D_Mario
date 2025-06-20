#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>    // std::unique_ptr 사용
#include <cstdlib>   // std::srand, rand 사용
#include <ctime>     // std::time 사용
#include <iostream>  // std::cerr, std::cout 사용

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
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Player Control");
    window.setFramerateLimit(60); // 프레임 속도를 60FPS로 제한

    float groundY = 920.f;

    Player player;
    player.setPosition(200.0f, groundY); // 플레이어 초기 위치 설정 (X: 200, Y: 지면)

    // --- 배경 초기화 ---
    // 배경 이미지를 로드하고 플레이어 초기 위치에 맞춰 정렬합니다.
    Background gameBackground("Background.png", player.getSprite().getPosition().x, 0.0f);
    gameBackground.setScale(1.f, 1.f); // 배경 스케일 설정

    // --- 적 객체 생성 및 벡터에 추가 ---
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
    std::vector<std::unique_ptr<Enemy>> enemies;
    enemies.push_back(std::make_unique<Enemy>(cupaTexture, sf::Vector2f(4500.f, groundY)));
    enemies.push_back(std::make_unique<Enemy>(goombaTexture, sf::Vector2f(1200.f, groundY)));
    enemies.push_back(std::make_unique<Enemy>(turtleTexture, sf::Vector2f(2750.f, groundY)));

    sf::Clock clock;

    // --- 아이템 객체 생성 및 벡터에 추가 ---
    std::vector<std::unique_ptr<Item>> items;
    items.push_back(std::make_unique<CoinItem>("Coin.png", Vector2f(1300.0f, groundY - 75.f))); // 코인
    items.push_back(std::make_unique<CoinItem>("Coin.png", Vector2f(1400.0f, groundY - 75.f))); // 코인
    items.push_back(std::make_unique<CoinItem>("Coin.png", Vector2f(1500.0f, groundY - 75.f))); // 코인
    items.push_back(std::make_unique<CoinItem>("Coin.png", Vector2f(1600.0f, groundY - 75.f))); // 코인
    items.push_back(std::make_unique<CoinItem>("Coin.png", Vector2f(1700.0f, groundY - 75.f))); // 코인
    items.push_back(std::make_unique<CoinItem>("Coin.png", Vector2f(2800.0f, groundY - 75.f))); // 코인
    items.push_back(std::make_unique<CoinItem>("Coin.png", Vector2f(2900.0f, groundY - 75.f))); // 코인
    items.push_back(std::make_unique<CoinItem>("Coin.png", Vector2f(3100.0f, groundY - 75.f))); // 코인
    items.push_back(std::make_unique<CoinItem>("Coin.png", Vector2f(3200.0f, groundY - 75.f))); // 코인
    items.push_back(std::make_unique<MushroomItem>("RedMushroom.png", Vector2f(800.0f, groundY - 75.f))); // 버섯
    items.push_back(std::make_unique<GreenMushroomItem>("GreenMushroom.png", Vector2f(3000.0f, groundY - 75.f))); // 버섯
    items.push_back(std::make_unique<FlagItem>("Flag.png", Vector2f(4750.0f, groundY - 500.f))); // 깃발

    // GameObject들을 담을 벡터 선언
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    // --- 텍스처의 원본 크기 정의 ---
    const float ORIGINAL_PIPE_HEIGHT = 720.f;     // "pipe.png"의 실제 픽셀 높이 (720)
    const float ORIGINAL_PIPE_WIDTH = 564.f;      // "pipe.png"의 실제 픽셀 너비 (564)
    const float ORIGINAL_BLOCK_HEIGHT = 336.f;     // "mysteryblock.png", "deadblock.png"의 실제 픽셀 높이 (336)
    const float ORIGINAL_BLOCK_WIDTH = 336.f;      // "mysteryblock.png"의 실제 픽셀 너비 (336.f)
    const float ORIGINAL_PLATFORM_HEIGHT = 336.f;  // "platform.png"의 실제 픽셀 높이 (336.f)
    const float ORIGINAL_PLATFORM_WIDTH = 336.f;   // "platform.png"의 실제 픽셀 너비 (336.f)

    // --- 파이프 추가 및 크기/위치 조절 (수동 Y 위치 조정) ---
    // 
    // 짧은 파이프
    float pipe1X = 1000.f;
    float pipe1ScaleY = 0.35f;
    float pipe1ScaleX = 0.25f;
    float pipe1Y = groundY - (ORIGINAL_PIPE_HEIGHT * pipe1ScaleY); // 파이프 높이에 맞춰 groundY에 정렬
    auto pipe1 = std::make_unique<Pipe>("Pipe.png", sf::Vector2f(pipe1X, pipe1Y));
    pipe1->getSprite().setScale(pipe1ScaleX, pipe1ScaleY);
    gameObjects.push_back(std::move(pipe1));
    // 중간 파이프
    float pipe2X = 2500.f;
    float pipe2ScaleY = 0.35f;
    float pipe2ScaleX = 0.3f;
    float pipe2Y = groundY - (ORIGINAL_PIPE_HEIGHT * pipe2ScaleY);
    auto pipe2 = std::make_unique<Pipe>("pipe.png", sf::Vector2f(pipe2X, pipe2Y));
    pipe2->getSprite().setScale(pipe2ScaleX, pipe2ScaleY);
    gameObjects.push_back(std::move(pipe2));
    // 긴 파이프
    float pipe3X = 3500.f;
    float pipe3ScaleY = 0.35f;
    float pipe3ScaleX = 0.3f;
    float pipe3Y = groundY - (ORIGINAL_PIPE_HEIGHT * pipe3ScaleY);
    auto pipe3 = std::make_unique<Pipe>("pipe.png", sf::Vector2f(pipe3X, pipe3Y));
    pipe3->getSprite().setScale(pipe3ScaleX, pipe3ScaleY);
    gameObjects.push_back(std::move(pipe3));

    // --- 플랫폼 & 미스터리 블록 쌍 추가 및 크기/위치 조절 (좌우 정렬) ---
    // 쌍 1: 플랫폼 옆에 미스터리 블록 (코인 아이템)
    float pair1CurrentX = 1400.f; // 이 구조의 시작 X 위치
    float pair1BaseY = groundY - 200.f; // 플랫폼과 블록의 Y 위치
    float platform1ScaleX = 0.3f;
    float platform1ScaleY = 0.3f;

    auto platform1 = std::make_unique<Platform>("platform.png", sf::Vector2f(pair1CurrentX, pair1BaseY));
    platform1->getSprite().setScale(platform1ScaleX, platform1ScaleY);
    gameObjects.push_back(std::move(platform1));
    pair1CurrentX += (ORIGINAL_PLATFORM_WIDTH * platform1ScaleX); // 다음 객체 시작 X 업데이트

    // 플랫폼 옆에 딱 붙는 미스터리 블록
    float mysteryBlock1ScaleX = 0.3f;
    float mysteryBlock1ScaleY = 0.3f;
    // Y 위치는 플랫폼과 동일하게 설정하거나, 플랫폼 위에 오도록 계산할 수 있습니다.
    // 지금은 플랫폼과 같은 Y 레벨에 배치합니다.
    float mysteryBlock1Y = pair1BaseY;

    auto mysteryBlock1 = std::make_unique<MysteryBlock>(
        "mysteryblock.png",
        "deadblock.png",
        sf::Vector2f(pair1CurrentX, mysteryBlock1Y),
        std::make_unique<CoinItem>("Coin.png", sf::Vector2f(0, 0))
    );
    mysteryBlock1->getSprite().setScale(mysteryBlock1ScaleX, mysteryBlock1ScaleY);
    gameObjects.push_back(std::move(mysteryBlock1));
    pair1CurrentX += (ORIGINAL_BLOCK_WIDTH * mysteryBlock1ScaleX); // 다음 객체 시작 X 업데이트 (선택 사항)

    // 쌍 2: 좀 더 높은 플랫폼 옆에 미스터리 블록 (버섯 아이템)
    float pair2CurrentX = 1800.f;
    float pair2BaseY = groundY - 400.f; // 더 높은 Y 위치

    float platform2ScaleX = 0.3f;
    float platform2ScaleY = 0.3f;

    auto platform2 = std::make_unique<Platform>("platform.png", sf::Vector2f(pair2CurrentX, pair2BaseY));
    platform2->getSprite().setScale(platform2ScaleX, platform2ScaleY);
    gameObjects.push_back(std::move(platform2));
    pair2CurrentX += (ORIGINAL_PLATFORM_WIDTH * platform2ScaleX); // 다음 객체 시작 X 업데이트

    float mysteryBlock2ScaleX = 0.3f;
    float mysteryBlock2ScaleY = 0.3f;
    float mysteryBlock2Y = pair2BaseY; // 플랫폼과 같은 Y 레벨에 배치

    auto mysteryBlock2 = std::make_unique<MysteryBlock>(
        "mysteryblock.png",
        "deadblock.png",
        sf::Vector2f(pair2CurrentX, mysteryBlock2Y),
        std::make_unique<MushroomItem>("Redmushroom.png", sf::Vector2f(0, 0))
    );
    mysteryBlock2->getSprite().setScale(mysteryBlock2ScaleX, mysteryBlock2ScaleY);
    gameObjects.push_back(std::move(mysteryBlock2));
    pair2CurrentX += (ORIGINAL_BLOCK_WIDTH * mysteryBlock2ScaleX); // 다음 객체 시작 X 업데이트 (선택 사항)

    // 쌍 3: 여러 개의 블록과 플랫폼으로 이루어진 구조 (예시) - 좌우 정렬
    float pair3CurrentX = 4000.f; // 이 구조의 시작 X 위치
    float pair3BaseY = groundY - 200.f; // 이 구조의 기본 Y 위치

    // 첫 번째 미스터리 블록
    float mysteryBlock3_1ScaleX = 0.3f;
    float mysteryBlock3_1ScaleY = 0.3f;
    auto mysteryBlock3_1 = std::make_unique<MysteryBlock>(
        "mysteryblock.png",
        "deadblock.png",
        sf::Vector2f(pair3CurrentX, pair3BaseY),
        std::make_unique<CoinItem>("Coin.png", sf::Vector2f(0, 0))
    );
    mysteryBlock3_1->getSprite().setScale(mysteryBlock3_1ScaleX, mysteryBlock3_1ScaleY);
    gameObjects.push_back(std::move(mysteryBlock3_1));
    pair3CurrentX += (ORIGINAL_BLOCK_WIDTH * mysteryBlock3_1ScaleX); // 다음 객체 시작 X 업데이트

    // 첫 번째 블록 옆에 플랫폼 (높이 동일)
    float platform3ScaleX = 0.3f;
    float platform3ScaleY = 0.3f;
    auto platform3 = std::make_unique<Platform>("platform.png", sf::Vector2f(pair3CurrentX, pair3BaseY));
    platform3->getSprite().setScale(platform3ScaleX, platform3ScaleY);
    gameObjects.push_back(std::move(platform3));
    pair3CurrentX += (ORIGINAL_PLATFORM_WIDTH * platform3ScaleX); // 다음 객체 시작 X 업데이트

    // 플랫폼 옆에 또 다른 미스터리 블록 (높이 동일)
    float mysteryBlock3_2ScaleX = 0.3f;
    float mysteryBlock3_2ScaleY = 0.3f;
    float mysteryBlock3_2Y = pair3BaseY; // 플랫폼과 같은 Y 레벨에 배치

    auto mysteryBlock3_2 = std::make_unique<MysteryBlock>(
        "mysteryblock.png",
        "deadblock.png",
        sf::Vector2f(pair3CurrentX, mysteryBlock3_2Y),
        std::make_unique<GreenMushroomItem>("Greenmushroom.png", sf::Vector2f(0, 0))
    );
    mysteryBlock3_2->getSprite().setScale(mysteryBlock3_2ScaleX, mysteryBlock3_2ScaleY);
    gameObjects.push_back(std::move(mysteryBlock3_2));

    // --- sf::View (카메라) 설정 ---
    View gameView(FloatRect(0, 0, 1920, 1080)); // 윈도우와 동일한 크기의 뷰 생성
    
    // 폰트 생성
    sf::Font font;
    font.loadFromFile("arial.ttf"); // 폰트 파일은 프로젝트에 있어야 함
    // 맵의 경계 설정: 플레이어가 "계속 갈 수 있게" 하려면 MAP_WIDTH를 매우 크게 설정
    // 이렇게 하면 뷰가 맵 끝에 도달하여 멈추는 일은 거의 없을 것입니다.
    const float MAP_WIDTH = 5000.f; // 매우 큰 값으로 설정
    const float VIEW_WIDTH = gameView.getSize().x;
    const float VIEW_HEIGHT = gameView.getSize().y;

    // 뷰의 초기 위치를 플레이어에게 맞춥니다.
    gameView.setCenter(player.getSprite().getPosition().x, VIEW_HEIGHT / 2.f);
    window.setView(gameView); // 윈도우에 뷰 적용

    while (window.isOpen()) {
        sf::Time dt = clock.restart(); // 이전 프레임으로부터 경과 시간 측정
        float deltaTime = dt.asSeconds(); // 시간을 초 단위로 변환

        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) // 윈도우 닫기 버튼 클릭 시
                window.close(); // 윈도우 닫기
        }
        // --- 플레이어 입력 및 업데이트 ---
        player.handleInput(deltaTime);
        player.update(deltaTime);

		// 플레이어의 위치가 맵 경계 내에 있도록 조정
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

        sf::FloatRect playerHitBox = player.getHitBox();    // 플레이어 히트박스

		// 충돌 처리및 적 업데이트
        for (auto& enemy : enemies) {
            enemy->update(deltaTime, groundY, player.getSprite().getPosition());
            sf::FloatRect enemyHitBox = enemy->getHitBox(); // 적 히트박스

            // 플레이어가 무적 상태가 아닐 때만 적과 충돌 처리
            if (!player.getisInvincible() && playerHitBox.intersects(enemyHitBox)) {
                float playerBottom = playerHitBox.top + playerHitBox.height;
                float enemyTop = enemyHitBox.top;

                // "적 위에서 밟은 경우"를 좀 더 엄격하게 체크
                if (player.getVelocity().y > 0.f && playerBottom <= enemyTop + 10.f) {
                    enemy->takeDamage(100);
                    player.addScore(100);
                    player.bounceJump();
                }
                else {
                    if (player.getIsBig()) {
                        player.setIsBig(false);
                        player.setInvincible(true); // 무적 시작
						printf("Player is now small.\n"); // 디버깅용 출력
                        player.addScore(-200);
                    }
                    else { // 플레이어가 작은 상태이면
                        if (player.getLife() > 1) { // 생명이 1보다 많으면
                            player.loseLife(1); // 생명 감소
                            player.setPosition(200.0f, groundY); // 플레이어 위치 초기화
                            gameView.setCenter(player.getSprite().getPosition().x, gameView.getCenter().y); // 뷰 위치 초기화
                            player.setInvincible(true); // 무적 시작
                            player.addScore(-500);
                        }
                        printf("Player hit.\n"); // 디버깅용 출력
                    }
                }
            }
            // 적 리스폰
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

        // 플레이어와 게임 오브젝트(파이프, 플랫폼, 미스터리 블록) 충돌 처리
        for (auto& obj : gameObjects) { // 모든 게임 오브젝트에 대해 반복
            sf::FloatRect objBounds = obj->getGlobalBounds(); // 현재 오브젝트의 충돌 영역

            if (player.getVelocity().y > 0 &&
                playerHitBox.intersects(objBounds) &&
                // 플레이어의 바닥이 오브젝트 상단에 닿았는지 확인 (deltaTime을 이용해 이전 위치 추정)
                (playerHitBox.top + playerHitBox.height - (player.getVelocity().y * deltaTime)) <= objBounds.top) {

                // 현재 오브젝트가 Pipe 또는 Platform 타입인지 확인합니다.
                if (dynamic_cast<Pipe*>(obj.get()) || dynamic_cast<Platform*>(obj.get())) {
                    player.setPosition(player.getPosition().x, objBounds.top - player.getGlobalBounds().height); // 플레이어의 Y 위치를 오브젝트 상단에 맞춥니다.
                    player.setVelocityY(0);    // 플레이어의 수직 속도를 0으로 설정하여 더 이상 떨어지지 않게 합니다.
                    player.setIsOnGround(true); // 플레이어가 땅(오브젝트 위)에 닿은 상태로 설정합니다.
                }
            }
            if (MysteryBlock* mysteryBlock = dynamic_cast<MysteryBlock*>(obj.get())) {
                if (mysteryBlock->isActive() &&
                    player.getVelocity().y < 0 && // 플레이어가 위로 점프하는 중이어야 함
                    playerHitBox.intersects(objBounds) &&
                    playerHitBox.top <= objBounds.top + objBounds.height &&
                    playerHitBox.top + playerHitBox.height > objBounds.top + objBounds.height - 10.f) {

                    mysteryBlock->hit(); // 물음표 블록의 hit() 함수를 호출하여 상태를 변경합니다.
                    std::unique_ptr<Item> spawnedItem = mysteryBlock->spawnItem(); // 블록에서 아이템을 생성합니다.
                    if (spawnedItem) {
                        items.push_back(std::move(spawnedItem)); // 생성된 아이템을 전역 아이템 벡터에 추가합니다.
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
            gameOverText.setFillColor(sf::Color::Red);       // 텍스트 색상 빨간색
            // 뷰의 중앙에 메시지 위치 설정
            gameOverText.setPosition(gameView.getCenter().x - gameOverText.getGlobalBounds().width / 2.f,
                gameView.getCenter().y - gameOverText.getGlobalBounds().height / 2.f);
            window.draw(gameOverText); // 텍스트 그리기

            window.display(); // 화면 업데이트
            sf::sleep(sf::seconds(3)); // 3초 대기
            window.close(); // 윈도우 닫기 (게임 종료)
            break; // 게임 루프 종료
        }
        else if (player.getisWin()) {
            if (font.loadFromFile("arial.ttf")) {
                sf::Text gameWinText("You Win!!", font, 100);
                gameWinText.setFillColor(sf::Color::Black);
                gameWinText.setPosition(gameView.getCenter().x - 300.f, gameView.getCenter().y - 100.f);
                window.draw(gameWinText);
            }
            window.display();
            sf::sleep(sf::seconds(3)); // 3초 대기 후 종료
            window.close();
            break;
        }
        // --- 렌더링 (그리기) ---
        window.clear(sf::Color::White); // 배경을 흰색으로 지웁니다. (사실상 배경 이미지로 덮힘)
        gameBackground.draw(window);      // 배경을 그립니다.

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

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(30);
        scoreText.setString("Score: " + std::to_string(player.getScore()));
        scoreText.setFillColor(sf::Color::Black);
        scoreText.setPosition(gameView.getCenter().x - 900.f, gameView.getCenter().y - 450.f);
        window.draw(scoreText);

        // 아이템 업데이트(그리기)  및 충돌 처리
        for (auto& item : items) {
            item->update(deltaTime);
            if (!item->isCollected() && item->checkCollision(player.getSprite())) {
                item->applyEffect(player);
                // Replace the problematic line with the following:
                std::cout << player.getIsBig() << std::endl;
            }
            item->draw(window);
        }
        window.display();
        // 아이템 업데이트 및 그리기
        // 수집된 아이템은 벡터에서 제거합니다.
        //items.erase(std::remove_if(items.begin(), items.end(),
        //    [&](std::unique_ptr<Item>& item) {
        //        item->update(deltaTime);
        //        if (!item->isCollected() && item->checkCollision(player.getSprite())) {
        //            item->applyEffect(player);
        //            std::cout << "Player is big: " << (player.getIsBig() ? "true" : "false") << ", Life: " << player.getLife() << std::endl; // 디버깅 출력
        //            return true; // 아이템을 수집했으면 벡터에서 제거
        //        }
        //        item->draw(window);
        //        return false; // 수집되지 않았으면 유지
        //    }),
        //    items.end());

        //window.display(); // 최종 렌더링된 내용을 화면에 표시
    }

    return 0; // 프로그램 정상 종료
}