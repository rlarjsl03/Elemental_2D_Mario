#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

class Background {
public:
    Background(const std::string& texturePath, float initialX = 0.0f, float initialY = 0.0f);

    void update(float deltaTime, sf::Vector2f viewCenter, float parallaxFactorX = 1.0f);

    void draw(RenderWindow& window);

    // setPosition�� setScale�� ���� ���� ������ ���� �����ǹǷ�
    // �ܺο��� ���� ȣ���� �ʿ�� ������, �ʿ�ø� ���� ���ܵӴϴ�.
    void setPosition(float x, float y);
    void setScale(float scaleX, float scaleY);

private:
    Texture m_texture;   // ��� �̹��� �ؽ�ó
    Sprite m_sprite1;   // ù ��° ��� ��������Ʈ
    Sprite m_sprite2;   // �� ��° ��� ��������Ʈ
    Sprite m_sprite3;   // �� ��° ��� ��������Ʈ

    float m_scrollSpeed;         // ���� ������ ���� �� �ֽ��ϴ�.
    float m_currentScrollOffset; // ���� ������ ���� �� �ֽ��ϴ�.
    float m_textureWidth;        // ��� �̹����� ���� �ʺ� (������ ���� ��)

    float m_lastViewCenterX; // ���� �������� �� �߽� X ��ǥ
};