// GameObject.cpp
#include "GameObject.h" // 해당 헤더 파일 포함
#include <iostream>     // 에러 메시지 출력을 위해 포함 (std::cerr 사용)

// --- GameObject 클래스 구현 ---
// GameObject 생성자 정의
GameObject::GameObject(const std::string& texturePath, sf::Vector2f position) {
    // 주어진 경로에서 텍스처를 로드합니다. 실패하면 에러 메시지를 출력합니다.
    if (!m_texture.loadFromFile(texturePath)) {
        std::cerr << "Error: Could not load game object texture from " << texturePath << std::endl;
        // 실제 게임에서는 여기서 더 강력한 에러 처리가 필요할 수 있습니다 (예: 게임 종료 또는 기본 텍스처 사용).
    }
    // 스프라이트에 로드된 텍스처를 설정합니다.
    m_sprite.setTexture(m_texture);
    // 스프라이트의 초기 위치를 설정합니다.
    // 스케일은 각 오브젝트 생성 시 main.cpp에서 직접 설정하도록 합니다.
    m_sprite.setPosition(position);
}

// 오브젝트를 윈도우에 그리는 함수 정의
void GameObject::draw(sf::RenderWindow& window) {
    window.draw(m_sprite); // 스프라이트를 윈도우에 그립니다.
}

// 오브젝트의 전역 바운딩 박스(충돌 영역)를 반환하는 함수 정의
sf::FloatRect GameObject::getGlobalBounds() const {
    return m_sprite.getGlobalBounds(); // 스프라이트의 현재 크기와 위치에 따른 충돌 영역을 반환합니다.
}

// 오브젝트의 현재 위치를 반환하는 함수 정의
sf::Vector2f GameObject::getPosition() const {
    return m_sprite.getPosition(); // 스프라이트의 현재 위치를 반환합니다.
}

// --- Pipe 클래스 구현 ---
// Pipe 생성자 정의: GameObject의 생성자를 호출하여 초기화합니다.
Pipe::Pipe(const std::string& texturePath, sf::Vector2f position)
    : GameObject(texturePath, position) {
    // Pipe에 특화된 추가 초기화가 필요하다면 여기에 코드를 추가합니다.
    // 스케일 조정은 main.cpp에서 getSprite().setScale()을 사용하여 직접 수행합니다.
}

// --- Platform 클래스 구현 ---
// Platform 생성자 정의: GameObject의 생성자를 호출하여 초기화합니다.
Platform::Platform(const std::string& texturePath, sf::Vector2f position)
    : GameObject(texturePath, position) {
    // Platform에 특화된 추가 초기화가 필요하다면 여기에 코드를 추가합니다.
    // 스케일 조정은 main.cpp에서 getSprite().setScale()을 사용하여 직접 수행합니다.
}

// --- MysteryBlock 클래스 구현 ---
// MysteryBlock 생성자 정의
MysteryBlock::MysteryBlock(const std::string& activeTexturePath, const std::string& hitTexturePath, sf::Vector2f position, std::unique_ptr<Item> itemToSpawn)
    : GameObject(activeTexturePath, position), // 활성 상태일 때의 텍스처로 기본 GameObject를 초기화합니다.
    m_isActive(true),                      // 처음에는 블록이 활성 상태(아이템을 줄 수 있음)입니다.
    m_itemSpawned(false),                  // 아직 아이템이 생성되지 않았습니다.
    m_itemToSpawn(std::move(itemToSpawn)) // 매개변수로 받은 아이템의 소유권을 이 블록으로 이동시킵니다.
{
    // 블록이 맞았을 때(비활성 상태) 사용할 텍스처를 로드합니다. 실패 시 에러 메시지를 출력합니다.
    if (!m_hitTexture.loadFromFile(hitTexturePath)) {
        std::cerr << "Error: Could not load hit block texture from " << hitTexturePath << std::endl;
    }
    // MysteryBlock의 스케일은 main.cpp에서 직접 설정됩니다.
}

// 블록이 플레이어에게 맞았을 때 호출되는 함수 정의
void MysteryBlock::hit() {
    // 블록이 현재 활성 상태인 경우에만 반응합니다.
    if (m_isActive) {
        m_isActive = false; // 블록을 비활성 상태로 변경합니다. (더 이상 아이템을 주지 않음)
        m_sprite.setTexture(m_hitTexture); // 스프라이트의 텍스처를 '맞은' 상태의 텍스처로 변경합니다.
        // 참고: 텍스처가 변경될 때 스프라이트의 원점(origin)이 바뀌면 위치가 틀어질 수 있습니다.
        // 필요하다면 setOrigin()을 사용하여 중앙 정렬 등을 할 수 있습니다.
        // 여기에 블록 히트 시 사운드 효과나 시각적 애니메이션을 추가할 수 있습니다.
    }
}

// 블록이 현재 활성 상태인지 (아이템을 줄 수 있는지) 여부를 반환하는 함수 정의
bool MysteryBlock::isActive() const {
    return m_isActive;
}

// 블록에서 아이템을 생성하여 반환하는 함수 정의
std::unique_ptr<Item> MysteryBlock::spawnItem() {
    // 생성할 아이템이 존재하고 (nullptr이 아니고) 아직 생성되지 않았다면
    if (m_itemToSpawn && !m_itemSpawned) {
        m_itemSpawned = true; // 아이템이 생성되었음을 표시합니다.
        // 아이템의 위치를 블록 바로 위로 설정합니다.
        // MysteryBlock의 y 위치에서 아이템 높이만큼 위로 올려 아이템이 블록 위에 나타나게 합니다.
        // 아이템 스프라이트의 getGlobalBounds().height를 사용하면 현재 스케일이 적용된 높이를 얻을 수 있습니다.
        m_itemToSpawn->setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - m_itemToSpawn->getSprite().getGlobalBounds().height);

        // std::move를 사용하여 m_itemToSpawn의 소유권을 이 함수를 호출한 쪽으로 이전합니다.
        // 이렇게 하면 블록은 아이템에 대한 소유권을 잃고, 아이템은 호출한 쪽에서 관리됩니다.
        return std::move(m_itemToSpawn);
    }
    return nullptr; // 아이템이 없거나 이미 생성되었다면 nullptr를 반환합니다.
}