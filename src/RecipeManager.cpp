#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <json.hpp>
#include <curl/curl.h>
#include "RecipeManager.h"

// Helper function to join a vector of strings
std::string join(const std::vector<std::string>& vec, const std::string& delimiter) {
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i != vec.size() - 1) oss << delimiter;
    }
    return oss.str();
}

RecipeManager::RecipeManager(const std::string& dbName) {
    this->dbName = dbName;  // Store the database name
    loadIngredientsFromDatabase();  // Load ingredients from the database
    loadRecipesFromDatabase();  // Load recipes from the database
}

void RecipeManager::loadRecipesFromDatabase() {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = "SELECT Name, Category, Ingredients, Condiments, Steps FROM Recipes;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Fetch recipe data
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string category = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string ingredientsStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            std::string condimentsStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            std::string stepsStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

            // Parse ingredients into vector of pairs
            std::vector<std::pair<std::string, std::string>> ingredients;
            size_t pos = 0;
            while ((pos = ingredientsStr.find(";")) != std::string::npos) {
                std::string ingredientPair = ingredientsStr.substr(0, pos);
                size_t separator = ingredientPair.find(":");
                if (separator != std::string::npos) {
                    std::string ingredientName = ingredientPair.substr(0, separator);
                    std::string ingredientQuantity = ingredientPair.substr(separator + 1);
                    ingredients.emplace_back(ingredientName, ingredientQuantity);
                }
                ingredientsStr.erase(0, pos + 1);
            }

            // Parse condiments into vector of pairs
            std::vector<std::pair<std::string, std::string>> condiments;
            pos = 0;
            while ((pos = condimentsStr.find(";")) != std::string::npos) {
                std::string condimentPair = condimentsStr.substr(0, pos);
                size_t separator = condimentPair.find(":");
                if (separator != std::string::npos) {
                    std::string condimentName = condimentPair.substr(0, separator);
                    std::string condimentQuantity = condimentPair.substr(separator + 1);
                    condiments.emplace_back(condimentName, condimentQuantity);
                }
                condimentsStr.erase(0, pos + 1);
            }

            // Parse steps into vector
            std::vector<std::string> steps;
            pos = 0;
            while ((pos = stepsStr.find(";")) != std::string::npos) {
                steps.push_back(stepsStr.substr(0, pos));
                stepsStr.erase(0, pos + 1);
            }

            // Construct Recipe object
            Recipe recipe(name, ingredients, condiments, steps, category);
            recipes.push_back(recipe);
        }
    } else {
        std::cerr << "Failed to load recipes: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}



void RecipeManager::loadIngredientsFromDatabase() {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = "SELECT Type, Name, Quantity, ExpirationDate FROM Storage;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int quantity = sqlite3_column_int(stmt, 2);
            std::string expirationDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

            Ingredient ingredient(name, quantity, expirationDate);
            if (type == "Fridge") {
                fridge.addIngredient(ingredient);
            } else if (type == "Pantry") {
                pantry.addIngredient(ingredient);
            }
        }
    } else {
        std::cerr << "Failed to load ingredients: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void RecipeManager::saveHistory(const Recipe& recipe) {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = "INSERT INTO History (Name, Date) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, recipe.getRecipeName().c_str(), -1, SQLITE_STATIC);

        // Add the current date
        time_t now = time(0);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&now));
        sqlite3_bind_text(stmt, 2, buffer, -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Failed to save history: " << sqlite3_errmsg(db) << std::endl;
        }
    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


