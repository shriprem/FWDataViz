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
   case WM_COMMAND :
      switch LOWORD(wParam) {
      //case IDC_VISUALIZE_SELECT:
      //   break;

      case IDOK :
         break;

      case IDCANCEL:
      case IDCLOSE :
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
   //::SetDlgItemText(_hSelf, IDC_VISUALIZE_STATIC, GOLINECOL_LABEL_GOLINE);
}

void VisualizerPanel::display(bool toShow) {
   DockingDlgInterface::display(toShow);

   if (toShow) {
      loadFileTypes();
      ::SetFocus(::GetDlgItem(_hSelf, IDC_VIZPANEL_FILETYPE_SELECT));
   }
};

void VisualizerPanel::setParent(HWND parent2set) {
   _hParent = parent2set;
};

void VisualizerPanel::loadPreferences() {
}

void VisualizerPanel::loadFileTypes() {
   std::vector<std::wstring> fileTypes{};
   std::unordered_map<std::wstring, std::wstring> ftMap;
   HWND hFTList{ ::GetDlgItem(_hSelf, IDC_VIZPANEL_FILETYPE_SELECT) };

   std::wstring fileTypeList{ _configIO.getConfigStringW(_configIO.getMainIniFile(), L"Base", L"FileTypes") };
   _configIO.TokenizeW(fileTypeList.c_str(), fileTypes);

   for (auto ft : fileTypes) {
      std::wstring fileLabel{_configIO.getConfigStringW(_configIO.getMainIniFile(), ft.c_str(), L"FileLabel")};
      ftMap[fileLabel] = ft;
      ::SendMessageW(hFTList, CB_ADDSTRING, NULL, (LPARAM)fileLabel.c_str());
   }
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
