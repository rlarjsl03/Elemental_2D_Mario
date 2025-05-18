#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
private:
    Texture texture;
    Sprite sprite;
    //CircleShape shape;

    Vector2f velocity;   // 현재 속도
    float speed = 200.0f;    // 좌우 이동 속도
    float gravity = 500.0f;  // 중력 가속도
    float groundY = 500.0f;  // 바닥 위치 (y 좌표)
    float jumpPower = -300.0f; // 점프 시 위로 가는 속도
    bool isOnGround = false;  // 바닥에 있는지 여부

public:
    Player() {
        if (!texture.loadFromFile("mario-2d.png")) {
            // 실패 시 대체 방법 (예: 색상 표시용 도형 만들기 등)
            throw std::runtime_error("이미지를 불러올 수 없습니다: mario_2d.png");
        }
        sprite.setTexture(texture);
        sprite.setScale(0.1f, 0.1f);  // 크기 조절 (필요에 따라)
        sprite.setPosition(300.0f, 100.0f);  // 공중에서 시작
        //shape.setRadius(50.0f);
        //shape.setFillColor(Color::Blue);
        //shape.setPosition(300.0f, 100.0f);  // 공중에서 시작
    }

	// 키 입력 처리
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
			// 게임 종료
			exit(0);
        }
    }

    // 중력 적용
    void update(float deltaTime) {
        velocity.y += gravity * deltaTime;
        sprite.move(0.f, velocity.y * deltaTime);
        //shape.move(0.f, velocity.y * deltaTime);

        // 바닥 충돌 처리
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