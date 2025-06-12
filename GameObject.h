#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// 게임 내 모든 고정 오브젝트의 기본 클래스
class GameObject {
protected:
    sf::Texture m_texture; // 오브젝트의 텍스처
    sf::Sprite m_sprite;   // 오브젝트의 스프라이트

public:
    // 생성자: 텍스처 파일 경로와 초기 위치, 스케일을 받아 오브젝트를 초기화합니다.
    GameObject(const std::string& texturePath, sf::Vector2f position, sf::Vector2f scale = sf::Vector2f(1.f, 1.f));

    // 오브젝트를 윈도우에 그립니다.
    void draw(sf::RenderWindow& window) const;

    // 오브젝트의 위치를 설정합니다.
    void setPosition(float x, float y);
    void setPosition(sf::Vector2f position);

    // 오브젝트의 스케일을 설정합니다.
    void setScale(float scaleX, float scaleY);
    void setScale(sf::Vector2f scale);

    // 오브젝트의 전역 경계(Global Bounds)를 반환합니다. (충돌 감지에 사용)
    sf::FloatRect getGlobalBounds() const;

    // 오브젝트의 현재 위치를 반환합니다.
    sf::Vector2f getPosition() const;
};

// --- Pipe 클래스 ---
class Pipe : public GameObject {
public:
    // Pipe 생성자: 텍스처 경로, 초기 위치, 스케일을 받아 초기화합니다.
    // 파이프는 주로 x 위치와 groundY를 기반으로 높이가 조절됩니다.
    Pipe(const std::string& texturePath, sf::Vector2f position, float groundY, sf::Vector2f scale = sf::Vector2f(1.f, 1.f));
};

// --- Platform 클래스 ---
class Platform : public GameObject {
public:
    // Platform 생성자: 텍스처 경로, 초기 위치, 스케일을 받아 초기화합니다.
    Platform(const std::string& texturePath, sf::Vector2f position, sf::Vector2f scale = sf::Vector2f(1.f, 1.f));
};

// --- MysteryBlock 클래스 ---
class MysteryBlock : public GameObject {
private:
    bool m_hit; // 블록이 한 번 닿았는지 여부
public:
    // MysteryBlock 생성자: 텍스처 경로, 초기 위치, 스케일을 받아 초기화합니다.
    MysteryBlock(const std::string& texturePath, sf::Vector2f position, sf::Vector2f scale = sf::Vector2f(1.f, 1.f));

    // 블록이 플레이어에게 닿았을 때 호출되는 함수
    void hit();

    // 블록이 이미 닿았는지 여부를 반환합니다.
    bool isHit() const;
};