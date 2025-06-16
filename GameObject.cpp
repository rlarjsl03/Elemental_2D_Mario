// GameObject.cpp
#include "GameObject.h" // �ش� ��� ���� ����
#include <iostream>     // ���� �޽��� ����� ���� ���� (std::cerr ���)

// --- GameObject Ŭ���� ���� ---
// GameObject ������ ����
GameObject::GameObject(const std::string& texturePath, sf::Vector2f position) {
    // �־��� ��ο��� �ؽ�ó�� �ε��մϴ�. �����ϸ� ���� �޽����� ����մϴ�.
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Error: Could not load game object texture from " << texturePath << std::endl;
        // ���� ���ӿ����� ���⼭ �� ������ ���� ó���� �ʿ��� �� �ֽ��ϴ� (��: ���� ���� �Ǵ� �⺻ �ؽ�ó ���).
    }
    // ��������Ʈ�� �ε�� �ؽ�ó�� �����մϴ�.
    m_sprite.setTexture(m_texture);
    // ��������Ʈ�� �ʱ� ��ġ�� �����մϴ�.
    // �������� �� ������Ʈ ���� �� main.cpp���� ���� �����ϵ��� �մϴ�.
    m_sprite.setPosition(position);
}

// ������Ʈ�� �����쿡 �׸��� �Լ� ����
void GameObject::draw(sf::RenderWindow& window) {
    window.draw(m_sprite); // ��������Ʈ�� �����쿡 �׸��ϴ�.
}

// ������Ʈ�� ���� �ٿ�� �ڽ�(�浹 ����)�� ��ȯ�ϴ� �Լ� ����
sf::FloatRect GameObject::getGlobalBounds() const {
    return m_sprite.getGlobalBounds(); // ��������Ʈ�� ���� ũ��� ��ġ�� ���� �浹 ������ ��ȯ�մϴ�.
}

// ������Ʈ�� ���� ��ġ�� ��ȯ�ϴ� �Լ� ����
sf::Vector2f GameObject::getPosition() const {
    return m_sprite.getPosition(); // ��������Ʈ�� ���� ��ġ�� ��ȯ�մϴ�.
}

// --- Pipe Ŭ���� ���� ---
// Pipe ������ ����: GameObject�� �����ڸ� ȣ���Ͽ� �ʱ�ȭ�մϴ�.
Pipe::Pipe(const std::string& texturePath, sf::Vector2f position)
    : GameObject(texturePath, position) {
    // Pipe�� Ưȭ�� �߰� �ʱ�ȭ�� �ʿ��ϴٸ� ���⿡ �ڵ带 �߰��մϴ�.
    // ������ ������ main.cpp���� getSprite().setScale()�� ����Ͽ� ���� �����մϴ�.
}

// --- Platform Ŭ���� ���� ---
// Platform ������ ����: GameObject�� �����ڸ� ȣ���Ͽ� �ʱ�ȭ�մϴ�.
Platform::Platform(const std::string& texturePath, sf::Vector2f position)
    : GameObject(texturePath, position) {
    // Platform�� Ưȭ�� �߰� �ʱ�ȭ�� �ʿ��ϴٸ� ���⿡ �ڵ带 �߰��մϴ�.
    // ������ ������ main.cpp���� getSprite().setScale()�� ����Ͽ� ���� �����մϴ�.
}

// --- MysteryBlock Ŭ���� ���� ---
// MysteryBlock ������ ����
MysteryBlock::MysteryBlock(const std::string& activeTexturePath, const std::string& hitTexturePath, sf::Vector2f position, std::unique_ptr<Item> itemToSpawn)
    : GameObject(activeTexturePath, position), // Ȱ�� ������ ���� �ؽ�ó�� �⺻ GameObject�� �ʱ�ȭ�մϴ�.
    m_isActive(true),                      // ó������ ����� Ȱ�� ����(�������� �� �� ����)�Դϴ�.
    m_itemSpawned(false),                  // ���� �������� �������� �ʾҽ��ϴ�.
    m_itemToSpawn(std::move(itemToSpawn)) // �Ű������� ���� �������� �������� �� ������� �̵���ŵ�ϴ�.
{
    // ����� �¾��� ��(��Ȱ�� ����) ����� �ؽ�ó�� �ε��մϴ�. ���� �� ���� �޽����� ����մϴ�.
    if (!m_hitTexture.loadFromFile(hitTexturePath)) {
        std::cerr << "Error: Could not load hit block texture from " << hitTexturePath << std::endl;
    }
    // MysteryBlock�� �������� main.cpp���� ���� �����˴ϴ�.
}

// ����� �÷��̾�� �¾��� �� ȣ��Ǵ� �Լ� ����
void MysteryBlock::hit() {
    // ����� ���� Ȱ�� ������ ��쿡�� �����մϴ�.
    if (m_isActive) {
        m_isActive = false; // ����� ��Ȱ�� ���·� �����մϴ�. (�� �̻� �������� ���� ����)
        m_sprite.setTexture(m_hitTexture); // ��������Ʈ�� �ؽ�ó�� '����' ������ �ؽ�ó�� �����մϴ�.
        // ����: �ؽ�ó�� ����� �� ��������Ʈ�� ����(origin)�� �ٲ�� ��ġ�� Ʋ���� �� �ֽ��ϴ�.
        // �ʿ��ϴٸ� setOrigin()�� ����Ͽ� �߾� ���� ���� �� �� �ֽ��ϴ�.
        // ���⿡ ��� ��Ʈ �� ���� ȿ���� �ð��� �ִϸ��̼��� �߰��� �� �ֽ��ϴ�.
    }
}

// ����� ���� Ȱ�� �������� (�������� �� �� �ִ���) ���θ� ��ȯ�ϴ� �Լ� ����
bool MysteryBlock::isActive() const {
    return m_isActive;
}

// ��Ͽ��� �������� �����Ͽ� ��ȯ�ϴ� �Լ� ����
std::unique_ptr<Item> MysteryBlock::spawnItem() {
    // ������ �������� �����ϰ� (nullptr�� �ƴϰ�) ���� �������� �ʾҴٸ�
    if (m_itemToSpawn && !m_itemSpawned) {
        m_itemSpawned = true; // �������� �����Ǿ����� ǥ���մϴ�.
        // �������� ��ġ�� ��� �ٷ� ���� �����մϴ�.
        // MysteryBlock�� y ��ġ���� ������ ���̸�ŭ ���� �÷� �������� ��� ���� ��Ÿ���� �մϴ�.
        // ������ ��������Ʈ�� getGlobalBounds().height�� ����ϸ� ���� �������� ����� ���̸� ���� �� �ֽ��ϴ�.
        m_itemToSpawn->setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - m_itemToSpawn->getSprite().getGlobalBounds().height);

        // std::move�� ����Ͽ� m_itemToSpawn�� �������� �� �Լ��� ȣ���� ������ �����մϴ�.
        // �̷��� �ϸ� ����� �����ۿ� ���� �������� �Ұ�, �������� ȣ���� �ʿ��� �����˴ϴ�.
        return std::move(m_itemToSpawn);
    }
    return nullptr; // �������� ���ų� �̹� �����Ǿ��ٸ� nullptr�� ��ȯ�մϴ�.
}