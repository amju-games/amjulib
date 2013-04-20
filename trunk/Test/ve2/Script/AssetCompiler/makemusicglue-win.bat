REM makemusicglue.bat
REM glue sound files into a Glue file.

set THIS_DIR=%cd%
set TOP_DIR=%THIS_DIR%\..\..
set DEST_DIR=%TOP_DIR%\Build\CompiledAssets\Win
set SRC_DIR=%TOP_DIR%\Assets

rem mkdir %DEST_DIR%
rem del /S /Q %DEST_DIR%\*.*


REM Copy other files to compiled dir
xcopy /S /Y %SRC_DIR%\*.wav %DEST_DIR%
xcopy /S /Y %SRC_DIR%\*.it %DEST_DIR%


cd %DEST_DIR%

set GLUE_EXE=%THIS_DIR%\glue.exe
set GLUE_FILE=%DEST_DIR%\..\music-win.glue
REM make glue file
%GLUE_EXE% -c %GLUE_FILE%

REM Add files in sound dir

for %%f in (*.wav) do %GLUE_EXE% -a %GLUE_FILE% %%f

for %%f in (*.it) do %GLUE_EXE% -a %GLUE_FILE% %%f


REM Verify contents
%GLUE_EXE% -d %GLUE_FILE%


cd %THIS_DIR%
