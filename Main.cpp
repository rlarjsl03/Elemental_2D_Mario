#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Player.h"
#include "Obstacle.h"
#include "BackGround.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Player Control");
    window.setFramerateLimit(60);

    Player player;
    sf::Clock clock;

    while (window.isOpen()) {
        Background background("C:/Users/GunHee/source/repos/Elemental_2D_Mariostage1.jpg");
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        player.handleInput(deltaTime);
        player.update(deltaTime); // 중력 적용
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.handleInput(deltaTime);  // 키 입력 처리

        window.clear(sf::Color::White);
        player.draw(window);            // 플레이어 그리기
        window.display();
    }

    return 0;
}