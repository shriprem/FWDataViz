@ECHO OFF

SET BIN_DIR=bin64
SET NOTEPAD_DIR=E:\Portable\Notepad++

COPY license.txt %BIN_DIR%
COPY %BIN_DIR%\FWDataViz.dll "%NOTEPAD_DIR%\plugins\FWDataViz\"

START /D "%NOTEPAD_DIR%" Notepa~1.exe
EXIT 0
