#include "SimpleBlocks.h"

void WallBlock::onTouch(Player& player) {
	player.kill();
}

WallBlock::WallBlock(int x, int y) : GameBlock(RESOURCES.getWall(), x, y) {

}

void AppleBlock::onTouch(Player& player) {
	player.grow();

	auto T = getLifeTime();
	T = (T > 5.0f) ? 1.0f : (6.0f - T);
	player.addScore((int)(100 * T));

	player.changeSpeedBy(1.1f);
	Destroy();
}

AppleBlock::AppleBlock(int x, int y) : BonusBlock(true, RESOURCES.getApple(), x, y) {

}

void BombBlock::onTouch(Player& player) {
	player.kill();
	Destroy();
}

BombBlock::BombBlock(int x, int y) : BonusBlock(false, RESOURCES.getBomb(), x, y) {
	setLifetime(25);
}
