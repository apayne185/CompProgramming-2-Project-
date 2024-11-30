#include <string>
#include "../include/json.hpp"
#include "../include/RecipeManager.h"
#include "../include/UIManager.h"

using json = nlohmann::json;

int main() {
    std::string recipeFilename = "../data/recipes.json";
    RecipeManager manager(recipeFilename);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Undecided");
    UIManager uiManager(window, manager);

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          window.close();
        }
        uiManager.handleEvent(event);
      }
      uiManager.update();
      window.clear(sf::Color::White);
      uiManager.render();
      window.display();
    }

    manager.menu();

    return 0;
}
