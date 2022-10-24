ECHO "running cmake files"
mkdir build
cd build
cmake .. -G "Unix Makefiles"

ECHO "Building everything..."
make
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)
ECHO "All assemblies built successfully."
ECHO "Copying AnomalyEngine shared lib..."
xcopy "AnomalyEngine\AnomalyEngine.dll" "AnomalyEditor\" /Y