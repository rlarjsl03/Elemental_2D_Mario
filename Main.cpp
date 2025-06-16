// main.cpp
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>    // std::unique_ptr ���
#include <cstdlib>   // std::srand, rand ���
#include <ctime>     // std::time ���
#include <iostream>  // std::cerr, std::cout ���

// Ŀ���� Ŭ���� ��� ���� ����
#include "Player.h"
#include "Enemy.h"
#include "Background.h"
#include "Item.h"
#include "GameObject.h" // ���� �߰��� ���� ������Ʈ ���

// --- ���� �Լ�: ���� ���� �ּ� �Ÿ� ������ ���� �Լ� ---
// ���ο� ���� ������ ��ġ�� �ٸ� ����� �ʹ� ������� Ȯ���մϴ�.
bool isTooCloseToOtherEnemies(const sf::Vector2f& pos, const std::vector<std::unique_ptr<Enemy>>& enemies, float minDistance) {
    for (const auto& other : enemies) {
        if (!other->isDead()) { // ���� ���� ���鸸 ���
            float dist = std::abs(other->getSprite().getPosition().x - pos.x); // X ��ǥ �� �Ÿ� ���
            if (dist < minDistance)
                return true; // �ʹ� ������ true ��ȯ
        }
    }
    return false; // ����� �ָ� false ��ȯ
}

