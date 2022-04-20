@ECHO OFF

SET PLUGIN_NAME=FWDataViz
SET DM_DIR=DM_includes\Win32\
SET BIN_DIR=bin\
SET NOTEPAD_DIR=C:\Portable\Notepad++ - x86\
SET PLUGINS_DIR=%NOTEPAD_DIR%\plugins\%PLUGIN_NAME%\

COPY license.txt %BIN_DIR%
COPY %DM_DIR%\NPP_Plugin_Darkmode.dll %BIN_DIR%
COPY %DM_DIR%\NPP_Plugin_Darkmode.dll "%NOTEPAD_DIR%"
COPY %DM_DIR%\NPP_Plugin_Darkmode.dll "%PLUGINS_DIR%"
COPY %BIN_DIR%\%PLUGIN_NAME%.dll "%PLUGINS_DIR%"

START /D "%NOTEPAD_DIR%" notepa~1.exe
EXIT 0
