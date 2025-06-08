#pragma once
#include <SFML/Graphics.hpp>

class Background {
public:
    // 생성자: 텍스처 경로와 초기 위치 (기본 0,0)
    Background(const std::string& texturePath, float initialX = 0.f, float initialY = 0.f);

    // 매 프레임 호출: 배경 위치 업데이트 (deltaTime, 스크롤 속도)
    void update(float deltaTime, float scrollSpeed);

    // 매 프레임 호출: 배경 그리기
    void draw(sf::RenderWindow& window);

    // 배경 위치 설정 (옵션)
    void setPosition(float x, float y);

    // 배경 스케일 설정 (옵션)
    void setScale(float scaleX, float scaleY);

private:
    sf::Texture m_texture;        // 배경 텍스처
    sf::Sprite m_sprite1;         // 첫 번째 스프라이트
    sf::Sprite m_sprite2;         // 두 번째 스프라이트
    sf::Sprite m_sprite3;         // 세 번째 스프라이트

    float m_scrollSpeed;          // 현재 스크롤 속도 (px/s)
    float m_textureWidth;         // 텍스처의 가로 길이 (스케일 적용 후)
};
