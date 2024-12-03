#include "Storage.h"
#include <iostream>

void Storage::addIngredient(const Ingredient& ingredient) {
      if (ingredient.getName().empty()) {
        std::cerr << "Ingredients can't be empty" << std::endl;
        return;
      } else if (ingredient.getQuantity() <= 0) {
        std::cerr << "Ingredients Quantity can't be empty" << std::endl;
        return;
      }

    for (auto& ing : ingredients) {
        if (ing.getName() == ingredient.getName()) {
            ing.setQuantity(ing.getQuantity() + ingredient.getQuantity());
            if (!ingredient.getExpirationDate().empty()) {
                ing.setExpirationDate(ingredient.getExpirationDate());
            }
            return;
        }
    }
    ingredients.push_back(ingredient);
}

const std::vector<Ingredient>& Storage::getIngredients() const {
    return ingredients;
}

json Storage::toJSON() const {
    json ingredientArray = json::array();
    for (const auto& ingredient : ingredients) {
        if(ingredient.getName().empty()) {
          std::cerr << "Ingredients can't be empty" << std::endl;
          continue;
        }
        ingredientArray.push_back(ingredient.toJSON());
    }
    return ingredientArray;
}

void Storage::fromJSON(const json& j) {
    try {
      if (!j.is_array()) {
        throw std::invalid_argument("Ingredients can't be empty");
      }
    for (const auto& item : j) {
      if (!item.contains("name") || !item["name"].is_string()) {
            throw std::invalid_argument("Ingredients is missing name or is not a string.");
            }
        ingredients.push_back(Ingredient::fromJSON(item));
        }
    } catch (std::invalid_argument& e) {
      std::cerr << "JSON Parsing Error - Storage" << e.what() << std::endl;
    }
}