void RecipeManager::collectIngredients() {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    while (true) {
        std::string name;
        int quantity;
        std::cout << "Please enter the ingredient name (or type 'done' to finish): ";
        std::cin.ignore();
        std::getline(std::cin, name);
        if (name == "done") break;

        std::cout << "Enter the quantity of " << name << ": ";
        std::cin >> quantity;

        std::string storageLocation;
        std::cout << "Is the ingredient stored in the (F)ridge or (P)antry? ";
        std::cin >> storageLocation;

        std::string expirationDate;
        if (storageLocation == "F" || storageLocation == "f") {
            std::cout << "Enter the expiration date (YYYY-MM-DD): ";
            std::cin >> expirationDate;
        }

        // Insert ingredient into database
        std::string sql = "INSERT INTO Storage (Type, Name, Quantity, ExpirationDate) VALUES (?, ?, ?, ?);";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, storageLocation == "F" || storageLocation == "f" ? "Fridge" : "Pantry", -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 3, quantity);
            sqlite3_bind_text(stmt, 4, expirationDate.empty() ? nullptr : expirationDate.c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                std::cerr << "Failed to add ingredient: " << sqlite3_errmsg(db) << std::endl;
            }
        } else {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
}


void RecipeManager::generateGroceryList(const std::vector<std::string>& missingIngredients) {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    for (const auto& ingredient : missingIngredients) {
        std::string sql = "INSERT INTO GroceryList (Name) VALUES (?);";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, ingredient.c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                std::cerr << "Failed to add to grocery list: " << sqlite3_errmsg(db) << std::endl;
            }
        }

        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
}

