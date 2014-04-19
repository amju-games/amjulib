set THIS_DIR=%cd%
set TOP_DIR=%THIS_DIR%\..\..
set COMPILED_ASSETS=%TOP_DIR%\Build\CompiledAssets
set DEST_DIR=%COMPILED_ASSETS%\Win
set SRC_DIR=%TOP_DIR%\Assets

mkdir %COMPILED_ASSETS%
mkdir %DEST_DIR%
del /S /Q %DEST_DIR%\*.*

REM Convert obj files to binary format, copy to compiled dir
call MakeObjsBin-win.bat

REM Make data.glue
call makeglue-win.bat

REM Now make music glue file
call makemusicglue-win.bat


