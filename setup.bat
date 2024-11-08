REM batch file - automates building the program and running the app

@echo off
echo Installing requirements.txt
pip install -r requirements.txt

if not exist build (
    mkdir build
)

cd build

if exist "CMakeList.txt" (
    echo CMake cache exists.
) else (
    cmake ..
)

cmake --build .

cd ..

echo Setup complete

echo Starting Flask app
flask run

pause