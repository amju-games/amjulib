REM Make windows release of MY GAME

set VERSION=0.3

cd ve1
devenv /build "Release|Win32" ve1.sln

cd ..

REM Run Inno Setup script
set PATH="\Program Files\Inno Setup 5\";%PATH%
set PATH="\Program Files (x86)\Inno Setup 5\";%PATH%
iscc.exe MyGameSetup.iss

cd Output
copy setup.exe "..\mygame-%VERSION%.exe"

