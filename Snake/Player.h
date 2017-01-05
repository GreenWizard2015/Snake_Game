#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "GridCell.h"
#include "ResourceManager.h"

struct PlayerScore {
	wchar_t Name[32];
	int Score;
};

struct PlayerInfo{
	int spawnX, spawnY;
	float Speed;
	int spawnLenght;

	sf::Keyboard::Key UP, DOWN, RIGHT, LEFT;
	sf::Color Color;
};

typedef struct{
	int X;
	int Y;
} TPoint;

typedef enum MoveDirection{
	UP = 0,
	DOWN = 1,
	RIGHT = 2,
	LEFT = 3,
	NONE = 4
};

class PlayerBody: public SpriteCell{
public:
	typedef enum PartType{
		HEAD, BODY, TURN_LEFT, TURN_RIGHT, TAIL
	};

	PlayerBody(PartType type, int x, int y, int rotation);
	void setType(PartType type);
	PartType getType() const { return partType; }

	void setRotate(int degs){
		getSprite().setRotation(degs * 1.0f);
	}

	int getRotate(){ return (int)(getSprite().getRotation()); }

	void setTurn(MoveDirection old, MoveDirection newD);
private:
	PartType partType;
};

class Player: public sf::Drawable {
public:
	Player(const PlayerInfo& Options);

	void pause();
	void unpause();

	bool isAlive() const;
	int getLives() const;
	int getScore() const;

	void addScore(int score);
	void changeSpeedBy(float delta);

	void kill();
	void grow();
	void shrink();

	bool isHitIn(Player& other);
	bool isTouch(sf::Vector2i p);
	bool isTouchBody(sf::Vector2i p);
	void Update();
private:
	bool FPaused;
	int FLives;
	int FScore;
	float FSpeed;

	PlayerInfo Options;
	int FDelayedPart;
	std::vector<PlayerBody> FBody;

	bool       FShowSpawn;
	SpriteCell FSpawn;

	MoveDirection FLastDirection;
	MoveDirection FNewDirection;
	sf::Clock FMoveTimer;

	void CheckKeys();

	PlayerBody& getHead(int i);
	PlayerBody& getHead();
	PlayerBody& getTail();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void spawn();
	void addPart(PlayerBody::PartType partType, int x, int y, int rot);

	void moveForward();
};