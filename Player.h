#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
private:
    Texture texture;
    Sprite sprite;
    //CircleShape shape;

    Vector2f velocity;   // ���� �ӵ�
    float speed = 200.0f;    // �¿� �̵� �ӵ�
    float gravity = 500.0f;  // �߷� ���ӵ�
    float groundY = 500.0f;  // �ٴ� ��ġ (y ��ǥ)
    float jumpPower = -300.0f; // ���� �� ���� ���� �ӵ�
    bool isOnGround = false;  // �ٴڿ� �ִ��� ����

public:
    Player() {
        if (!texture.loadFromFile("mario-2d.png")) {
            // ���� �� ��ü ��� (��: ���� ǥ�ÿ� ���� ����� ��)
            throw std::runtime_error("�̹����� �ҷ��� �� �����ϴ�: mario_2d.png");
        }
        sprite.setTexture(texture);
        sprite.setScale(0.1f, 0.1f);  // ũ�� ���� (�ʿ信 ����)
        sprite.setPosition(300.0f, 100.0f);  // ���߿��� ����
        //shape.setRadius(50.0f);
        //shape.setFillColor(Color::Blue);
        //shape.setPosition(300.0f, 100.0f);  // ���߿��� ����
    }

	// Ű �Է� ó��
    void handleInput(float deltaTime) {
        Vector2f direction(0.f, 0.f);
        if (Keyboard::isKeyPressed(Keyboard::Left))
            direction.x -= 1.f;
        else if (Keyboard::isKeyPressed(Keyboard::Right))
            direction.x += 1.f;

        sprite.move(direction * speed * deltaTime);
        //shape.move(direction * speed * deltaTime);
        
        if (Keyboard::isKeyPressed(Keyboard::Space) && isOnGround) {
            velocity.y = jumpPower;
            isOnGround = false;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			// ���� ����
			exit(0);
        }
    }

    // �߷� ����
    void update(float deltaTime) {
        velocity.y += gravity * deltaTime;
        sprite.move(0.f, velocity.y * deltaTime);
        //shape.move(0.f, velocity.y * deltaTime);

        // �ٴ� �浹 ó��
        float bottom = sprite.getPosition().y + sprite.getGlobalBounds().height;
        //float bottom = shape.getPosition().y + shape.getRadius() * 2;
        if (bottom >= groundY) {
            sprite.setPosition(sprite.getPosition().x, groundY - sprite.getGlobalBounds().height);
            //shape.setPosition(shape.getPosition().x, groundY - shape.getRadius() * 2);
            velocity.y = 0;
            isOnGround = true;
        } else
            isOnGround = false;
    }

    void draw(RenderWindow& window) {
        window.draw(sprite);
        //window.draw(shape);
    }
};