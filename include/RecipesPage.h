#ifndef RECIPESPAGE_H
#define RECIPESPAGE_H

#include "Recipe.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <iostream>


class RecipesPage {
  public:
    RecipesPage(sf::RenderWindow &window) : window(window) {
        font.loadFromFile("font.ttf");  //need to improt still
        createRecipeList();
    };

    void createRecipeList(){
      //include call to backend for data
      //recipeList.push_back(Recipe("Spaghetti", "delicious pasta dish", 30, "spaghetti.jpg", {"Pasta", "Tomato Sauce", "Cheese"}, {"Boil pasta", "Add sauce", "Serve"}, "300 cal"));

      float yPos = 150;
      for(size_t i = 0; i < recipeList.size(); i++){
        sf::RectangleShape recipeBox(sf::Vector2f(300,100));
        recipeBox.setPosition(50,yPos);
        recipeBox.setFillColor(sf::Color::White);
        recipeButton.push_back(recipeBox);

        sf::Text recipeText(recipeList[i].name, font, 20);
        recipeText.setPosition(60, yPos+10);
        recipeText.push_back(recipeText);

        yPos += 120;   //will need to adjust for a scrollable list
      }
    }

    void showRecipeModal(const Recipe& recipe){
      isModalOpen = true;
      modalBackground.setSize(sf::Vector2f(600, 400));
      modalBackground.setFillColor(sf::Color::White);

      modalTitle.setString(recipe.name);
      modalTitle.setFont(font);
      modalTitle.setCharacterSize(40);
      modalTitle.setPosition(50, 50);

      modalDescription.setString(recipe.description);
      modalDescription.setFont(font);
      modalDescription.setCharacterSize(11);
      modalDescription.setPosition(50, 100);

      modalTime.setString("Cook Time: " + std::to_string(recipe.cookTime)+ " min");
      modalTime.setFont(font);
      modalTime.setCharacterSize(18);
      modalTime.setPosition(550, 100);

      sf::Texture recipeImage;
      //load image from backend
      recipeImage.setPosition(100, 200);


      modalIngredient.setString("INGREDIENTS");
      modalIngredient.setFont(font);
      modalIngredient.setCharacterSize(28);
      modalIngredient.setPosition(250, 250);
      //need to add the ingredients in a list dynamically after getting them from the backend
      float yPos = 250;
      for (ingredient in recipe.ingredients){
        modalIngredientList.setString(ingredient);
        modalIngredientList.setFont(font);
        modalIngredientList.setCharacterSize(12);
        modalIngredientList.setPosition(250, yPos+18);
      }

      //i need to dynamically add this section position based on how many ingredients have been added
      modalSteps.setString("STEPS");
      modalSteps.setFont(font);
      modalSteps.setCharacterSize(28);
      modalSteps.setPosition(250, yPos+40);

      for (step in recipe.steps){
        modalStepList.setString(step);
        modalStepList.setFont(font);
        modalStepList.setCharacterSize(12);
        modalStepList.setPosition(250, yPos+18);
      }
      //i need to dynamically add this sections position based on how many steps
      modalNutrition.setString("NUTRITION");
      modalNutrition.setFont(font);
      modalNutrition.setCharacterSize(28);
      modalNutrition.setPosition(250, yPos+40);

      modalNutritionInfo.setString(recipe.nutrition);
      modalNutritionInfo.setFont(font);
      modalNutritionInfo.setCharacterSize(12);
      modalNutritionInfo.setPosition(250, yPos+18);


      window.draw(modalBackground);
      window.draw(modalTitle);
      window.draw(modalDescription);
      window.draw(modalTime);
      window.draw(recipeImage);
      window.draw(modalIngredients);
      window.draw(modalIngredientList);    //did i set up ingredientlist correctly to be drawn?
      window.draw(modalSteps);
      window.draw(modalStepList);
      window.draw(modalNutrition);
      window.draw(modalNutritionInfo);

      //NEED THE MAKE RECIPE BUTTON

    }

    void recipeHistoryModal(){
      isModalOpen = true;
      modalBackground.setSize(sf::Vector2f(600, 400));
      modalBackground.setFillColor(sf::Color::White);

      modalTitle.setString("RECIPE HISTORY");
      modalTitle.setCharacterSize(40);
      modalTitle.setFont(font);
      modalTitle.setPosition(50, 50);

      //need to call backend for recipe history data
      float yPos = 150;
      for(const auto& item : recipeHistory){
        sf::RectangleShape recipeBox(sf::Vector2f(300,100));
        historyBox.setPosition(50,yPos);
        historyBox.setFillColor(sf::Color::White);
        historyButton.push_back(historyBox);

        sf::Text historyText(item.first+ " - " + item.second, font, 20);
        historyText.setPosition(60, yPos+10);
        historyText.push_back(recipeHistoryText);  //remove this once we have workign be

        yPos += 120;   //will need to adjust for a scrollable list
      }
    }

    void GroceryListModal (){
      isModalOpen = true;
      modalBackground.setSize(sf::Vector2f(600, 400));
      modalBackground.setFillColor(sf::Color(214,126,12,255));

      modalTitle.setString("GROCERY LIST");
      modalTitle.setFont(font);
      modalTitle.setCharacterSize(40);
      modalTitle.setPosition(50, 50);
      modalTitle.setColor(sf::Color::White);

    }




    void run();
    void loadRecipes();
    void update();

   private:
     void handleEvents();
     void render();
     void createButtons();
     void createRecipeList();
     void showRecipeModal();


};



#endif //RECIPESPAGE_H
