#include "GameBlock.h"

bool GameBlock::isGood() const{
	return false;
}

bool GameBlock::isBonus() const{
	return false;
}

void GameBlock::Update(){}

bool GameBlock::isDestroyed() const{
	return FDestroyed;
}

void GameBlock::Destroy(){
	FDestroyed = true;
}

GameBlock::GameBlock(const sf::Texture& tex, int x, int y) : 
	SpriteCell(tex, x, y), FDestroyed(false){
}
/////////////////////////////////////
void BonusBlock::setLifetime(float LT){
	FMaxLifetime = LT;
}

float BonusBlock::getLifeTime() const{
	return (GetTickCount() - FSpawnTime) * 0.001f;
}

void BonusBlock::Update(){
	if(FMaxLifetime <= 0) return;
	auto dT = FMaxLifetime - getLifeTime();

	if(dT > 0){
		if(dT < 2.0f){
			const int alphaArr[] = {255, 128};
			auto alpha = alphaArr[((int)(dT * 5)) % 2];

			auto clr = getSprite().getColor();
			if(alpha != clr.a){
				clr.a = alpha;
				getSprite().setColor(clr);
			}
		}
	} else {
		Destroy();
	}
}

bool BonusBlock::isBonus() const{
	return true;
}

bool BonusBlock::isGood() const{
	return FIsGood;
}

BonusBlock::BonusBlock(bool isGood_, const sf::Texture& tex, int x, int y) :
	GameBlock(tex, x, y), 
	FIsGood(isGood_), 
	FMaxLifetime(10.0f), 
	FSpawnTime(GetTickCount()) {}
