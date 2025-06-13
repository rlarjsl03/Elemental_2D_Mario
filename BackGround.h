#pragma once
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

class Background {
public:
    // ������: ��� �̹��� ���� ���, �ʱ� ��ġ (���� ����)�� �޽��ϴ�.
    Background(const std::string& texturePath, float initialX = 0.0f, float initialY = 0.0f);

    // ���� �߽��� �޾� ��� ��ġ�� ������Ʈ�ϵ��� ���� (scrollSpeed�� �ʿ� ���� �� ����)
    void update(float deltaTime, sf::Vector2f viewCenter, float parallaxFactorX = 1.0f);

    // ����� ������ �����쿡 �׸��� �Լ�
    void draw(RenderWindow& window);

    // (���� ����) ����� ��ġ�� ���� �����ϴ� �Լ�
    void setPosition(float x, float y);

    // (���� ����) ����� �������� �����ϴ� �Լ�
    void setScale(float scaleX, float scaleY);

private:
    Texture m_texture;   // ��� �̹��� �ؽ�ó
    Sprite m_sprite1;   // ù ��° ��� ��������Ʈ
    Sprite m_sprite2;   // �� ��° ��� ��������Ʈ
    Sprite m_sprite3;   // �� ��° ��� ��������Ʈ �߰�!

    float m_scrollSpeed;         // ����� ��ũ�� �ӵ�
    float m_currentScrollOffset; // ���� ��ũ�� ������ (X��)
    float m_textureWidth;        // ��� �̹����� ���� �ʺ� (������ ���� ��)
};