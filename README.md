
notes: 
   - CMakeCache.txt, cmake_install.cmake, etc are automatically created and included in gitignore
   - cmake-build-debug & CMakeFiles folders are created when you run cmake .., do not delete them but they will be included in the gitignore. If you for some reason delete build, delete these too then. 
   - .vscode is needed for yall to run on mac/vs code. the .idea folder is generated automatically for jetbrains (clion), so its in the gitignore
   - .gitmodules is tracking our submodule (json folder) which holds our nlohmann_json package. DO NOT put in gitignore.

future updates/comments

Within Code
   - recipemanager.cpp error handling 
   - Memory management (& rule of 3 and 5 - handles memory leaks, dangling pointers, efficient transfers, smart pointers) --> Makenna
   - Integrating database API into our classes
   - integration testing for sqlite database

Within Documentation
   - UML - diagrams 
      - structural diagrams (class, components, deployment diagrams)
   - FINISH README 

      


# CompProgramming-2-Project-

## Table of Contents
1. [Project Description](#project-description)
2. [Features](#features)
3. [Installation and Setup](#installation-and-setup)
4. [Usage](#usage)
5. [Code Structure](#code-structure)
6. [Memory Management](#memory-management)
7. [Error Handling](#error-handling)
8. [Testing](#testing)
9. [Version Control](#version-control)
10. [Future Enhancements](#future-enhancements)
11. [Contributors](#contributors)

---

### Project Description

The Recipe Manager is a C++ program created to handle kitchen ingredients, recommend recipes according to available ingredients, and keep track of recipe history. Users have the ability to input items into their fridge and pantry, access recipes that can be made with those ingredients, and come up with meal suggestions depending on what items they have. The program uses file handling in C++ to store and load ingredients and recipes in JSON files effectively. This system also monitors the expiration dates of ingredients and notifies users when ingredients are close to running out or expiring.

---

### Features

The main features of our program consist of:

- **Management of Ingredients:** the program keeps track of ingredients stored in either the fridge or the pantry, it monitors their quantities, and remembers expiration dates for perishable items.

- **Recipe Suggestions:** the program also provides a way to recommend recipes utilizing the ingredients on hand, while notifying about any missing ingredients if a recipe is chosen by the user.

- **JSON-based storage:** To retrieve and store ingredient and recipe information, the program loads and saves these items in JSON format, facilitating access and making it easy to expand if the ingredients saved are a huge number.

- **Ingredient Expiry Notifications:** Notifications will be sent when ingredients in the fridge are about to expire.

- **Low-Stock Notifications:** The pantry keeps track of ingredient quantities and alerts the users when supplies are running low.

- **Recipe History:** This feature adds previously made recipes by the users, and stores them with a access date to indicate the date the recipe was made.


---

### Installation and Setup

The updated code section for your README should be adjusted to align with the corrections you need. Here’s the revised text:

Installation and Setup

To install and run the Recipe Manager locally:
	1.	Clone the repository:

git clone https://github.com/apayne185/CompProgramming-2-Project.git


	2.	Navigate to the project directory:

cd CompProgramming-2-Project


	3.	Create a build directory:

mkdir build


	4.	Navigate into the build directory:

cd build


	5.	Run CMake to generate the necessary build system files:

cmake ..

	•	This reads the CMakeLists.txt file from the root directory and prepares the build system.

	6.	Build the project:

cmake --build .


	7.	Run the executable:

./RecipeManagerExecutable

---

### Usage

The Recipe Manager functions through a command-line interface. When starting, users have the ability to carry out the subsequent actions:

1. **Add Ingredients:** Includes fresh ingredients in the refrigerator or cupboard, indicating the amount and expiry date (for items that spoil).

2. **Generate Recipes:** Using the ingredients on hand, the system will propose potential recipes and alert users about any ingredients they may be missing.

3. **View History:** Show a roster of cooked recipes along with the corresponding preparation date.

4. **Notification:** Monitor for ingredients nearing expiry or low in stock.

Sample interaction when the program is ran (main menu):
```bash
What would you like to do?
1. Add ingredients
2. Generate recipes
3. View recipe history
4. Check notifications (expiring soon and running low)
5. Exit
```

---

### Code Structure

- **`finalcode.cpp`:** The main file that contains the classes `Ingredient`, `Storage`, `Fridge`, `Pantry`, `Recipe`, and `RecipeManager`. These classes manage the functionality of ingredient tracking, recipe matching, and user interaction.
- **`storage.json`:** Stores the current ingredients in the fridge and pantry, along with their quantities and expiration dates for perishable items.
- **`recipes.json`:** Contains a list of recipes, including ingredients, condiments, and steps for preparation.
- **`history.json`:** Tracks the recipes that have been made, along with the date of preparation.

---

### Memory Management

The Recipe Manager efficiently manages memory by utilizing contemporary C++ techniques like dynamic memory allocation, references, and pointers. Here is a summary of memory management:

#### **Use of References**
- **Function Parameters:** The code efficiently passes objects without making copies by using references. In the `addIngredient` function, the `ingredient` is passed by reference in order to prevent the need to copy large objects, as an example.
   ```cpp
   void addIngredient(const Ingredient& ingredient);
   ```
   This guarantees that only a pointer to the object is transferred, decreasing memory consumption and enhancing efficiency.
  
- **Data Access:** Functions like `getIngredients()` that return large objects such as vectors use `const` references to avoid extra copying:
   ```cpp
   const std::vector<Ingredient>& getIngredients() const;
   ```
   This enables effective access to the data in a read-only manner without generating duplicates.

#### **Use of Pointers**
- **Dynamic Allocation:** Even though direct raw pointers are not used in the code for allocating dynamic memory, the utilization of STL containers like `std::vector` indirectly manages dynamic memory for storing ingredients and recipes. Vectors handle memory allocation and deallocation automatically when elements are added or removed.

---

### Error Handling

- **File Access Errors**: Handled for file opening failures.
- **User Input Validation**: For ingredient quantities and storage locations.
- **Ingredient Availability**: Ensures that missing ingredients are reported when generating recipes.
- **Data Integrity**: Prevents duplicate entries for ingredients by updating quantities when needed.
- **Potential JSON Parsing Errors**: Currently unhandled but can be improved by catching `json::parse_error`.

---

### Testing

We used Google Tests to ensure expected functionality. Unit tests are written for each class (Ingredient, Storage, Fridge, Pantry, Recipe, and RecipeManager) to verify that they work in isolation.

### Testing Setup

To run the Google Tests properly, we created separate `.h` (header) and `.cpp` (implementation) files for each class. This ensures modularity and allows each class to be tested in isolation.

The following files were created:

- **Ingredient.h** and **Ingredient.cpp**: Defines and implements the `Ingredient` class.
- **Storage.h** and **Storage.cpp**: Defines and implements the `Storage` class.
- **Fridge.h** and **Fridge.cpp**: Defines and implements the `Fridge` class.
- **Pantry.h** and **Pantry.cpp**: Defines and implements the `Pantry` class.
- **Recipe.h** and **Recipe.cpp**: Defines and implements the `Recipe` class.
- **RecipeManager.h** and **RecipeManager.cpp**: Defines and implements the `RecipeManager` class.


#### Test Coverage

We aimed to cover the key functionalities of the program, including:

Ingredient Class:
- Verifying correct initialization of ingredients.
- Checking if quantities and expiration dates are properly set and updated.

Storage Class:
- Ensuring that ingredients are correctly added and updated in the fridge and pantry.
- Testing conversions to and from JSON for storing and retrieving ingredients.

Fridge Class:
- Testing the expiringSoon feature to ensure users are notified when items are about to expire.

Pantry Class:
- Testing the runningLow feature to ensure users are notified when stock levels are low.

Recipe Class:
- Verifying that recipes can be matched correctly based on available ingredients.
- Ensuring that missing ingredients are accurately identified.

RecipeManager Class:
- Ensuring that the system correctly handles ingredient collection, recipe matching, and notifications for low stock and expiring items.
- Verifying that recipe history is recorded accurately.


---

### Version Control

The GitHub repository has basic protection rules, where non of the member (including the owner of the repository) are able to push or commit a file without another member approving the request. the project division and workloads could be tracked with numerous commits marking important development stages, such as adding core features and error handling. Branching is utilized for making pull requests to commit new files/changes etc...

---

### Future Enhancements

Mention any features or improvements that could be added:
- Adding a UI for better user interaction.
- Expanding recipe categories to include cuisuines where the user could choose from.
- Adding a grocery list: when the user gets a notification that an item is running low or expiring soon, he can add the item to this list and he then can later access this list when doing grocery shopping.

---

### Contributors

Here’s a detailed list of all classes, functions, and JSON files used in our program and who did each.

#### **Classes and Functions Developed:**

##### **Class: Ingredient** 
Done by India except `setExpirationDate` which was done by Aya
- **Functions:**
  1. `Ingredient(std::string n, int q, std::string exp = "")`
  2. `std::string getName() const`
  3. `int getQuantity() const`
  4. `std::string getExpirationDate() const`
  5. `void setQuantity(int q)`
  6. `void setExpirationDate(const std::string& expDate)`
  7. `json toJSON() const`
  8. `static Ingredient fromJSON(const json& j)`

##### **Class: Storage**
Done by India
- **Functions:**
  1. `void addIngredient(const Ingredient& ingredient)`
  2. `const std::vector<Ingredient>& getIngredients() const`
  3. `json toJSON() const`
  4. `void fromJSON(const json& j)`

##### **Class: Fridge (inherits Storage)**
Done by India except `expiringSoon` which was done by Aya
- **Functions:**
  1. `void addIngredient(const Ingredient& ingredient) override`
  2. `void expiringSoon() const`

##### **Class: Pantry (inherits Storage)**
Done by India except `runningLow` which was done by Aya
- **Functions:**
  1. `void addIngredient(const Ingredient& ingredient) override`
  2. `void runningLow() const`

##### **Class: Recipe**
Done by India and syntax problems were fixed by Makenna
- **Functions:**
  1. `Recipe(std::string name, std::vector<std::pair<std::string, std::string>> ingredients, std::vector<std::pair<std::string, std::string>> condiments, std::vector<std::string> steps, std::string type)`
  2. `std::string getRecipeName() const`
  3. `std::string getType() const`
  4. `bool canMakeRecipe(const std::vector<Ingredient>& userIngredients, std::vector<std::string>& missingIngredients) const`
  5. `std::vector<std::pair<std::string, std::string>> getRequiredIngredients() const`
  6. `std::vector<std::pair<std::string, std::string>> getCondiments() const`
  7. `std::vector<std::string> getSteps() const`

##### **Class: RecipeManager**
Done by India except `menu` and `viewRecipeHistory` which were done by Aya
- **Functions:**
  1. `RecipeManager(const std::string& recipeFilename)`
  2. `void loadIngredientsFromFile(const std::string& filename)`
  3. `void collectIngredients()`
  4. `void matchRecipes()`
  5. `void displayFullRecipe(const Recipe& recipe)`
  6. `void saveIngredientsToFile(const std::string& filename)`
  7. `void menu()`
  8. `void viewRecipeHistory()`

---

#### **JSON Files Developed:**

1. **`storage.json`**  Done By India
   - Stores the list of ingredients in both the fridge and pantry, along with quantities and expiration dates where applicable.

2. **`recipes.json`**  Done By India
   - Contains all recipes, including ingredients, condiments, preparation steps, and categories.

3. **`history.json`**  Done By Aya
   - Tracks the history of recipes that have been made, recording the name of the recipe and the date it was prepared.

#### **Testing:**
##### **Unit Testing** 
Done By Makenna


