    #include "GameObject.h"
    #include <iostream>

    // GameObject 생성자 구현
    GameObject::GameObject(const std::string& texturePath, sf::Vector2f position, sf::Vector2f scale) {
        if (!m_texture.loadFromFile(texturePath)) {
            std::cerr << "Error: Could not load game object texture from " << texturePath << std::endl;
            // 텍스처 로드 실패 시 오류 메시지 출력
        }
        m_sprite.setTexture(m_texture); // 스프라이트에 텍스처 설정
        m_sprite.setPosition(position);  // 초기 위치 설정
        m_sprite.setScale(scale);        // 초기 스케일 설정
    }

    // 오브젝트 그리기
    void GameObject::draw(sf::RenderWindow& window) const {
        window.draw(m_sprite);
    }

    // 오브젝트 위치 설정
    void GameObject::setPosition(float x, float y) {
        m_sprite.setPosition(x, y);
    }

    void GameObject::setPosition(sf::Vector2f position) {
        m_sprite.setPosition(position);
    }

    // 오브젝트 스케일 설정
    void GameObject::setScale(float scaleX, float scaleY) {
        m_sprite.setScale(scaleX, scaleY);
    }

    void GameObject::setScale(sf::Vector2f scale) {
        m_sprite.setScale(scale);
    }

    // 오브젝트의 전역 경계 반환
    sf::FloatRect GameObject::getGlobalBounds() const {
        return m_sprite.getGlobalBounds();
    }

    // 오브젝트의 현재 위치 반환
    sf::Vector2f GameObject::getPosition() const {
        return m_sprite.getPosition();
    }

    // --- Pipe 클래스 구현 ---
    Pipe::Pipe(const std::string& texturePath, sf::Vector2f position, float groundY, sf::Vector2f scale)
        : GameObject(texturePath, position, scale) {
        // 파이프의 하단이 groundY에 닿도록 y 위치를 조정합니다.
        // 이미지의 원본 크기(픽셀)에 스케일을 곱한 후, groundY에서 그 높이를 뺍니다.
        float adjustedY = groundY - (m_texture.getSize().y * scale.y);
        m_sprite.setPosition(position.x, adjustedY);
    }

    // --- Platform 클래스 구현 ---
    Platform::Platform(const std::string& texturePath, sf::Vector2f position, sf::Vector2f scale)
        : GameObject(texturePath, position, scale) {
        // 플랫폼은 생성 시점에 지정된 위치를 사용합니다.
    }

    // --- MysteryBlock 클래스 구현 ---
    MysteryBlock::MysteryBlock(const std::string& texturePath, sf::Vector2f position, sf::Vector2f scale)
        : GameObject(texturePath, position, scale), m_hit(false) {
        // 미스터리 블록도 생성 시점에 지정된 위치를 사용합니다.
    }

    void MysteryBlock::hit() {
        if (!m_hit) {
            m_hit = true;
            // 블록이 닿았을 때 아이템이 나오거나 블록 이미지가 변경되는 로직을 여기에 추가할 수 있습니다.
            // 예를 들어, 닿으면 빈 블록 이미지로 변경:
            // if (m_texture.loadFromFile("empty_block.png")) {
            //     m_sprite.setTexture(m_texture);
            // }
            std::cout << "Mystery Block hit!" << std::endl;
        }
    }

    bool MysteryBlock::isHit() const {
        return m_hit;
    }