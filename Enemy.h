#pragma once
#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(const sf::Texture& tex, sf::Vector2f startPos);

    void update(float deltaTime, float groundY, const sf::Vector2f& playerPos);
    void draw(sf::RenderWindow& window);
    void takeDamage(int amount);
    int getHp() const;
    bool isDead() const;
    bool isActive() const;
    const sf::Sprite& getSprite() const;
    sf::FloatRect getHitBox() const;

    bool canRespawn() const;
    void respawnAt(sf::Vector2f newPos);

private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f position;
    int hp;
    bool facingRight;
    sf::Clock respawnTimer;
    float respawnDelay = 10.0f;
};
