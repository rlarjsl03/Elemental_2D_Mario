#include <stdio.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


int main() {
	sf::RenderWindow __window;
	__window.create(sf::VideoMode(800, 600), "SFML window");
}
//sf::Texture texture;
//if (!texture.loadFromFile("player.png")) {
//	return -1; // Error loading texture
//}
//sf::Sprite sprite;
//sprite.setTexture(texture);
//sf::Vector2f velocity(0.f, 0.f);
//float speed = 200.f;
//float gravity = 9.81f;
//float groundY = 500.f; // Example ground level
//float jumpPower = -300.f;
//bool isOnGround = false;
//while (__window.isOpen()) {
//	sf::Event event;
//	while (__window.pollEvent(event)) {
//		if (event.type == sf::Event::Closed)
//			__window.close();
//	}
//	float deltaTime = __window.getFrameTime();
//	handleInput(deltaTime);
//	update(deltaTime);
//	__window.clear();
//	draw(__window);
//	__window.display();
//}
//return 0;
//}