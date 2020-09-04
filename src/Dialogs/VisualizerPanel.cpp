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
         clearVisualize();
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

   fileTypeList = _configIO.getConfigString(L"Base", L"FileTypes");
   _configIO.Tokenize(fileTypeList, fileTypes);

   mapFileDescToType.clear();
   mapFileTypeToDesc.clear();

   ::SendMessageW(hFTList, CB_RESETCONTENT, NULL, NULL);
   ::SendMessageW(hFTList, CB_ADDSTRING, NULL, (LPARAM)L"-");

   for (auto fType : fileTypes) {
      std::string fTypeAnsi;
      std::wstring fileLabel;

      fileLabel = _configIO.getConfigString(fType.c_str(), L"FileLabel");

      mapFileDescToType[fileLabel] = fType;
      mapFileTypeToDesc[fType] = fileLabel;
      ::SendMessageW(hFTList, CB_ADDSTRING, NULL, (LPARAM)fileLabel.c_str());
   }
}

void VisualizerPanel::syncListFileType() {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   std::wstring fileType;
   std::wstring fDesc;

   if (!getDocFileType(hScintilla, fileType)) {
      ::SendMessage(hFTList, CB_SELECTSTRING, (WPARAM)0, (LPARAM)L"-");
      return;
   }

   fDesc = mapFileTypeToDesc[fileType];

   ::SendMessage(hFTList, CB_SELECTSTRING, (WPARAM)0, (LPARAM)
      ((::SendMessage(hFTList, CB_FINDSTRING, (WPARAM)0, (LPARAM)fDesc.c_str()) != CB_ERR) ? fDesc.c_str() : L"-"));
}

void VisualizerPanel::visualizeFile() {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   wchar_t fDesc[MAX_PATH];
   std::wstring sDesc;

   ::SendMessage(hFTList, WM_GETTEXT, MAX_PATH, (LPARAM)fDesc);
   sDesc =  mapFileDescToType[fDesc];

   if (sDesc.length() < 2) {
      clearVisualize();
      return;
   }

   ::SendMessage(hScintilla, SCI_SETCARETLINEFRAME, (WPARAM)2, NULL);

   clearVisualize(FALSE);
   setDocFileType(hScintilla, sDesc);
   loadStyles();
   applyStyles();
   loadLexer();
}

void VisualizerPanel::clearVisualize(bool sync) {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   ::SendMessage(hScintilla, SCI_STYLECLEARALL, NULL, NULL);
   ::SendMessage(hScintilla, SCI_STARTSTYLING, 0, NULL);
   ::SendMessage(hScintilla, SCI_SETSTYLING,
      ::SendMessage(hScintilla, SCI_GETLENGTH, NULL, NULL), STYLE_DEFAULT);

   setDocFileType(hScintilla, L"");
   clearLexer();

   if (sync) syncListFileType();
}

int VisualizerPanel::loadStyles() {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return -1;

   std::wstring fileType;
   std::wstring fileTheme;

   if (!getDocFileType(hScintilla, fileType)) {
      return 0;
   }

   fileTheme = _configIO.getConfigString(fileType.c_str(), L"FileTheme");
   if (fileTheme.compare(currentStyleTheme) == 0) {
      return 0;
   }

   _configIO.setThemeFilePath(fileTheme);
   currentStyleTheme = fileTheme;

   _configIO.getStyleColor(L"EOL_Back", styleEOL.backColor, FALSE);
   _configIO.getStyleColor(L"EOL_Fore", styleEOL.foreColor, TRUE);
   _configIO.getStyleBool(L"EOL_Bold", styleEOL.bold);
   _configIO.getStyleBool(L"EOL_Italics", styleEOL.italics);

   int styleCount{};
   wchar_t cPre[10];
   std::wstring sPrefix;

   if (_configIO.getStyleValue(L"Count").length() > 0)
      styleCount = std::stoi (_configIO.getStyleValue(L"Count"));

   styleSet.clear();
   styleSet.resize(styleCount);

   for (int i{}; i < styleCount; i++) {
      swprintf(cPre, 10, L"C%02i_", i);
      sPrefix = std::wstring(cPre);
      _configIO.getStyleColor((sPrefix + L"Back").c_str(), styleSet[i].backColor, FALSE);
      _configIO.getStyleColor((sPrefix + L"Fore").c_str(), styleSet[i].foreColor, TRUE);
      _configIO.getStyleBool((sPrefix + L"Bold").c_str(), styleSet[i].bold);
      _configIO.getStyleBool((sPrefix + L"Italics").c_str(), styleSet[i].italics);
   }

#if FW_DEBUG_LOAD_STYLES
   wchar_t test[500];

   for (int i{}; i < styleCount; i++) {
      swprintf(test, 500, L"C0%i_Back = %i\n C0%i_Fore = %i\n C0%i_Bold = %i\nC0%i_Italics = %i\n",
         i, styleSet[i].backColor,
         i, styleSet[i].foreColor,
         i, styleSet[i].bold,
         i, styleSet[i].italics);
      ::MessageBox(NULL, test, L"Theme Styles", MB_OK);
   }
#endif

   return styleCount;
}

