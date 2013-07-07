REM Make windows release of MY GAME

set VERSION=0.9

cd ve2
devenv /build "Release|Win32" ve2.sln

cd ..

cd ..\AssetCompiler
call makeglue-win.bat

cd ..\Windows

REM Run Inno Setup script
set PATH="\Program Files\Inno Setup 5\";%PATH%
set PATH="\Program Files (x86)\Inno Setup 5\";%PATH%
iscc.exe MyGameSetup.iss

cd Output
copy setup.exe "..\..\..\..\..\..\HungryPeople-v.%VERSION%-setup.exe"

