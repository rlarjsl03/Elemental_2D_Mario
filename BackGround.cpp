#include "Background.h"
#include <iostream>

using namespace sf;

// ������ ����
Background::Background(const std::string& texturePath, float initialX, float initialY)
    : m_scrollSpeed(0.0f), m_currentScrollOffset(0.0f), m_textureWidth(0.0f), m_lastViewCenterX(initialX) // initialX�� �ʱ�ȭ�Ͽ� ù ������Ʈ �� �ùٸ� ��Ÿ ���
{
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Error: Could not load background texture from " << texturePath << std::endl;
        // ���� ���ӿ����� ���⼭ �� ������ ���� ó���� �ʿ��� �� �ֽ��ϴ� (��: ���� ����).
    }
    else {
        m_texture.setRepeated(true);
        m_textureWidth = static_cast<float>(m_texture.getSize().x);

        // ù ��° ��������Ʈ ����
        m_sprite1.setTexture(m_texture);
        m_sprite1.setPosition(initialX, initialY);

        // �� ��° ��������Ʈ ���� (ù ��° ��������Ʈ �ٷ� �ڿ� ��ġ)
        m_sprite2.setTexture(m_texture);
        m_sprite2.setPosition(initialX + m_textureWidth, initialY);

        // �� ��° ��������Ʈ ���� (�� ��° ��������Ʈ �ٷ� �ڿ� ��ġ)
        m_sprite3.setTexture(m_texture);
        m_sprite3.setPosition(initialX + (m_textureWidth * 2), initialY);
    }
}

// ������Ʈ �Լ� ����
void Background::update(float deltaTime, sf::Vector2f viewCenter, float parallaxFactorX) {
    // ���� ���� �߽ɰ� ���� �߽��� ���̸� ���
    float deltaViewX = viewCenter.x - m_lastViewCenterX;

    // ����� �������� �� �� ���. ���� ȿ�� ����.
    // �÷��̾ ���������� ���� (viewCenter.x ����), ����� �������� ���� �ڿ�������.
    // ���� deltaViewX�� -1�� ���ϰų�, parallaxFactorX�� ������ �����ϴ� ���� �½��ϴ�.
    // ���⼭�� parallaxFactorX�� ������ �ްų�, ���ο��� ��ȣ�� ������Ű�� ���� ����մϴ�.
    // ������ player.getVelocity().x * 0.5f�� ���� �̹� �ݴ�������� ���޵ǹǷ�,
    // deltaViewX�� �״�� ����ϰ� move() �ÿ��� -backgroundMoveAmount�� ����� ���� �ֽ��ϴ�.
    // ������ ���� �������� �����, �䰡 ���������� �����̸� ��� ��������Ʈ�� �������� �����̰� �ϴ� ���Դϴ�.
    float backgroundMoveAmount = deltaViewX * parallaxFactorX; // �䰡 ������ ��ŭ ��浵 �����̵�, �ӵ� ����

    // �� ��������Ʈ�� ���� �縸ŭ �̵���ŵ�ϴ�.
    // �䰡 ���������� �����̸�(deltaViewX > 0), backgroundMoveAmount�� ���.
    // ����� �������� �̵��ؾ� �ϹǷ�, -backgroundMoveAmount
    m_sprite1.move(-backgroundMoveAmount, 0.f);
    m_sprite2.move(-backgroundMoveAmount, 0.f);
    m_sprite3.move(-backgroundMoveAmount, 0.f);

    // ���� ���� �� ��ǥ�� ����մϴ�. (1920.f�� ������ �ʺ�)
    float viewLeftEdge = viewCenter.x - (1920.f / 2.f);
    float viewRightEdge = viewCenter.x + (1920.f / 2.f);

    // ��������Ʈ ���ġ ����
    // �� ��������Ʈ�� ȭ�� ���� ������ ������ ����� ��, ���� ���������� ���ġ
    // (��������Ʈ�� ������ ���� ���� ���� ������ ���ʿ� ������)
    if (m_sprite1.getPosition().x + m_textureWidth < viewLeftEdge) {
        // sprite1�� ������ ��������Ʈ���� ���� ������ �ڷ� �̵� (3�� �ʺ�ŭ)
        m_sprite1.setPosition(m_sprite1.getPosition().x + (m_textureWidth * 3), m_sprite1.getPosition().y);
    }
    if (m_sprite2.getPosition().x + m_textureWidth < viewLeftEdge) {
        m_sprite2.setPosition(m_sprite2.getPosition().x + (m_textureWidth * 3), m_sprite2.getPosition().y);
    }
    if (m_sprite3.getPosition().x + m_textureWidth < viewLeftEdge) {
        m_sprite3.setPosition(m_sprite3.getPosition().x + (m_textureWidth * 3), m_sprite3.getPosition().y);
    }

    // �� ��������Ʈ�� ȭ�� ������ ������ ������ ����� ��, ���� �������� ���ġ
    // (��������Ʈ�� ���� ���� ���� ������ ������ �����ʿ� ������)
    if (m_sprite1.getPosition().x > viewRightEdge) {
        m_sprite1.setPosition(m_sprite1.getPosition().x - (m_textureWidth * 3), m_sprite1.getPosition().y);
    }
    if (m_sprite2.getPosition().x > viewRightEdge) {
        m_sprite2.setPosition(m_sprite2.getPosition().x - (m_textureWidth * 3), m_sprite2.getPosition().y);
    }
    if (m_sprite3.getPosition().x > viewRightEdge) {
        m_sprite3.setPosition(m_sprite3.getPosition().x - (m_textureWidth * 3), m_sprite3.getPosition().y);
    }

    // ���� �� �߽��� ���� �������� ���� �����մϴ�.
    m_lastViewCenterX = viewCenter.x;
}

// �׸��� �Լ� ���� (������ ����)
void Background::draw(sf::RenderWindow& window) {
    window.draw(m_sprite1);
    window.draw(m_sprite2);
    window.draw(m_sprite3);
}

// (���� ����) ����� ��ġ ���� �Լ� ����
// �� �Լ��� �ʱ� �������� ����ϰ�, update������ ������� �ʴ� ���� �����ϴ�.
void Background::setPosition(float x, float y) {
    m_sprite1.setPosition(x, y);
    m_sprite2.setPosition(x + m_textureWidth, y);
    m_sprite3.setPosition(x + (m_textureWidth * 2), y);
    // m_currentScrollOffset = x; // �� ��Ŀ����� �� �̻� �ʿ� ���� �� �ֽ��ϴ�.
}

// (���� ����) ����� ������ ���� �Լ� ���� (������ ����)
void Background::setScale(float scaleX, float scaleY) {
    m_sprite1.setScale(scaleX, scaleY);
    m_sprite2.setScale(scaleX, scaleY);
    m_sprite3.setScale(scaleX, scaleY);

    m_textureWidth = static_cast<float>(m_texture.getSize().x) * scaleX;
    m_sprite2.setPosition(m_sprite1.getPosition().x + m_textureWidth, m_sprite1.getPosition().y);
    m_sprite3.setPosition(m_sprite1.getPosition().x + (m_textureWidth * 2), m_sprite1.getPosition().y);
}