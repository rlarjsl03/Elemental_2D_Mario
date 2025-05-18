#include <SFML/Graphics.hpp>
#include "Player.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "2D_Mario");
	sf::Style::Fullscreen;
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Red);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}