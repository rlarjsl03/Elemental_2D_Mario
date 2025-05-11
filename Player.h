#pragma once
#include <SFML/Graphics.hpp>

class Player {
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;   // 현재 속도
    float speed = 200.0f;    // 좌우 이동 속도
    float gravity = 500.0f;  // 중력 가속도
    float groundY = 500.0f;  // 바닥 위치 (y 좌표)

public:
    Player() {
        shape.setRadius(50.0f);
        shape.setFillColor(sf::Color::Blue);
        shape.setPosition(300.0f, 100.0f);  // 공중에서 시작
    }

    // 좌우 이동만 처리
    void handleInput(float deltaTime) {
        sf::Vector2f direction(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            direction.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            direction.x += 1.f;

        shape.move(direction * speed * deltaTime);
    }

    // 중력 적용
    void update(float deltaTime) {
        velocity.y += gravity * deltaTime;
        shape.move(0.f, velocity.y * deltaTime);

        // 바닥 충돌 처리
        float bottom = shape.getPosition().y + shape.getRadius() * 2;
        if (bottom >= groundY) {
            shape.setPosition(shape.getPosition().x, groundY - shape.getRadius() * 2);
            velocity.y = 0;
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }
};