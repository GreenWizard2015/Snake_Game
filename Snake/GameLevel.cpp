#include "GameLevel.h"

const int GAME_GRID_X = 32; 
const int GAME_GRID_Y = 32; 

const int GAME_GRID_ROWS = 15; 
const int GAME_GRID_COLLS = 14; 

void GameLevel::Update(){
	if(!FPaused){
		updateAllObjects();
		checkCollisions();
		removeDestroyed();

		updateBonuses();
	}
	updateInfo();
}

GameLevel::GameLevel(const LevelOptions& Options_): 
	Options(Options_), FPaused(false) {
	buildWalls();
	RESOURCES.setupText(playerInfo);
	playerInfo.setPosition(32.0f * (GAME_GRID_COLLS - 0.5f), -32);

	FBackground.setTexture(RESOURCES.getBackground());
	FBackground.setTextureRect(
		sf::IntRect(0, 0, 32 * (GAME_GRID_COLLS - 1), 
		32 * (GAME_GRID_ROWS - 1)));
}

void GameLevel::buildWalls(){
	// left and right walls
	for(int y = 0; y < GAME_GRID_ROWS; ++y){
		addBlock(new WallBlock(0, y));
		addBlock(new WallBlock(GAME_GRID_COLLS - 1, y));
	}

	// left and bottom walls
	for(int x = 1; x < GAME_GRID_COLLS - 1; ++x){
		addBlock(new WallBlock(x, 0));
		addBlock(new WallBlock(x, GAME_GRID_ROWS - 1));
	}
}

void GameLevel::addBlock(GameBlock* block){
	blocks.push_back(block);
}

GameLevel::~GameLevel(){
	freeBlocks();
}

void GameLevel::freeBlocks(){
	for(auto block = blocks.cbegin(); block != blocks.cend(); ++block){
		delete *block;
	}
	blocks.clear();
}

void GameLevel::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::Transform oldT = states.transform;
	states.transform.translate(32, 32);

	target.draw(FBackground, states);

	for(auto block = blocks.cbegin(); block != blocks.cend(); ++block){
		target.draw(**block, states);
	}

	for(auto player = players.cbegin(); player != players.cend(); ++player){
		target.draw(*player, states);
	}

	target.draw(playerInfo, states);

	states.transform = oldT;
}

void GameLevel::startSinglePlayer(){
	players.clear();
	PlayerInfo pi;
	pi.Color = sf::Color::White;
	pi.UP = sf::Keyboard::Up;
	pi.DOWN = sf::Keyboard::Down;
	pi.LEFT = sf::Keyboard::Left;
	pi.RIGHT = sf::Keyboard::Right;
	pi.spawnLenght = 5;
	pi.Speed = 0.5f;
	pi.spawnX = GAME_GRID_COLLS / 2;
	pi.spawnY = GAME_GRID_ROWS / 2;

	players.push_back(Player(pi));
}

void GameLevel::startMultiplayer(){
	players.clear();
	PlayerInfo pi;
	pi.Color = sf::Color::White;
	pi.UP = sf::Keyboard::Up;
	pi.DOWN = sf::Keyboard::Down;
	pi.LEFT = sf::Keyboard::Left;
	pi.RIGHT = sf::Keyboard::Right;
	pi.spawnLenght = 5;
	pi.Speed = 0.5f;
	pi.spawnX = GAME_GRID_COLLS / 3;
	pi.spawnY = GAME_GRID_ROWS / 2;

	players.push_back(Player(pi));
	//////////////////////////////////////

	pi.Color = sf::Color(128, 255, 128);
	pi.UP = sf::Keyboard::W;
	pi.DOWN = sf::Keyboard::S;
	pi.LEFT = sf::Keyboard::A;
	pi.RIGHT = sf::Keyboard::D;
	pi.spawnLenght = 5;
	pi.Speed = 0.5f;
	pi.spawnX = 2 * (GAME_GRID_COLLS / 3);
	pi.spawnY = GAME_GRID_ROWS / 2;

	players.push_back(Player(pi));
}

void GameLevel::updateInfo(){
	std::wstringstream buff;
	if(FPaused){
		buff << L"Пауза\n\n";
	}

	for(UINT i = 0; i < players.size(); ++i){
		Player& plr = players[i];
		if(players.size() > 1)
			buff << L"Игрок " << (i + 1) << "\n";

		if(plr.getLives() > 0){
			buff << L"Жизней: " << (plr.getLives() - 1); 
		} else {
			buff << L"Мёртв";
		}
		buff << "\n";

		buff << L"Очков: " << plr.getScore() << "\n";
		buff << "\n";
	}
	playerInfo.setString(buff.str());
}

