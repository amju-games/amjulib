REM makeglue.bat
REM glue every file (which we want) in this directory into a Glue file.

set THIS_DIR=%cd%
set TOP_DIR=%THIS_DIR%\..\..\..\..
set COMPILED_ASSETS=%TOP_DIR%\Build\CompiledAssets
set DEST_DIR=%COMPILED_ASSETS%\Win
set SRC_DIR=%THIS_DIR%\..\..\Assets
set GUIASSETSDIR=%THIS_DIR%\..\..\..\..\Assets\gui
set FONT2D=%THIS_DIR%\..\..\..\..\Assets\font2d

mkdir %COMPILED_ASSETS%
del /S /Q %DEST_DIR%\*.*

mkdir %DEST_DIR%
mkdir %DEST_DIR%\characters\2d\
mkdir %DEST_DIR%\font2d\
mkdir %DEST_DIR%\obj\
mkdir %DEST_DIR%\locations\location\
mkdir %DEST_DIR%\Sound\

REM Copy other files to compiled dir
REM /Y when we would get overwrite confirm prompt

copy %SRC_DIR%\*.png %DEST_DIR%
copy %SRC_DIR%\*.txt %DEST_DIR%
copy %SRC_DIR%\characters\2d\*.png %DEST_DIR%\characters\2d\
copy %SRC_DIR%\font2d\*.png %DEST_DIR%\font2d\
copy %SRC_DIR%\font2d\*.txt %DEST_DIR%\font2d\
copy %FONT2D%\arial*.png %DEST_DIR%\font2d\
copy %FONT2D%\arial*.txt %DEST_DIR%\font2d\
copy %GUIASSETSDIR%\*.png %DEST_DIR%
copy %GUIASSETSDIR%\*.txt %DEST_DIR%
copy %SRC_DIR%\obj\*.obj %DEST_DIR%\obj\
copy %SRC_DIR%\obj\*.mtl %DEST_DIR%\obj\
copy %SRC_DIR%\obj\*.png %DEST_DIR%\obj\
copy %SRC_DIR%\locations\location1\*.obj %DEST_DIR%\locations\location1\
copy %SRC_DIR%\locations\location1\*.mtl %DEST_DIR%\locations\location1\
copy %SRC_DIR%\locations\location1\*.png %DEST_DIR%\locations\location1\
copy %SRC_DIR%\Sound\*.it  %DEST_DIR%\Sound\
copy %SRC_DIR%\Sound\*.wav %DEST_DIR%\Sound\

cd %DEST_DIR%

set GLUE_EXE=%THIS_DIR%\glue.exe
set GLUE_FILE=%DEST_DIR%\..\data-win.glue
REM make glue file
%GLUE_EXE% -c %GLUE_FILE%

REM for each file, add to glue file.
for %%f in (*.txt, *.png, characters\2d\*, font2d\*, obj\*, locations\location1\*, Sound\*) do %GLUE_EXE% -a %GLUE_FILE% %%f


REM Verify contents
%GLUE_EXE% -d %GLUE_FILE%


cd %THIS_DIR%

