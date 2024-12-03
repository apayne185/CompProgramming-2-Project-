#include "../include/Pantry.h"
#include <iostream>
#include <stdexcept>

void Pantry::addIngredient(const Ingredient& ingredient) {
    for (auto& existing : ingredients) {
      if (existing.getName() == ingredient.getName()) {
        existing.setQuantity(existing.getQuantity() + ingredient.getQuantity());
        std::cout << "Updated " << existing.getName() <<" quantity to " << existing.getQuantity();
        return;
      }
    }
    try {
        Storage::addIngredient(ingredient);
        std::cout << ingredient.getName() << " added to Pantry.\n";
    } catch (const std::exception& e) {
      std::cerr << "Failed to add ingredient: " << e.what() << " to Pantry \n";
    }
}

void Pantry::runningLow() const {
    if (ingredients.empty()) {
      std::cout << "Pantry is empty.\n";
      return;
    }

    bool lowQuantity = false;

    for (const auto& ingredient : ingredients) {
        if (ingredient.getQuantity() < 2) {
            std::cout << ingredient.getName() << " is running low.\n";
            lowQuantity = true;
        }
    }
    if (!lowQuantity) {
      std::cout << "All ingredients are sufficienly full.";
    }
}
