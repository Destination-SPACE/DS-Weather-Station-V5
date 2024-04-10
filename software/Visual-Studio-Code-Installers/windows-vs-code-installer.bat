REM Install Microsoft Visual Studio Code
echo Installing Microsoft Visual Studio Code...
winget install --id=Microsoft.VisualStudioCode

:: Install Microsoft Visual Studio Code Extensions
echo Installing C/C++ Extension...
code --install-extension ms-vscode.cpptools

echo Installing CMake Tools Extension...
code --install-extension ms-vscode.cmake-tools

echo Installing PlatformIO Extension...
code --install-extension platformio.platformio-ide

echo Installing Serial Monitor Extension...
code --install-extension ms-vscode.vscode-serial-monitor

echo Installing Python...
code --install-extension ms-python.python

echo Installation Complete!