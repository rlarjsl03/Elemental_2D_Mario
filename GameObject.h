// GameObject.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Player.h"
#include "Item.h"

// --- ��� ���� ������Ʈ�� �⺻�� �Ǵ� �߻� Ŭ���� ---
class GameObject {
public:
    GameObject(const std::string& texturePath, sf::Vector2f position);
    virtual ~GameObject() = default;

    virtual void draw(sf::RenderWindow& window);
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getPosition() const;

    // ��������Ʈ�� ������ �� �ִ� �Լ� �߰�: �� �κ��� �ٽ��Դϴ�.
    // �� �Լ��� ���� �Ļ� Ŭ������ main.cpp���� ���� ��������Ʈ�� ������, ��ġ ���� ������ �� �ֽ��ϴ�.
    sf::Sprite& getSprite() { return m_sprite; }
    const sf::Sprite& getSprite() const { return m_sprite; } // const ������ �����Ͽ� �����ϰ� ����

protected:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
};

// --- Pipe Ŭ���� ---
class Pipe : public GameObject {
public:
    Pipe(const std::string& texturePath, sf::Vector2f position);
};

// --- Platform Ŭ���� ---
class Platform : public GameObject {
public:
    Platform(const std::string& texturePath, sf::Vector2f position);
};

// --- MysteryBlock Ŭ���� ---
class MysteryBlock : public GameObject {
public:
    // itemToSpawn�� �⺻���� nullptr�̹Ƿ�, ������ ���� ��ϵ� ���� �� �ֽ��ϴ�.
    MysteryBlock(const std::string& activeTexturePath, const std::string& hitTexturePath, sf::Vector2f position, std::unique_ptr<Item> itemToSpawn = nullptr);
    void hit(); // ����� �¾��� �� ȣ��Ǵ� �Լ�
    bool isActive() const; // ����� Ȱ�� �������� (�������� �� �� �ִ���) ��ȯ
    std::unique_ptr<Item> spawnItem(); // ��Ͽ��� �������� �����Ͽ� ��ȯ

private:
    sf::Texture m_hitTexture; // ����� �¾��� �� ����� �ؽ�ó
    bool m_isActive;          // ����� ���� Ȱ�� �������� (�������� �� �� �ִ���)
    bool m_itemSpawned;       // �������� �̹� �����Ǿ����� ����
    std::unique_ptr<Item> m_itemToSpawn; // �� ��Ͽ��� ���� ������ (unique_ptr�� ������ ����)
};