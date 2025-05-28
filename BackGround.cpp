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
        m_textureWidth = static_cast<float>(m_texture.getSize().x);

        // ù ��° ��������Ʈ ����
        m_sprite1.setTexture(m_texture);
        m_sprite1.setPosition(initialX, initialY);

        // �� ��° ��������Ʈ ���� (ù ��° ��������Ʈ �ٷ� �ڿ� ��ġ)
        m_sprite2.setTexture(m_texture);
        m_sprite2.setPosition(initialX + m_textureWidth, initialY);
    }
}

// ������Ʈ �Լ� ����
void Background::update(float deltaTime, float scrollSpeed) {
    m_scrollSpeed = scrollSpeed; // �ܺο��� ��ũ�� �ӵ��� �޾� ������Ʈ�� �� �ֵ���

    // ��ũ�� ������ ������Ʈ (�������� ��ũ��)
    m_currentScrollOffset -= m_scrollSpeed * deltaTime;

    // ��ũ�� �������� �̹��� �ʺ� ����� �ʱ�ȭ�Ͽ� ���� ��ũ�� ȿ��
    if (m_currentScrollOffset < -m_textureWidth) {
        m_currentScrollOffset += m_textureWidth;
    }

    // ��������Ʈ ��ġ ������Ʈ
    m_sprite1.setPosition(m_currentScrollOffset, m_sprite1.getPosition().y);
    m_sprite2.setPosition(m_currentScrollOffset + m_textureWidth, m_sprite2.getPosition().y);
}

// �׸��� �Լ� ����
void Background::draw(sf::RenderWindow& window) {
    window.draw(m_sprite1);
    window.draw(m_sprite2);
}

// (���� ����) ����� ��ġ ���� �Լ� ����
void Background::setPosition(float x, float y) {
    m_sprite1.setPosition(x, y);
    m_sprite2.setPosition(x + m_textureWidth, y);
    m_currentScrollOffset = x; // ��ġ�� ����Ǹ� ��ũ�� �����µ� �缳��
}

// (���� ����) ����� ������ ���� �Լ� ����
void Background::setScale(float scaleX, float scaleY) {
    m_sprite1.setScale(scaleX, scaleY);
    m_sprite2.setScale(scaleX, scaleY);

    // ������ ���� �� �ؽ�ó �ʺ� �����ؾ� �մϴ�.
    m_textureWidth = static_cast<float>(m_texture.getSize().x) * scaleX;
    m_sprite2.setPosition(m_sprite1.getPosition().x + m_textureWidth, m_sprite1.getPosition().y);
}