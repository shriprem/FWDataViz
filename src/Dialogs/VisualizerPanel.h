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

#ifndef VISUALIZER_DLG_H
#define VISUALIZER_DLG_H

#include "../PluginDefinition.h"
#include "../NPP/DockingDlgInterface.h"
#include <regex>

#define FW_DEBUG_LOAD_STYLES FALSE
#define FW_DEBUG_SET_STYLES FALSE
#define FW_DEBUG_LOAD_REGEX FALSE
#define FW_DEBUG_APPLY_LEXER FALSE

#define FW_DOC_FILE_TYPE "FWVisualizerType"
#define FW_DOC_FILE_THEME "FWVisualizerTheme"
#define FW_STYLE_RANGE_START 101
#define FW_LINE_MAX_LENGTH 32767

extern NppData nppData;
extern ConfigIO _configIO;

using std::string;
using std::wstring;
using std::regex;
using std::vector;

class VisualizerPanel : public DockingDlgInterface {
public :
   VisualizerPanel() :DockingDlgInterface(IDD_VISUALIZER_DOCKPANEL) {};

   void localize();
   virtual void display(bool toShow=true);
   void setParent(HWND parent2set);
   void loadFileTypes();
   void syncListFileType();
   void visualizeFile();
   void clearVisualize(bool sync=TRUE);
   int loadStyles();
   int applyStyles();
   int loadLexer();
   void applyLexer(const size_t startLine, const size_t endLine);
   void updateCurrentPage();

protected :
   virtual INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
   static HWND getCurrentScintilla();
   bool getDocFileType(HWND hScintilla, wstring &fileType);
   void setDocFileType(HWND hScintilla, wstring fileType);
   static int setFocusOnEditor();
   void clearLexer();

   // File Type data
   HWND hFTList;
   std::unordered_map<wstring, wstring> mapFileDescToType;
   std::unordered_map<wstring, wstring> mapFileTypeToDesc;

   // Styleset data
   struct StyleInfo {
      int backColor;
      int foreColor;
      int bold;
      int italics;
   };

   wstring currentStyleTheme{};
   StyleInfo styleEOL;
   vector<StyleInfo> styleSet;

   // Regex data
   wstring fwVizRegexed{};

   struct FieldInfo {
      wstring recLabel;
      string recMarker;
      regex regexMarker;
      vector<int> startPositions;
      vector<int> fieldWidths;
      vector<wstring> fieldLabels;
   };

   vector<FieldInfo> fieldInfoList;
};

#endif //VISUALIZER_DLG_H
