#include "HomePage.h"

HomePage::HomePage(sf::RenderWindow *window) : window(window) {
  if (!font.loadFromFile("path/to.ttf")) {
    std::cerr << "Failed to load font." << std::endl;
  }
  initialize();
}

void HomePage::initialize() {
  title1.setFont(font);
  title1.setString("WELCOME TO");
  title1.setCharacterSize(18);
  title1.setColor(sf::Color::Black);     //find right color
  title1.setPosition(100,200);

  title2.setFont(font);
  title2.setString("UNDECIDED");
  title2.setCharacterSize(80);
  title2.setColor(sf::Color::Black);
  title2.setPosition(100,300);

  //need to research how to add paragraphs/alignment
  description.setFont(font);
  description.setString("HERE TO ELIMINATE YOUR FOOD WASTE");
  description.setString("THROUGH AMAZING RECIPES!");
  description.setString("ENTER WHAT’S ALREADY IN YOUR FRIDGE.");


  continueButton.setSize(stf::Vector2f(150,50));
  continueButton.setFillColor(sf::Color(214,126,12,255));   //orange
  continueButton.setPosition(400,500);

  continueButtonText.setFont(font);
  continueButtonText.setString("LET'S GET STARTED!");
  continueButtonText.setCharacterSize(14);
  continueButtonText.setFillColor(sf::Color::White);
  continueButtonText.setPosition(330, 310);
}

void HomePage::display(){
  window->clear(sf::Color::White);
  window->draw(title1);
  window->draw(title2);
  window->draw(description);
  window->draw(continueButton);
  window->draw(continueButtonText);

  window->display();

}


bool HomePage::isContinueButtonClicked(sf::Vector2f mousePos) {
  return continueButton.getGlobalBounds().contains(mousePos);
}