#ifndef RECIPEMANAGER_H
#define RECIPEMANAGER_H

#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include "Fridge.h"
#include "Pantry.h"
#include "Recipe.h"
#include "Ingredient.h"

class RecipeManager {
private:
    std::string dbName;              // Database file name
    Fridge fridge;                   // Fridge object
    Pantry pantry;                   // Pantry object
    std::vector<Recipe> recipes;     // List of recipes

    void loadRecipesFromDatabase();          // Load recipes from the database
    void loadIngredientsFromDatabase();      // Load ingredients from the database
    void saveHistory(const Recipe& recipe);  // Save recipe history to the database
    void displayFullRecipe(const Recipe& recipe);  // Display full recipe details

public:
    // Constructor
    RecipeManager(const std::string& dbName);

    // Member functions
    void collectIngredients();                             // Collect ingredients from user input
    void generateGroceryList(const std::vector<std::string>& missingIngredients);  // Generate a grocery list
    void matchRecipes();                                   // Match available ingredients to recipes
    void viewRecipeHistory();                              // View saved recipe history
    void clearGroceryList();                               // Clear the grocery list
    void printGroceryList();                               // Print the grocery list
    void checkNotifications();                             // Check for expiring/low ingredients
    void fetchAndSaveRecipeDetails(const std::string& recipeName);  // Fetch and save recipe details via API
    void menu();                                           // Display the menu for user interaction
};

#endif
