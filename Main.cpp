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

    Background gameBackground("Background.jpg");
    Player player;

    // 텍스처 미리 로드
    Texture cupa, mushroom4, turtle3;
    cupa.loadFromFile("cupa2.png");
    mushroom4.loadFromFile("mushroom4.png");
    turtle3.loadFromFile("turtle3.png");

    std::vector<std::unique_ptr<Enemy>> enemies;
    enemies.push_back(std::make_unique<Enemy>(cupa, Vector2f(300.f, 0.f)));
    enemies.push_back(std::make_unique<Enemy>(mushroom4, Vector2f(700.f, 0.f)));
    enemies.push_back(std::make_unique<Enemy>(turtle3, Vector2f(1100.f, 0.f)));

    Clock clock;

    // 아이템 객체 생성
    std::vector<std::unique_ptr<Item>> items;
    items.push_back(std::make_unique<CoinItem>("Coin.png", Vector2f(900.0f, 700.0f))); // 코인
    items.push_back(std::make_unique<MushroomItem>("Mushroom.png", Vector2f(800.0f, 720.0f))); // 버섯

    while (window.isOpen()) {
        Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        player.handleInput(deltaTime);  // 키 입력 처리
        // 배경 업데이트 (예: 100 픽셀/초 속도로 스크롤)
        // 이 scrollSpeed는 게임 캐릭터의 이동 속도에 따라 동적으로 조절할 수 있습니다.
        gameBackground.update(deltaTime, 100.0f);
        player.update(deltaTime); // 중력 적용


        // 충돌 체크
        sf::FloatRect playerBounds = player.getSprite().getGlobalBounds();

        for (auto& enemy : enemies) {
            enemy->update(deltaTime, 800.f);    // groundY = 500

            sf::FloatRect enemyBounds = enemy->getSprite().getGlobalBounds();

            if (playerBounds.intersects(enemyBounds)) {
                float playerBottom = playerBounds.top + playerBounds.height;
                float enemyTop = enemyBounds.top;

                if (player.getVelocity().y > 0.f && playerBottom < enemyTop + 10.f) {
                    enemy->takeDamage(100); // 적 제거
                    player.bounceJump();    // 튕기기
                }
                else {
                    player.takeDamage(10);
                }
            }
        }

        window.clear(sf::Color::White);
        gameBackground.draw(window); // 배경 그리기
        player.draw(window);    // 플레이어 그리기
        // 적 그리기 및 업데이트
        for (auto& enemy : enemies)
            enemy->draw(window);

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
