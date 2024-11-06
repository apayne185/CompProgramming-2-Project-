// FRONTEND
// THIS FILE HANDLES FLASK ENDPOINTS, FETCHES DATA, UPDATES UI, MANAGES USER INTERACTIONS


//NOTES:
// 1. need to modify the /routes for API requests --> filler names
// 2. need to modify names of the ${} variables  --> filler names

// Captures ingredient data, sends it to server to be saved in database (POST request)
async function addIngredient() {
    const name = document.getElementById("IngredientName").value;      //retrieves value in input field
    const quantity = document.getElementById("IngredientQuantity").value;    //.getElementById finds an id='' within HTML

    const response = await fetch(`/add-ingredient`, {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({ name, quantity })
    });

    //checks if response from server was successful, if so, converts response into JS object
    if(response.ok){
        //reads JSON data from response, waits for it parsed into object before going to next line
        const result = await response.json()       //result = ingredientName
        //adds ingredient to ingredientList on the screen
        document.getElementById("ingredientList").innerHTML += `<li>${result.name}</li>`;   //.innerHTML allows for access to whatever is inside the <...id=''> innerHTML </..>
    }
}



// Requests list of recipes, iterates over it for list of recipe names, appends each with button (viewRecipe()) for seeing more details
async function generateRecipes(){
    const response = await fetch(`/generate-recipes`);   //GET request to /endpoint
    const recipes = await response.json();

    //this starts an HTML string, then the loop adds each recipe's info to it, then closes it out
    let recipeList= '<h2>Recipes</h2><ul>';
    recipes.forEach(recipe=> {
        recipeList += `<li>${recipe.name} - <button onclick="viewRecipes('${recipe.id}')">See Full Recipe</button></li>`;
    });
    recipeList += '</ul>';

    document.getElementById("recipesSection").innerHTML = recipeList;
}




// Fetches details for a recipe and displays then in a modal -- function for a button for each recipe after generating
async function viewRecipes(recipeId){
    const response = await fetch(`/recipe-details/${recipeId}`);
    const recipe = await response.json()

    const recipeDetails= `
        <h2>${recipe.name}</h2>
        <img src="${recipe.image}" alt="${recipe.name}">
        <p>${recipe.description}</p>
        <ul>
            ${recipe.ingredients.map(ing => `<li>${ing.quantity} ${ing.name}</li>`).join('')}
        </ul>
        <p>Steps: ${recipe.steps.join(', ')}</p>
        <br>
        <p>Nutritional Facts: ${recipe.nutrition}</p>
        <button onclick="selectRecipe(${recipeId})">Make this Recipe!</button>
    `;

    //button above calls selectRecipe(), which subtracts ingredients from database **NEEDS TO ADD TO HISTORY DATABASE TOO
    document.getElementById("recipeDetailsModal").innerHTML = recipeDetails;
    document.getElementById("recipeDetailsModal").style.display = 'block';
}





// When user selects a recipe, it updates the ingredients in Storage and adds to History
async function selectRecipe(recipeId){
    await fetch(`/select-recipe`, {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({recipe_id: recipeId})
    });
    closeRecipeDetails();
    generateRecipes();   //refreshes recipe list
}



// Creates modal for viewing grocery list ** need to update to include an add/remove option for ingredients to remove ingredient from both window and database
async function viewGroceryList(){
    const response = await fetch(`/grocery-list-data`);
    const groceryItems = await response.json();

    let groceryList = `<h2>Grocery List</h2><ul>`;
    groceryItems.forEach(item => {
        groceryList += `<li>${item.quantity} ${item.name}</li>`;
    });
    groceryList += `</ul>`;

    document.getElementById("groceryListModal").innerHTML = groceryList;
    document.getElementById("groceryListModal").style.display = 'block';
}



// Creates modal for viewing Recipe History, allows for users to click and remake recipes
async function viewHistory(){
    const response = await fetch(`/history-data`);
    const historyItems = await response.json();

    let historyList = `<h2>Recipe History</h2><ul>`;
    historyItems.forEach(item => {
        historyList += `<li>${item.name} - Made on ${item.date} <button onclick = "selectRecipe(${item.id})">Remake Recipe</button></li>`;
    });
    historyList += `</ul>`;

    document.getElementById("historyModal").innerHTML = historyList;
    document.getElementById("historyModal").style.display = 'block';
}



