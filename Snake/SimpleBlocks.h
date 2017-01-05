#pragma once

#include "GameBlock.h"
#include "Player.h"

class WallBlock: public GameBlock{
public:
	WallBlock(int x, int y);

	void virtual onTouch(Player& player);
};

class AppleBlock: public BonusBlock{
public:
	AppleBlock(int x, int y);

	void virtual onTouch(Player& player);
};

class BombBlock: public BonusBlock{
public:
	BombBlock(int x, int y);

	void virtual onTouch(Player& player);
};