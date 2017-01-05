#include "ResourceManager.h"

ResourceManager::ResourceManager() {
	apple.loadFromFile("..\\res\\apple.png");
	bg.loadFromFile("..\\res\\bg.png");
	body.loadFromFile("..\\res\\body.png");
	bomb.loadFromFile("..\\res\\bomb.png");
	head.loadFromFile("..\\res\\head.png");
	life.loadFromFile("..\\res\\life.png");
	tail.loadFromFile("..\\res\\tail.png");
	turnL.loadFromFile("..\\res\\turnL.png");
	turnR.loadFromFile("..\\res\\turnR.png");
	wall.loadFromFile("..\\res\\wall.png");
	spawn.loadFromFile("..\\res\\spawn.png");
	mainBG.loadFromFile("..\\res\\mainBG.png");
	ConfirmButton.loadFromFile("..\\res\\ConfirmButton.png");

	myFont.loadFromFile("..\\res\\myFont.ttf");

	bg.setRepeated(true);
}

void ResourceManager::setupText(sf::Text& txt) {
	txt.setFont(getFont());
	txt.setCharacterSize(18);
	txt.setStyle(sf::Text::Regular);
	txt.setColor(sf::Color::White);
}

sf::Font& ResourceManager::getFont() {
	return myFont;
}

sf::Texture& ResourceManager::getConfirmButton() {
	return ConfirmButton;
}

sf::Texture& ResourceManager::getMainBG() {
	return mainBG;
}

sf::Texture& ResourceManager::getSpawn() {
	return spawn;
}

sf::Texture& ResourceManager::getWall() {
	return wall;
}

sf::Texture& ResourceManager::getTurnRight() {
	return turnR;
}

sf::Texture& ResourceManager::getTurnLeft() {
	return turnL;
}

sf::Texture& ResourceManager::getTail() {
	return tail;
}

sf::Texture& ResourceManager::getLife() {
	return life;
}

sf::Texture& ResourceManager::getHead() {
	return head;
}

sf::Texture& ResourceManager::getBomb() {
	return bomb;
}

sf::Texture& ResourceManager::getBody() {
	return body;
}

sf::Texture& ResourceManager::getBackground() {
	return bg;
}

sf::Texture& ResourceManager::getApple() {
	return apple;
}
