// main.cpp
#include <SFML/Graphics.hpp>
#include "Background.h" // Background 클래스 헤더 포함

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mario Game - Background");
    window.setFramerateLimit(60); // 60 FPS로 프레임 제한

    // Background 객체 생성
    Background gameBackground("background1.jpg");
    //test
    // 배경의 초기 위치를 설정 (선택 사항)
    // gameBackground.setPosition(0, 0);

    // 배경의 스케일을 설정 (예: 창 크기에 맞추기 위해)
    // float scaleX = static_cast<float>(window.getSize().x) / gameBackground.getTexture().getSize().x; // getTexture() 함수가 있다면
    // float scaleY = static_cast<float>(window.getSize().y) / gameBackground.getTexture().getSize().y;
    // gameBackground.setScale(scaleX, scaleY);


    sf::Clock clock; // deltaTime 계산을 위한 클럭

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // deltaTime 계산
        float deltaTime = clock.restart().asSeconds();

        // 배경 업데이트 (예: 100 픽셀/초 속도로 스크롤)
        // 이 scrollSpeed는 게임 캐릭터의 이동 속도에 따라 동적으로 조절할 수 있습니다.
        gameBackground.update(deltaTime, 100.0f);

        window.clear(); // 화면 지우기
        gameBackground.draw(window); // 배경 그리기
        // 여기에 마리오, 블록, 적 등의 다른 게임 오브젝트 그리기 코드
        window.display(); // 화면에 최종 이미지 표시
    }

    return 0;
}