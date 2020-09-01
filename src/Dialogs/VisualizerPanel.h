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

#ifndef VISUALIZER_DLG_H
#define VISUALIZER_DLG_H

#include "../PluginDefinition.h"
#include "../NPP/DockingDlgInterface.h"

#define FW_DEBUG_LOAD_STYLES FALSE
#define FW_DEBUG_SET_STYLES FALSE
#define FW_DOC_FILE_TYPE "FWVisualizerType"
#define FW_DOC_FILE_THEME "FWVisualizerTheme"
#define FW_STYLE_RANGE_START 101

extern NppData nppData;
extern ConfigIO _configIO;

static bool idemPotentKey {FALSE};

class VisualizerPanel : public DockingDlgInterface {
public :
   int instance_id{ 0 };
   std::unordered_map<std::wstring, std::wstring> mapFileDescToType;
   std::unordered_map<std::wstring, std::wstring> mapFileTypeToDesc;

   VisualizerPanel() :DockingDlgInterface(IDD_VISUALIZER_DOCKPANEL) {};

   void localize();
   virtual void display(bool toShow=true);
   void setParent(HWND parent2set);
   void loadFileTypes();
   void syncListFileType();
   void visualizeFile();
   void clearVisualize(bool sync=TRUE);
   int loadStyles();
   int setStyles();

protected :
   virtual INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
   static HWND getCurrentScintilla();
   bool getDocFileType(HWND hScintilla, std::wstring &fileType);
   void setDocFileType(HWND hScintilla, std::wstring fileType);
   static int setFocusOnEditor();

   char callTip[500];
   HWND hFTList;

   struct StyleInfo {
      int backColor;
      int foreColor;
      int bold;
      int italics;
   };

   std::wstring currentStyleTheme{};
   StyleInfo styleEOL;
   std::vector<StyleInfo> styleSet;
};

#endif //VISUALIZER_DLG_H
