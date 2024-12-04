#include "../include/Pantry.h"

void Pantry::addIngredient(const Ingredient& ingredient) {
    try {
        Storage::addIngredient(ingredient);
        std::cout << ingredient.getName() << " added to Pantry.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error adding ingredient to pantry: " << e.what() << std::endl;
    }
}

void Pantry::runningLow() const {
    try {
        for (const auto& ingredient : ingredients) {
            if (ingredient.getQuantity() < 2) {
                std::cout << ingredient.getName() << " is running low.\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error processing runningLow: " << e.what() << std::endl;
    }
}