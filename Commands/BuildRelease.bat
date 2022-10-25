call GenerateProject.bat

ECHO "Building Release..."
cmake --build ../build --config Release

ECHO "Finished building Release"
