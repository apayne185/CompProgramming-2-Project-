#include "../include/UIManager.h"
#include "../include/RecipeManager.h"

#include <SFML/Graphics.h>
#include <iostream>



/*
Visualiser::Visualiser(Board &board, int bSize, int cSize, int maxValue, sf::Color bCol, sf::Color cCol) :
        window(sf::RenderWindow(sf::VideoMode(bSize * cSize, bSize * cSize), "Cellular automaton visualiser")),
        board(board),
        boardSize(bSize),
        cellSize(cSize),
        maxCellValue(maxValue),
        backgroundColor(bCol),
        cellColor(cCol) {
    cout << "Creating Window" << endl;
}
*/

//INITIALIZES SFML WINDOW, SETS DEFAULT TO HOMEPAGE
UIManager::UIManager() {
  /*font.loadFromFile("font.ttf");      //path/to/file.ttf
  setupHomePage();*/
  : window(sf::VideoMode(800,600), "Undecided"),
  currentState(AppState:HOME) {
    window.setFramerateLimit(60);
  }
}

//CONTINULLY CALLS WHILE WINDOW IS OPEN
void UIManager::run() {
  while (window.isOpen()) {
    handleEvents();
    update();
    render();
  }
}


//DEPENDING ON CURRENTSTATE, HANDLES EVENTS LIKE BUTTON CLICKS
void UIManager::handleEvents() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
    }
    if (currentState == AppState::HOME) {
      if (event.type == sf::Event::MouseButtonPressed) {
        if (homePage.handleStartButtonClick(sf::Mouse::getPosition(window))) {
          switchToState(AppState:: ADD_INGREDIENTS);
        }
      }
    } else if (currentState == AppState::ADD_INGREDIENTS) {
      if (event.type == sf::Event::MouseButtonPressed) {
        addIngredientPage.handleAddIngredientsClick(sf::Mouse::getPosition(window));
      }
    } else if (currentState == AppState::RECIPES) {
      if (event.type == sf::Event::MouseButtonPressed) {
        recipesPage.handleRecipesClick(sf::Mouse::getPosition(window));
  }
}


//UPDATES THE PAGE -- REFRESHES AFTER ADDING INFORMATION
void UIManager::update() {
  if (currentState == AppState::HOME) {
     homePage.update();
  } else if (currentState == AppState::ADD_INGREDIENTS) {
      addIngredientPage.update();
  } else if (currentState == AppState::RECIPES) {
      recipesPage.update();
  } else if (currentState == AppState::GROCERY_LIST) {
    groceryListPage.update();
  } else if (currentState == AppState::RECIPE_HISTORY) {
    recipeHistoryPage.update();
  }
}

//CLEARS WINDOW, DRAWS NEW PAGE
void UIManager::render() {
  window.clear();
  if (currentState == AppState::HOME) {
    homePage.draw(window);
  } else if (currentState == AppState::ADD_INGREDIENTS) {
    addIngredientPage.draw(window);
  } else if (currentState == AppState::RECIPES) {
    recipesPage.draw(window);
  } else if (currentState == AppState::GROCERY_LIST) {
    groceryListPage.draw(window);
  } else if (currentState == AppState::RECIPE_HISTORY) {
    recipeHistoryPage.draw(window);
  }
  window.display();
}


//ALLOWS FOR UI NAVIGATION
void UIManager::switchState(AppState newState) {
  currentState = newState;
}



