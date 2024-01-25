set "installPath=%USERPROFILE%\AppData\Local\Programs"
call winget install -e --id Microsoft.VisualStudioCode --location "%installPath"
call "%installPath" --force --install-extension ms-vscode.cpptools
call "%installPath" --force --install-extension ms-vscode.cmake-tools
call "%installPath" --force --install-extension platformio.platformio-idecd
