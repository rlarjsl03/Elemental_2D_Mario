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

        player.handleInput(deltaTime);  // Ű �Է� ó��
        player.update(deltaTime); // �߷� ����

        window.clear(sf::Color::White);
        player.draw(window);            // �÷��̾� �׸���
        window.display();
    }

    return 0;
}