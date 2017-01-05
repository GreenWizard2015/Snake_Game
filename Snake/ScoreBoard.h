#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <Windows.h>

#include "ResourceManager.h"
#include "Player.h"
#include "ScoreRow.h"

class ScoreBoard: public sf::Drawable{
public:
	void onEvent(const sf::Event& event);
	void Update();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	ScoreBoard(
		const std::vector<PlayerScore>& scores, 
		const std::wstring Title,
		const bool editable);

	bool isConfirmed() const;
	bool isEditable() const;
	void appendScores(std::vector<PlayerScore>& res);
private:
	bool FEditable;
	bool FConfirmed;
	std::vector<ScoreRow> FScores;

	sf::Text FTitle;
	sf::Sprite FNextButton;
};