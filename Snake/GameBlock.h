#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "Player.h"
#include "GridCell.h"

class GameBlock: public SpriteCell {
public:
	GameBlock(const sf::Texture& tex, int x, int y);

	void virtual onTouch(Player& player) = 0;

	void Destroy();
	bool isDestroyed() const;

	virtual void Update();
	virtual bool isBonus() const;
	virtual bool isGood() const;
private:
	bool FDestroyed;
};


class BonusBlock: public GameBlock{
public:
	BonusBlock(bool isGood_, const sf::Texture& tex, int x, int y);
	virtual bool isGood() const;
	virtual bool isBonus() const;
	virtual void Update();
protected:
	float getLifeTime() const;

	void setLifetime(float LT);
private:
	UINT FSpawnTime;
	bool FIsGood;
	float FMaxLifetime;
};