void GameLevel::unpause(){
	if(!FPaused) return;

	FPaused = false;
	for(auto player = players.begin(); player != players.end(); ++player){
		player->unpause();
	}
}

void GameLevel::pause(){
	if(FPaused) return;

	FPaused = true;
	for(auto player = players.begin(); player != players.end(); ++player){
		player->pause();
	}
}

void GameLevel::onEvent(const sf::Event& event){
	switch(event.type){
	case sf::Event::KeyPressed:
		if(event.key.code == sf::Keyboard::P){
			if(FPaused){
				unpause();
			} else {
				pause();
			}
		}
		break;

	case sf::Event::LostFocus:
		pause();
		break;
	}
}

void GameLevel::removeDestroyed(){
	// remove destroyed blocks
	for(auto i = blocks.size(); i > 0; --i){
		GameBlock* block = blocks[i - 1];
		if(block->isDestroyed()){
			delete block;
			blocks.erase(blocks.begin() + (i - 1));
		}
	}
}

void GameLevel::checkPlayersHits(){
	if(players.size() <= 1) return;

	for(UINT i = 0; i < players.size(); ++i){
		Player& current = players[i];
		if(current.isAlive()){
			for(UINT j = 0; j < players.size(); ++j){
				if(i != j){
					Player& other = players[j];
					if(other.isAlive() && other.isHitIn(current))
						other.kill();
				}
			}
		}
	}
}

void GameLevel::checkCollisions(){
	// check collisions with blocks
	for(auto block = blocks.cbegin(); block != blocks.cend(); ++block){
		if(!(*block)->isDestroyed()){
			auto blockPos = (*block)->getPos();
			for(auto player = players.begin(); player != players.end(); ++player){
				if(player->isAlive() && player->isTouch(blockPos)){
					(*block)->onTouch(*player);
				}
			}
		}
	}

	checkPlayersHits();
}

void GameLevel::updateAllObjects(){
	// update blocks
	for(auto block = blocks.cbegin(); block != blocks.cend(); ++block){
		(*block)->Update();
	}
	// update players
	for(auto player = players.begin(); player != players.end(); ++player){
		player->Update();
	}
}

GameBlock* createGoodBonus(int x, int y){
	switch(rand() % 1){
	default: break;
	}
	return new AppleBlock(x, y);
}

GameBlock* createBadBonus(int x, int y){
	switch(rand() % 1){
	default: break;
	}
	return new BombBlock(x, y);
}

void GameLevel::updateBonuses(){
	if(FBonusSpawnTimer.getElapsedTime().asSeconds() < 5.0f)
		return;

	int goodBonuses = Options.MaxGoodBonuses;
	int badBonuses = Options.MaxBadBonuses;
	for(auto block = blocks.begin(); block != blocks.end(); ++block){
		if((*block)->isBonus()){
			if((*block)->isGood()){
				goodBonuses--;
			} else {
				badBonuses--;
			}
		}
	}

	if((goodBonuses <= 0) && (badBonuses <= 0))
		return;

	int x, y;
	do{
		x = rand() % GAME_GRID_COLLS;
		y = rand() % GAME_GRID_ROWS;
	} while (!isFreeSpace(x, y));

	bool spawnGood = goodBonuses > 0;
	if((goodBonuses > 0) && (badBonuses > 0)){
		spawnGood = (rand() % 2) > 0;
	}

	GameBlock* block;
	if(spawnGood){
		block = createGoodBonus(x, y);
	} else {
		block = createBadBonus(x, y);
	}

	addBlock(block);
	FBonusSpawnTimer.restart();
}

bool GameLevel::isFreeSpace(int x, int y){
	for(auto block = blocks.cbegin(); block != blocks.cend(); ++block){
		auto pos = (*block)->getPos();
		if((pos.x == x) && (pos.y == y))
			return(false);
	}

	sf::Vector2i p(x, y);
	for(auto player = players.begin(); player != players.end(); ++player){
		if(player->isAlive())
			if(player->isTouch(p) || player->isTouchBody(p))
				return(false);
	}
	return(true);
}

bool GameLevel::isGameOver(){
	for(auto player = players.begin(); player != players.end(); ++player){
		if(player->isAlive() || (player->getLives() > 0))
			return(false);
	}
	return(true);
}

void GameLevel::appendScores(std::vector<PlayerScore>& res){
	const wchar_t fmt[] = L"Игрок %d";
	for(UINT i = 0; i < players.size(); ++i){
		auto&& plr = players[i];

		PlayerScore tmp;
		swprintf(tmp.Name, 32, fmt, i + 1);
		tmp.Score = players[i].getScore();
		res.push_back(tmp);
	}
}