// --- ���� ���� �Լ� ---
int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr))); // ���� ������ �ʱ�ȭ (�� ������ � ���)

    // ������ ���� (�ػ� 1920x1080, ���� "Player Control")
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Player Control");
    window.setFramerateLimit(60); // ������ �ӵ��� 60FPS�� ����

    // --- ���� �⺻ ���� ---
    float groundY = 920.f; // ���� �� ������ Y ��ǥ

    // --- �÷��̾� �ʱ�ȭ ---
    Player player;
    player.setPosition(200.0f, groundY); // �÷��̾� �ʱ� ��ġ ���� (X: 200, Y: ����)

    // --- ��� �ʱ�ȭ ---
    // ��� �̹����� �ε��ϰ� �÷��̾� �ʱ� ��ġ�� ���� �����մϴ�.
    Background gameBackground("Background.png", player.getSprite().getPosition().x, 0.0f);
    gameBackground.setScale(1.f, 1.f); // ��� ������ ����

    // --- �� �ؽ�ó �̸� �ε� ---
    sf::Texture cupaTexture, goombaTexture, turtleTexture;
    if (!cupaTexture.loadFromFile("cupa.png")) { // �̹��� �ε� ���� �� ���� ���
        std::cerr << "Error loading cupa.png" << std::endl;
        return 1; // ���α׷� ����
    }
    if (!goombaTexture.loadFromFile("Goomba.png")) {
        std::cerr << "Error loading Goomba.png" << std::endl;
        return 1;
    }
    if (!turtleTexture.loadFromFile("Turtle.png")) {
        std::cerr << "Error loading Turtle.png" << std::endl;
        return 1;
    }

    // --- �� ��ü ���� �� ���Ϳ� �߰� ---
    std::vector<std::unique_ptr<Enemy>> enemies;
    enemies.push_back(std::make_unique<Enemy>(cupaTexture, sf::Vector2f(9500.f, groundY)));
    enemies.push_back(std::make_unique<Enemy>(goombaTexture, sf::Vector2f(3000.f, groundY)));
    enemies.push_back(std::make_unique<Enemy>(turtleTexture, sf::Vector2f(4000.f, groundY)));

    // --- ���� Ÿ�̹� ���� ---
    sf::Clock clock; // ���� �ð� ������ ���� Ŭ��

    // --- ������ ��ü ���� �� ���Ϳ� �߰� ---
    std::vector<std::unique_ptr<Item>> items;
    // ����: �̽��͸� ��Ͽ��� �����Ǵ� �������� �� ���Ϳ� �������� �߰��˴ϴ�.

    // GameObject���� ���� ���� ����
    std::vector<std::unique_ptr<GameObject>> gameObjects;

    // --- �߿�: �ؽ�ó�� ���� ũ�� ���� ---
    // �� ������ ����ϴ� �̹��� ������ ���� �ȼ� ũ�⿡ ����� �մϴ�.
    // �̹��� ��� ������ �� �̹����� ���� �ȼ� ũ�⸦ Ȯ���Ͽ� ��Ȯ�� �Է����ּ���.
    // �� ���� ��Ȯ�� �Է��ؾ� setPosition ����� �ùٸ��� �˴ϴ�.
    const float ORIGINAL_PIPE_HEIGHT = 720.f;     // "pipe.png"�� ���� �ȼ� ���� (��: 160)
    const float ORIGINAL_PIPE_WIDTH = 564.f;      // "pipe.png"�� ���� �ȼ� �ʺ� (��: 112)
    const float ORIGINAL_BLOCK_HEIGHT = 336.f;     // "mysteryblock.png", "deadblock.png"�� ���� �ȼ� ���� (��: 70)
    const float ORIGINAL_BLOCK_WIDTH = 336.f;      // "mysteryblock.png"�� ���� �ȼ� �ʺ� (��: 70)
    const float ORIGINAL_PLATFORM_HEIGHT = 336.f;  // "platform.png"�� ���� �ȼ� ���� (��: 70)
    const float ORIGINAL_PLATFORM_WIDTH = 336.f;   // "platform.png"�� ���� �ȼ� �ʺ� (��: 70)


    // --- ������ �߰� �� ũ��/��ġ ���� (���� Y ��ġ ����) ---

    // ª�� ������
    float pipe1X = 1000.f;
    float pipe1ScaleY = 0.35f;
    float pipe1ScaleX = 0.25f;
    float pipe1Y = groundY - (ORIGINAL_PIPE_HEIGHT * pipe1ScaleY); // ������ ���̿� ���� groundY�� ����
    auto pipe1 = std::make_unique<Pipe>("Pipe.png", sf::Vector2f(pipe1X, pipe1Y));
    pipe1->getSprite().setScale(pipe1ScaleX, pipe1ScaleY);
    gameObjects.push_back(std::move(pipe1));

    // �߰� ������
    float pipe2X = 2500.f;
    float pipe2ScaleY = 0.35f;
    float pipe2ScaleX = 0.3f;
    float pipe2Y = groundY - (ORIGINAL_PIPE_HEIGHT * pipe2ScaleY);
    auto pipe2 = std::make_unique<Pipe>("pipe.png", sf::Vector2f(pipe2X, pipe2Y));
    pipe2->getSprite().setScale(pipe2ScaleX, pipe2ScaleY);
    gameObjects.push_back(std::move(pipe2));

    // �� ������
    float pipe3X = 3500.f;
    float pipe3ScaleY = 0.35f;
    float pipe3ScaleX = 0.3f;
    float pipe3Y = groundY - (ORIGINAL_PIPE_HEIGHT * pipe3ScaleY);
    auto pipe3 = std::make_unique<Pipe>("pipe.png", sf::Vector2f(pipe3X, pipe3Y));
    pipe3->getSprite().setScale(pipe3ScaleX, pipe3ScaleY);
    gameObjects.push_back(std::move(pipe3));

    // --- �÷��� & �̽��͸� ��� �� �߰� �� ũ��/��ġ ���� (�¿� ����) ---

    // �� 1: �÷��� ���� �̽��͸� ��� (���� ������)
    float pair1CurrentX = 1400.f; // �� ������ ���� X ��ġ
    float pair1BaseY = groundY - 200.f; // �÷����� ����� Y ��ġ

    float platform1ScaleX = 0.3f;
    float platform1ScaleY = 0.3f;

    auto platform1 = std::make_unique<Platform>("platform.png", sf::Vector2f(pair1CurrentX, pair1BaseY));
    platform1->getSprite().setScale(platform1ScaleX, platform1ScaleY);
    gameObjects.push_back(std::move(platform1));
    pair1CurrentX += (ORIGINAL_PLATFORM_WIDTH * platform1ScaleX); // ���� ��ü ���� X ������Ʈ

    // �÷��� ���� �� �ٴ� �̽��͸� ���
    float mysteryBlock1ScaleX = 0.3f;
    float mysteryBlock1ScaleY = 0.3f;
    // Y ��ġ�� �÷����� �����ϰ� �����ϰų�, �÷��� ���� ������ ����� �� �ֽ��ϴ�.
    // ������ �÷����� ���� Y ������ ��ġ�մϴ�.
    float mysteryBlock1Y = pair1BaseY;

    auto mysteryBlock1 = std::make_unique<MysteryBlock>(
        "mysteryblock.png",
        "deadblock.png",
        sf::Vector2f(pair1CurrentX, mysteryBlock1Y),
        std::make_unique<CoinItem>("Coin.png", sf::Vector2f(0, 0))
    );
    mysteryBlock1->getSprite().setScale(mysteryBlock1ScaleX, mysteryBlock1ScaleY);
    gameObjects.push_back(std::move(mysteryBlock1));
    pair1CurrentX += (ORIGINAL_BLOCK_WIDTH * mysteryBlock1ScaleX); // ���� ��ü ���� X ������Ʈ (���� ����)


    // �� 2: �� �� ���� �÷��� ���� �̽��͸� ��� (���� ������)
    float pair2CurrentX = 1800.f;
    float pair2BaseY = groundY - 400.f; // �� ���� Y ��ġ

    float platform2ScaleX = 0.3f;
    float platform2ScaleY = 0.3f;

    auto platform2 = std::make_unique<Platform>("platform.png", sf::Vector2f(pair2CurrentX, pair2BaseY));
    platform2->getSprite().setScale(platform2ScaleX, platform2ScaleY);
    gameObjects.push_back(std::move(platform2));
    pair2CurrentX += (ORIGINAL_PLATFORM_WIDTH * platform2ScaleX); // ���� ��ü ���� X ������Ʈ

    float mysteryBlock2ScaleX = 0.3f;
    float mysteryBlock2ScaleY = 0.3f;
    float mysteryBlock2Y = pair2BaseY; // �÷����� ���� Y ������ ��ġ

    auto mysteryBlock2 = std::make_unique<MysteryBlock>(
        "mysteryblock.png",
        "deadblock.png",
        sf::Vector2f(pair2CurrentX, mysteryBlock2Y),
        std::make_unique<MushroomItem>("Mushroom.png", sf::Vector2f(0, 0))
    );
    mysteryBlock2->getSprite().setScale(mysteryBlock2ScaleX, mysteryBlock2ScaleY);
    gameObjects.push_back(std::move(mysteryBlock2));
    pair2CurrentX += (ORIGINAL_BLOCK_WIDTH * mysteryBlock2ScaleX); // ���� ��ü ���� X ������Ʈ (���� ����)


    // �� 3: ���� ���� ��ϰ� �÷������� �̷���� ���� (����) - �¿� ����
    float pair3CurrentX = 4000.f; // �� ������ ���� X ��ġ
    float pair3BaseY = groundY - 200.f; // �� ������ �⺻ Y ��ġ

    // ù ��° �̽��͸� ���
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
    pair3CurrentX += (ORIGINAL_BLOCK_WIDTH * mysteryBlock3_1ScaleX); // ���� ��ü ���� X ������Ʈ

    // ù ��° ��� ���� �÷��� (���� ����)
    float platform3ScaleX = 0.3f;
    float platform3ScaleY = 0.3f;
    auto platform3 = std::make_unique<Platform>("platform.png", sf::Vector2f(pair3CurrentX, pair3BaseY));
    platform3->getSprite().setScale(platform3ScaleX, platform3ScaleY);
    gameObjects.push_back(std::move(platform3));
    pair3CurrentX += (ORIGINAL_PLATFORM_WIDTH * platform3ScaleX); // ���� ��ü ���� X ������Ʈ

    // �÷��� ���� �� �ٸ� �̽��͸� ��� (���� ����)
    float mysteryBlock3_2ScaleX = 0.3f;
    float mysteryBlock3_2ScaleY = 0.3f;
    float mysteryBlock3_2Y = pair3BaseY; // �÷����� ���� Y ������ ��ġ

    auto mysteryBlock3_2 = std::make_unique<MysteryBlock>(
        "mysteryblock.png",
        "deadblock.png",
        sf::Vector2f(pair3CurrentX, mysteryBlock3_2Y),
        std::make_unique<MushroomItem>("Mushroom.png", sf::Vector2f(0, 0))
    );
    mysteryBlock3_2->getSprite().setScale(mysteryBlock3_2ScaleX, mysteryBlock3_2ScaleY);
    gameObjects.push_back(std::move(mysteryBlock3_2));
    // �� �ڿ� �� ���� ������Ʈ�� �߰��� ��� pair3CurrentX�� ��� ������Ʈ�ϸ� �˴ϴ�.


    // --- SFML View (ī�޶�) ���� ---
    sf::View gameView(sf::FloatRect(0, 0, 1920, 1080)); // ������� ������ ũ���� �� ����

    // ��Ʈ ���� (Game Over �޽��� �� UI��)
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) { // "arial.ttf" ��Ʈ ������ ������Ʈ�� �־�� ��
        std::cerr << "Error loading arial.ttf" << std::endl;
        return 1;
    }

    // ���� ��� ����: �÷��̾ ��� �� �� �ְ� �ſ� ũ�� ����
    const float MAP_WIDTH = 10000.f; // �ſ� ū �� �ʺ�
    const float VIEW_WIDTH = gameView.getSize().x;
    const float VIEW_HEIGHT = gameView.getSize().y;

    // ���� �ʱ� ��ġ�� �÷��̾�� ����ϴ�.
    gameView.setCenter(player.getSprite().getPosition().x, VIEW_HEIGHT / 2.f);
    window.setView(gameView); // �����쿡 �� ����

    // --- ���� ���� ���� ---
    while (window.isOpen()) {
        sf::Time dt = clock.restart(); // ���� ���������κ��� ��� �ð� ����
        float deltaTime = dt.asSeconds(); // �ð��� �� ������ ��ȯ

        // --- �̺�Ʈ ó�� ---
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) // ������ �ݱ� ��ư Ŭ�� ��
                window.close(); // ������ �ݱ�
        }

        // --- �÷��̾� �Է� �� ������Ʈ ---
        player.handleInput(deltaTime);
        player.update(deltaTime);

        // --- View (ī�޶�) ������Ʈ �� ��� ���� ---
        // ���� ��ǥ �߽� X ��ǥ�� �÷��̾��� X ��ġ�� 400px �������� ���� ������ ����
        float targetViewCenterX = player.getSprite().getPosition().x + 400.f;
        float targetViewCenterY = VIEW_HEIGHT / 2.f;

        // ���� X ��ġ�� �� ��� ���� ����
        float minViewCenterX = VIEW_WIDTH / 2.f;
        float maxViewCenterX = MAP_WIDTH - (VIEW_WIDTH / 2.f);

        if (targetViewCenterX < minViewCenterX) {
            targetViewCenterX = minViewCenterX;
        }
        else if (targetViewCenterX > maxViewCenterX) {
            targetViewCenterX = maxViewCenterX;
        }

        gameView.setCenter(targetViewCenterX, targetViewCenterY); // �� �߽� ����
        window.setView(gameView); // �����쿡 �� �ٽ� ����

        // --- ��� ������Ʈ ---
        // ���� �߽ɰ� ���� ���(0.5f)�� �����Ͽ� ����� ������Ʈ�մϴ�.
        gameBackground.update(deltaTime, gameView.getCenter(), 0.5f);

        // --- �÷��̾�� �� �浹 ó�� �� �� ������Ʈ ---
        sf::FloatRect playerHitBox = player.getHitBox(); // �÷��̾��� ��Ʈ�ڽ� ��������

        for (auto& enemy : enemies) {
            enemy->update(deltaTime, groundY, player.getSprite().getPosition()); // �� ������Ʈ

            sf::FloatRect enemyHitBox = enemy->getHitBox(); // ���� ��Ʈ�ڽ� ��������

            // �÷��̾ ���� ���°� �ƴ� ���� ���� �浹 ó��
            if (!player.getisInvincible() && playerHitBox.intersects(enemyHitBox)) {
                float playerBottom = playerHitBox.top + playerHitBox.height;
                float enemyTop = enemyHitBox.top;

                // "�� ������ ���� ���"�� �� �� �����ϰ� üũ
                // �÷��̾ �Ʒ��� �̵� ���̰�(���� �� ���� ��), �÷��̾� �ٴ��� �� �Ӹ����� �ణ ���� ���� ��
                if (player.getVelocity().y > 0.f && playerBottom <= enemyTop + 5.f) {
                    enemy->takeDamage(100); // ������ ������
                    player.bounceJump();    // �÷��̾�� ƨ�ܿ���
                }
                else { // ������ ��/�Ʒ����� �ε��� ��� (������ ����)
                    if (player.getIsBig()) { // �÷��̾ ū �����̸�
                        player.setIsBig(false); // ���� ���·� ���ư���
                        player.setInvincible(true); // ���� ���� ����
                        printf("Player is now small.\n"); // ������ ���
                    }
                    else { // �÷��̾ ���� �����̸�
                        if (player.getLife() > 1) { // ������ 1���� ������
                            player.loseLife(1); // ���� ����
                            player.setPosition(200.0f, groundY); // �÷��̾� ��ġ �ʱ�ȭ
                            gameView.setCenter(player.getSprite().getPosition().x, gameView.getCenter().y); // �� ��ġ �ʱ�ȭ
                            player.setInvincible(true); // ���� ���� ����
                        }
                        else { // ������ 1�ۿ� ������
                            player.die(); // ���� ó��
                        }
                        printf("Player hit.\n"); // ������ ���
                    }
                }
            }
            // �� ������ ����
            if (enemy->isDead() && enemy->canRespawn()) {
                float offsetX = 500.f + static_cast<float>(rand() % 400); // �÷��̾� �������� ���� ������
                float spawnX = player.getSprite().getPosition().x + offsetX;
                sf::Vector2f proposedPos(spawnX, groundY);

                // �ٸ� ���� �ʹ� ������ ������ ������
                if (!isTooCloseToOtherEnemies(proposedPos, enemies, 100.f)) {
                    enemy->respawnAt(proposedPos);
                }
            }
        }

        // --- �÷��̾�� ���� ������Ʈ(������, �÷���, �̽��͸� ���) �浹 ó�� ---
        for (auto& obj : gameObjects) { // ��� ���� ������Ʈ�� ���� �ݺ�
            sf::FloatRect objBounds = obj->getGlobalBounds(); // ���� ������Ʈ�� �浹 ����

            // 1. ������ �Ǵ� �÷��� ���� �����ϴ� ��� (�����ؼ� ��� �ö� �� �ֵ���)
            // �÷��̾ �Ʒ��� �������� ��(getVelocity().y > 0)�̰�,
            // �÷��̾��� ��Ʈ�ڽ��� ������Ʈ�� �ٿ�� �ڽ��� ��ġ��,
            // �÷��̾ ���� �����ӿ��� ������Ʈ ��� '����' �־����� Ȯ���Ͽ� ��Ȯ�� ������ �����մϴ�.
            if (player.getVelocity().y > 0 &&
                playerHitBox.intersects(objBounds) &&
                // �÷��̾��� �ٴ��� ������Ʈ ��ܿ� ��Ҵ��� Ȯ�� (deltaTime�� �̿��� ���� ��ġ ����)
                (playerHitBox.top + playerHitBox.height - (player.getVelocity().y * deltaTime)) <= objBounds.top) {

                // ���� ������Ʈ�� Pipe �Ǵ� Platform Ÿ������ Ȯ���մϴ�.
                if (dynamic_cast<Pipe*>(obj.get()) || dynamic_cast<Platform*>(obj.get())) {
                    player.setPosition(player.getPosition().x, objBounds.top - player.getGlobalBounds().height); // �÷��̾��� Y ��ġ�� ������Ʈ ��ܿ� ����ϴ�.
                    player.setVelocityY(0);    // �÷��̾��� ���� �ӵ��� 0���� �����Ͽ� �� �̻� �������� �ʰ� �մϴ�.
                    player.setIsOnGround(true); // �÷��̾ ��(������Ʈ ��)�� ���� ���·� �����մϴ�.
                }
            }

            // 2. ����ǥ ����� �Ʒ����� �Ӹ��� ġ�� ���
            // ���� ������Ʈ�� MysteryBlock Ÿ������ Ȯ���ϰ�, �׷��ٸ� �ش� �����͸� ����ϴ�.
            if (MysteryBlock* mysteryBlock = dynamic_cast<MysteryBlock*>(obj.get())) {
                // ����� Ȱ�� �����̰�, �÷��̾ ���� �����ϴ� ��(getVelocity().y < 0)�̸�,
                // �÷��̾��� ��Ʈ�ڽ��� ��ϰ� ��ġ��,
                // �÷��̾��� �Ӹ� �κ�(hitBox�� ���)�� ����� �Ʒ��κп� ��Ҵ��� Ȯ���մϴ�.
                if (mysteryBlock->isActive() &&
                    player.getVelocity().y < 0 && // �÷��̾ ���� �����ϴ� ���̾�� ��
                    playerHitBox.intersects(objBounds) &&
                    // �÷��̾��� �Ӹ��� ����� �ٴ� �κп� ��Ҵ��� Ȯ�� (�ణ�� ���� ���)
                    playerHitBox.top <= objBounds.top + objBounds.height &&
                    playerHitBox.top + playerHitBox.height > objBounds.top + objBounds.height - 10.f) {

                    mysteryBlock->hit(); // ����ǥ ����� hit() �Լ��� ȣ���Ͽ� ���¸� �����մϴ�.
                    std::unique_ptr<Item> spawnedItem = mysteryBlock->spawnItem(); // ��Ͽ��� �������� �����մϴ�.
                    if (spawnedItem) {
                        items.push_back(std::move(spawnedItem)); // ������ �������� ���� ������ ���Ϳ� �߰��մϴ�.
                    }
                    player.setVelocityY(0); // ����� ġ�� �÷��̾��� ���� �ӵ��� 0���� ����� �� �̻� ���� �ö��� �ʰ� �մϴ�.
                }
            }
        }

        // --- ������ ��� �� Game Over ó�� ---
        if (!player.isAlive()) { // �÷��̾ �׾�����
            // �����츦 �ٽ� �׸��� ���� ��� ���� ����ϴ�.
            window.clear(sf::Color::Black); // ������ ������� ����

            // ������ Game Over �޽��� ǥ��
            sf::Text gameOverText("Game Over", font, 100); // ��Ʈ�� ũ�� ����
            gameOverText.setFillColor(sf::Color::Red);       // �ؽ�Ʈ ���� ������
            // ���� �߾ӿ� �޽��� ��ġ ����
            gameOverText.setPosition(gameView.getCenter().x - gameOverText.getGlobalBounds().width / 2.f,
                gameView.getCenter().y - gameOverText.getGlobalBounds().height / 2.f);
            window.draw(gameOverText); // �ؽ�Ʈ �׸���

            window.display(); // ȭ�� ������Ʈ
            sf::sleep(sf::seconds(3)); // 3�� ���
            window.close(); // ������ �ݱ� (���� ����)
            break; // ���� ���� ����
        }

        // --- ������ (�׸���) ---
        window.clear(sf::Color::White); // ����� ������� ����ϴ�. (��ǻ� ��� �̹����� ����)
        gameBackground.draw(window);      // ����� �׸��ϴ�.

        // ���� ������Ʈ(������, �÷���, �̽��͸� ���) �׸���
        // �÷��̾�� ���� �׷��� �÷��̾ ������Ʈ ���� �ö� ��ó�� ���Դϴ�.
        for (auto& obj : gameObjects) {
            obj->draw(window);
        }

        player.draw(window); // �÷��̾ �׸��ϴ�.

        // �� �׸���
        for (auto& enemy : enemies) {
            enemy->draw(window);
        }

        // �÷��̾� ���� ǥ�� UI
        sf::Text lifeText;
        lifeText.setFont(font);
        lifeText.setCharacterSize(30);
        lifeText.setString("Life: " + std::to_string(player.getLife()));
        lifeText.setFillColor(sf::Color::Black);
        // ���� ���� ��ܿ� ���� UI ��ġ ����
        lifeText.setPosition(gameView.getCenter().x - VIEW_WIDTH / 2.f + 10.f, gameView.getCenter().y - VIEW_HEIGHT / 2.f + 10.f);
        window.draw(lifeText);

        // ������ ������Ʈ �� �׸���
        // ������ �������� ���Ϳ��� �����մϴ�.
        items.erase(std::remove_if(items.begin(), items.end(),
            [&](std::unique_ptr<Item>& item) {
                item->update(deltaTime);
                if (!item->isCollected() && item->checkCollision(player.getSprite())) {
                    item->applyEffect(player);
                    std::cout << "Player is big: " << (player.getIsBig() ? "true" : "false") << ", Life: " << player.getLife() << std::endl; // ����� ���
                    return true; // �������� ���������� ���Ϳ��� ����
                }
                item->draw(window);
                return false; // �������� �ʾ����� ����
            }),
            items.end());

        window.display(); // ���� �������� ������ ȭ�鿡 ǥ��
    }

    return 0; // ���α׷� ���� ����
}