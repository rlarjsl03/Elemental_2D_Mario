#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"

class Item {    // ������ Ŭ����
protected:
    sf::Texture texture;
    sf::Sprite sprite;
    bool collected = false;

public:
    Item(const std::string& texturePath, sf::Vector2f position);
    virtual ~Item() = default;

    virtual void applyEffect(class Player& player) = 0; // �ٽ�: ȿ�� ó��
    virtual void update(float deltaTime);
    virtual void draw(sf::RenderWindow& window);
    bool checkCollision(const sf::Sprite& playerSprite);
    bool isCollected() const;

    const sf::Sprite& getSprite() const;
};

class MushroomItem : public Item {  // ���� ������ Ŭ����
public:
    MushroomItem(const std::string& texturePath, sf::Vector2f position);
    void applyEffect(Player& player) override;
};

class CoinItem : public Item {  // ���� ������ Ŭ����
public:
    CoinItem(const std::string& texturePath, sf::Vector2f position);
    void applyEffect(Player& player) override;
};