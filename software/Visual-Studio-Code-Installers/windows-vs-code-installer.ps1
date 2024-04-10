$Username = [System.Security.Principal.WindowsIdentity]::GetCurrent().Name

$FileUri = "https://code.visualstudio.com/sha/download?build=stable&os=win32-x64"
$Destination = "downloads/vscodeInstaller.exe"
$exeArgs = '/verysilent /tasks=addcontextmenufiles,addcontextmenufolders,addtopath'

$bitsJobObj = Start-BitsTransfer $FileUri -Destination $Destination

$vsCodeExec = ($Env:PROGRAMFILES) + "\Visual Studio Code\Bin\code.cmd"
$extensions =@(
    "ms-vscode.cpptools",               #C/C++ Language Support
    "ms-python.python",                 #Python Language Support
    "ms-vscode.cmake-tools",            #CMake Tools Support
    "ms-vscode.vscode-serial-monitor",  #Serial Monitor Extension
    "platformio.platformio-ide"         #PlatformIO Support
) | SORT-OBJECT

switch($bitsJobObj.JobState){
    'Transferred'{
        Complete-BitsTransfer -BitsJob $bitsJobObj
        break
    }
    'Error'{
        throw 'Error downloading'
    }
}



Start-Process -Wait $Destination -ArgumentList $exeArgs

$extensions | ForEach-Object{
    try{
        Invoke-Expression "& '$vsCodeExec' --install-extension $_ --force"
        Write-Host
    }
    catch{
        $_
        Exit(1)
    }
}

Exit(0)