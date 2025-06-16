#include "Item.h"

Item::Item(const std::string& texturePath, sf::Vector2f startPos) : position(startPos) {
    if (!texture.loadFromFile(texturePath))
        throw std::runtime_error("������ �ؽ�ó �ε� ����");
    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void Item::update(float deltaTime) {}

void Item::draw(sf::RenderWindow& window) {
    if (!collected)
        window.draw(sprite);
}

bool Item::checkCollision(const sf::Sprite& playerSprite) {
    if (!collected && sprite.getGlobalBounds().intersects(playerSprite.getGlobalBounds())) {
        collected = true;
        return true;
    }
    return false;
}
sf::FloatRect Item::getHitBox() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    if (isflag) {
    bounds.left += 100.f; // �¿� ���� ����
    bounds.width -= 20.f;
    //bounds.top += 10.f;  // ���� ���� ����
    //bounds.height -= 15.f;
     }

    return bounds;
}

bool Item::isCollected() const { return collected; }

const sf::Sprite& Item::getSprite() const { return sprite; }

// ���� ������ cpp
MushroomItem::MushroomItem(const std::string& texturePath, sf::Vector2f position)
    : Item(texturePath, position) {
}
void MushroomItem::applyEffect(Player& player) {
    // Ŀ���� ȿ��
    player.setIsBig(true);
}
ItemType MushroomItem::getType() const {
    return ItemType::RedMushroom;
}
GreenMushroomItem::GreenMushroomItem(const std::string& texturePath, sf::Vector2f position)
    : Item(texturePath, position) {
}
void GreenMushroomItem::applyEffect(Player& player) {
    // ��� ���� ȿ��
    player.increaseLife(1);    // ���� ����
}
ItemType GreenMushroomItem::getType() const {
    return ItemType::GreenMushroom;
}

// ���� ������ cpp
CoinItem::CoinItem(const std::string& texturePath, sf::Vector2f position)
    : Item(texturePath, position) {
}
void CoinItem::applyEffect(Player& player) {
    player.addScore(100); // Player�� addScore �Լ� �ʿ�
}
ItemType CoinItem::getType() const {
    return ItemType::Coin;
}
FlagItem::FlagItem(const std::string& texturePath, sf::Vector2f position)
    :Item(texturePath, position) {
}
void FlagItem::applyEffect(Player& player) {
    player.setisWin();
	isflag = true; // �÷��� �������� Ȱ��ȭ�Ǿ����� ǥ��
}
ItemType FlagItem::getType() const {
    return ItemType::Flag;
}