REM Make windows release of MY GAME

set VERSION=0.3

cd ve1
devenv /build "Release|Win32" ve1.sln

cd ..

REM Run Inno Setup script
iscc MyGameSetup.iss

cd Output
copy setup.exe mygame-%SERVERSION%.exe
