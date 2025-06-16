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
sf::FloatRect Item::getHitBox() const {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    if (isflag) {
    bounds.left += 100.f; // 좌우 여백 제거
    bounds.width -= 20.f;
    //bounds.top += 10.f;  // 위쪽 여백 제거
    //bounds.height -= 15.f;
     }

    return bounds;
}

bool Item::isCollected() const { return collected; }

const sf::Sprite& Item::getSprite() const { return sprite; }

// 버섯 아이템 cpp
MushroomItem::MushroomItem(const std::string& texturePath, sf::Vector2f position)
    : Item(texturePath, position) {
}
void MushroomItem::applyEffect(Player& player) {
    // 커지기 효과
    player.setIsBig(true);
}
ItemType MushroomItem::getType() const {
    return ItemType::RedMushroom;
}
GreenMushroomItem::GreenMushroomItem(const std::string& texturePath, sf::Vector2f position)
    : Item(texturePath, position) {
}
void GreenMushroomItem::applyEffect(Player& player) {
    // 목숨 증가 효과
    player.increaseLife(1);    // 생명 증가
}
ItemType GreenMushroomItem::getType() const {
    return ItemType::GreenMushroom;
}

// 코인 아이템 cpp
CoinItem::CoinItem(const std::string& texturePath, sf::Vector2f position)
    : Item(texturePath, position) {
}
void CoinItem::applyEffect(Player& player) {
    player.addScore(100); // Player에 addScore 함수 필요
}
ItemType CoinItem::getType() const {
    return ItemType::Coin;
}
FlagItem::FlagItem(const std::string& texturePath, sf::Vector2f position)
    :Item(texturePath, position) {
}
void FlagItem::applyEffect(Player& player) {
    player.setisWin();
	isflag = true; // 플래그 아이템이 활성화되었음을 표시
}
ItemType FlagItem::getType() const {
    return ItemType::Flag;
}