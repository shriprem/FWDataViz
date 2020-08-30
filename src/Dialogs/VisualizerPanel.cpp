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

#include "VisualizerPanel.h"
#include <time.h>
#include <wchar.h>

INT_PTR CALLBACK VisualizerPanel::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
   switch (message) {
   case WM_COMMAND:
      switch LOWORD(wParam) {
      case IDC_VIZPANEL_FILETYPE_SELECT:
         break;

      case IDOK:
         visualizeFile();
         break;

      case IDC_VIZPANEL_CLEAR_BUTTON:
         visualizeClear();
         break;

      case IDCANCEL:
      case IDCLOSE:
         setFocusOnEditor();
         ShowVisualizerPanel(false);
         break;
      }

      break;

   case WM_LBUTTONDOWN:
   case WM_MBUTTONDOWN:
   case WM_RBUTTONDOWN:
      ::SetFocus(_hSelf);
      break;

   case WM_SETFOCUS:
      break;

   default :
      return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
   }

   return FALSE;
}

void VisualizerPanel::localize() {
   //::SetDlgItemText(_hSelf, IDC_VIZPANEL_FILETYPE_LABEL, GOLINECOL_LABEL_GOLINE);
   //::SetDlgItemText(_hSelf, IDOK, GOLINECOL_LABEL_GOLINE);
   //::SetDlgItemText(_hSelf, IDC_VIZPANEL_CLEAR_BUTTON, GOLINECOL_LABEL_GOLINE);
   //::SetDlgItemText(_hSelf, IDCLOSE, GOLINECOL_LABEL_GOLINE);
}

void VisualizerPanel::display(bool toShow) {
   DockingDlgInterface::display(toShow);
   hFTList = ::GetDlgItem(_hSelf, IDC_VIZPANEL_FILETYPE_SELECT);

   if (toShow) {
      loadFileTypes();
      ::SetFocus(hFTList);
      syncListFileType();
   }
};

void VisualizerPanel::setParent(HWND parent2set) {
   _hParent = parent2set;
};

void VisualizerPanel::loadFileTypes() {
   std::vector<std::wstring> fileTypes;
   std::wstring fileTypeList;

   fileTypeList = _configIO.getConfigString(_configIO.FWDataVizIniFile(), L"Base", L"FileTypes");
   _configIO.Tokenize(fileTypeList, fileTypes);

   mapFileDescToType.clear();
   mapFileTypeToDesc.clear();

   ::SendMessageW(hFTList, CB_RESETCONTENT, NULL, NULL);
   ::SendMessageW(hFTList, CB_ADDSTRING, NULL, (LPARAM)L"-");

   for (auto fType : fileTypes) {
      std::string fTypeAnsi;
      std::wstring fileLabel;

      fTypeAnsi = _configIO.WideToNarrow(fType);
      fileLabel = _configIO.getConfigString(_configIO.FWDataVizIniFile(), fType.c_str(), L"FileLabel");

      mapFileDescToType[fileLabel] = fTypeAnsi;
      mapFileTypeToDesc[fTypeAnsi] = fileLabel;
      ::SendMessageW(hFTList, CB_ADDSTRING, NULL, (LPARAM)fileLabel.c_str());
   }
}

void VisualizerPanel::syncListFileType()
{
   HWND hScintilla = getCurrentScintilla();
   if (!hScintilla)
      return;

   char fType[MAX_PATH];
   std::wstring fDesc;

   ::SendMessage(hScintilla, SCI_GETPROPERTY, (WPARAM)"FWVisualizerType", (LPARAM)fType);
   fDesc = mapFileTypeToDesc[std::string{ fType }];

   ::SendMessage(hFTList, CB_SELECTSTRING, (WPARAM)0, (LPARAM)
      ((::SendMessage(hFTList, CB_FINDSTRING, (WPARAM)0, (LPARAM)fDesc.c_str()) != CB_ERR) ? fDesc.c_str() : L"-"));
}

void VisualizerPanel::visualizeFile()
{
   HWND hScintilla = getCurrentScintilla();
   if (!hScintilla)
      return;

   ::SendMessage(hScintilla, SCI_SETCARETLINEFRAME, (WPARAM)2, NULL);

   wchar_t fDesc[MAX_PATH];
   LPCSTR descType;

   ::SendMessage(hFTList, WM_GETTEXT, MAX_PATH, (LPARAM)fDesc);
   descType = mapFileDescToType[fDesc].c_str();

   if (std::wstring{fDesc}.length() < 2) {
      visualizeClear();
   }
   else {
      ::SendMessage(hScintilla, SCI_SETPROPERTY, (WPARAM)"FWVisualizerType", (LPARAM)descType);
      //::SendMessage(hScintilla, SCI_SETLEXER, (WPARAM)SCLEX_CONTAINER, NULL);
   }
}

void VisualizerPanel::visualizeClear()
{
   HWND hScintilla = getCurrentScintilla();
   if (!hScintilla)
      return;

   ::SendMessage(hScintilla, SCI_SETPROPERTY, (WPARAM)"FWVisualizerType", (LPARAM)L"");
   ::SendMessage(hScintilla, SCI_SETLEXER, (WPARAM)SCLEX_NULL, NULL);
   ::SendMessage(hScintilla, SCI_STYLECLEARALL, 0, 0);
   syncListFileType();
}

/// *** Private Functions: *** ///

HWND VisualizerPanel::getCurrentScintilla() {
   int which = -1;
   ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)& which);
   if (which < 0)
      return (HWND)FALSE;
   return (HWND)(which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
}

int VisualizerPanel::setFocusOnEditor() {
   HWND hScintilla = getCurrentScintilla();
   if (!hScintilla)
      return -1;

   return (int)::SendMessage(hScintilla, SCI_GRABFOCUS, 0, 0);
};
