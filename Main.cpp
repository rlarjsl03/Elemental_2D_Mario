#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Player.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Player Control");
    window.setFramerateLimit(60);

    Player player;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.handleInput(deltaTime);  // 키 입력 처리
        player.update(deltaTime); // 중력 적용

        window.clear(sf::Color::White);
        player.draw(window);            // 플레이어 그리기
        window.display();
    }

    return 0;
}