#pragma once
#include <SFML/Graphics.hpp>

class Player {
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;   // ���� �ӵ�
    float speed = 200.0f;    // �¿� �̵� �ӵ�
    float gravity = 500.0f;  // �߷� ���ӵ�
    float groundY = 500.0f;  // �ٴ� ��ġ (y ��ǥ)

public:
    Player() {
        shape.setRadius(50.0f);
        shape.setFillColor(sf::Color::Blue);
        shape.setPosition(300.0f, 100.0f);  // ���߿��� ����
    }

    // �¿� �̵��� ó��
    void handleInput(float deltaTime) {
        sf::Vector2f direction(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            direction.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            direction.x += 1.f;

        shape.move(direction * speed * deltaTime);
    }

    // �߷� ����
    void update(float deltaTime) {
        velocity.y += gravity * deltaTime;
        shape.move(0.f, velocity.y * deltaTime);

        // �ٴ� �浹 ó��
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