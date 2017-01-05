#include "ScoreRow.h"

void ScoreRow::disableEditing(){
	FIsEditing = false;
	if(FData.Name[0] == 0){
		swprintf(FData.Name, 32, L"%s", L"Noname");
		updateText();
	}
	FName.setColor(sf::Color::White);
}

void ScoreRow::enableEditing(){
	FIsEditing = true;
	FData.Name[0] = 0;
	updateText();
	FName.setColor(sf::Color::Red);
}

void ScoreRow::onClickEvent(const sf::Vector2f& p){
	if(!canEdit) return;
	if(FName.getGlobalBounds().contains(p)){
		enableEditing();
	} else {
		disableEditing();
	}
}

void ScoreRow::onKeyEvent(const sf::Event& event){
	if(!FIsEditing) return;

	switch(event.type){
	case sf::Event::TextEntered:
		if(isprint(event.text.unicode)){
			swprintf(FData.Name, 32, L"%s%c", 
				FData.Name, (wchar_t)(event.text.unicode));
			updateText();
		}
		break;

	case sf::Event::KeyPressed:
		switch(event.key.code){
		case sf::Keyboard::Return:
		case sf::Keyboard::Escape:
			disableEditing();
			break;
		}
		break;
	}
}

void ScoreRow::editable(){
	canEdit = true;
}

float ScoreRow::getHeight(){
	return FName.getLocalBounds().height + 15.0f;
}

void ScoreRow::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(FName, states);
	target.draw(FScore, states);
}

ScoreRow::ScoreRow(const PlayerScore& score, float rowY) 
	: FIsEditing(false), canEdit(false), FData(score) {
	RESOURCES.setupText(FName);
	RESOURCES.setupText(FScore);

	FName.setString(score.Name);
	FName.setPosition(30, rowY);

	std::wstringstream tmp;
	tmp << score.Score;
	FScore.setString(tmp.str());
	FScore.setPosition(640 - 30 - FScore.getLocalBounds().width, rowY);
}

PlayerScore ScoreRow::getScore() const {
	return FData;
}

void ScoreRow::updateText(){
	FName.setString(FData.Name);
}
