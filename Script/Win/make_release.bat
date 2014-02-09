REM Make amjulib for Windows

REM Set up MSVC paths
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat"
REM try the other program files dir
call "C:\Program Files\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat"

cd ..\AmjuLibMsvc\
REM devenv /clean "Release|Win32" AmjuLibMsvc.sln
devenv /build "Release|Win32" AmjuLibMsvc.sln

set LIB=..\..\Build\amjulib\lib\
mkdir %LIB%
del /Q %LIB%\*
REM Copy .lib binary to lib folder
copy ..\..\Build\Release\AmjuLibMsvc.lib %LIB%\amju.lib

cd ..\Win\

call copy_files.bat

