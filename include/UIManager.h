#include <SFML\Graphics.h>
#include "HomePgae.h"
#include "IngredientsPage.h"
#include "RecipesPage.h"
#include "GroceryListPage.h"
#include "RecipeHistoryPage.h"

#ifndef UI_MANAGER_H
#define UI_MANAGER_H



enum class AppState {
  HOME,
  ADD_INGREDIENTS,
  RECIPES,
  GROCERY_LIST,
  RECIPE_HISTORY
}


class UIManager {
    public:
      UIManager();
      void run();



    private:
      sf::RenderWindow window;
      AppState currentState;

      HomePage homePage;
      IngredientsPage ingredientsPage;
      RecipesPage recipesPage;
      GroceryListPage groceryListPage;
      RecipeHistoryPage recipeHistoryPage;

      void handleEvents();
      void update();
      void render();
      void switchState(AppState newState);

de
};


#endif //UI_MANAGER_H
