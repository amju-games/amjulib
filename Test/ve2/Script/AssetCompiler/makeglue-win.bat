REM makeglue.bat
REM glue every file (which we want) in this directory into a Glue file.

set THIS_DIR=%cd%
set TOP_DIR=%THIS_DIR%\..\..
set COMPILED_ASSETS=%TOP_DIR%\Build\CompiledAssets
set DEST_DIR=%COMPILED_ASSETS%\Win
set SRC_DIR=%TOP_DIR%\Assets

mkdir %COMPILED_ASSETS%
mkdir %DEST_DIR%
mkdir %DEST_DIR%\font2d
rem del /S /Q %DEST_DIR%\*.*

REM Copy other files to compiled dir
REM /Y when we would get overwrite confirm prompt

xcopy /Y /S %SRC_DIR%\*.png %DEST_DIR%
xcopy /Y /S %SRC_DIR%\*.txt %DEST_DIR%
xcopy /Y /S %SRC_DIR%\*.text %DEST_DIR%
xcopy /Y /S %SRC_DIR%\*.attr %DEST_DIR%
xcopy /Y /S %SRC_DIR%\*.bill %DEST_DIR%
xcopy /Y /S %SRC_DIR%\*.move %DEST_DIR%
xcopy /Y /S %SRC_DIR%\*.room %DEST_DIR%
xcopy /Y /S %SRC_DIR%\*.lite %DEST_DIR%
xcopy /Y /S %SRC_DIR%\*.ini %DEST_DIR%
xcopy /Y /S %SRC_DIR%\*.amju2 %DEST_DIR%
xcopy /Y /S %SRC_DIR%\*.comp %DEST_DIR%
xcopy /Y /S %SRC_DIR%\*.md2 %DEST_DIR%

cd %DEST_DIR%

set GLUE_EXE=%THIS_DIR%\glue.exe
set GLUE_FILE=%DEST_DIR%\..\data-win.glue
REM make glue file
%GLUE_EXE% -c %GLUE_FILE%

REM for each file, add to glue file.
for %%f in (*.txt, *.text, *.attr, *.bill, *.move, *.room, *.lite, *.ini, *.amju2, *.md2, *.png, *.obj, *.comp) do %GLUE_EXE% -a %GLUE_FILE% %%f


REM Verify contents
%GLUE_EXE% -d %GLUE_FILE%


cd %THIS_DIR%

