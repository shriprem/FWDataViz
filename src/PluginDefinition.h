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

#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <string>

#include <windows.h>
#include <CommCtrl.h>
#include <Shlwapi.h>
#include <shellapi.h>

#include "NPP/PluginInterface.h"
#include "Resources/resource.h"
#include "Resources/control_ids.h"
#include "Resources/FWVIZ_messages.h"
#include "Resources/localization.h"

#define PLUGIN_FOLDER_NAME L"FWDataViz"

using std::string;
using std::string_view;
using std::wstring;
using std::to_string;
using std::to_wstring;

constexpr int FW_LINE_MAX_LENGTH{ 32767 };
constexpr int ADFT_MAX{ 3 };
constexpr int _gLanguage{ LANG_ENGLISH };

const enum MenuIndex {
   MI_FWVIZ_PANEL,
   MI_CONFIG_DIALOG,
   MI_CONFIG_THEMES,
   MI_SEPARATOR_1,
   MI_FIELD_JUMP,
   MI_FIELD_LEFT,
   MI_FIELD_RIGHT,
   MI_FIELD_COPY,
   MI_FIELD_PASTE,
   MI_DATA_EXTRACTION,
   MI_SEPARATOR_2,
   MI_DEMO_SINGLE_REC_FILES,
   MI_DEMO_MULTI_REC_FILES,
   MI_DEMO_MULTI_LINE_FILES,
   MI_SEPARATOR_3,
   MI_ABOUT_DIALOG,
   MI_COUNT
};

typedef LRESULT (*PSCIFUNC_T)(void*, int, WPARAM, LPARAM);


// Internal use functions
void pluginInit(HANDLE hModule);
void pluginCleanUp();
void commandMenuInit();
void commandMenuCleanUp();
bool setCommand(size_t index, const wstring& cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey* sk = NULL, bool checkOnInit = false);


// Internal/External use functions
LRESULT NppMessage(UINT messageID, WPARAM wparam = 0, LPARAM lparam = 0);
HWND GetCurrentScintilla();
bool GetDirectScintillaFunc(PSCIFUNC_T& fn, void*& ptr);

void ShowVisualizerPanel(bool show);
void ToggleVisualizerPanel();
void RefreshVisualizerPanel();

void ShowConfigDialog();
void ShowThemeDialog();

void ShowJumpDialog();
void FieldLeft();
void FieldRight();
void FieldCopy();
void FieldPaste();
void ShowDataExtractDialog();
void ShowAboutDialog();

void RegisterDockPanelIcon();
void RefreshDarkMode();