void RecipeManager::printGroceryList() {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = "SELECT Name FROM GroceryList;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        std::cout << "Your Grocery List:\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::cout << "- " << name << "\n";
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void RecipeManager::matchRecipes() {
    int option;
    std::cout << "Do you want to generate (1) random recipes based on all ingredients, or (2) select ingredients manually? ";
    std::cin >> option;

    std::vector<Ingredient> selectedIngredients;
    if (option == 1) {
        selectedIngredients = fridge.getIngredients();
        selectedIngredients.insert(selectedIngredients.end(), pantry.getIngredients().begin(), pantry.getIngredients().end());
    } else if (option == 2) {
        std::vector<Ingredient> allIngredients = fridge.getIngredients();
        allIngredients.insert(allIngredients.end(), pantry.getIngredients().begin(), pantry.getIngredients().end());

        std::cout << "Available ingredients in Fridge and Pantry:\n";
        for (size_t i = 0; i < allIngredients.size(); ++i) {
            std::cout << i + 1 << ". " << allIngredients[i].getName() << " (Quantity: " << allIngredients[i].getQuantity() << ")\n";
        }

        int num;
        std::cout << "How many ingredients do you want to select? ";
        std::cin >> num;

        for (int i = 0; i < num; ++i) {
            int ingredientIndex;
            std::cout << "Enter the number of the ingredient (1 to " << allIngredients.size() << "): ";
            std::cin >> ingredientIndex;

            if (ingredientIndex >= 1 && ingredientIndex <= allIngredients.size()) {
                selectedIngredients.push_back(allIngredients[ingredientIndex - 1]);
            } else {
                std::cout << "Invalid selection.\n";
            }
        }
    } else {
        std::cout << "Invalid option.\n";
        return;
    }


    std::string recipeType;
    std::cout << "Do you want a (S)weet or (Sa)vory recipe? ";
    std::cin >> recipeType;
    std::transform(recipeType.begin(), recipeType.end(), recipeType.begin(), ::tolower);

    for (const auto& recipe : recipes) {
        std::string category = recipe.getType();
        std::transform(category.begin(), category.end(), category.begin(), ::tolower);

        if ((recipeType == "s" && category == "sweet") || (recipeType == "sa" && category == "savory")) {
            std::vector<std::string> missingIngredients;
            if (recipe.canMakeRecipe(selectedIngredients, missingIngredients)) {
                std::cout << "You can make the following recipe:\n";
                std::cout << "- " << recipe.getRecipeName() << "\n";

                char viewFullRecipe;
                std::cout << "Would you like to see the full recipe? (y/n): ";
                std::cin >> viewFullRecipe;

                if (viewFullRecipe == 'y' || viewFullRecipe == 'Y') {
                    displayFullRecipe(recipe);
                    return;  // Exit the function after displaying the recipe
                } else {
                    std::cout << "Returning to menu...\n";
                    return;  // Stop iterating once the user declines
                }
            }
        }
    }

    std::cout << "Sorry, you cannot make any recipes with the ingredients you have.\n";
}



// Helper function to write data received from CURL into a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Fetch recipe details from the API
void fetchRecipeFromAPI(const std::string& recipeName) {
    const std::string baseUrl = "https://api.edamam.com/api/recipes/v2?type=public&q=";
    const std::string appId = "d1e22f44";  // Replace with your actual App ID
    const std::string apiKey = "902a662f52555a094e6aaa17e0e47a37";  // Replace with your actual API key

    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL." << std::endl;
        return;
    }

    // Construct URL with escaped recipe name
    char* escapedRecipeName = curl_easy_escape(curl, recipeName.c_str(), recipeName.length());
    if (!escapedRecipeName) {
        std::cerr << "Failed to escape recipe name for URL." << std::endl;
        curl_easy_cleanup(curl);
        return;
    }
    std::string fullUrl = baseUrl + escapedRecipeName + "&app_id=" + appId + "&app_key=" + apiKey;
    curl_free(escapedRecipeName);  // Free the memory allocated by curl_easy_escape

    std::string readBuffer;
    CURLcode res;

    curl_easy_setopt(curl, CURLOPT_URL, fullUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return;
    } else {
      try {
        auto jsonResponse = nlohmann::json::parse(readBuffer);
        if (jsonResponse.contains("hits") && !jsonResponse["hits"].empty()) {
          std::cout << "Recipe details located. \n";
        } else { std::cout<< "No recipes found for " << recipeName << ".\n";
          }
      } catch (const nlohmann::json::parse_error& e) {
          std::cerr << "Failed to parse JSON response: " << e.what() << std::endl;
      }
    }
    curl_easy_cleanup(curl);

    // Parse the API response
    try {
        json response = json::parse(readBuffer);

        if (response.contains("hits") && !response["hits"].empty()) {
            // Use the first hit
            auto recipeData = response["hits"][0]["recipe"];
            std::cout << "Recipe Name: " << recipeData["label"].get<std::string>() << std::endl;
            std::cout << "Time: " << (recipeData.contains("totalTime") && recipeData["totalTime"].is_number() ? std::to_string(recipeData["totalTime"].get<int>()) + " minutes" : "N/A") << std::endl;
            std::cout << "Ingredients:\n";
            for (const auto& ingredient : recipeData["ingredientLines"]) {
                std::cout << "- " << ingredient.get<std::string>() << std::endl;
            }
            std::cout << "Link: " << recipeData["url"].get<std::string>() << std::endl;
            std::cout << "Nutritional Information:\n";
            if (recipeData.contains("totalNutrients")) {
                std::cout << "  Calories: " << recipeData["totalNutrients"]["ENERC_KCAL"]["quantity"].get<double>() << " kcal" << std::endl;
                std::cout << "  Protein: " << recipeData["totalNutrients"]["PROCNT"]["quantity"].get<double>() << " g" << std::endl;
                std::cout << "  Fat: " << recipeData["totalNutrients"]["FAT"]["quantity"].get<double>() << " g" << std::endl;
                std::cout << "  Carbohydrates: " << recipeData["totalNutrients"]["CHOCDF"]["quantity"].get<double>() << " g" << std::endl;
            } else {
                std::cout << "  Nutritional information not available." << std::endl;
            }
        } else {
            std::cerr << "Recipe not found in API response." << std::endl;
        }
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }
}




