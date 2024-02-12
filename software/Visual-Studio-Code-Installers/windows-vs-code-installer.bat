REM Install Microsoft Visual Studio Code
echo Installing Microsoft Visual Studio Code...
call winget install -e --id Microsoft.VisualStudioCode

REM Install Microsoft Visual Studio Code Extensions
echo Installing C/C++ Extension...
call code --install-extension ms-vscode.cpptools

echo Installing CMake Tools Extension...
call code --install-extension ms-vscode.cmake-tools

echo Installing PlatformIO Extension...
call code --install-extension platformio.platformio-ide

echo Installing Serial Monitor Extension...
call code --install-extension ms-vscode.vscode-serial-monitor

ech Installing Python...
call code --install-extension ms-python.python

echo Installation Complete!