#include "Background.h"
#include <iostream> // ���� �α��� ���� ����

using namespace sf;

// ������ ����
Background::Background(const std::string& texturePath, float initialX, float initialY)
    : m_scrollSpeed(0.0f), m_currentScrollOffset(0.0f), m_textureWidth(0.0f) // ��� �ʱ�ȭ
{
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Error: Could not load background texture from " << texturePath << std::endl;
        // ���� ���ӿ����� ���⼭ �� ������ ���� ó���� �ʿ��� �� �ֽ��ϴ� (��: ���� ����).
    }
    else {
        m_texture.setRepeated(true); // �̹����� �ݺ��ؼ� ����ϵ��� ���� (���� ����, ��ũ�Ѹ��� ����)
        m_textureWidth = static_cast<float>(m_texture.getSize().x); // ������ ���� �� �ʺ�

        // ù ��° ��������Ʈ ����
        m_sprite1.setTexture(m_texture);
        m_sprite1.setPosition(initialX, initialY);

        // �� ��° ��������Ʈ ���� (ù ��° ��������Ʈ �ٷ� �ڿ� ��ġ)
        m_sprite2.setTexture(m_texture);
        m_sprite2.setPosition(initialX + m_textureWidth, initialY);

        // �� ��° ��������Ʈ ���� (�� ��° ��������Ʈ �ٷ� �ڿ� ��ġ)
        m_sprite3.setTexture(m_texture);
        m_sprite3.setPosition(initialX + (m_textureWidth * 2), initialY); // 3��° ��������Ʈ �߰�
    }
}

// ������Ʈ �Լ� ���� (���� �߽��� �޾� ���� ��ġ ���)
void Background::update(float deltaTime, sf::Vector2f viewCenter, float parallaxFactorX) {
    // ���� ���� ���� �������� ��� ��������Ʈ�� X ��ġ�� ����մϴ�.
    // ���� ���� ��: viewCenter.x - (ȭ�� �ʺ� / 2)
    // ����� ���� �����ӿ� 'parallaxFactorX'��ŭ ������ �����ϵ��� �մϴ�.
    float viewLeft = viewCenter.x - (1920.f / 2.f); // 1920.f�� ������ �ʺ�

    // ����� ���� X ������ (���� �����ӿ� ���)
    float backgroundXOffset = viewLeft * parallaxFactorX;

    // �� �������� m_textureWidth �������� �ݺ���ŵ�ϴ�.
    // �̷��� �ϸ� ����� ���� ��ũ�ѵǴ� ��ó�� ���Դϴ�.
    while (backgroundXOffset < 0) {
        backgroundXOffset += m_textureWidth;
    }
    while (backgroundXOffset >= m_textureWidth) {
        backgroundXOffset -= m_textureWidth;
    }

    // ���� ��������Ʈ���� ��ġ�� �����մϴ�.
    // backgroundXOffset�� �ؽ�ó�� ���� ��ġ�� �� �� �ֽ��ϴ�.
    // ����, ���� ���� ���� �������� backgroundXOffset�� ���� ��������� ��ũ�ѵ� ��ó�� ���̰� �մϴ�.
    m_sprite1.setPosition(viewLeft - backgroundXOffset, m_sprite1.getPosition().y);
    m_sprite2.setPosition(viewLeft - backgroundXOffset + m_textureWidth, m_sprite2.getPosition().y);
    m_sprite3.setPosition(viewLeft - backgroundXOffset + (m_textureWidth * 2), m_sprite3.getPosition().y);

    // Y ��ġ�� �����ϰų�, viewCenter.y�� ���� ������ �� �ֽ��ϴ�.
    // ��: m_sprite1.setPosition(..., viewCenter.y * parallaxFactorY);
}

// �׸��� �Լ� ����
void Background::draw(sf::RenderWindow& window) {
    window.draw(m_sprite1);
    window.draw(m_sprite2);
    window.draw(m_sprite3); // 3��° ��������Ʈ �׸���
}

// (���� ����) ����� ��ġ ���� �Լ� ����
void Background::setPosition(float x, float y) {
    m_sprite1.setPosition(x, y);
    m_sprite2.setPosition(x + m_textureWidth, y);
    m_sprite3.setPosition(x + (m_textureWidth * 2), y); // 3��° ��������Ʈ ��ġ ����
    m_currentScrollOffset = x; // ��ġ�� ����Ǹ� ��ũ�� �����µ� �缳��
}

// (���� ����) ����� ������ ���� �Լ� ����
void Background::setScale(float scaleX, float scaleY) {
    m_sprite1.setScale(scaleX, scaleY);
    m_sprite2.setScale(scaleX, scaleY);
    m_sprite3.setScale(scaleX, scaleY); // 3��° ��������Ʈ ������ ����

    // ������ ���� �� �ؽ�ó �ʺ� �����ؾ� �մϴ�.
    m_textureWidth = static_cast<float>(m_texture.getSize().x) * scaleX;
    // ��������Ʈ 2, 3�� ��ġ�� ����� m_textureWidth�� �������� �ٽ� ����
    m_sprite2.setPosition(m_sprite1.getPosition().x + m_textureWidth, m_sprite1.getPosition().y);
    m_sprite3.setPosition(m_sprite1.getPosition().x + (m_textureWidth * 2), m_sprite1.getPosition().y);
}