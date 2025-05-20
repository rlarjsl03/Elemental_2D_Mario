
#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
private:
    Texture texture;
    Sprite sprite;
    Vector2f velocity;
    float speed;
    float gravity;
    float groundY;
    float jumpPower;
    bool isOnGround;

public:
    Player();
    void handleInput(float deltaTime);
    void update(float deltaTime);
    void draw(RenderWindow& window);
};
