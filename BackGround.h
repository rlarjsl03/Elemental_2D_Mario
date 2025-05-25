// BackGround.h
#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;


class Background {
private:
    Texture texture;
    Sprite sprite;

public:
    Background(const std::string& filename) {
        if (!texture.loadFromFile(filename)) {
            throw std::runtime_error("��� �̹��� �ε� ����: " + filename);
        }
        sprite.setTexture(texture);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};