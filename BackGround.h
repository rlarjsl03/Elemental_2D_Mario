#pragma once
#include <SFML/Graphics.hpp>

class Background {
public:
    Background(const std::string& texturePath, float initialX = 0.f, float initialY = 0.f);
    void update(float deltaTime, float scrollSpeed);
    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
    void setScale(float scaleX, float scaleY);

private:
    sf::Texture m_texture;
    sf::Sprite m_sprite1, m_sprite2, m_sprite3;
    float m_scrollSpeed;
    float m_textureWidth;
};
