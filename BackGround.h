#pragma once
#include <SFML/Graphics.hpp>
#include <string> // std::string�� ����ϱ� ���� ����

using namespace sf;
class Background {
public:
    // ������: ��� �̹��� ���� ���, �ʱ� ��ġ (���� ����)�� �޽��ϴ�.
    Background(const std::string& texturePath, float initialX = 0.0f, float initialY = 0.0f);

    // ����� ���� ��ġ�� ������Ʈ�ϴ� �Լ� (�ַ� ��ũ�Ѹ��� ���)
    void update(float deltaTime, float scrollSpeed);

    // ����� ������ �����쿡 �׸��� �Լ�
    void draw(RenderWindow& window);

    // (���� ����) ����� ��ġ�� ���� �����ϴ� �Լ�
    void setPosition(float x, float y);

    // (���� ����) ����� �������� �����ϴ� �Լ�
    void setScale(float scaleX, float scaleY);

private:
    Texture m_texture;  // ��� �̹��� �ؽ�ó
    Sprite m_sprite1;   // ù ��° ��� ��������Ʈ (��ũ�Ѹ��� ���� 2�� ���)
    Sprite m_sprite2;   // �� ��° ��� ��������Ʈ

    float m_scrollSpeed; // ����� ��ũ�� �ӵ�
    float m_currentScrollOffset; // ���� ��ũ�� ������ (X��)
    float m_textureWidth; // ��� �̹����� ���� �ʺ�
};