int VisualizerPanel::applyStyles() {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return -1;

   if (currentStyleTheme.length() < 1)
      return 0;

   if (::SendMessage(hScintilla, SCI_GETLEXER, NULL, NULL) == SCLEX_CONTAINER)
      return 0;

   ::SendMessage(hScintilla, SCI_STYLESETBACK, (WPARAM)(FW_STYLE_RANGE_START - 1), (LPARAM)styleEOL.backColor);
   ::SendMessage(hScintilla, SCI_STYLESETFORE, (WPARAM)(FW_STYLE_RANGE_START - 1), (LPARAM)styleEOL.foreColor);
   ::SendMessage(hScintilla, SCI_STYLESETBOLD, (WPARAM)(FW_STYLE_RANGE_START - 1), (LPARAM)styleEOL.bold);
   ::SendMessage(hScintilla, SCI_STYLESETITALIC, (WPARAM)(FW_STYLE_RANGE_START - 1), (LPARAM)styleEOL.italics);

   int styleCount{ static_cast<int>(styleSet.size()) };

   for (int i{}; i < styleCount; i++) {
      ::SendMessage(hScintilla, SCI_STYLESETBACK, (WPARAM)(FW_STYLE_RANGE_START + i), (LPARAM)styleSet[i].backColor);
      ::SendMessage(hScintilla, SCI_STYLESETFORE, (WPARAM)(FW_STYLE_RANGE_START + i), (LPARAM)styleSet[i].foreColor);
      ::SendMessage(hScintilla, SCI_STYLESETBOLD, (WPARAM)(FW_STYLE_RANGE_START + i), (LPARAM)styleSet[i].bold);
      ::SendMessage(hScintilla, SCI_STYLESETITALIC, (WPARAM)(FW_STYLE_RANGE_START + i), (LPARAM)styleSet[i].italics);
   }

   ::SendMessage(hScintilla, SCI_SETLEXER, (WPARAM)SCLEX_CONTAINER, NULL);

#if FW_DEBUG_SET_STYLES
   wchar_t test[500];
   int back, fore, bold, italics;

   for (int i{ -1 }; i < styleCount; i++) {
      back = ::SendMessage(hScintilla, SCI_STYLEGETBACK, (WPARAM)(FW_STYLE_RANGE_START + i), NULL);
      fore = ::SendMessage(hScintilla, SCI_STYLEGETFORE, (WPARAM)(FW_STYLE_RANGE_START + i), NULL);
      bold = ::SendMessage(hScintilla, SCI_STYLEGETBOLD, (WPARAM)(FW_STYLE_RANGE_START + i), NULL);
      italics = ::SendMessage(hScintilla, SCI_STYLEGETITALIC, (WPARAM)(FW_STYLE_RANGE_START + i), NULL);

      swprintf(test, 500, L"C0%i_STYLES = %i,%i,%i,%i\n", i, back, fore, bold, italics);
      ::MessageBox(NULL, test, L"Theme Styles", MB_OK);
   }
#endif

   return styleCount;
}

