#include "Background.h"
#include <iostream> // 에러 로깅을 위해 포함

using namespace sf;

// 생성자 구현
Background::Background(const std::string& texturePath, float initialX, float initialY)
    : m_scrollSpeed(0.0f), m_currentScrollOffset(0.0f), m_textureWidth(0.0f) // 멤버 초기화
{
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Error: Could not load background texture from " << texturePath << std::endl;
        // 실제 게임에서는 여기서 더 강력한 에러 처리가 필요할 수 있습니다 (예: 게임 종료).
    }
    else {
        m_texture.setRepeated(true); // 이미지를 반복해서 사용하도록 설정 (선택 사항, 스크롤링에 유용)
        m_textureWidth = static_cast<float>(m_texture.getSize().x);

        // 첫 번째 스프라이트 설정
        m_sprite1.setTexture(m_texture);
        m_sprite1.setPosition(initialX, initialY);

        // 두 번째 스프라이트 설정 (첫 번째 스프라이트 바로 뒤에 위치)
        m_sprite2.setTexture(m_texture);
        m_sprite2.setPosition(initialX + m_textureWidth, initialY);
    }
}

// 업데이트 함수 구현
void Background::update(float deltaTime, float scrollSpeed) {
    m_scrollSpeed = scrollSpeed; // 외부에서 스크롤 속도를 받아 업데이트할 수 있도록

    // 스크롤 오프셋 업데이트 (왼쪽으로 스크롤)
    m_currentScrollOffset -= m_scrollSpeed * deltaTime;

    // 스크롤 오프셋이 이미지 너비를 벗어나면 초기화하여 무한 스크롤 효과
    if (m_currentScrollOffset < -m_textureWidth) {
        m_currentScrollOffset += m_textureWidth;
    }

    // 스프라이트 위치 업데이트
    m_sprite1.setPosition(m_currentScrollOffset, m_sprite1.getPosition().y);
    m_sprite2.setPosition(m_currentScrollOffset + m_textureWidth, m_sprite2.getPosition().y);
}

// 그리기 함수 구현
void Background::draw(sf::RenderWindow& window) {
    window.draw(m_sprite1);
    window.draw(m_sprite2);
}

// (선택 사항) 배경의 위치 설정 함수 구현
void Background::setPosition(float x, float y) {
    m_sprite1.setPosition(x, y);
    m_sprite2.setPosition(x + m_textureWidth, y);
    m_currentScrollOffset = x; // 위치가 변경되면 스크롤 오프셋도 재설정
}

// (선택 사항) 배경의 스케일 설정 함수 구현
void Background::setScale(float scaleX, float scaleY) {
    m_sprite1.setScale(scaleX, scaleY);
    m_sprite2.setScale(scaleX, scaleY);

    // 스케일 변경 시 텍스처 너비도 재계산해야 합니다.
    m_textureWidth = static_cast<float>(m_texture.getSize().x) * scaleX;
    m_sprite2.setPosition(m_sprite1.getPosition().x + m_textureWidth, m_sprite1.getPosition().y);
}