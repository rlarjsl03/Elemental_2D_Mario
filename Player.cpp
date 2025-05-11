#include <SFML/Window.hpp>  
#include <SFML/Graphics.hpp>  
#include <iostream>  

using namespace sf;

class Game {
public:
	RenderWindow _window;
	CircleShape shape;

	Game() : _window(VideoMode(1920, 1080), "SFML Game", Style::Default) {
		_window.setFramerateLimit(60);
	}
	void setWindow() {
		_window.clear(Color::White);
		_window.draw(shape);
		_window.display();
	}
	RenderWindow* getWindow() {
		return &_window;
	}
	void setPlayer() {
		shape.setRadius(50.0f);
		shape.setFillColor(Color::Blue);
		shape.setPosition(320.0f - 50, 240.0f - 50);
	}
	CircleShape* getPlayer() {
		shape.setFillColor(Color::Blue);
		return &shape;
	}

};

int main() {
	Game game;
	game.setPlayer();
	game.setWindow();
	game.getPlayer();
	while (game._window.isOpen()) {
		Event event;

		while (game._window.pollEvent(event)) {
			if (Keyboard::isKeyPressed(Keyboard::Left) == true) {
				game.getPlayer()->move(-30.0f, 0.0f);
			}
			else if (Keyboard::isKeyPressed(Keyboard::Right) == true) {
				game.getPlayer()->move(30.0f, 0.0f);
			}
			else if (Keyboard::isKeyPressed(Keyboard::Up) == true) {
				game.getPlayer()->move(0.0f, -30.0f);
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down) == true) {
				game.getPlayer()->move(0.0f, 30.0f);
			}
			else if (Keyboard::isKeyPressed(Keyboard::Escape) == true) {
				game.getWindow()->close();
			}
			game.setWindow();
			game.getWindow();
		}
	}
	return 0;
}