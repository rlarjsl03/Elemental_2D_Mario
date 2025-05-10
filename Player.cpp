#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class Player
{
private:
	CircleShape shape;

public:
	Player(CircleShape shape)
	{
		this->shape = shape;
	}
	void move(Keyboard a)
	{
		if (Keyboard::isKeyPressed(Keyboard::Left) == true)
		{
			shape.move(30.0f, 0.0f);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right) == true) {
		}
		else if (Keyboard::isKeyPressed(Keyboard::Up) == true) {
			shape.move(0.0f, -30.0f);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down) == true) {
			shape.move(0.0f, 30.0f);
		}
	}

};

int main()
{
	// Create a window
    RenderWindow window(VideoMode(1920, 1080), "WIndow");
    Style::Fullscreen;
	CircleShape shape(50.0f);
	shape.setFillColor(Color::Green);
	shape.setPosition(320.0f - 50, 800.0f - 50);
	Player player(shape);
	while (window.isOpen())
	{

		window.draw(shape);
		window.display();
		window.clear(Color::Black);

		Event event;
		while (window.pollEvent(event)) {

			switch (event.type) {
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
				if (Keyboard::isKeyPressed(Keyboard::Escape) == true) {
					window.close();
				}
				Keyboard a;
				Keyboard::isKeyPressed(a) == true;
				player.move(a);
				break;
			default:
				break;
			}
		}
	}
    return 0;
}