int VisualizerPanel::loadLexer() {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return -1;

   std::wstring fileType;
   std::wstring recTypeList;
   std::vector<std::wstring> recTypes;
   int recTypeCount;

   if (!getDocFileType(hScintilla, fileType)) {
      clearLexer();
      return 0;
   }

   if (fwVizRegexed.compare(fileType) != 0) {
      clearLexer();
   }

   if (regexMarkers.size() > 0) {
      return static_cast<int>(regexMarkers.size());
   }

   recTypeList = _configIO.getConfigString(fileType.c_str(), L"RecordTypes");
   recTypeCount = _configIO.Tokenize(recTypeList, recTypes);

   regexMarkers.resize(recTypeCount);
   fieldInfoList.resize(recTypeCount);

   for (int i{}; i < recTypeCount; i++) {
      regexMarkers[i] = std::regex{
         _configIO.getConfigStringA(fileType.c_str(), (recTypes[i] + L"_Marker").c_str(), L".") };

      std::wstring fieldWidthList;
      std::vector<int> fieldWidths;
      int fieldCount;

      fieldWidthList = _configIO.getConfigString(fileType.c_str(), (recTypes[i] + L"_FieldWidths").c_str());
      fieldCount = _configIO.Tokenize(fieldWidthList, fieldWidths);

      fieldInfoList[i].fieldWidths.clear();
      fieldInfoList[i].fieldWidths.resize(fieldCount);

      fieldInfoList[i].startPositions.clear();
      fieldInfoList[i].startPositions.resize(fieldCount);

      for (int fnum{}, startPos{}; fnum < fieldCount; fnum++) {
         fieldInfoList[i].startPositions[fnum] = startPos;
         fieldInfoList[i].fieldWidths[fnum] = fieldWidths[fnum];

         startPos += fieldWidths[fnum];
      }
   }

   fwVizRegexed = fileType;

#if FW_DEBUG_LOAD_REGEX
   int fieldCount;

   for (int i{}; i < recTypeCount; i++) {
      wchar_t test[2000];
      swprintf(test, 2000, L"\n%s", (recTypes[i] + L"_FieldWidths =").c_str());
      fieldCount = static_cast<int>(fieldInfoList[i].fieldWidths.size());

      for (int j{}; j < fieldCount; j++) {
         swprintf(test, 2000, L"%s (%i, %i),", test,
            fieldInfoList[i].startPositions[j], fieldInfoList[i].fieldWidths[j]);
      }

      ::MessageBox(NULL, test, fwVizRegexed.c_str(), MB_OK);
   }
#endif

   return recTypeCount;
}

void VisualizerPanel::applyLexer(size_t currentPos) {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   std::wstring fileType;
   if (!getDocFileType(hScintilla, fileType)) return;

   //const int styleCount{ static_cast<int>(styleSet.size()) };
   std::string eolMarker;
   size_t eolMarkerLen, startLine, endLine, currentLine;

   eolMarker =  _configIO.getConfigStringA(fileType.c_str(), L"RecordTerminator");
   eolMarkerLen = eolMarker.length();

   startLine = ::SendMessage(hScintilla, SCI_LINEFROMPOSITION,
      ::SendMessage(hScintilla, SCI_GETENDSTYLED, NULL, NULL), NULL);
   endLine = ::SendMessage(hScintilla, SCI_LINEFROMPOSITION, currentPos, NULL);

   char lineText[FW_LINE_MAX_LENGTH];
   std::string recStartText;
   size_t endPos, recStartPos;
   bool newRec{ TRUE };

   currentLine = startLine;

   while (currentLine < endLine) {
      if (::SendMessage(hScintilla, SCI_LINELENGTH, currentLine, NULL) > FW_LINE_MAX_LENGTH) {
         currentLine++;
         continue;
      }

      ::SendMessage(hScintilla, SCI_GETLINE, (WPARAM)lineText, NULL);
      endPos = ::SendMessage(hScintilla, SCI_GETLINEENDPOSITION, currentLine, NULL);

      if (newRec) {
         recStartPos = ::SendMessage(hScintilla, SCI_POSITIONFROMLINE, currentLine, NULL);
         recStartText = lineText;
      }

      currentLine++;

      //if (eolMarkerLen == 0 || std::string{ lineText }.e) {

      //}
   }
}

void VisualizerPanel::onStyleNeeded(SCNotification* notifyCode) {
   if (loadLexer() > 0)
      applyLexer(notifyCode->position);
}

/// *** Private Functions: *** ///

HWND VisualizerPanel::getCurrentScintilla() {
   int which = -1;
   ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)& which);
   if (which < 0)
      return (HWND)FALSE;
   return (HWND)(which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
}

bool VisualizerPanel::getDocFileType(HWND hScintilla, std::wstring& fileType) {
   char fType[MAX_PATH];

   ::SendMessage(hScintilla, SCI_GETPROPERTY, (WPARAM)FW_DOC_FILE_TYPE, (LPARAM)fType);
   fileType = _configIO.NarrowToWide(fType);

   return (fileType.length() > 1);
}

void VisualizerPanel::setDocFileType(HWND hScintilla, std::wstring fileType) {
   ::SendMessage(hScintilla, SCI_SETLEXER, (WPARAM)SCLEX_NULL, NULL);
   ::SendMessage(hScintilla, SCI_SETPROPERTY, (WPARAM)FW_DOC_FILE_TYPE,
      (LPARAM)_configIO.WideToNarrow(fileType).c_str());
}

int VisualizerPanel::setFocusOnEditor() {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla)
      return -1;

   return (int)::SendMessage(hScintilla, SCI_GRABFOCUS, 0, 0);
}
void VisualizerPanel::clearLexer() {
   regexMarkers.clear();
   fieldInfoList.clear();
   fwVizRegexed = L"";
}
