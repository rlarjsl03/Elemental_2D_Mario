#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(const sf::Texture& tex, sf::Vector2f startPos);

    void update(float deltaTime, float groundY);
    void draw(sf::RenderWindow& window);

    void takeDamage(int amount);
    int getHp() const;
    bool isDead() const;
    bool isActive() const;

    const sf::Sprite& getSprite() const;
    sf::FloatRect getHitBox() const;

private:
    sf::Sprite sprite;
    sf::Vector2f position;
    int hp;
    bool facingRight;
    const sf::Texture& texture;
};
