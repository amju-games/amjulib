rem Make binary versions of .obj files

set RESTORE_DIR=%cd%
set TOP_DIR=%RESTORE_DIR%\..\..
set DEST_DIR=%TOP_DIR%\Build\CompiledAssets\Win
mkdir %DEST_DIR%
set SRC_DIR=%TOP_DIR%\Assets

cd %SRC_DIR%

for %%f in (*.obj) do %RESTORE_DIR%\obj2bin %%f %DEST_DIR%\%%f

cd %RESTORE_DIR%
