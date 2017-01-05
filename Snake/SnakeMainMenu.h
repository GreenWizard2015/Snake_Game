#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <fstream>

#include "ResourceManager.h"
#include "GameLevel.h"
#include "ScoreBoard.h"

class SnakeMainMenu: public sf::Drawable{
public:
	SnakeMainMenu();
	~SnakeMainMenu();
	void onEvent(const sf::Event& event);
	void Update(); 

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; 
	bool isCloseApp() const;
protected:
	void selectMenu(int id_);
	void activateMenu(int id_);

	void startSinglePlayer();
	void startMultiPlayer();
	void switchToGameOver();
	void saveNewScores();
	void showHighscoreBoard();

	void freeLevel();

	void freeBoard();

	void freeAll();
private:
	bool FCloseApp;
	int FCurrentItem;
	std::vector<sf::Text> FMenuItems;
	GameLevel* FGameLevel;
	ScoreBoard* FScoreBoard;
	sf::Sprite FBackground;
};