void RecipeManager::displayFullRecipe(const Recipe& recipe) {
    std::cout << "Recipe: " << recipe.getRecipeName() << "\n";
    std::cout << "Category: " << recipe.getType() << "\n";

    std::cout << "Ingredients:\n";
    for (const auto& ingredient : recipe.getRequiredIngredients()) {
        std::cout << "- " << ingredient.first << ": " << ingredient.second << "\n";
    }

    std::cout << "Steps:\n";
    for (const auto& step : recipe.getSteps()) {
        std::cout << "- " << step << "\n";
    }

    // Fetch nutritional values from the database
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);
    std::string sql = "SELECT Calories, Fat, Protein, Carbohydrates FROM Recipes WHERE Name = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, recipe.getRecipeName().c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << "Nutritional Information:\n";
            std::cout << "Calories: " << sqlite3_column_double(stmt, 0) << "\n";
            std::cout << "Fat: " << sqlite3_column_double(stmt, 1) << "g\n";
            std::cout << "Protein: " << sqlite3_column_double(stmt, 2) << "g\n";
            std::cout << "Carbohydrates: " << sqlite3_column_double(stmt, 3) << "g\n";
        }
    } else {
        std::cerr << "Failed to fetch nutritional values: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


void RecipeManager::viewRecipeHistory() {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = "SELECT Name, Date FROM History;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        int count = 1;
        std::cout << "Recipe History:\n";
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::cout << count++ << ". " << name << " (Date: " << date << ")\n";
        }

        if (count == 1) {
            std::cout << "No recipe history found.\n";
        }
    } else {
        std::cerr << "Failed to load recipe history: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


void RecipeManager::clearGroceryList() {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = "DELETE FROM GroceryList;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_DONE) {
            std::cout << "Grocery list cleared successfully.\n";
        } else {
            std::cerr << "Failed to clear grocery list: " << sqlite3_errmsg(db) << std::endl;
        }
    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void RecipeManager::checkNotifications() {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    // Expiring soon query
    std::string expiringQuery = "SELECT Name, ExpirationDate FROM Storage WHERE Type = 'Fridge' AND ExpirationDate <= date('now', '+3 days');";
    sqlite3_stmt* stmt;

    std::cout << "Expiring Soon Items:\n";
    if (sqlite3_prepare_v2(db, expiringQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string expirationDate = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::cout << "- " << name << " (Expires: " << expirationDate << ")\n";
        }
    } else {
        std::cerr << "Failed to fetch expiring items: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);

    // Running low query
    std::string lowStockQuery = "SELECT Name, Quantity FROM Storage WHERE Quantity <= 2;";
    std::cout << "Running Low Items:\n";
    if (sqlite3_prepare_v2(db, lowStockQuery.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int quantity = sqlite3_column_int(stmt, 1);
            std::cout << "- " << name << " (Quantity: " << quantity << ")\n";
        }
    } else {
        std::cerr << "Failed to fetch low-stock items: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


void RecipeManager::menu() {
    int option;
    do {
        std::cout << "What would you like to do?\n";
        std::cout << "1. Add ingredients and clear grocery list\n";
        std::cout << "2. Generate recipes\n";
        std::cout << "3. Use our API (Fetch recipe details)\n";
        std::cout << "4. View recipe history\n";
        std::cout << "5. Check notifications (expiring soon and running low)\n";
        std::cout << "6. Print Grocery List\n";
        std::cout << "7. Exit\n";
        std::cin >> option;

        switch (option) {
            case 1:
                collectIngredients();
                clearGroceryList();
                break;
            case 2:
                matchRecipes();
                break;
            case 3: {
                std::cout << "Enter the recipe name: ";
                std::string recipeName;
                std::cin.ignore();
                std::getline(std::cin, recipeName);
                fetchRecipeFromAPI(recipeName);
                break;
            }
            case 4:
                viewRecipeHistory();
                break;
            case 5:
                checkNotifications();
                break;
            case 6:
                printGroceryList();
                break;
            case 7:
                std::cout << "Goodbye!\n";
                break;
            default:
                std::cout << "Invalid option. Please try again.\n";
                break;
        }
    } while (option != 7);  // Exit only on case 7
}

