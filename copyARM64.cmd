@ECHO OFF

SET DM_DIR=DM_includes\ARM64
SET BIN_DIR=arm64
REM SET NOTEPAD_DIR=C:\Portable\Notepad++

COPY license.txt %BIN_DIR%
COPY %DM_DIR%\NPP_Plugin_Darkmode.dll %BIN_DIR%
REM COPY %DM_DIR%\NPP_Plugin_Darkmode.dll "%NOTEPAD_DIR%"
REM COPY %BIN_DIR%\FWDataViz.dll "%NOTEPAD_DIR%\plugins\FWDataViz"

REM START /D "%NOTEPAD_DIR%" Notepa~1.exe
EXIT 0
