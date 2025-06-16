// item.h
#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h" // Player Ŭ���� ����� ���� ����

enum class ItemType {
    Coin,
    RedMushroom,
    GreenMushroom,
    Flag
};
class Item {    // ������ Ŭ����
protected:
    Texture texture;
    Vector2f position;
    Sprite sprite;
    bool collected = false;
    bool isflag = false; // �÷��� ������ ����

public:
    Item(const std::string& texturePath, sf::Vector2f position);
    virtual ~Item() = default;

    virtual void applyEffect(class Player& player) = 0; // �ٽ�: ȿ�� ó��
    virtual void update(float deltaTime);
    virtual void draw(sf::RenderWindow& window);
    virtual ItemType getType() const = 0; // Ÿ�� ��ȯ ���� �Լ�
    bool checkCollision(const sf::Sprite& playerSprite);
    bool isCollected() const;

    const Sprite& getSprite() const;
    void setPosition(float x, float y); // setPosition �Լ� ���� �߰�
    sf::FloatRect getHitBox() const;
};

class MushroomItem : public Item {  // ���� ������ Ŭ����
public:
    MushroomItem(const std::string& texturePath, sf::Vector2f position);
    void applyEffect(Player& player) override;
    ItemType getType() const;
};
class GreenMushroomItem : public Item {  // ��� ���� ������ Ŭ����
public:
    GreenMushroomItem(const std::string& texturePath, sf::Vector2f position);
    void applyEffect(Player& player) override;
    ItemType getType() const;
};

class CoinItem : public Item {  // ���� ������ Ŭ����
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
