#include "Item.h"

Item::Item(const std::string& texturePath, sf::Vector2f startPos) : position(startPos) {
    if (!texture.loadFromFile(texturePath))
        throw std::runtime_error("아이템 텍스처 로드 실패");
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

bool Item::isCollected() const { return collected; }

const sf::Sprite& Item::getSprite() const { return sprite; }

// 버섯 아이템 cpp
MushroomItem::MushroomItem(const std::string& texturePath, sf::Vector2f position)
    : Item(texturePath, position) {
}
void MushroomItem::applyEffect(Player& player) {
    // 커지기 효과
    player.setScale(2.0f); // Player에 setScale 함수 필요
}

// 코인 아이템 cpp
CoinItem::CoinItem(const std::string& texturePath, sf::Vector2f position)
    : Item(texturePath, position) {
}
void CoinItem::applyEffect(Player& player) {
    player.addScore(100); // Player에 addScore 함수 필요
}
