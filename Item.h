// item.h
#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h" // Player 클래스 사용을 위해 포함

enum class ItemType {
    Coin,
    RedMushroom,
    GreenMushroom,
    Flag
};
class Item {    // 아이템 클래스
protected:
    Texture texture;
    Vector2f position;
    Sprite sprite;
    bool collected = false;
    bool isflag = false; // 플래그 아이템 여부

public:
    Item(const std::string& texturePath, sf::Vector2f position);
    virtual ~Item() = default;

    virtual void applyEffect(class Player& player) = 0; // 핵심: 효과 처리
    virtual void update(float deltaTime);
    virtual void draw(sf::RenderWindow& window);
    virtual ItemType getType() const = 0; // 타입 반환 가상 함수
    bool checkCollision(const sf::Sprite& playerSprite);
    bool isCollected() const;

    const Sprite& getSprite() const;
    void setPosition(float x, float y); // setPosition 함수 선언 추가
    sf::FloatRect getHitBox() const;
};

class MushroomItem : public Item {  // 버섯 아이템 클래스
public:
    MushroomItem(const std::string& texturePath, sf::Vector2f position);
    void applyEffect(Player& player) override;
    ItemType getType() const;
};
class GreenMushroomItem : public Item {  // 녹색 버섯 아이템 클래스
public:
    GreenMushroomItem(const std::string& texturePath, sf::Vector2f position);
    void applyEffect(Player& player) override;
    ItemType getType() const;
};

class CoinItem : public Item {  // 동전 아이템 클래스
public:
    CoinItem(const std::string& texturePath, sf::Vector2f position);
    void applyEffect(Player& player) override;
    ItemType getType() const;
};
class FlagItem : public Item{
public:
    FlagItem(const std::string & texturePath, sf::Vector2f position);
    void applyEffect(Player& player) override;
    ItemType getType() const;
};
