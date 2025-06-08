#include "Background.h"
#include <iostream>   // 에러 출력용
#include <cmath>      // std::floor() 함수 사용
#include <algorithm>  // std::max() 함수 사용

using namespace sf;

// 생성자: 텍스처 로드 및 3개의 스프라이트 위치 초기화
Background::Background(const std::string& texturePath, float initialX, float initialY)
    : m_scrollSpeed(0.f), m_textureWidth(0.f)
{
    // 텍스처 로드 시도
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Error: Could not load background texture from " << texturePath << std::endl;
        // 실패 시 메시지 출력 (필요시 예외 처리 추가 가능)
    }
    else {
        // 반복 텍스처는 false (수동 반복 구현)
        m_texture.setRepeated(false);

        // 텍스처 가로 길이 저장 (원본 크기)
        m_textureWidth = static_cast<float>(m_texture.getSize().x);

        // 스프라이트에 텍스처 적용 및 위치 초기화
        m_sprite1.setTexture(m_texture);
        m_sprite2.setTexture(m_texture);
        m_sprite3.setTexture(m_texture);

        // 스프라이트들을 가로로 이어서 배치
        m_sprite1.setPosition(initialX, initialY);
        m_sprite2.setPosition(initialX + m_textureWidth, initialY);
        m_sprite3.setPosition(initialX + 2 * m_textureWidth, initialY);
    }
}

// 업데이트: 배경 스프라이트 위치를 이동시키고 무한 스크롤 처리
void Background::update(float deltaTime, float scrollSpeed) {
    m_scrollSpeed = scrollSpeed;                 // 현재 속도 저장
    float moveX = m_scrollSpeed * deltaTime;     // 이번 프레임 이동할 픽셀 수 계산

    // 세 스프라이트 모두 왼쪽으로 moveX 만큼 이동
    m_sprite1.move(-moveX, 0);
    m_sprite2.move(-moveX, 0);
    m_sprite3.move(-moveX, 0);

    // 스프라이트 너비 (스케일 적용된 실제 크기)
    float width = m_sprite1.getGlobalBounds().width;

    // 위치가 화면 왼쪽 바깥으로 벗어나면 가장 오른쪽 끝으로 이동시키는 함수
    auto repositionIfNeeded = [&](sf::Sprite& sprite, const sf::Sprite& other1, const sf::Sprite& other2) {
        // 스프라이트의 오른쪽 끝이 0보다 작으면 (완전히 화면 밖)
        if (sprite.getPosition().x + width < 0) {
            // 나머지 두 스프라이트 중 오른쪽 끝이 가장 큰 x 좌표 구하기
            float rightMostX = std::max(other1.getPosition().x, other2.getPosition().x);
            // 현재 스프라이트를 오른쪽 끝 바로 뒤로 이동 (연결되도록)
            sprite.setPosition(std::floor(rightMostX + width), sprite.getPosition().y);
        }
        };

    // 3개의 스프라이트 각각에 대해 위치 재배치 실행
    repositionIfNeeded(m_sprite1, m_sprite2, m_sprite3);
    repositionIfNeeded(m_sprite2, m_sprite1, m_sprite3);
    repositionIfNeeded(m_sprite3, m_sprite1, m_sprite2);
}

// 배경을 윈도우에 그림
void Background::draw(sf::RenderWindow& window) {
    window.draw(m_sprite1);
    window.draw(m_sprite2);
    window.draw(m_sprite3);
}

// 배경 위치를 수동으로 설정할 수 있는 함수 (기본 초기 위치 변경용)
void Background::setPosition(float x, float y) {
    m_sprite1.setPosition(x, y);
    m_sprite2.setPosition(x + m_textureWidth, y);
    m_sprite3.setPosition(x + 2 * m_textureWidth, y);
}

// 배경 스케일 설정 함수 (크기 변경 시 호출)
void Background::setScale(float scaleX, float scaleY) {
    // 3개 스프라이트 모두 스케일 적용
    m_sprite1.setScale(scaleX, scaleY);
    m_sprite2.setScale(scaleX, scaleY);
    m_sprite3.setScale(scaleX, scaleY);

    // 스케일 적용 후 텍스처 너비 갱신
    m_textureWidth = static_cast<float>(m_texture.getSize().x) * scaleX;

    // 스프라이트 위치도 다시 조정 (스케일 때문에 간격이 달라짐)
    m_sprite2.setPosition(m_sprite1.getPosition().x + m_textureWidth, m_sprite1.getPosition().y);
    m_sprite3.setPosition(m_sprite2.getPosition().x + m_textureWidth, m_sprite2.getPosition().y);
}
