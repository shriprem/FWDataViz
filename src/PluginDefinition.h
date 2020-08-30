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

#ifndef PLUGINDEFINITION_H
#define PLUGINDEFINITION_H

#define WIN32_LEAN_AND_MEAN      // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <Shlwapi.h>
#include <shellapi.h>
#include <CommCtrl.h>
#include <string>

#include "NPP/PluginInterface.h"
#include "ConfigIO.h"
#include "Resources/resource.h"
#include "Resources/localization.h"

const int nbFunc = 2;

void pluginInit(HANDLE hModule);
void pluginCleanUp();
void commandMenuInit();
void commandMenuCleanUp();
bool setCommand(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk = NULL, bool checkOnInit = false);
HWND createToolTip(HWND hDlg, int toolID, LPWSTR pTitle, LPWSTR pMessage);

// Plugin Command Functions
void ToggleVisualizerPanel();
void ShowVisualizerPanel(bool show);

void ShowAboutDialog();

#endif //PLUGINDEFINITION_H
