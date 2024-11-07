# assumes C++ BE is running a rest api on http://localhost:5001 with endpoints defined in c++ application

from flask import Flask, render_template, request, jsonify
import requests
from datetime import datetime

#need to import models

app = Flask(__name__)

# Python Frontend
@app.route('/')
def home():
    return render_template('home.html')

@app.route('/recipes')
def recipes():
    return render_template('recipes.html')

@app.route('/grocery-list')
def grocery_list():
    return render_template('grocery_list.html')

@app.route('/history')
def history():
    return render_template('history.html')




# Calls C++ Backend
# ADD/VIEW INGREDIENTS
@app.route('/add-ingredient', methods=['POST'])
def add_ingredient():
    data= request.get_json()
    ingredient= { 'name': data['name'], 'quantity': data['quantity'] } # API call to backend to save ingredients
    response= requests.post('http://localhost:5001/add_ingredient', json=ingredient)
    return jsonify(response.json()) #responds with updated list


# fetches ingredient list from backend (need to configure to sqlalchemy database) --> posts it on the ingredient_list on UI
@app.route('/ingredient-list')
def ingredient_list():
    response= requests.get('http://localhost:5001/get_ingredients')    #need to make sure API endpoint /get_ingredients is correct
    ingredients= response.json()
    return jsonify(ingredients)



# RECIPE GENERATION
# should return a name, image URL, recipe description
@app.route('/generate-recipes')
def generate_recipes():
    response= requests.get('http://localhost:5001/get_matching_recipes')
    recipes= response.json()
    #if we do not have any complete matches, this shows partial recipe matches
    if not recipes:
        response = requests.get('http://localhost:5001/get_alternative_recipes')
        alternative_recipes = response.json()
        return jsonify({'recipes': [], 'alternatives': alternative_recipes})
    return jsonify(recipes)

# gets the recipe details --> ingredients, name, picture, etc from backend
@app.route('/recipe-details/<recipe_id>')
def recipe_details(recipe_id):
    response= requests.get(f'http://localhost:5001/get_recipe/{recipe_id}')
    details= response.json()
    return jsonify(details)

# updates the quantity of ingredients once you select a recipe --> decreases amount
@app.route('/select-recipe', methods=['POST'])
def select_recipe():
    data= request.get_json()
    recipe_id= data['recipe_id']

    #get a recipe and its ingredients from database
    recipe = Recipe.query.get(recipe_id)    #recipe is the model i am modifying in our database
    if not recipe:
        return jsonify({'message': 'Recipe not found'}), 404

    #subract ingredient quantity (grams) from amount in database
    ingredients = recipe.ingredients       #need to analyze the ingredients relationship on recipe model once its created
    for i in ingredients:
        storage_ingredient = Ingredient.query.filter_by(name=i.name).first()    #Ingredient is another model
        if storage_ingredient:
            storage_ingredient.quantity -= i.quantity
            db.session.commit()                #db = new database, will modify after updates to database
        else:
            return jsonify({'message': f'{i.name} not found in storage'}), 404

    new_history_entry = RecipeHistory(     #RecipeHistory is a model
        recipe_name = recipe.name,
        date_selected = datetime.utcnow
    )
    db.session.add(new_history_entry)
    db.session.commit()

    return jsonify({'message': 'recipe selection successful, history updated'}), 200



# GROCERY LIST
@app.route('/grocery-list-data')
def grocery_list():
    response= requests.get('http://localhost:5001/get_grocery_list')
    return jsonify(response.json())

@app.route('/add-to-grocery-list', methods=['POST'])
def add_to_grocery_list():
    data= request.get_json()
    grocery_item= {'name': data['name'], 'quantity': data['quantity']}
    response= requests.post('http://localhost:5001/add_to_grocery_list', json= grocery_item)
    return jsonify(response.json())



# RECIPE HISTORY
@app.route('/history-data')
def recipe_history():
    response = requests.get('http://localhost:5001/get_recipe_history')
    return jsonify(response.json())


@app.route('/remake-recipe', methods=['POST'])
def remake_recipe():
    data = request.get_json()
    recipe_id = data['recipe_id']
    response = requests.post(f'http://localhost:5001/remake_recipe', json={'recipe_id': recipe_id})
    return jsonify(response.json())



if __name__ == '__main__':
    app.run(debug=True)