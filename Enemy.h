#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <stdexcept>

using namespace sf;

class Enemy
{
private:
    Texture texture;
    Sprite sprite;
    Vector2f position;
    int hp;
    bool facingRight;

public:
    Enemy(const std::string& textureFile, Vector2f startPos);
    void update(float deltaTime, float groundY);
    void draw(RenderWindow& window);
    void takeDamage(int amount);
    int getHp() const;
    bool isDead() const;
    const Sprite& getSprite() const;
};
