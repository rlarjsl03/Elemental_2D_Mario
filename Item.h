#pragma once
#include <SFML/Graphics.hpp>

class Item {
private:
    sf::Texture texture;
    sf::Sprite sprite;
    bool collected = false;

public:
    Item(const std::string& texturePath, sf::Vector2f position);

    void draw(sf::RenderWindow& window);
    void update(float deltaTime);
    bool checkCollision(const sf::Sprite& playerSprite);
    bool isCollected() const;

    const sf::Sprite& getSprite() const;
};