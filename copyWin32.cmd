@ECHO OFF

SET DM_DIR=DM_includes\Win32
SET BIN_DIR=bin
SET NOTEPAD_DIR=C:\Portable\Notepad++ - x86

COPY license.txt %BIN_DIR%
COPY %DM_DIR%\NPP_Plugin_Darkmode.dll %BIN_DIR%
COPY %DM_DIR%\NPP_Plugin_Darkmode.dll "%NOTEPAD_DIR%"
COPY %DM_DIR%\NPP_Plugin_Darkmode.dll "%NOTEPAD_DIR%\plugins\FWDataViz"
COPY %BIN_DIR%\FWDataViz.dll "%NOTEPAD_DIR%\plugins\FWDataViz"

START /D "%NOTEPAD_DIR%" Notepa~1.exe
EXIT 0
