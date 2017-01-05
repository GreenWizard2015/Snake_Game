#include "ScoreBoard.h"

void ScoreBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const{
	target.draw(FTitle, states);
	for(auto row = FScores.cbegin(); row != FScores.cend(); ++row){
		target.draw(*row, states);
	}

	target.draw(FNextButton, states);
}

void ScoreBoard::Update(){

}

void ScoreBoard::onEvent(const sf::Event& event){
	switch(event.type){
	case sf::Event::MouseButtonPressed:
		if(event.mouseButton.button == sf::Mouse::Left){
			sf::Vector2f p(
				1.0f * event.mouseButton.x, 1.0f * event.mouseButton.y);

			if(FNextButton.getGlobalBounds().contains(p)){
				FConfirmed = true;
			}

			for(auto row = FScores.begin(); row != FScores.end(); ++row){
				row->onClickEvent(p);
			}
		}
		break;

	case sf::Event::TextEntered:
	case sf::Event::KeyPressed:
		for(auto row = FScores.begin(); row != FScores.end(); ++row){
			row->onKeyEvent(event);
		}
		break;
	}
}

ScoreBoard::ScoreBoard(
	const std::vector<PlayerScore>& scores, 
	const std::wstring Title,
	const bool editable)
	: FNextButton(RESOURCES.getConfirmButton()), 
	FConfirmed(false), FEditable(editable) {
	RESOURCES.setupText(FTitle);
	FTitle.setColor(sf::Color::Red);
	FTitle.setCharacterSize(30);
	FTitle.setString(Title);
	auto TSZ = FTitle.getLocalBounds();
	float x = (640.0f - TSZ.width) / 2.0f;
	FTitle.setPosition(x, 20.0f);
 	float y = TSZ.height + TSZ.top + 35.0f;

	float plrY = y;
	for(auto plr = scores.begin(); plr != scores.end(); ++plr){
		FScores.push_back(ScoreRow(*plr, plrY));
		plrY += FScores[FScores.size() - 1].getHeight();
		if(editable)
			FScores[FScores.size() - 1].editable();
	}

	auto sz = FNextButton.getTexture()->getSize();
	FNextButton.setPosition((640.0f - sz.x) / 2.0f, 480.0f - 20.0f - sz.y);
}

void ScoreBoard::appendScores(std::vector<PlayerScore>& res){
	for(auto row = FScores.begin(); row != FScores.end(); ++row){
		res.push_back(row->getScore());
	}
}

bool ScoreBoard::isConfirmed() const{
	return FConfirmed;
}

bool ScoreBoard::isEditable() const{
	return FEditable;
}
