#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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

// Create tables for all JSON data
void createTables(const std::string& dbName) {
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string recipesTable = R"(
        CREATE TABLE IF NOT EXISTS Recipes (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            Name TEXT NOT NULL,
            Category TEXT,
            Ingredients TEXT,
            Condiments TEXT,
            Steps TEXT,
            Time TEXT,
            Calories REAL,
            Fat REAL,
            Protein REAL,
            Carbohydrates REAL
        );
    )";

    std::string storageTable = R"(
        CREATE TABLE IF NOT EXISTS Storage (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            Type TEXT NOT NULL,
            Name TEXT NOT NULL,
            Quantity INTEGER,
            ExpirationDate TEXT
        );
    )";

    std::string groceryListTable = R"(
        CREATE TABLE IF NOT EXISTS GroceryList (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            Name TEXT NOT NULL
        );
    )";

    std::string historyTable = R"(
        CREATE TABLE IF NOT EXISTS History (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            Name TEXT NOT NULL,
            Date TEXT
        );
    )";

    char* errMsg = nullptr;

    if (sqlite3_exec(db, recipesTable.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating Recipes table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    if (sqlite3_exec(db, storageTable.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating Storage table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    if (sqlite3_exec(db, groceryListTable.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating GroceryList table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    if (sqlite3_exec(db, historyTable.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "Error creating History table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
    std::cout << "Tables created successfully.\n";
}



void insertData(const std::string& dbName, const std::string& jsonFilename, const std::string& table) {
    std::ifstream file(jsonFilename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << jsonFilename << std::endl;
        return;
    }

    json jsonData;
    file >> jsonData;
    file.close();

    sqlite3* db;
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::string sql;
    sqlite3_stmt* stmt;

    if (table == "Recipes") {
        sql = "INSERT INTO Recipes (Name, Category, Ingredients, Condiments, Steps, Time, Calories, Fat, Protein, Carbohydrates) "
              "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return;
        }

        for (const auto& item : jsonData["recipes"]) {
            sqlite3_bind_text(stmt, 1, item["name"].get<std::string>().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, item["category"].get<std::string>().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, item["ingredients"].dump().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, item["condiments"].dump().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 5, item["steps"].dump().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 6, item["time"].get<std::string>().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_double(stmt, 7, item["calories"].get<double>());
            sqlite3_bind_double(stmt, 8, item["fat"].get<double>());
            sqlite3_bind_double(stmt, 9, item["protein"].get<double>());
            sqlite3_bind_double(stmt, 10, item["carbohydrates"].get<double>());

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                std::cerr << "Error inserting data into Recipes: " << sqlite3_errmsg(db) << std::endl;
            }
            sqlite3_reset(stmt);
        }
    } else if (table == "Storage") {
        sql = "INSERT INTO Storage (Type, Name, Quantity, ExpirationDate) VALUES (?, ?, ?, ?);";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return;
        }

        for (const auto& [type, items] : jsonData.items()) {
            for (const auto& item : items) {
                sqlite3_bind_text(stmt, 1, type.c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_text(stmt, 2, item["name"].get<std::string>().c_str(), -1, SQLITE_STATIC);
                sqlite3_bind_int(stmt, 3, item["quantity"].get<int>());
                sqlite3_bind_text(stmt, 4, item["expirationDate"].get<std::string>().c_str(), -1, SQLITE_STATIC);

                if (sqlite3_step(stmt) != SQLITE_DONE) {
                    std::cerr << "Error inserting data into Storage: " << sqlite3_errmsg(db) << std::endl;
                }
                sqlite3_reset(stmt);
            }
        }
    } else if (table == "GroceryList") {
        sql = "INSERT INTO GroceryList (Name) VALUES (?);";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return;
        }

        for (const auto& item : jsonData) {
            sqlite3_bind_text(stmt, 1, item["name"].get<std::string>().c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                std::cerr << "Error inserting data into GroceryList: " << sqlite3_errmsg(db) << std::endl;
            }
            sqlite3_reset(stmt);
        }
    } else if (table == "History") {
        sql = "INSERT INTO History (Name, Date) VALUES (?, ?);";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return;
        }

        for (const auto& item : jsonData) {
            sqlite3_bind_text(stmt, 1, item["name"].get<std::string>().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, item["date"].get<std::string>().c_str(), -1, SQLITE_STATIC);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                std::cerr << "Error inserting data into History: " << sqlite3_errmsg(db) << std::endl;
            }
            sqlite3_reset(stmt);
        }
    } else {
        std::cerr << "Unsupported table: " << table << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


int main() {
    const std::string dbName = "data.db";

    initializeDatabase(dbName);
    createTables(dbName);

    insertData(dbName, "../data/recipes.json", "Recipes");
    insertData(dbName, "../data/storage.json", "Storage");
    insertData(dbName, "../data/grocery_list.json", "GroceryList");
    insertData(dbName, "../data/history.json", "History");

    return 0;
}
