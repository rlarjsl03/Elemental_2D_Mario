#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "Player.h"
#include "Enemy.h"
#include "Background.h"
#include "Item.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));  // 랜덤 초기화

    RenderWindow window(VideoMode(1920, 1080), "Player Control");
    window.setFramerateLimit(60);
    //Item coin("coin.png", Vector2f(500.f, 600.f));   // 아이템 객체 생성 06/05 추가

    // 게임 변수
    float groundY = 920.f;

    // 플레이어 초기 위치 설정
    Player player;
    player.setPosition(200.0f, groundY);

    // Background 객체 생성 (플레이어 초기 위치를 배경의 초기 X 위치로 사용)
    // 이렇게 하면 게임 시작 시 배경이 플레이어 위치에 맞춰져 보입니다.
    // 플레이어의 실제 X 위치를 얻기 위해 getSprite().getPosition().x 사용
    Background gameBackground("Background.png", player.getSprite().getPosition().x, 0.0f); // Y는 0.0f으로 임의 설정, 실제 게임에 따라 조절
    gameBackground.setScale(1.f, 1.f);


    // 텍스처 미리 로드
    Texture cupa, Goomba, Turtle;
    cupa.loadFromFile("cupa.png");
    Goomba.loadFromFile("Goomba.png");
    Turtle.loadFromFile("Turtle.png");

	std::vector<std::unique_ptr<Enemy>> enemies;    // 적 객체 생성
	// 적 객체 생성 및 초기 위치 설정
    enemies.push_back(std::make_unique<Enemy>(cupa, Vector2f(9500.f, groundY)));
    enemies.push_back(std::make_unique<Enemy>(Goomba, Vector2f(3000.f, groundY)));
    enemies.push_back(std::make_unique<Enemy>(Turtle, Vector2f(4000.f, groundY)));

    Clock clock;

    // 아이템 객체 생성
    std::vector<std::unique_ptr<Item>> items;
    items.push_back(std::make_unique<CoinItem>("Coin.png", Vector2f(900.0f, groundY-75.f))); // 코인
    items.push_back(std::make_unique<MushroomItem>("Mushroom.png", Vector2f(800.0f, groundY-75.f))); // 버섯
    items.push_back(std::make_unique<MushroomItem>("Mushroom.png", Vector2f(5000.0f, groundY - 75.f))); // 버섯


    // --- sf::View (카메라) 설정 ---
    View gameView(FloatRect(0, 0, 1920, 1080)); // 윈도우와 동일한 크기의 뷰 생성
    
    // 폰트 생성
    sf::Font font;
    font.loadFromFile("arial.ttf"); // 폰트 파일은 프로젝트에 있어야 함
    // 맵의 경계 설정: 플레이어가 "계속 갈 수 있게" 하려면 MAP_WIDTH를 매우 크게 설정
    // 이렇게 하면 뷰가 맵 끝에 도달하여 멈추는 일은 거의 없을 것입니다.
    const float MAP_WIDTH = 10000.f; // 매우 큰 값으로 설정
    const float VIEW_WIDTH = gameView.getSize().x;
    const float VIEW_HEIGHT = gameView.getSize().y;

    // 뷰의 초기 위치를 플레이어에게 맞춥니다.
    // 게임 시작부터 화면이 플레이어를 따라가도록 합니다.
    // 플레이어의 실제 X 위치를 얻기 위해 getSprite().getPosition().x 사용
    gameView.setCenter(player.getSprite().getPosition().x, VIEW_HEIGHT / 2.f);
    window.setView(gameView);



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

        // --- View (카메라) 업데이트 및 경계 제한 ---
        // 뷰의 목표 중심 X 좌표를 플레이어의 X 위치로 설정
        // 플레이어의 실제 X 위치를 얻기 위해 getSprite().getPosition().x 사용
        float targetViewCenterX = player.getSprite().getPosition().x + 400.f;
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



        // 충돌 체크
        sf::FloatRect playerBounds = player.getSprite().getGlobalBounds();

        for (auto& enemy : enemies) {
            enemy->update(deltaTime, groundY);    // groundY = 920.f

            //sf::FloatRect enemyBounds = enemy->getSprite().getGlobalBounds();
            sf::FloatRect playerHitBox = player.getHitBox();
            sf::FloatRect enemyHitBox = enemy->getHitBox();  // 적 클래스에 이 함수가 있다고 했으니


            if (!player.getisInvincible() && playerHitBox.intersects(enemyHitBox)) {
                float playerBottom = playerHitBox.top + playerHitBox.height;
                float enemyTop = enemyHitBox.top;

                // "적 위에서 밟은 경우"를 좀 더 엄격하게 체크
                if (player.getVelocity().y > 0.f && playerBottom <= enemyTop + 5.f) {
                    enemy->takeDamage(100);
                    player.bounceJump();
                }
                else {
                    if (player.getIsBig()) {
                        player.setIsBig(false);
                        player.setInvincible(true); // 무적 시작
                    }
                    else {
                        if (player.getLife() > 1) {
                            player.loseLife(1); // 생명 감소
                            player.setPosition(200.0f, groundY); // 플레이어 위치 초기화
                            gameView.setCenter(player.getSprite().getPosition().x, gameView.getCenter().y); // 뷰 위치 초기화
                            player.setInvincible(true); // 무적 시작
                        }
                        else
                            player.die(); // 작은 상태면 죽음
                    }
                    
                }
            }
        }
        // 마리오가 죽었을 때 Game Over 창 띄우기
        if (!player.isAlive()) {
            // 죽었을 때 처리: 게임 오버, 화면 정지, 리셋, 종료 등
            //window.clear(sf::Color::Black);

            // 간단한 Game Over 메시지 표시
            if (font.loadFromFile("arial.ttf")) {
                sf::Text gameOverText("Game Over", font, 100);
                gameOverText.setFillColor(sf::Color::Red);
                gameOverText.setPosition(gameView.getCenter().x - 300.f, gameView.getCenter().y - 100.f);
                window.draw(gameOverText);
            }
            window.display();
            sf::sleep(sf::seconds(3)); // 3초 대기 후 종료
            window.close();
            break;
        }


		// 배경 초기화
        window.clear(sf::Color::White);
        gameBackground.draw(window); // 배경 그리기
        player.draw(window);    // 플레이어 그리기
        // 적 그리기 및 업데이트
        for (auto& enemy : enemies)
            enemy->draw(window);

        // 플레이어의 생명 표시
        sf::Text lifeText;
        lifeText.setFont(font);
        lifeText.setCharacterSize(30);
        lifeText.setString("Life: " + std::to_string(player.getLife()));
        lifeText.setFillColor(sf::Color::Black);
        lifeText.setPosition(gameView.getCenter().x - 900.f, gameView.getCenter().y - 500.f);
        window.draw(lifeText);

        // 아이템 업데이트(그리기)  및 충돌 처리
        for (auto& item : items) {
            item->update(deltaTime);
            if (!item->isCollected() && item->checkCollision(player.getSprite())) {
                item->applyEffect(player);
            }
            item->draw(window);
        }
        window.display();
    }

    return 0;

}
