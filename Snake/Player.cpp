#include "Player.h"

const MoveDirection ReverseMove[] = {DOWN, UP, LEFT, RIGHT}; 

const TPoint Deltas[] = { {0, -1}, {0, 1}, {1, 0}, {-1, 0}
};
const int Rotations[] = {0, 180, 90, 270};
//////////////////////////////////////////////
PlayerBody::PlayerBody(PartType type, int x, int y, int rotation): SpriteCell(x, y) {
	setType(type);
	setRotate(rotation);
}

sf::Texture& getTextureFor(PlayerBody::PartType type){
	switch(type){
		case PlayerBody::HEAD: return RESOURCES.getHead();
		case PlayerBody::BODY: return RESOURCES.getBody();
		case PlayerBody::TAIL: return RESOURCES.getTail();
		case PlayerBody::TURN_LEFT: return RESOURCES.getTurnLeft();
		case PlayerBody::TURN_RIGHT: return RESOURCES.getTurnRight();
	}
	return RESOURCES.getBody();
}

void PlayerBody::setType(PartType type){
	partType = type;
	getSprite().setTexture(getTextureFor(type));
}

void PlayerBody::setTurn(MoveDirection old, MoveDirection newD) {
	static const PartType turnType[4][4] = { {BODY, BODY, TURN_LEFT, TURN_RIGHT}, // UP {BODY, BODY, TURN_RIGHT, TURN_LEFT}, // DOWN {TURN_RIGHT, TURN_LEFT, BODY, BODY}, // LEFT {TURN_LEFT, TURN_RIGHT, BODY, BODY} // RIGHT
	};

	setType(turnType[newD][old]);
}
////////////////////////////////////////////
void Player::spawn(){
	FBody.clear();
	FDelayedPart = Options.spawnLenght - 1;
	addPart(PlayerBody::HEAD, Options.spawnX, Options.spawnY, 0);

	FSpeed = Options.Speed;
	FLastDirection = UP;
	FNewDirection = NONE;
	FShowSpawn = true;
	FMoveTimer.restart();
}

void Player::CheckKeys(){
	MoveDirection newDirection = NONE;
	if(sf::Keyboard::isKeyPressed(Options.UP))
		newDirection = UP;
	if(sf::Keyboard::isKeyPressed(Options.DOWN))
		newDirection = DOWN;
	if(sf::Keyboard::isKeyPressed(Options.LEFT))
		newDirection = LEFT;
	if(sf::Keyboard::isKeyPressed(Options.RIGHT))
		newDirection = RIGHT;

	if((NONE != newDirection) && (newDirection != ReverseMove[FLastDirection])){
		FNewDirection = newDirection;
	}
}

void Player::moveForward(){
	// head pos
	auto pos = getHead().getPos();
	// remove tail
	if(FDelayedPart <= 0){
		FBody.erase(FBody.begin());
	} else {
		FDelayedPart--;
	}

	// move head
	pos.x += Deltas[FNewDirection].X;
	pos.y += Deltas[FNewDirection].Y;

	auto rot = Rotations[FNewDirection];
	addPart(PlayerBody::HEAD, pos.x, pos.y, rot);

	if(FBody.size() > 2)
		getHead(2).setTurn(FLastDirection, FNewDirection);
	// tail
	FBody[0].setType(PlayerBody::TAIL);
	FBody[0].setRotate(FBody[1].getRotate());
	
	getHead().setType(PlayerBody::HEAD);
	FLastDirection = FNewDirection;

	if(isTouchBody(getHead().getPos()))
		kill();
}

void Player::Update(){
	if(FPaused) return;
	if(!isAlive()) return;
	CheckKeys();

	if(FMoveTimer.getElapsedTime().asSeconds() > FSpeed){
		FMoveTimer.restart();
		// move body
		FNewDirection = (FNewDirection == NONE) ? FLastDirection : FNewDirection;
		moveForward();
		FNewDirection = NONE;
	}

	if(FShowSpawn && isAlive()){
		auto tail = FBody[0].getPos(); 
		FShowSpawn = (tail.x == Options.spawnX) && (tail.y == Options.spawnY);
	}
}

void Player::addPart(PlayerBody::PartType partType, int x, int y, int rot){
	FBody.push_back(PlayerBody(partType, x, y, rot));
	getHead().getSprite().setColor(Options.Color);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	if(!isAlive()) return;
	if(FShowSpawn){
		target.draw(FSpawn, states);
	}

	for(auto body = FBody.cbegin(); body != FBody.cend(); ++body)
		target.draw(*body, states);
}

PlayerBody& Player::getHead() {
	return FBody[FBody.size() - 1];
}

PlayerBody& Player::getHead(int i){
	return FBody[FBody.size() - i];
}

Player::Player(const PlayerInfo& Options) : Options(Options), 
FSpawn(RESOURCES.getSpawn(), Options.spawnX, Options.spawnY) {
	FScore = 0;
	FLives = 3;
	unpause();
	spawn();
}

bool Player::isTouch(sf::Vector2i p){
	auto head = getHead().getPos();
	return (p.x == head.x) && (p.y == head.y);
}

bool Player::isTouchBody(sf::Vector2i p){
	for(auto i = 0; i < (int)FBody.size() - 1; ++i){
		auto pos = FBody[i].getPos();
		if((p.x == pos.x) && (p.y == pos.y))
			return true;
	}
	return false;
}

void Player::grow(){
	FDelayedPart++;
}

void Player::kill(){
	FLives--;
	FDelayedPart = 0;
	FBody.clear();

	if(FLives > 0)
		spawn();
}

PlayerBody& Player::getTail(){
	return FBody[0];
}

void Player::shrink(){
	if(FDelayedPart > 0){
		FDelayedPart--;
	} else {
		if(FBody.size() > 1){
			FBody.erase(FBody.begin());
		} else {
			kill();
		}
	}
}

void Player::changeSpeedBy(float delta){
	FSpeed /= delta;
	if(FSpeed < 0.1f) FSpeed = 0.1f;
	if(FSpeed > 2.0f) FSpeed = 2.0f;
}

void Player::addScore(int score){
	FScore += score;
}

int Player::getScore() const{
	return FScore;
}

int Player::getLives() const{
	return FLives;
}

bool Player::isAlive() const{
	return (FDelayedPart + FBody.size()) > 0;
}

void Player::unpause(){
	FPaused = false;
	FMoveTimer.restart();
}

void Player::pause(){
	FPaused = true;
}

bool Player::isHitIn(Player& other){
	auto head = getHead().getPos();
	return other.isTouch(head) || other.isTouchBody(head);
}
