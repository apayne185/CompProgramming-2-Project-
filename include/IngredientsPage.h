#include <SFML/Graphics.h>
#include <string>
#include <vector>

#ifndef INGREDIENTSPAGE_H
#define INGREDIENTSPAGE_H



class IngredientsPage {
    public:
      IngredientsPage();
      void run(sf::RenderWindow &window);

    private:
      sf::Font font;
      sf::Text title1, title2, title3, description;
      sf::Text ingredientLabel, quantityLabel, expirationLabel;

      sf::RectangleShape ingredientInputBox, quantityInputBox, expirationInputBox;
      sf::RectangleShape addButton, continueButton, listBackground;
      sf::Text addButtonText, continueButtonText;

      std::vector<std::pair<std::string,int>> ingredientsList;
      std::string inputIngredient, inputQuantity, inputExpiration;
      bool ingredientActive = false, quantityActive = false, expirationActive = false;

      void drawUI(sf::RenderWindow& window);
      void handleAddIngredient();
      void handleContinue();
      void handleTextInput(sf::Event event);
      void resetInputFields();

};


#endif //INGREDIENTSPAGE_H
