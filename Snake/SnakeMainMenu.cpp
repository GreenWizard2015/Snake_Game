#include "SnakeMainMenu.h"

bool byScore(const PlayerScore& A, const PlayerScore& B) { 
	return A.Score > B.Score;
}

void sortScores(std::vector<PlayerScore>& scores){
	std::sort(scores.begin(), scores.end(), byScore);
}

//////////////////////
const char ScoreFile[] = "scores.dat";

void loadScores(std::vector<PlayerScore>& scores){
	std::ifstream file(ScoreFile, std::ifstream::binary | std::ifstream::in);
	if(file.is_open()){
		while(!file.eof()){
			PlayerScore sc;
			if(file.read((char*)&sc, sizeof(sc)))
				scores.push_back(sc);
		}
	}
}

void saveScores(std::vector<PlayerScore>& scores){
	std::ofstream file(ScoreFile, 
		std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);
	if(file.is_open()){
		sortScores(scores);

		int cnt = scores.size();
		cnt = (cnt > 10) ? 10: cnt;
		for(int i = 0; i < cnt; ++i){
			auto tmp = scores[i];
			file.write((char*)&tmp, sizeof(tmp));
		}
	}
}
///////////////////////
void SnakeMainMenu::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	if(FGameLevel){
		target.draw(*FGameLevel, states);
		return;
	}

	if(FScoreBoard){
		target.draw(*FScoreBoard, states);
		return;
	}

	auto old = states.transform;
	target.draw(FBackground, states);

	for(auto mi = FMenuItems.cbegin(); mi != FMenuItems.cend(); ++mi){
		target.draw(*mi, states);
	}

	states.transform = old;
}

void SnakeMainMenu::Update(){
	if(FGameLevel){
		FGameLevel->Update();
		if(FGameLevel->isGameOver())
			switchToGameOver();
		return;
	}

	if(FScoreBoard){
		FScoreBoard->Update();
		if(FScoreBoard->isConfirmed()){
			if(FScoreBoard->isEditable())
				saveNewScores();

			freeAll();
		}
		return;
	}
}

void SnakeMainMenu::onEvent(const sf::Event& event){
	if(FGameLevel){
		FGameLevel->onEvent(event);
		return;
	}

	if(FScoreBoard){
		FScoreBoard->onEvent(event);
		return;
	}

	switch(event.type){
	case sf::Event::KeyPressed:
		if(event.key.code == sf::Keyboard::Up)
			selectMenu(FCurrentItem - 1);
		if(event.key.code == sf::Keyboard::Down)
			selectMenu(FCurrentItem + 1);

		if(event.key.code == sf::Keyboard::Return)
			activateMenu(FCurrentItem);
		break;

	case sf::Event::MouseButtonPressed:
		if(event.mouseButton.button == sf::Mouse::Left){
			sf::Vector2f p(
				1.0f * event.mouseButton.x, 1.0f * event.mouseButton.y);

			for(UINT i = 0; i < FMenuItems.size(); ++i){
				if(FMenuItems[i].getGlobalBounds().contains(p)){
					activateMenu(i);
					break;
				}
			}
		}

		break;
	}
}

SnakeMainMenu::SnakeMainMenu(): 
	FBackground(RESOURCES.getMainBG()),
	FCloseApp(false),
	FGameLevel(NULL),
	FScoreBoard(NULL) {
	auto sz = FBackground.getTexture()->getSize();
	FBackground.setScale(640.0f / sz.x, 480.0f / sz.y);
	
	std::wstring menuItems[4] = {
		L"Single player",
		L"Multiplayer",
		L"Scoreboard",
		L"Exit"
	};

	float y = 225;
	for(int i = 0; i < 4; ++i){
		FMenuItems.push_back(sf::Text());
		sf::Text& mi = FMenuItems[i];
		mi.setString(menuItems[i]);
		RESOURCES.setupText(mi);
		mi.setPosition(25, y);
		y += mi.getCharacterSize() * 1.5f;
	}

	selectMenu(0);
}

void SnakeMainMenu::selectMenu(int id_){
	id_ = (id_ < -1) ? -1: id_;
	if(id_ >= (int)FMenuItems.size())
		id_ = (int)FMenuItems.size() - 1;

	FCurrentItem = id_;

	sf::Color clr[] = {sf::Color::White, sf::Color::Green};
	for(UINT i = 0; i < FMenuItems.size(); ++i){
		FMenuItems[i].setColor(clr[i == FCurrentItem]);
	}
}

void SnakeMainMenu::activateMenu(int id_){
	switch(id_){
	case 0:
		startSinglePlayer();
		break;
	case 1:
		startMultiPlayer();
		break;

	case 2:
		showHighscoreBoard();
		break;

	case 3: 
		FCloseApp = true;
		break;
	default:
		break;
	}
	selectMenu(-1);
}

LevelOptions makeGameOptions(){
	LevelOptions opt;
	opt.MaxBadBonuses = 1 + (rand() % 5); // 1..6
	opt.MaxGoodBonuses = 3 + (rand() % 3); // 3..6
	return opt;
}

void SnakeMainMenu::startMultiPlayer(){
	freeLevel();
	FGameLevel = new GameLevel(makeGameOptions());
	FGameLevel->startMultiplayer();
}

void SnakeMainMenu::startSinglePlayer(){
	freeLevel();
	FGameLevel = new GameLevel(makeGameOptions());
	FGameLevel->startSinglePlayer();
}

bool SnakeMainMenu::isCloseApp() const{
	return FCloseApp;
}

SnakeMainMenu::~SnakeMainMenu(){
	freeAll();
}

void SnakeMainMenu::switchToGameOver(){
	std::vector<PlayerScore> scores;
	FGameLevel->appendScores(scores);
	sortScores(scores);
	freeAll();

	FScoreBoard = new ScoreBoard(scores, L"Конец игры", true);
}

void SnakeMainMenu::saveNewScores(){
	std::vector<PlayerScore> scores;
	loadScores(scores);
	FScoreBoard->appendScores(scores);
	saveScores(scores);
}

void SnakeMainMenu::showHighscoreBoard(){
	freeAll();
	std::vector<PlayerScore> scores;
	loadScores(scores);
	FScoreBoard = new ScoreBoard(scores, L"Рекорды", false);
}

void SnakeMainMenu::freeAll(){
	freeBoard();
	freeLevel();
}

void SnakeMainMenu::freeBoard(){
	if(FScoreBoard){
		delete FScoreBoard;
		FScoreBoard = NULL;
	}
}

void SnakeMainMenu::freeLevel(){
	if(FGameLevel){
		delete FGameLevel;
		FGameLevel = NULL;
	}
}
