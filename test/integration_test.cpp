#include <sqlite3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include "json.hpp" // Include the nlohmann/json library

using json = nlohmann::json;

// Test database initialization
void testDatabaseInitialization() {
    const std::string dbName = "test_data.db";

    sqlite3* db;
    int rc = sqlite3_open(dbName.c_str(), &db);
    assert(rc == SQLITE_OK && "Failed to initialize database");
    sqlite3_close(db);

    std::cout << "Database initialization test passed.\n";
}

// Test table creation
void testTableCreation() {
    const std::string dbName = "test_data.db";
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS TestTable (
            ID INTEGER PRIMARY KEY AUTOINCREMENT,
            Name TEXT NOT NULL,
            Value INTEGER NOT NULL
        );
    )";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, createTableSQL.c_str(), nullptr, nullptr, &errMsg);
    assert(rc == SQLITE_OK && "Failed to create table");
    sqlite3_close(db);

    std::cout << "Table creation test passed.\n";
}

// Test inserting data into a table
void testInsertData() {
    const std::string dbName = "test_data.db";
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string insertSQL = R"(
        INSERT INTO TestTable (Name, Value) VALUES ('Sample', 42);
    )";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, insertSQL.c_str(), nullptr, nullptr, &errMsg);
    assert(rc == SQLITE_OK && "Failed to insert data");
    sqlite3_close(db);

    std::cout << "Data insertion test passed.\n";
}

// Test querying data from a table
void testQueryData() {
    const std::string dbName = "test_data.db";
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    std::string querySQL = "SELECT * FROM TestTable;";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, querySQL.c_str(), -1, &stmt, nullptr);
    assert(rc == SQLITE_OK && "Failed to prepare query");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int value = sqlite3_column_int(stmt, 2);

        std::cout << "Retrieved Row - ID: " << id << ", Name: " << name
                  << ", Value: " << value << "\n";
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    std::cout << "Data query test passed.\n";
}

// Test loading data from a JSON file into the database
void testJsonDataInsertion() {
    const std::string dbName = "test_data.db";
    sqlite3* db;
    sqlite3_open(dbName.c_str(), &db);

    // Read the JSON file
    std::ifstream jsonFile("test_data.json"); // File copied to the build directory
    assert(jsonFile.is_open() && "Failed to open JSON file");

    json jsonData;
    jsonFile >> jsonData;

    std::string insertSQL = "INSERT INTO TestTable (Name, Value) VALUES (?, ?);";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, insertSQL.c_str(), -1, &stmt, nullptr);
    assert(rc == SQLITE_OK && "Failed to prepare SQL statement");

    for (const auto& item : jsonData) {
        sqlite3_bind_text(stmt, 1, item["name"].get<std::string>().c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, item["value"].get<int>());

        rc = sqlite3_step(stmt);
        assert(rc == SQLITE_DONE && "Failed to insert JSON data");

        sqlite3_reset(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    std::cout << "JSON data insertion test passed.\n";
}

int main() {
    // Run integration tests
    testDatabaseInitialization();
    testTableCreation();
    testInsertData();
    testQueryData();
    testJsonDataInsertion();

    // Clean up test database
    remove("test_data.db");

    std::cout << "All integration tests passed and cleaned up!\n";
    return 0;
}