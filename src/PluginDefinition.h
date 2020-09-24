//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#pragma once

#define FW_LINE_MAX_LENGTH 32767

#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <string>

#include <windows.h>
#include <CommCtrl.h>
#include <Shlwapi.h>
#include <shellapi.h>

#include "NPP/PluginInterface.h"
#include "Resources/resource.h"
#include "Resources/localization.h"

using std::string;
using std::string_view;
using std::wstring;
using std::to_string;
using std::to_wstring;

static int _gLanguage = LANG_ENGLISH;

const enum MenuIndex {
   MI_GOTO_PANEL,
   MI_CONFIG_DIALOG,
   MI_SEPARATOR,
   MI_ABOUT_DIALOG,
   MI_COUNT
};

typedef LRESULT (*PSCIFUNC_T)(void*, int, WPARAM, LPARAM);

void pluginInit(HANDLE hModule);
void pluginCleanUp();
void commandMenuInit();
void commandMenuCleanUp();
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk=NULL, bool checkOnInit=false);

HWND getCurrentScintilla();
bool getDirectScintillaFunc(PSCIFUNC_T &fn, void* &ptr);
LRESULT nppMessage(UINT messageID, WPARAM wparam, LPARAM lparam);

// Plugin Command Functions
void ToggleVisualizerPanel();
void ShowVisualizerPanel(bool show);

void ShowConfigDialog();
void ShowAboutDialog();
