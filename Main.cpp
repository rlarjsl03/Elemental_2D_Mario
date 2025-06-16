// main.cpp
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>    // std::unique_ptr ���
#include <cstdlib>    // std::srand, rand ���
#include <ctime>      // std::time ���
#include <iostream>  // std::cerr, std::cout ���
#include <algorithm> // std::remove_if �� ���� �߰�

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
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "2D Mario Game");
    window.setFramerateLimit(60); // ������ �ӵ��� 60FPS�� ����

    // --- ���� �⺻ ���� ---
    float groundY = 920.f; // ���� �� ������ Y ��ǥ
    // �������� �ִ� ���� ���̸� ����Ͽ�, ���� ������ �ִ� Y ��ǥ (�� �������� ����)
    const float MARIO_MAX_JUMP_HEIGHT = 600.f; // �������� �������κ��� �ִ�� ������ �� �ִ� ����
    const float MARIO_REACHABLE_MIN_Y = groundY - MARIO_MAX_JUMP_HEIGHT; // �������� ������ �� �ִ� ���� ���� ������ Y ��ǥ

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
    const float ORIGINAL_BLOCK_HEIGHT = 336.f;    // "mysteryblock.png", "deadblock.png"�� ���� �ȼ� ���� (��: 70)
    const float ORIGINAL_BLOCK_WIDTH = 336.f;     // "mysteryblock.png"�� ���� �ȼ� �ʺ� (��: 70)
    const float ORIGINAL_PLATFORM_HEIGHT = 336.f; // "platform.png"�� ���� �ȼ� ���� (��: 70)
    const float ORIGINAL_PLATFORM_WIDTH = 336.f;  // "platform.png"�� ���� �ȼ� �ʺ� (��: 70)


    // --- ���� ��� ����: �÷��̾ ��� �� �� �ְ� �ſ� ũ�� ����
    const float MAP_WIDTH = 10000.f; // �ſ� ū �� �ʺ�

    // --- �� ������ ������, �÷���, �̽��͸� ��� �߰� ---
    // ���� X ��ġ ���� (���� ������Ʈ ���ĺ��� ����)
    float currentX = 1000.f; // ù ������ ���� ��ġ���� �̾ ��ġ

    // �������� �����Ͽ� ������ �� �ִ� ������Ʈ�� �ּ� ���� (�������κ���)
    // ������Ʈ�� �Ʒ����� �� ������ �Ʒ�(Y���� Ŭ����) �־�� �������� ���� �� �ֽ��ϴ�.
    // ������ ��������Ʈ�� ���̵� ����ؾ� �����ؼ� �ö��� �� ��ġ�� �ʰ� �� �� ���� �� �ֽ��ϴ�.
    // �÷��̾��� ��������Ʈ ���̿� ���� �� ���� �����ؾ� �� �� �ֽ��ϴ�.
    // ���⼭�� �ϴ� ������ �Ӹ� �� ������ ����� Ȯ���ϴ� ������ �����մϴ�.
    const float MARIO_PLAYER_HEIGHT_OFFSET = 100.f; // �������� �뷫���� ���� (�ȼ�) �Ǵ� ���� ����
    const float MAX_OBJECT_TOP_Y = groundY - (MARIO_MAX_JUMP_HEIGHT - MARIO_PLAYER_HEIGHT_OFFSET); // ������Ʈ�� ����� �� Y ��ǥ���� �Ʒ��� �־�� �� (Y���� Ŭ���� �Ʒ�)

    while (currentX < MAP_WIDTH - 500.f) { // �� ������ 500px ���� ���ߵ��� ����
        int objectType = rand() % 3; // 0: ������, 1: �÷���, 2: �̽��͸� ��� (Ȥ�� ����)

        if (objectType == 0) { // ������
            float pipeScaleY = 0.3f + static_cast<float>(rand() % 20) / 100.f; // 0.3 ~ 0.49 ���� ���� ������
            float pipeScaleX = 0.25f + static_cast<float>(rand() % 10) / 100.f; // 0.25 ~ 0.34 ���� ���� ������
            float pipeY = groundY - (ORIGINAL_PIPE_HEIGHT * pipeScaleY); // �������� �׻� ���鿡 ����

            // ������ ����� �������� �����ؼ� ���� ������ ������ ���� �ʵ��� ����
            // ������ ��� Y = pipeY
            if (pipeY < MARIO_REACHABLE_MIN_Y) { // ���� �������� �ʹ� ���ٸ�
                pipeScaleY = (groundY - MARIO_REACHABLE_MIN_Y) / ORIGINAL_PIPE_HEIGHT; // ���� ���� ���̿� ���� ������ ������
                pipeY = groundY - (ORIGINAL_PIPE_HEIGHT * pipeScaleY); // Y ��ġ�� ����
            }

            auto pipe = std::make_unique<Pipe>("Pipe.png", sf::Vector2f(currentX, pipeY));
            pipe->getSprite().setScale(pipeScaleX, pipeScaleY);
            gameObjects.push_back(std::move(pipe));
            currentX += (ORIGINAL_PIPE_WIDTH * pipeScaleX) + (rand() % 300 + 100.f); // ������ �ʺ� + ���� ���� �߰�
        }
        else if (objectType == 1) { // �÷��� �Ǵ� �÷���-��� ����
            float platformScaleX = 0.3f;
            float platformScaleY = 0.3f;
            // �÷����� Y ��ġ�� groundY - (�������� ������ �� �ִ� �ִ� ���� ���� ��)�� ����
            // platformY�� �������� ���� ��ġ�մϴ�.
            // groundY - 200.f (�ּ� ����) ~ MAX_OBJECT_TOP_Y (�ִ� ����) �������� ����
            float platformY = groundY - (200.f + static_cast<float>(rand() % static_cast<int>(MARIO_MAX_JUMP_HEIGHT - 200.f - MARIO_PLAYER_HEIGHT_OFFSET)));
            if (platformY < MARIO_REACHABLE_MIN_Y + ORIGINAL_PLATFORM_HEIGHT * platformScaleY) { // �ʹ� ������ ���� (�÷��� �ٴ� ����)
                platformY = MARIO_REACHABLE_MIN_Y + ORIGINAL_PLATFORM_HEIGHT * platformScaleY;
            }
            if (platformY > groundY - (ORIGINAL_PLATFORM_HEIGHT * platformScaleY)) { // ���鿡 �ʹ� ������ ����
                platformY = groundY - (ORIGINAL_PLATFORM_HEIGHT * platformScaleY);
            }

            auto platform = std::make_unique<Platform>("platform.png", sf::Vector2f(currentX, platformY));
            platform->getSprite().setScale(platformScaleX, platformScaleY);
            gameObjects.push_back(std::move(platform));
            currentX += (ORIGINAL_PLATFORM_WIDTH * platformScaleX); // �÷��� �ʺ�ŭ �̵�

            // 50% Ȯ���� �÷��� ���� �̽��͸� ��� �߰�
            if (rand() % 2 == 0) {
                float mysteryBlockScaleX = 0.3f;
                float mysteryBlockScaleY = 0.3f;
                float mysteryBlockY = platformY; // �÷����� ���� ���̿� ��ġ

                // � �������� �������� �������� ����
                std::unique_ptr<Item> itemToSpawn;
                int itemType = rand() % 3; // 0: ����, 1: ����, 2: ��� ����
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
                currentX += (ORIGINAL_BLOCK_WIDTH * mysteryBlockScaleX); // ��� �ʺ�ŭ �̵�
            }
            currentX += (rand() % 400 + 150.f); // �÷���/��� ���� ���� ���� ���� �߰�
        }
        else { // �̽��͸� ��� �ܵ� �Ǵ� ����
            float mysteryBlockScaleX = 0.3f;
            float mysteryBlockScaleY = 0.3f;
            // �̽��͸� ����� Y ��ġ�� ������ ���� ���� ���� ���� ����
            float mysteryBlockY = groundY - (200.f + static_cast<float>(rand() % static_cast<int>(MARIO_MAX_JUMP_HEIGHT - 200.f - MARIO_PLAYER_HEIGHT_OFFSET)));
            if (mysteryBlockY < MARIO_REACHABLE_MIN_Y + ORIGINAL_BLOCK_HEIGHT * mysteryBlockScaleY) { // �ʹ� ������ ���� (��� �ٴ� ����)
                mysteryBlockY = MARIO_REACHABLE_MIN_Y + ORIGINAL_BLOCK_HEIGHT * mysteryBlockScaleY;
            }
            if (mysteryBlockY > groundY - (ORIGINAL_BLOCK_HEIGHT * mysteryBlockScaleY)) { // ���鿡 �ʹ� ������ ����
                mysteryBlockY = groundY - (ORIGINAL_BLOCK_HEIGHT * mysteryBlockScaleY);
            }

            // � �������� �������� �������� ����
            std::unique_ptr<Item> itemToSpawn;
            int itemType = rand() % 3; // 0: ����, 1: ����, 2: ��� ����
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
            currentX += (ORIGINAL_BLOCK_WIDTH * mysteryBlockScaleX) + (rand() % 200 + 50.f); // ��� �ʺ� + ���� ���� �߰�

            // 30% Ȯ���� ��� �ϳ� �� �߰� (���� ���ó�� ���̰�)
            if (rand() % 10 < 3) {
                // � �������� �������� �������� ���� (�� ��° ��Ͽ�)
                std::unique_ptr<Item> itemToSpawn2;
                int itemType2 = rand() % 3; // 0: ����, 1: ����, 2: ��� ����
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
                    sf::Vector2f(currentX, mysteryBlockY), // ���� Y ����
                    std::move(itemToSpawn2)
                );
                mysteryBlock2->getSprite().setScale(mysteryBlockScaleX, mysteryBlockScaleY);
                gameObjects.push_back(std::move(mysteryBlock2));
                currentX += (ORIGINAL_BLOCK_WIDTH * mysteryBlockScaleX) + (rand() % 100 + 20.f); // ��� �ʺ� + ���� ����
            }
        }
    }

    // --- SFML View (ī�޶�) ���� ---
    sf::View gameView(sf::FloatRect(0, 0, 1920, 1080)); // ������� ������ ũ���� �� ����

    // ��Ʈ ���� (Game Over �޽��� �� UI��)
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) { // "arial.ttf" ��Ʈ ������ ������Ʈ�� �־�� ��
        std::cerr << "Error loading arial.ttf" << std::endl;
        return 1;
    }

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

        // --- �÷��̾�� ���� ������Ʈ(������, �÷���, �̽��͸� ���) �浹 ó�� ---
        // �� �κ��� Player::checkPlatformCollision() �Լ��� �̵��Ǿ����ϴ�.
        player.checkPlatformCollision(deltaTime, gameObjects); // �÷��̾��� ���� �浹 ���� ȣ��

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

        // --- ����ǥ ����� �Ʒ����� �Ӹ��� ġ�� ��� (main.cpp�� �״�� ����) ---
        // �� ������ �������� �����ϰ� items ���Ϳ� �߰��ؾ� �ϹǷ� Player Ŭ���� ���ο� �α� ��ƽ��ϴ�.
        for (auto& obj : gameObjects) {
            if (MysteryBlock* mysteryBlock = dynamic_cast<MysteryBlock*>(obj.get())) {
                sf::FloatRect objBounds = obj->getGlobalBounds();
                sf::FloatRect playerHitBox = player.getHitBox(); // �÷��̾��� ��Ʈ�ڽ� ��������

                if (mysteryBlock->isActive() &&
                    player.getVelocity().y < 0 && // �÷��̾ ���� �����ϴ� ���̾�� ��
                    playerHitBox.intersects(objBounds) &&
                    // �÷��̾��� �Ӹ��� ����� �ٴ� �κп� ��Ҵ��� Ȯ�� (�ణ�� ���� ���)
                    playerHitBox.top <= objBounds.top + objBounds.height &&
                    playerHitBox.top + playerHitBox.height > objBounds.top + objBounds.height - 10.f) {

                    mysteryBlock->hit();
                    std::unique_ptr<Item> spawnedItem = mysteryBlock->spawnItem();
                    if (spawnedItem) {
                        items.push_back(std::move(spawnedItem));
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
            gameOverText.setFillColor(sf::Color::Red);         // �ؽ�Ʈ ���� ������
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
        gameBackground.draw(window);        // ����� �׸��ϴ�.

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