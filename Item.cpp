#include "Item.h"

Item::Item(const std::string& texturePath, sf::Vector2f position) {
    if (!texture.loadFromFile(texturePath)) {
        throw std::runtime_error("아이템 텍스처 로드 실패");
    }
    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setScale(1.0f, 1.0f); // 필요 시 조절
}

void Item::draw(sf::RenderWindow& window) {
    if (!collected)
        window.draw(sprite);
}

void Item::update(float deltaTime) {
    // 애니메이션이나 회전 등 추가 가능
}

bool Item::checkCollision(const sf::Sprite& playerSprite) {
    if (!collected && sprite.getGlobalBounds().intersects(playerSprite.getGlobalBounds())) {
        collected = true;
        return true;
    }
    return false;
}

bool Item::isCollected() const {
    return collected;
}

const sf::Sprite& Item::getSprite() const {
    return sprite;
}