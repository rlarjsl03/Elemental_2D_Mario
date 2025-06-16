// GameObject.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Player.h"
#include "Item.h"

// --- 모든 게임 오브젝트의 기본이 되는 추상 클래스 ---
class GameObject {
public:
    GameObject(const std::string& texturePath, sf::Vector2f position);
    virtual ~GameObject() = default;

    virtual void draw(sf::RenderWindow& window);
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getPosition() const;

    // 스프라이트에 접근할 수 있는 함수 추가: 이 부분이 핵심입니다.
    // 이 함수를 통해 파생 클래스나 main.cpp에서 직접 스프라이트의 스케일, 위치 등을 조작할 수 있습니다.
    sf::Sprite& getSprite() { return m_sprite; }
    const sf::Sprite& getSprite() const { return m_sprite; } // const 버전도 제공하여 안전하게 접근

protected:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
};

// --- Pipe 클래스 ---
class Pipe : public GameObject {
public:
    Pipe(const std::string& texturePath, sf::Vector2f position);
};

// --- Platform 클래스 ---
class Platform : public GameObject {
public:
    Platform(const std::string& texturePath, sf::Vector2f position);
};

// --- MysteryBlock 클래스 ---
class MysteryBlock : public GameObject {
public:
    // itemToSpawn은 기본값이 nullptr이므로, 아이템 없는 블록도 만들 수 있습니다.
    MysteryBlock(const std::string& activeTexturePath, const std::string& hitTexturePath, sf::Vector2f position, std::unique_ptr<Item> itemToSpawn = nullptr);
    void hit(); // 블록이 맞았을 때 호출되는 함수
    bool isActive() const; // 블록이 활성 상태인지 (아이템을 줄 수 있는지) 반환
    std::unique_ptr<Item> spawnItem(); // 블록에서 아이템을 생성하여 반환

private:
    sf::Texture m_hitTexture; // 블록이 맞았을 때 변경될 텍스처
    bool m_isActive;          // 블록이 현재 활성 상태인지 (아이템을 줄 수 있는지)
    bool m_itemSpawned;       // 아이템이 이미 생성되었는지 여부
    std::unique_ptr<Item> m_itemToSpawn; // 이 블록에서 나올 아이템 (unique_ptr로 소유권 관리)
};