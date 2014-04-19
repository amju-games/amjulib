REM Make windows release of MY GAME ("Hungry People")

set VERSION=2.0

cd ve3
devenv /build "Release|Win32" ve3.sln

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

