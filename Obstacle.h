
// Obstacle.h
#pragma once
#include <SFML/Graphics.hpp>

class Obstacle {
private:
    sf::RectangleShape shape;

public:
    Obstacle(sf::Vector2f position, sf::Vector2f size) {
        shape.setSize(size);
        shape.setFillColor(sf::Color::Red); // �ð� Ȯ�ο�
        shape.setPosition(position);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    const sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }
};