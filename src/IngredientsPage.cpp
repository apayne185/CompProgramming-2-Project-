#include "IngredientsPage.h"
#include "RecipePage.h"
#include <iostream>

IngredientsPage::IngredientsPage(){
  font.loadFromFile("path.ttf");

  title1.setFont(font); title1.setString("ADD YOUR"); title1.setCharacterSize(28);
  title1.setColor(Color::Black);title1.setPosition(450, 20);
  title2.setFont(font); title2.setString("INGREDIENTS"); title2.setCharacterSize(75);
  title2.setColor(Color::Black); title2.setPosition(250, 60);

  description.setFont(font);
  description.setString("Not sure what to make with everything in your fridge? Food about to go bad? Enter your ingredients and we will find you recipes! Our goal is to help you eliminate food waste! ");
  description.setCharacterSize(9); description.setColor(Color::Black);
  description.setPosition(275, 60);


  //INGREDIENT LIST
  title3.setFont(font); title3.setString("YOUR INGREDIENTS"); title3.setCharacterSize(18);
  title3.setColor(Color::White); title3.setPosition(40, 40);

  listBackground.setSize(sf::Vector2f(300, 600));
  listBackground.setFillColor(sf::Color(214,126,12,255));
  listBackground.setPosition(0,0);


  //INPUT FIELDS & LABELS
  ingredientLabel.setFont(font); ingredientLabel.setString("Ingredient"); ingredientLabel.setCharacterSize(9);
  ingredientLabel.setColor(Color::Grey); ingredientLabel.setPosition(350, 400);
  inputIngredient.setSize(sf::Vector2f(80, 30)); inputIngredient.setPosition(350,280);
  inputIngredient.setFillColor(sf::Color::White);

  inputQuantity.setSize(sf::Vector2f(50, 30)); inputQuantity.setPosition(425,280);
  inputQuantity.setFillColor(sf::Color::White);
  quantityLabel.setFont(font); quantityLabel.setString("Grams");quantityLabel.setCharacterSize(9);
  quantityLabel.setColor(Color::Grey);quantityLabel.setPosition(450, 400);

  inputExpiration.setSize(sf::Vector2f(50, 30)); inputExpiration.setPosition(500,280);
  inputExpiration.setFillColor(sf::Color::White);
  expirationLabel.setFont(font); expirationLabel.setString("Expiration Date (dd/mm/yyyy)"); expirationLabel.setCharacterSize(9);
  expirationLabel.setColor(Color::Grey); expirationLabel.setPosition(525, 400);


  //BUTTONS & LABELS
  addButton.setSize(sf::Vector2f(100, 50)); addButton.setPosition(450, 475);
  addButton.setFillColor(sf::Color::Grey);

  addButtonText.setString("ENTER"); addButtonText.setCharacterSize(9); addButtonText.setColor(Color::White);
  addButtonText.setFont(font); addButtonText.setPosition(475, 485);

  continueButton.setSize(sf::Vector2f(150, 50)); continueButton.setPosition(75, 475);
  continueButton.setFillColor(Color::White);

  continueButtonText.setString("CONTINUE"); continueButtonText.setCharacterSize(11); continueButtonText.setColor(sf::Color(214,126,12,255);
  continueButtonText.setFont(font); continueButtonText.setPosition(100, 485);
}



void IngredientsPage::run(sf::RenderWindow &window){
  while(window.isOpen()){
    sf::Event event;
    while(window.pollEvent(event)){
      if(event.type == sf::Event::Closed){
        window.close();
      }
      if(event.type == sf::Event::MouseButtonPressed){
        if(inputIngredient.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
          ingredientActive = true; quantityActive = false; expirationActive = false;
        } else if (inputQuantity.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
          ingredientActive = false; quantityActive = true; expirationActive = false;
        } else if (inputExpiration.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
          ingredientActive = false; quantityActive = false; expirationActive = true;
        } else if (addButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
          handleAddIngredients();
        } else if (continueButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
          handleContinue();
          return;   //allows us to exit the loop after moving to the next page
        }
      }
      if (event.type == sf::Event::TextEntered) {
        handleTextInput();
      }
    }
    window.clear(sf::Color::White);
    drawUI(window);
    window.display();
  }
}



//concatenates input characters to active field -- we might need validation for int/date fields
void IngredientsPage::handleTextInput(sf::Event event){
  if (ingredientActive){
    inputIngredient.setString(inputIngredient.getString() + event.text.unicode);
  } else if (quantityActive){
    inputQuantity.setString(inputQuantity.getString() + event.text.unicode);
  } else if (expirationActive){
    inputExpiration.setString(inputExpiration.getString() + event.text.unicode);
  }
}



void IngredientPage::drawUI(sf::RenderWindow &window){
  window.draw(title1); window.draw(title2); window.draw(title3);
  window.draw(description);
  window.draw(listBackground);

  window.draw(ingredientLabel); window.draw(quantityLabel); window.draw(expirationLabel);
  window.draw(inputIngredient); window.draw(inputQuantity); window.draw(inputExpiration);

  window.draw(addButton); window.draw(continueButton);
  window.draw(addButtonText); window.draw(continueButtonText);

  //displays list of ingredients
  float yOffset = 150;
  for (const auto &ingredient : ingredientList){
    sf::Text ingredientText;
    ingredientText.setFont(font);
    ingredientText.setString(ingredient.first);
    ingredientText.setCharacterSize(16);
    ingredientText.setPosition(50, yOffset);
    window.draw(ingredientText);
    yOffset += 30;
  }
}


void IngredientsPage::handleAddIngredient(){
  if (!inputIngredient.empty() && !inputQuantity.empty() && !inputExpiration.empty()){
    // once database is integrated, replace the push_back, this is temporary
    std::string ingredient = inputIngredient.getString();
    int quantity = std::stoi(inputQuantity..getString.toAnsiString());
    std::string expiration = inputExpiration.getString().toAnsiString();

    ingredientList.push_back({ingredient, quantity, expiration});
    resetInputFields();
  } else {
    std::cout << "Please fill out all fields before pressing Enter" << std::endl;
  }
}


void IngredientsPage::resetInputFields(){
  inputIngredient = ""; inputQuantity = ""; inputExpiration = "";
  ingredientActive = quantityActive = expirationActive = false;
}

void IngredientsPage::handleContinue(){
  sf::RenderWindow recipeWindow(sf::VideoMode(800, 600), "Recipes");
  RecipePage recipePage;
  recipePage.run(recipeWindow);
}
