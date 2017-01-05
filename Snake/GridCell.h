#pragma once
#include <SFML/Graphics.hpp>

class GridCell{
public:
	void setPos(int X, int Y){
		this->X = X;
		this->Y = Y;
		updatePos();
	}

	sf::Vector2i getPos() const { return sf::Vector2i(X, Y); }
	int getX() const { return X; }
	int getY() const { return Y; }
protected:
	void virtual setRealPos(float X, float Y) = 0;
private:
	int X, Y;

	void updatePos(){
		setRealPos(X * 32.0f, Y * 32.0f);
	}
};

class SpriteCell: public sf::Drawable, public GridCell{
public:
	SpriteCell(const sf::Texture& tex, int X, int Y): FSprite(tex){
		setPos(X, Y);
		FSprite.setScale(0.5f, 0.5f);
		FSprite.setOrigin(32.0f,  32.0f);
	}

	SpriteCell(int X, int Y): FSprite() {
		setPos(X, Y);
		FSprite.setScale(0.5f, 0.5f);
		FSprite.setOrigin(32.0f,  32.0f);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(FSprite, states);
	}

	sf::Sprite& getSprite() { return FSprite; }
protected:
	void virtual setRealPos(float X, float Y){
		FSprite.setPosition(X - 16.0f, Y - 16.0f);
	}
private:
	sf::Sprite FSprite;
};