#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <Windows.h>
#include <ctype.h>

#include "ResourceManager.h"
#include "Player.h"

class ScoreRow: public sf::Drawable {
public:
	ScoreRow(const PlayerScore& score, float rowY);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	float getHeight();

	void editable();
	void onKeyEvent(const sf::Event& event);
	void onClickEvent(const sf::Vector2f& p);

	PlayerScore getScore() const;
protected:
	void updateText();
	void enableEditing();
	void disableEditing();
private:
	PlayerScore FData;

	bool FIsEditing;
	bool canEdit;
	sf::Text FName;
	sf::Text FScore;
};
