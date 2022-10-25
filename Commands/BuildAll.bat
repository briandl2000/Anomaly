call GenerateProject.bat

ECHO "Building Debug..."
cmake --build ../build --config Debug

ECHO "Building Release..."
cmake --build ../build --config Release

ECHO "Finished building Everything"