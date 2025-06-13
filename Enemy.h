#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Enemy {
public:
    Enemy(std::shared_ptr<sf::Texture> texturePtr, sf::Vector2f startPos);

    void update(float deltaTime, float groundY);
    void draw(sf::RenderWindow& window);

    void takeDamage(int amount);
    int getHp() const;
    bool isDead() const;
    bool isActive() const;

    const sf::Sprite& getSprite() const;

private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    int hp;
    bool facingRight;
    float speed;
};
