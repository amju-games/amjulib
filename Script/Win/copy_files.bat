REM Make amjulib for Windows
REM Run from <amjulib>/Script/Win

REM Set path to perl
set PERL=\perl-5.10.0\bin\perl.exe

set INC=..\..\Build\amjulib\include\
set SRC=..\..\Source\

REM Create release dir structure
mkdir %INC%

del /Q %INC%\*

REM Copy . headers required by client code to include folder
copy %SRC%\AmjuGL\*.h %INC%\
copy %SRC%\AmjuGLDX9\*.h %INC%\
copy %SRC%\AmjuGLDX11\*.h %INC%\
copy %SRC%\AmjuGLOpenGL\*.h %INC%\
copy %SRC%\Collision\*.h %INC%\
copy %SRC%\Community\*.h %INC%\
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
copy %SRC%\Mem\*.h %INC%\
copy %SRC%\Main\*.h %INC%\
copy %SRC%\Md2\Md2Model.h %INC%\
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
copy %SRC%\Utils\Xml\*.h %INC%\


%PERL% ..\CreateHeaderFile.pl %INC% > %INC%\amju.h

