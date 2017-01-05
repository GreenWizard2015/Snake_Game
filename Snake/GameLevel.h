#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <wchar.h>

#include "Player.h"
#include "GameBlock.h"
#include "SimpleBlocks.h"

struct LevelOptions{
	int MaxGoodBonuses;
	int MaxBadBonuses;
};

class GameLevel: public sf::Drawable{
public:
	GameLevel(const LevelOptions& Options_);
	~GameLevel();
	void onEvent(const sf::Event& event);

	void buildWalls();
	void Update();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void startSinglePlayer();
	void startMultiplayer();

	void pause();
	void unpause();

	bool isGameOver();
	void appendScores(std::vector<PlayerScore>& res);
protected:
	void updateInfo();
	void updateAllObjects();
	void checkCollisions();
	void checkPlayersHits();
	void removeDestroyed();
	void updateBonuses();

	bool isFreeSpace(int x, int y);
private:
	LevelOptions Options;
	bool FPaused;

	std::vector<Player>    players;
	std::vector<GameBlock*> blocks;

	sf::Sprite FBackground;
	sf::Text playerInfo;
	sf::Clock FBonusSpawnTimer;

	void freeBlocks();
	void addBlock(GameBlock* block);
};