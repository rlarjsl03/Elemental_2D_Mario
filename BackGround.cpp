#include "Background.h"
#include <iostream> // 에러 출력용
#include <cmath>    // std::floor() 함수 사용
#include <algorithm> // std::max() 함수 사용

using namespace sf;

// 생성자: 배경 텍스처 로드 및 3개의 스프라이트 초기 위치 설정
Background::Background(const std::string& texturePath, float initialX, float initialY)
    : m_scrollSpeed(0.0f), m_textureWidth(0.0f)
{
    // 텍스처 로드 시도
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Error: Could not load background texture from " << texturePath << std::endl;
        // 텍스처 로드 실패 시 에러 메시지 출력
    }
    else {
        // 텍스처 반복 설정은 false로 (수동으로 3개 스프라이트로 반복 구현)
        m_texture.setRepeated(false);

        // 텍스처의 가로 길이(픽셀)를 저장
        m_textureWidth = static_cast<float>(m_texture.getSize().x);

        // 첫 번째 스프라이트에 텍스처 적용 후 위치 설정
        m_sprite1.setTexture(m_texture);
        m_sprite1.setPosition(initialX, initialY);

        // 두 번째 스프라이트를 첫 번째 바로 오른쪽에 위치시키기
        m_sprite2.setTexture(m_texture);
        m_sprite2.setPosition(initialX + m_textureWidth, initialY);

        // 세 번째 스프라이트를 두 번째 바로 오른쪽에 위치시키기
        m_sprite3.setTexture(m_texture);
        m_sprite3.setPosition(initialX + 2 * m_textureWidth, initialY);
    }
}

// 업데이트 함수: 배경 스프라이트 위치 이동 및 무한 반복 처리
void Background::update(float deltaTime, float scrollSpeed) {
    m_scrollSpeed = scrollSpeed; // 현재 프레임 속도 저장
    float moveX = m_scrollSpeed * deltaTime; // 이번 프레임에 움직일 거리 계산 (px)

    // 3개 스프라이트를 왼쪽으로 moveX 만큼 이동
    m_sprite1.move(-moveX, 0);
    m_sprite2.move(-moveX, 0);
    m_sprite3.move(-moveX, 0);

    // 각 스프라이트 너비(가로길이)를 구함 (전부 동일하므로 하나만)
    float width = m_sprite1.getGlobalBounds().width;

    // 람다 함수: 스프라이트가 왼쪽 화면 밖으로 완전히 나가면 오른쪽 끝으로 이동시키기
    auto repositionIfNeeded = [&](sf::Sprite& sprite, const sf::Sprite& other1, const sf::Sprite& other2) {
        // 스프라이트의 오른쪽 끝이 0보다 작으면 (왼쪽 화면 완전 벗어남)
        if (sprite.getPosition().x + width < 0) {
            // 나머지 두 스프라이트 중 가장 오른쪽 끝 위치를 계산
            float rightMostX = std::max(other1.getPosition().x, other2.getPosition().x);
            // 현재 스프라이트를 오른쪽 끝 위치 + 너비 만큼 이동 (연결되도록)
            sprite.setPosition(std::floor(rightMostX + width), sprite.getPosition().y);
            // std::floor() 사용으로 부동소수점 오차 방지
        }
        };

    // 3개 스프라이트 각각에 대해 위치 재조정 실행
    repositionIfNeeded(m_sprite1, m_sprite2, m_sprite3);
    repositionIfNeeded(m_sprite2, m_sprite1, m_sprite3);
    repositionIfNeeded(m_sprite3, m_sprite1, m_sprite2);
}

// 배경 그리기 함수: 3개의 스프라이트를 모두 그림
void Background::draw(sf::RenderWindow& window) {
    window.draw(m_sprite1);
    window.draw(m_sprite2);
    window.draw(m_sprite3);
}

// 배경 위치 수동으로 설정하는 함수 (초기 위치 변경용)
void Background::setPosition(float x, float y) {
    m_sprite1.setPosition(x, y);
    m_sprite2.setPosition(x + m_textureWidth, y);
    m_sprite3.setPosition(x + 2 * m_textureWidth, y);
}

// 배경 스케일 설정 함수 (가로, 세로 크기 조절 가능)
void Background::setScale(float scaleX, float scaleY) {
    m_sprite1.setScale(scaleX, scaleY);
    m_sprite2.setScale(scaleX, scaleY);
    m_sprite3.setScale(scaleX, scaleY);

    // 스케일 적용 후 실제 너비 재계산 (원본 너비 * 스케일)
    m_textureWidth = static_cast<float>(m_texture.getSize().x) * scaleX;

    // 스프라이트 위치도 다시 맞춤 (스케일 변경으로 간격이 변하기 때문)
    m_sprite2.setPosition(m_sprite1.getPosition().x + m_textureWidth, m_sprite1.getPosition().y);
    m_sprite3.setPosition(m_sprite2.getPosition().x + m_textureWidth, m_sprite2.getPosition().y);
}
