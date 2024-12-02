#include <sqlite3.h>
#include <iostream>
#include <string>

// Initialize the SQLite database
void initializeDatabase(const std::string& dbName) {
    sqlite3* db;
    if (sqlite3_open(dbName.c_str(), &db) == SQLITE_OK) {
        std::cout << "Database initialized: " << dbName << std::endl;
    } else {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
    }
    sqlite3_close(db);
}

// Create the Recipes table
void createRecipesTable(const std::string& dbName) {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = R"(
        CREATE TABLE IF NOT EXISTS Recipes (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            Name TEXT NOT NULL,
            Ingredients TEXT,
            Steps TEXT
        );
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "Table created successfully: Recipes" << std::endl;
    }

    sqlite3_close(db);
}

// Add a recipe to the Recipes table
void addRecipe(
    const std::string& dbName,
    const std::string& name,
    const std::string& ingredients,
    const std::string& steps
) {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = R"(
        INSERT INTO Recipes (Name, Ingredients, Steps)
        VALUES (?, ?, ?);
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Error preparing insert statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // Bind values
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, ingredients.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, steps.c_str(), -1, SQLITE_STATIC);

    // Execute statement
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Error inserting data: " << sqlite3_errmsg(db) << std::endl;
    } else {
        std::cout << "Recipe added successfully: " << name << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Show all recipes in the Recipes table
void showAllRecipes(const std::string& dbName) {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string sql = "SELECT * FROM Recipes;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        std::cout << "Table: Recipes" << std::endl;
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << "ID | Name           | Ingredients       | Steps" << std::endl;
        std::cout << "------------------------------------------------" << std::endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << sqlite3_column_int(stmt, 0) << " | ";
            std::cout << sqlite3_column_text(stmt, 1) << " | ";
            std::cout << sqlite3_column_text(stmt, 2) << " | ";
            std::cout << sqlite3_column_text(stmt, 3) << std::endl;
        }

        std::cout << "------------------------------------------------" << std::endl;
    } else {
        std::cerr << "Error querying table: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

int main() {
    const std::string dbName = "recipes.db";

    // Initialize the database
    initializeDatabase(dbName);

    // Create the Recipes table
    createRecipesTable(dbName);

    // Add sample recipes
    addRecipe(dbName, "Pasta", "Noodles, Tomato Sauce, Garlic", "Boil noodles, mix with sauce");
    addRecipe(dbName, "Salad", "Lettuce, Tomato, Cucumber", "Chop veggies, mix ingredients");
    addRecipe(dbName, "Omelette", "Eggs, Cheese, Onions", "Whisk eggs, fry with toppings");

    // Show all recipes
    showAllRecipes(dbName);

    return 0;
}
