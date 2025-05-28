#pragma once
#include <SFML/Graphics.hpp>
#include <string> // std::string을 사용하기 위해 포함

using namespace sf;
class Background {
public:
    // 생성자: 배경 이미지 파일 경로, 초기 위치 (선택 사항)를 받습니다.
    Background(const std::string& texturePath, float initialX = 0.0f, float initialY = 0.0f);

    // 배경의 현재 위치를 업데이트하는 함수 (주로 스크롤링에 사용)
    void update(float deltaTime, float scrollSpeed);

    // 배경을 렌더링 윈도우에 그리는 함수
    void draw(RenderWindow& window);

    // (선택 사항) 배경의 위치를 직접 설정하는 함수
    void setPosition(float x, float y);

    // (선택 사항) 배경의 스케일을 설정하는 함수
    void setScale(float scaleX, float scaleY);

private:
    Texture m_texture;  // 배경 이미지 텍스처
    Sprite m_sprite1;   // 첫 번째 배경 스프라이트 (스크롤링을 위해 2개 사용)
    Sprite m_sprite2;   // 두 번째 배경 스프라이트

    float m_scrollSpeed; // 배경의 스크롤 속도
    float m_currentScrollOffset; // 현재 스크롤 오프셋 (X축)
    float m_textureWidth; // 배경 이미지의 실제 너비
};