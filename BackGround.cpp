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
        m_textureWidth = static_cast<float>(m_texture.getSize().x); // 스케일 적용 전 너비

        // 첫 번째 스프라이트 설정
        m_sprite1.setTexture(m_texture);
        m_sprite1.setPosition(initialX, initialY);

        // 두 번째 스프라이트 설정 (첫 번째 스프라이트 바로 뒤에 위치)
        m_sprite2.setTexture(m_texture);
        m_sprite2.setPosition(initialX + m_textureWidth, initialY);

        // 세 번째 스프라이트 설정 (두 번째 스프라이트 바로 뒤에 위치)
        m_sprite3.setTexture(m_texture);
        m_sprite3.setPosition(initialX + (m_textureWidth * 2), initialY); // 3번째 스프라이트 추가
    }
}

// 업데이트 함수 구현 (뷰의 중심을 받아 직접 위치 계산)
void Background::update(float deltaTime, sf::Vector2f viewCenter, float parallaxFactorX) {
    // 뷰의 왼쪽 끝을 기준으로 배경 스프라이트의 X 위치를 계산합니다.
    // 뷰의 왼쪽 끝: viewCenter.x - (화면 너비 / 2)
    // 배경이 뷰의 움직임에 'parallaxFactorX'만큼 느리게 반응하도록 합니다.
    float viewLeft = viewCenter.x - (1920.f / 2.f); // 1920.f는 윈도우 너비

    // 배경의 실제 X 오프셋 (뷰의 움직임에 비례)
    float backgroundXOffset = viewLeft * parallaxFactorX;

    // 이 오프셋을 m_textureWidth 기준으로 반복시킵니다.
    // 이렇게 하면 배경이 무한 스크롤되는 것처럼 보입니다.
    while (backgroundXOffset < 0) {
        backgroundXOffset += m_textureWidth;
    }
    while (backgroundXOffset >= m_textureWidth) {
        backgroundXOffset -= m_textureWidth;
    }

    // 이제 스프라이트들의 위치를 설정합니다.
    // backgroundXOffset은 텍스처의 시작 위치가 될 수 있습니다.
    // 따라서, 뷰의 왼쪽 시작 지점에서 backgroundXOffset을 빼서 상대적으로 스크롤된 것처럼 보이게 합니다.
    m_sprite1.setPosition(viewLeft - backgroundXOffset, m_sprite1.getPosition().y);
    m_sprite2.setPosition(viewLeft - backgroundXOffset + m_textureWidth, m_sprite2.getPosition().y);
    m_sprite3.setPosition(viewLeft - backgroundXOffset + (m_textureWidth * 2), m_sprite3.getPosition().y);

    // Y 위치는 고정하거나, viewCenter.y에 따라 조정할 수 있습니다.
    // 예: m_sprite1.setPosition(..., viewCenter.y * parallaxFactorY);
}

// 그리기 함수 구현
void Background::draw(sf::RenderWindow& window) {
    window.draw(m_sprite1);
    window.draw(m_sprite2);
    window.draw(m_sprite3); // 3번째 스프라이트 그리기
}

// (선택 사항) 배경의 위치 설정 함수 구현
void Background::setPosition(float x, float y) {
    m_sprite1.setPosition(x, y);
    m_sprite2.setPosition(x + m_textureWidth, y);
    m_sprite3.setPosition(x + (m_textureWidth * 2), y); // 3번째 스프라이트 위치 설정
    m_currentScrollOffset = x; // 위치가 변경되면 스크롤 오프셋도 재설정
}

// (선택 사항) 배경의 스케일 설정 함수 구현
void Background::setScale(float scaleX, float scaleY) {
    m_sprite1.setScale(scaleX, scaleY);
    m_sprite2.setScale(scaleX, scaleY);
    m_sprite3.setScale(scaleX, scaleY); // 3번째 스프라이트 스케일 설정

    // 스케일 변경 시 텍스처 너비도 재계산해야 합니다.
    m_textureWidth = static_cast<float>(m_texture.getSize().x) * scaleX;
    // 스프라이트 2, 3의 위치도 재계산된 m_textureWidth를 기준으로 다시 설정
    m_sprite2.setPosition(m_sprite1.getPosition().x + m_textureWidth, m_sprite1.getPosition().y);
    m_sprite3.setPosition(m_sprite1.getPosition().x + (m_textureWidth * 2), m_sprite1.getPosition().y);
}