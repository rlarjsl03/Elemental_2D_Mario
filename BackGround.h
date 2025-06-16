#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

class Background {
public:
    Background(const std::string& texturePath, float initialX = 0.0f, float initialY = 0.0f);

    void update(float deltaTime, sf::Vector2f viewCenter, float parallaxFactorX = 1.0f);

    void draw(RenderWindow& window);

    // setPosition과 setScale은 이제 내부 로직에 의해 관리되므로
    // 외부에서 자주 호출할 필요는 없지만, 필요시를 위해 남겨둡니다.
    void setPosition(float x, float y);
    void setScale(float scaleX, float scaleY);

private:
    Texture m_texture;   // 배경 이미지 텍스처
    Sprite m_sprite1;   // 첫 번째 배경 스프라이트
    Sprite m_sprite2;   // 두 번째 배경 스프라이트
    Sprite m_sprite3;   // 세 번째 배경 스프라이트

    float m_scrollSpeed;         // 이제 사용되지 않을 수 있습니다.
    float m_currentScrollOffset; // 이제 사용되지 않을 수 있습니다.
    float m_textureWidth;        // 배경 이미지의 실제 너비 (스케일 적용 후)

    float m_lastViewCenterX; // 이전 프레임의 뷰 중심 X 좌표
};