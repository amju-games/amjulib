REM Make amjulib for Windows

REM Set up MSVC paths
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat"
REM try the other program files dir
call "C:\Program Files\Microsoft Visual Studio 10.0\VC\bin\vcvars32.bat"

cd ..\AmjuLibMsvc\
REM devenv /clean "Release|Win32" AmjuLibMsvc.sln
devenv /build "Release|Win32" AmjuLibMsvc.sln

cd ..\Win\

set LIB=..\..\Build\amjulib\lib\
set INC=..\..\Build\amjulib\include\
set SRC=..\..\Source\

REM Create release dir structure
mkdir %LIB%
mkdir %INC%

del /Q %INC%\*
del /Q %LIB%\*

REM Copy .lib binary to lib folder
copy ..\..\Build\Release\AmjuLibMsvc.lib %LIB%\amju.lib

REM Copy . headers required by client code to include folder
copy %SRC%\AmjuGL\*.h %INC%\
copy %SRC%\AmjuGLDX9\*.h %INC%\
copy %SRC%\AmjuGLDX11\*.h %INC%\
copy %SRC%\AmjuGLOpenGL\*.h %INC%\
copy %SRC%\Collision\*.h %INC%\
copy %SRC%\Events\*.h %INC%\
copy %SRC%\EventsSDL\*.h %INC%\
copy %SRC%\EventsWin32\*.h %INC%\
copy %SRC%\FileUtils\*.h %INC%\
copy %SRC%\Font\*.h %INC%\
copy %SRC%\Game\*.h %INC%\
copy %SRC%\Geom\*.h %INC%\
copy %SRC%\GeomDraw\*.h %INC%\
copy %SRC%\GeomLoad\*.h %INC%\
copy %SRC%\Gui\*.h %INC%\
copy %SRC%\Image\*.h %INC%\
copy %SRC%\Localise\*.h %INC%\
copy %SRC%\Lua\*.h %INC%\
copy %SRC%\Main\*.h %INC%\
copy %SRC%\Md2\*.h %INC%\
copy %SRC%\Network\*.h %INC%\
copy %SRC%\Obj\*.h %INC%\
copy %SRC%\Resource\*.h %INC%\
copy %SRC%\Scene\*.h %INC%\
copy %SRC%\Sound\*.h %INC%\
copy %SRC%\SoundBass\*.h %INC%\
copy %SRC%\SoundSDLMixer\*.h %INC%\
copy %SRC%\Strings\*.h %INC%\
copy %SRC%\Thread\*.h %INC%\
copy %SRC%\TimeUtils\*.h %INC%\
copy %SRC%\Utils\*.h %INC%\



