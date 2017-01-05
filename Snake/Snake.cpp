#include <string>
#include <list>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "SnakeMainMenu.h"
using namespace std;

const wstring WINDOW_TITLE = L"Snake.";

int main(){
	sf::RenderWindow window(sf::VideoMode(640, 480), WINDOW_TITLE);

	SnakeMainMenu mainGame;
	while (window.isOpen())	{
		sf::Event event;
		while (window.pollEvent(event))	{
			mainGame.onEvent(event);
			switch(event.type){
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::KeyPressed:
					if(event.key.code == sf::Keyboard::Escape)
						window.close();
					break;
			}
		}

		if(mainGame.isCloseApp())
			window.close();
		
		window.clear();
		mainGame.Update();
		window.draw(mainGame);
		window.display();
	}

	return 0;
}