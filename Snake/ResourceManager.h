#pragma once
#include <SFML/Graphics.hpp>

class ResourceManager{
public:
	sf::Texture& getApple();
	sf::Texture& getBackground();
	sf::Texture& getBody();
	sf::Texture& getBomb();
	sf::Texture& getHead();
	sf::Texture& getLife();
	sf::Texture& getTail();
	sf::Texture& getTurnLeft();
	sf::Texture& getTurnRight();
	sf::Texture& getWall();
	sf::Texture& getSpawn();
	sf::Texture& getMainBG();
	sf::Texture& getConfirmButton();

	sf::Font& getFont();

	void setupText(sf::Text& txt);

	ResourceManager();
	
private:
	sf::Texture	apple, bg, body, bomb, head, life, tail, turnR, turnL, wall,
		spawn, mainBG, ConfirmButton;

	sf::Font myFont;
};

static ResourceManager RESOURCES;