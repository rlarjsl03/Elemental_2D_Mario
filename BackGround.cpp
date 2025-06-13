#include "Background.h"
#include <iostream>

using namespace sf;

// 생성자 구현
Background::Background(const std::string& texturePath, float initialX, float initialY)
    : m_scrollSpeed(0.0f), m_currentScrollOffset(0.0f), m_textureWidth(0.0f), m_lastViewCenterX(initialX) // initialX로 초기화하여 첫 업데이트 시 올바른 델타 계산
{
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Error: Could not load background texture from " << texturePath << std::endl;
        // 실제 게임에서는 여기서 더 강력한 에러 처리가 필요할 수 있습니다 (예: 게임 종료).
    }
    else {
        m_texture.setRepeated(true);
        m_textureWidth = static_cast<float>(m_texture.getSize().x);

        // 첫 번째 스프라이트 설정
        m_sprite1.setTexture(m_texture);
        m_sprite1.setPosition(initialX, initialY);

        // 두 번째 스프라이트 설정 (첫 번째 스프라이트 바로 뒤에 위치)
        m_sprite2.setTexture(m_texture);
        m_sprite2.setPosition(initialX + m_textureWidth, initialY);

        // 세 번째 스프라이트 설정 (두 번째 스프라이트 바로 뒤에 위치)
        m_sprite3.setTexture(m_texture);
        m_sprite3.setPosition(initialX + (m_textureWidth * 2), initialY);
    }
}

// 업데이트 함수 구현
void Background::update(float deltaTime, sf::Vector2f viewCenter, float parallaxFactorX) {
    // 뷰의 현재 중심과 이전 중심의 차이를 계산
    float deltaViewX = viewCenter.x - m_lastViewCenterX;

    // 배경이 움직여야 할 양 계산. 시차 효과 적용.
    // 플레이어가 오른쪽으로 가면 (viewCenter.x 증가), 배경은 왼쪽으로 가야 자연스러움.
    // 따라서 deltaViewX에 -1을 곱하거나, parallaxFactorX를 음수로 전달하는 것이 맞습니다.
    // 여기서는 parallaxFactorX를 음수로 받거나, 내부에서 부호를 반전시키는 것을 고려합니다.
    // 지금은 player.getVelocity().x * 0.5f로 인해 이미 반대방향으로 전달되므로,
    // deltaViewX를 그대로 사용하고 move() 시에는 -backgroundMoveAmount를 사용할 수도 있습니다.
    // 하지만 가장 직관적인 방법은, 뷰가 오른쪽으로 움직이면 배경 스프라이트는 왼쪽으로 움직이게 하는 것입니다.
    float backgroundMoveAmount = deltaViewX * parallaxFactorX; // 뷰가 움직인 만큼 배경도 움직이되, 속도 조절

    // 각 스프라이트를 계산된 양만큼 이동시킵니다.
    // 뷰가 오른쪽으로 움직이면(deltaViewX > 0), backgroundMoveAmount도 양수.
    // 배경은 왼쪽으로 이동해야 하므로, -backgroundMoveAmount
    m_sprite1.move(-backgroundMoveAmount, 0.f);
    m_sprite2.move(-backgroundMoveAmount, 0.f);
    m_sprite3.move(-backgroundMoveAmount, 0.f);

    // 뷰의 왼쪽 끝 좌표를 계산합니다. (1920.f는 윈도우 너비)
    float viewLeftEdge = viewCenter.x - (1920.f / 2.f);
    float viewRightEdge = viewCenter.x + (1920.f / 2.f);

    // 스프라이트 재배치 로직
    // 각 스프라이트가 화면 왼쪽 밖으로 완전히 벗어났을 때, 가장 오른쪽으로 재배치
    // (스프라이트의 오른쪽 끝이 뷰의 왼쪽 끝보다 왼쪽에 있으면)
    if (m_sprite1.getPosition().x + m_textureWidth < viewLeftEdge) {
        // sprite1을 나머지 스프라이트들의 가장 오른쪽 뒤로 이동 (3개 너비만큼)
        m_sprite1.setPosition(m_sprite1.getPosition().x + (m_textureWidth * 3), m_sprite1.getPosition().y);
    }
    if (m_sprite2.getPosition().x + m_textureWidth < viewLeftEdge) {
        m_sprite2.setPosition(m_sprite2.getPosition().x + (m_textureWidth * 3), m_sprite2.getPosition().y);
    }
    if (m_sprite3.getPosition().x + m_textureWidth < viewLeftEdge) {
        m_sprite3.setPosition(m_sprite3.getPosition().x + (m_textureWidth * 3), m_sprite3.getPosition().y);
    }

    // 각 스프라이트가 화면 오른쪽 밖으로 완전히 벗어났을 때, 가장 왼쪽으로 재배치
    // (스프라이트의 왼쪽 끝이 뷰의 오른쪽 끝보다 오른쪽에 있으면)
    if (m_sprite1.getPosition().x > viewRightEdge) {
        m_sprite1.setPosition(m_sprite1.getPosition().x - (m_textureWidth * 3), m_sprite1.getPosition().y);
    }
    if (m_sprite2.getPosition().x > viewRightEdge) {
        m_sprite2.setPosition(m_sprite2.getPosition().x - (m_textureWidth * 3), m_sprite2.getPosition().y);
    }
    if (m_sprite3.getPosition().x > viewRightEdge) {
        m_sprite3.setPosition(m_sprite3.getPosition().x - (m_textureWidth * 3), m_sprite3.getPosition().y);
    }

    // 현재 뷰 중심을 다음 프레임을 위해 저장합니다.
    m_lastViewCenterX = viewCenter.x;
}

// 그리기 함수 구현 (이전과 동일)
void Background::draw(sf::RenderWindow& window) {
    window.draw(m_sprite1);
    window.draw(m_sprite2);
    window.draw(m_sprite3);
}

// (선택 사항) 배경의 위치 설정 함수 구현
// 이 함수는 초기 설정에만 사용하고, update에서는 사용하지 않는 것이 좋습니다.
void Background::setPosition(float x, float y) {
    m_sprite1.setPosition(x, y);
    m_sprite2.setPosition(x + m_textureWidth, y);
    m_sprite3.setPosition(x + (m_textureWidth * 2), y);
    // m_currentScrollOffset = x; // 이 방식에서는 더 이상 필요 없을 수 있습니다.
}

// (선택 사항) 배경의 스케일 설정 함수 구현 (이전과 동일)
void Background::setScale(float scaleX, float scaleY) {
    m_sprite1.setScale(scaleX, scaleY);
    m_sprite2.setScale(scaleX, scaleY);
    m_sprite3.setScale(scaleX, scaleY);

    m_textureWidth = static_cast<float>(m_texture.getSize().x) * scaleX;
    m_sprite2.setPosition(m_sprite1.getPosition().x + m_textureWidth, m_sprite1.getPosition().y);
    m_sprite3.setPosition(m_sprite1.getPosition().x + (m_textureWidth * 2), m_sprite1.getPosition().y);
}