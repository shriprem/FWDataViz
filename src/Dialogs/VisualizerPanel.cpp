#include "VisualizerPanel.h"
#include "PreferencesDialog.h"
//#include "FieldTypeDialog.h"
#include "JumpToField.h"
#include "DataExtractDialog.h"
#include <WindowsX.h>

extern HINSTANCE _gModule;
extern SubmenuManager _submenu;
extern FuncItem pluginMenuItems[MI_COUNT];
//extern FieldTypeDialog _fieldTypeDlg;

PreferencesDialog _prefsDlg;
JumpToField _jumpDlg;
DataExtractDialog _dataExtractDlg;

INT_PTR CALLBACK VisualizerPanel::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
   switch (message) {
   case WM_COMMAND:
      switch LOWORD(wParam) {
      case IDC_VIZPANEL_FILETYPE_SELECT:
         switch HIWORD(wParam) {
         case CBN_SELCHANGE:
            visualizeFile("", FALSE, FALSE, FALSE);
            break;
         }
         break;

      case IDC_VIZPANEL_INFO_BUTTON:
         ShellExecute(NULL, L"open", VIZPANEL_INFO_README, NULL, NULL, SW_SHOW);
         break;

      case IDC_VIZPANEL_FILE_SAMPLES_BTN:
         popupSamplesMenu();
         break;

      case IDC_VIZPANEL_FILETYPE_CONFIG:
         if (_configIO.fixIfUTF16File(_configIO.CONFIG_VIZ))
            ShowConfigDialog();
         break;

      case IDC_VIZPANEL_THEME_SELECT:
         switch HIWORD(wParam) {
         case CBN_SELCHANGE:
            visualizeTheme();
            break;
         }
         break;

      case IDC_VIZPANEL_THEME_CONFIG:
         if (_configIO.fixIfUTF16File(_configIO.CONFIG_THEMES))
            ShowThemeDialog();
         break;

      case IDC_VIZPANEL_CLEAR_BTN:
         if (_configIO.getPreferenceBool(PREF_CLEARVIZ_AUTO, FALSE)) {
            CheckDlgButton(_hSelf, IDC_VIZPANEL_AUTO_DETECT_FT, BST_UNCHECKED);
            setADFTCheckbox();
         }
         clearVisualize();
         break;

      case IDCANCEL:
      case IDCLOSE:
         setFocusOnEditor();
         ShowVisualizerPanel(FALSE);
         break;

      case IDC_VIZPANEL_PREFERENCES_BTN:
         _prefsDlg.doDialog((HINSTANCE)_gModule);
         break;

      case IDC_VIZPANEL_AUTO_DETECT_FT:
         setADFTCheckbox();
         break;

      case IDC_VIZPANEL_MCBS_OVERRIDE:
         setPanelMBCharState();
         break;

      case IDC_VIZPANEL_CARET_FRAMED:
         ToggleCaretFramedState();
         break;

      case IDC_VIZPANEL_FIELD_COPY_TRIM:
         _configIO.setPreferenceBool(PREF_COPY_TRIM,
            IsDlgButtonChecked(_hSelf, IDC_VIZPANEL_FIELD_COPY_TRIM) == BST_CHECKED);
         break;

      case IDC_VIZPANEL_JUMP_FIELD_BTN:
         showJumpDialog();
         break;

      case IDC_VIZPANEL_FIELD_LEFT_BUTTON:
         fieldLeft();
         break;

      case IDC_VIZPANEL_FIELD_RIGHT_BUTTON:
         fieldRight();
         break;

      case IDC_VIZPANEL_FIELD_COPY_BUTTON:
         fieldCopy();
         break;

      case IDC_VIZPANEL_FIELD_PASTE_BUTTON:
         fieldPaste();
         break;

      case IDC_VIZPANEL_PASTE_LEFT_LABEL:
      case IDC_VIZPANEL_PASTE_RPAD_LABEL:
         setFieldAlign(TRUE);
         break;

      case IDC_VIZPANEL_PASTE_RIGHT_LABEL:
      case IDC_VIZPANEL_PASTE_LPAD_LABEL:
         setFieldAlign(FALSE);
         break;

      case IDC_VIZPANEL_PASTE_RPAD_FIELD:
      case IDC_VIZPANEL_PASTE_LPAD_FIELD:
      {
         int ctrlID{ LOWORD(wParam) };
         bool leftEdge{ ctrlID == IDC_VIZPANEL_PASTE_RPAD_FIELD };

         switch HIWORD(wParam) {
         case EN_CHANGE:
            wchar_t padChars[MAX_PATH];
            GetWindowText(GetDlgItem(_hSelf, ctrlID), padChars, MAX_PATH);
            _configIO.setPreference(leftEdge ? PREF_PASTE_RPAD : PREF_PASTE_LPAD, padChars);
            break;

         case EN_SETFOCUS:
            setFieldAlign(leftEdge);
            break;
         }
         break;
      }

      case IDC_VIZPANEL_EXTRACT_DATA_BTN:
         if (_configIO.fixIfUTF16File(_configIO.CONFIG_EXTRACTS))
            showExtractDialog();
         break;

         //case IDC_FWVIZ_FIELD_TYPE_BUTTON:
         //   _fieldTypeDlg.doDialog((HINSTANCE)_gModule);
         //   break;
      }

      break;

   case WM_LBUTTONDOWN:
   case WM_MBUTTONDOWN:
   case WM_RBUTTONDOWN:
      SetFocus(hFTList);
      break;

   case WM_SHOWWINDOW:
      Utils::checkMenuItem(MI_FWVIZ_PANEL, wParam);
      showCaretFramedState(_configIO.getPreferenceBool(PREF_CARET_FRAMED));
      if (wParam) visualizeFile("", TRUE, TRUE, TRUE);
      break;

   case WM_NOTIFY:
   {
      LPNMHDR pnmh = reinterpret_cast<LPNMHDR>(lParam);
      if (pnmh->hwndFrom == _hParent && LOWORD(pnmh->code) == DMN_CLOSE) {
         unlexed = (_configIO.getPreferenceBool(PREF_CLEARVIZ_PANEL, FALSE));
         if (unlexed) clearVisualize(FALSE);
      }
      break;
   }

   case WM_SIZE:
      resizeCaretFieldInfo(LOWORD(lParam));
      break;

   case WM_INITDIALOG:
      NPPDM_AutoSubclassAndThemeChildControls(_hSelf);
      break;

   case WM_CTLCOLORDLG:
   case WM_CTLCOLORBTN:
   case WM_CTLCOLORLISTBOX:
      if (NPPDM_IsEnabled()) {
         return NPPDM_OnCtlColorDarker(reinterpret_cast<HDC>(wParam));
      }
      break;

   case WM_CTLCOLOREDIT:
      if (NPPDM_IsEnabled()) {
         return NPPDM_OnCtlColorSofter(reinterpret_cast<HDC>(wParam));
      }
      break;

   case WM_CTLCOLORSTATIC:
      switch (GetDlgCtrlID((HWND)lParam)) {
      case IDC_VIZPANEL_THEME_LABEL:
         return NPPDM_OnCtlColorIfEnabled(reinterpret_cast<HDC>(wParam), themeEnabled);

      case IDC_VIZPANEL_PASTE_RPAD_INDIC:
      case IDC_VIZPANEL_PASTE_LPAD_INDIC:
         return NPPDM_OnCtlHiliteIfEnabled(reinterpret_cast<HDC>(wParam), fieldEnabled);

      default:
         if (NPPDM_IsEnabled()) {
            return NPPDM_OnCtlColorDarker(reinterpret_cast<HDC>(wParam));
         }
      }
      break;

   default:
      return DockingDlgInterface::run_dlgProc(message, wParam, lParam);
   }

   return FALSE;
}

void VisualizerPanel::initPanel() {
   bool recentOS = Utils::checkBaseOS(WV_VISTA);
   wstring fontName = recentOS ? L"Consolas" : L"Courier New";
   int fontHeight = recentOS ? 10 : 8;

   using Utils::addTooltip;
   using Utils::loadBitmap;
   using Utils::setFont;

   utf8Config = _configIO.checkConfigFilesforUCS16();
   if (!utf8Config) return;

   ShowWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_CARET_FRAMED), IsFramingControlNeeded());

   setFont(_hSelf, IDC_VIZPANEL_FIELD_LABEL, fontName, fontHeight, FW_BOLD, FALSE, TRUE);
   setFont(_hSelf, IDC_VIZPANEL_FIELD_INFO, fontName, fontHeight);

   loadBitmap(_hSelf, IDC_VIZPANEL_INFO_BUTTON, IDB_VIZ_INFO_BITMAP);
   addTooltip(_hSelf, IDC_VIZPANEL_INFO_BUTTON, NULL, VIZ_PANEL_INFO_TIP, FALSE);

   loadBitmap(_hSelf, IDC_VIZPANEL_FILE_SAMPLES_BTN, IDB_VIZ_FILE_SAMPLES_BITMAP);
   addTooltip(_hSelf, IDC_VIZPANEL_FILE_SAMPLES_BTN, NULL, VIZ_PANEL_FILE_SAMPLES_TIP, FALSE);

   loadBitmap(_hSelf, IDC_VIZPANEL_FILETYPE_CONFIG, IDB_VIZ_FILE_CONFIG_BITMAP);
   Utils::addTooltip(_hSelf, IDC_VIZPANEL_FILETYPE_CONFIG, NULL, VIZ_PANEL_FILE_CONFIG_TIP, FALSE);

   loadBitmap(_hSelf, IDC_VIZPANEL_THEME_CONFIG, IDB_VIZ_COLOR_CONFIG_BITMAP);
   addTooltip(_hSelf, IDC_VIZPANEL_THEME_CONFIG, NULL, VIZ_PANEL_THEME_CONFIG_TIP, FALSE);

   SetWindowText(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_RPAD_FIELD), _configIO.getPreference(PREF_PASTE_RPAD).c_str());
   SetWindowText(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_LPAD_FIELD), _configIO.getPreference(PREF_PASTE_LPAD).c_str());

   addTooltip(_hSelf, IDC_VIZPANEL_CLEAR_BTN, NULL, VIZ_PANEL_CLEAR_BTN_TIP, FW_TIP_MEDIUM, TRUE);
   addTooltip(_hSelf, IDC_VIZPANEL_FIELD_COPY_TRIM, NULL, VIZ_PANEL_FIELD_TRIM_TIP, FW_TIP_SHORT, TRUE);

   addTooltip(_hSelf, IDC_VIZPANEL_FIELD_LEFT_BUTTON, NULL, VIZ_PANEL_FIELD_LEFT_TIP, FW_TIP_SHORT, TRUE);
   addTooltip(_hSelf, IDC_VIZPANEL_FIELD_RIGHT_BUTTON, NULL, VIZ_PANEL_FIELD_RIGHT_TIP, FW_TIP_SHORT, TRUE);

   addTooltip(_hSelf, IDC_VIZPANEL_FIELD_COPY_BUTTON, NULL, VIZ_PANEL_FIELD_COPY_TIP, FW_TIP_MEDIUM, TRUE);
   addTooltip(_hSelf, IDC_VIZPANEL_FIELD_PASTE_BUTTON, NULL, VIZ_PANEL_FIELD_PASTE_TIP, FW_TIP_LONG, TRUE);

   addTooltip(_hSelf, IDC_VIZPANEL_PASTE_LEFT_LABEL, NULL, VIZ_PANEL_FIELD_RPAD_TIP, FW_TIP_LONG, TRUE);
   addTooltip(_hSelf, IDC_VIZPANEL_PASTE_RPAD_LABEL, NULL, VIZ_PANEL_FIELD_RPAD_TIP, FW_TIP_LONG, TRUE);
   addTooltip(_hSelf, IDC_VIZPANEL_PASTE_RPAD_INDIC, NULL, VIZ_PANEL_FIELD_RPAD_TIP, FW_TIP_LONG, TRUE);

   addTooltip(_hSelf, IDC_VIZPANEL_PASTE_RIGHT_LABEL, NULL, VIZ_PANEL_FIELD_LPAD_TIP, FW_TIP_LONG, TRUE);
   addTooltip(_hSelf, IDC_VIZPANEL_PASTE_LPAD_LABEL, NULL, VIZ_PANEL_FIELD_LPAD_TIP, FW_TIP_LONG, TRUE);
   addTooltip(_hSelf, IDC_VIZPANEL_PASTE_LPAD_INDIC, NULL, VIZ_PANEL_FIELD_LPAD_TIP, FW_TIP_LONG, TRUE);

   setFont(_hSelf, IDC_VIZPANEL_MCBS_OVERRIDE_IND, fontName, 9);

   if (_gLanguage != LANG_ENGLISH) localize();
}

void VisualizerPanel::localize() {
   SetWindowText(_hSelf, FWVIZ_DIALOG_TITLE);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_FILETYPE_LABEL, VIZ_PANEL_FILETYPE_LABEL);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_THEME_LABEL, VIZ_PANEL_THEME_LABEL);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_CLEAR_BTN, VIZ_PANEL_CLEAR_BUTTON);
   SetDlgItemText(_hSelf, IDCLOSE, VIZ_PANEL_CLOSE);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_PREFERENCES_BTN, VIZ_PANEL_PREFERENCES);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_AUTO_DETECT_FT, VIZ_PANEL_AUTO_DETECT_FT);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_MCBS_OVERRIDE, VIZ_PANEL_MCBS_OVERRIDE);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_CARET_FRAMED, VIZ_PANEL_CARET_FRAMED);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_FIELD_COPY_TRIM, VIZ_PANEL_FIELD_COPY_TRIM);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_FIELD_LABEL, VIZ_PANEL_FIELD_LABEL);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_JUMP_FIELD_BTN, VIZ_PANEL_JUMP_FIELD_BTN);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_EXTRACT_DATA_BTN, VIZ_PANEL_EXTRACT_DATA_BTN);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_FIELD_COPY_BUTTON, VIZ_PANEL_FIELD_COPY_BTN);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_FIELD_PASTE_BUTTON, VIZ_PANEL_FIELD_PASTE_BTN);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_PASTE_LEFT_LABEL, VIZ_PANEL_PASTE_LEFT_LABEL);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_PASTE_RIGHT_LABEL, VIZ_PANEL_PASTE_RIGHT_LABEL);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_PASTE_RPAD_LABEL, VIZ_PANEL_PASTE_RPAD_LABEL);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_PASTE_LPAD_LABEL, VIZ_PANEL_PASTE_LPAD_LABEL);
}

void VisualizerPanel::display(bool toShow) {
   DockingDlgInterface::display(toShow);

   if (!utf8Config) return;

   if (!toShow) {
      unlexed = (_configIO.getPreferenceBool(PREF_CLEARVIZ_PANEL, FALSE));
      if (unlexed) clearVisualize(FALSE);
      return;
   }

   hFTList = GetDlgItem(_hSelf, IDC_VIZPANEL_FILETYPE_SELECT);
   hThemesLB = GetDlgItem(_hSelf, IDC_VIZPANEL_THEME_SELECT);
   hFieldInfo = GetDlgItem(_hSelf, IDC_VIZPANEL_FIELD_INFO);

   CheckDlgButton(_hSelf, IDC_VIZPANEL_AUTO_DETECT_FT,
      _configIO.getPreferenceBool(PREF_ADFT) ? BST_CHECKED : BST_UNCHECKED);
   showCaretFramedState(_configIO.getPreferenceBool(PREF_CARET_FRAMED));

   initMBCharsCheckbox();

   CheckDlgButton(_hSelf, IDC_VIZPANEL_FIELD_COPY_TRIM,
      _configIO.getPreferenceBool(PREF_COPY_TRIM, FALSE) ? BST_CHECKED : BST_UNCHECKED);

   unlexed = TRUE;
   visualizeFile("", TRUE, TRUE, TRUE);

   if (unlexed)
      SetFocus(hFTList);
   else
      setFocusOnEditor();
}

void VisualizerPanel::initMBCharsCheckbox() {
   int showMCBS{ _configIO.getPreferenceBool(PREF_MBCHARS_SHOW, FALSE) ? SW_SHOW : SW_HIDE };
   ShowWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_MCBS_OVERRIDE), showMCBS);
   ShowWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_MCBS_OVERRIDE_IND), showMCBS);

   wstring mbcState{ _configIO.getPreference(PREF_MBCHARS_STATE, "FT") };
   if (mbcState == L"FT")
      CheckDlgButton(_hSelf, IDC_VIZPANEL_MCBS_OVERRIDE, BST_INDETERMINATE);
   else
      CheckDlgButton(_hSelf, IDC_VIZPANEL_MCBS_OVERRIDE, (mbcState == L"Y") ? BST_CHECKED : BST_UNCHECKED);
}

void VisualizerPanel::setParent(HWND parent2set) {
   _hParent = parent2set;
}

void VisualizerPanel::showCaretFramedState(bool framed) {
   CheckDlgButton(_hSelf, IDC_VIZPANEL_CARET_FRAMED, framed ? BST_CHECKED : BST_UNCHECKED);
}

void VisualizerPanel::loadListFileTypes() {
   SendMessage(hFTList, CB_RESETCONTENT, NULL, NULL);

   if (!utf8Config) return;

   vector<string> fileTypes;
   _configIO.getConfigValueList(fileTypes, "Base", "FileTypes");

   mapFileDescToType.clear();
   mapFileTypeToDesc.clear();

   SendMessage(hFTList, CB_ADDSTRING, NULL, (LPARAM)L"-");

   for (string fType : fileTypes) {
      wstring fileLabel{ _configIO.getConfigWideChar(fType, "FileLabel") };

      mapFileDescToType[fileLabel] = fType;
      mapFileTypeToDesc[fType] = fileLabel;
      SendMessage(hFTList, CB_ADDSTRING, NULL, (LPARAM)fileLabel.c_str());
   }
}

void VisualizerPanel::loadListThemes() {
   SendMessage(hThemesLB, CB_RESETCONTENT, NULL, NULL);

   if (!utf8Config) return;

   vector<wstring> themesList;
   _configIO.getThemesList(themesList);

   for (const wstring theme : themesList) {
      SendMessage(hThemesLB, CB_ADDSTRING, NULL, (LPARAM)theme.c_str());
   }
}

void VisualizerPanel::syncListFileTypes() {
   if (!isVisible()) return;

   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   string fileType;

   getDocFileType(hScintilla, fileType);
   _configIO.setVizConfig(fileType);

   if (fileType.length() < 1 && _configIO.getPreferenceBool(PREF_ADFT))
      detectFileType(hScintilla, fileType);

   loadListFileTypes();
   enableThemeList(fileType.length() > 0);

   if (fileType.length() < 1) {
      SendMessage(hFTList, CB_SETCURSEL, (WPARAM)0, NULL);
      return;
   }

   SendMessage(hFTList, CB_SETCURSEL, (WPARAM)
      SendMessage(hFTList, CB_FINDSTRING, (WPARAM)-1, (LPARAM)mapFileTypeToDesc[fileType].c_str()), NULL);
}

void VisualizerPanel::syncListThemes() {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   wstring theme;

   getDocTheme(hScintilla, theme);
   loadListThemes();

   if (theme.length() < 1) {
      SendMessage(hThemesLB, CB_SETCURSEL, (WPARAM)-1, NULL);
      return;
   }

   SendMessage(hThemesLB, CB_SETCURSEL, (WPARAM)
      SendMessage(hThemesLB, CB_FINDSTRING, (WPARAM)-1, (LPARAM)theme.c_str()), NULL);
}

void VisualizerPanel::enableFieldControls(bool enable) {
   if (!isVisible()) return;

   EnableWindow(hFieldInfo, enable);

   bool recEnabled{ enable && (caretRecordRegIndex >= 0) };
   EnableWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_JUMP_FIELD_BTN), recEnabled);
   EnableWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_EXTRACT_DATA_BTN), recEnabled);
   EnableWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_FIELD_LEFT_BUTTON), recEnabled);
   EnableWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_FIELD_RIGHT_BUTTON), recEnabled);

   fieldEnabled = recEnabled && (caretFieldIndex >= 0);
   EnableWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_FIELD_COPY_BUTTON), fieldEnabled);
   EnableWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_FIELD_PASTE_BUTTON), fieldEnabled);

   EnableWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_RPAD_FIELD), fieldEnabled);
   InvalidateRect(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_RPAD_INDIC), NULL, TRUE);
   EnableWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_LPAD_FIELD), fieldEnabled);
   InvalidateRect(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_LPAD_INDIC), NULL, TRUE);

   HMENU hPluginMenu = (HMENU)nppMessage(NPPM_GETMENUHANDLE, 0, 0);

   UINT recMenu{ static_cast<UINT>(MF_BYCOMMAND | (recEnabled ? MF_ENABLED : MF_DISABLED)) };
   EnableMenuItem(hPluginMenu, (UINT)pluginMenuItems[MI_FIELD_JUMP]._cmdID, recMenu);
   EnableMenuItem(hPluginMenu, (UINT)pluginMenuItems[MI_DATA_EXTRACTION]._cmdID, recMenu);
   EnableMenuItem(hPluginMenu, (UINT)pluginMenuItems[MI_FIELD_LEFT]._cmdID, recMenu);
   EnableMenuItem(hPluginMenu, (UINT)pluginMenuItems[MI_FIELD_RIGHT]._cmdID, recMenu);

   UINT fieldMenu{ static_cast<UINT>(MF_BYCOMMAND | (fieldEnabled ? MF_ENABLED : MF_DISABLED)) };
   EnableMenuItem(hPluginMenu, (UINT)pluginMenuItems[MI_FIELD_COPY]._cmdID, fieldMenu);
   EnableMenuItem(hPluginMenu, (UINT)pluginMenuItems[MI_FIELD_PASTE]._cmdID, fieldMenu);
}

void VisualizerPanel::enableThemeList(bool enable) {
   if (!isVisible()) return;

   themeEnabled = enable;
   InvalidateRect(GetDlgItem(_hSelf, IDC_VIZPANEL_THEME_LABEL), NULL, TRUE);
   EnableWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_THEME_SELECT), enable);
}

void VisualizerPanel::visualizeFile(string fileType, bool ab_cachedFT, bool autoFT, bool syncFT) {
   if (!isVisible()) return;

   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   if (ab_cachedFT) {
      getDocFileType(hScintilla, fileType);
      _configIO.setVizConfig(fileType);
   }

   if (fileType.length() < 1) {
      if (autoFT) {
         if (_configIO.getPreferenceBool(PREF_ADFT))
            detectFileType(hScintilla, fileType);
      }
      else {
         wchar_t fDesc[MAX_PATH]{};
         SendMessage(hFTList, WM_GETTEXT, MAX_PATH, (LPARAM)fDesc);
         fileType = mapFileDescToType[fDesc];
      }
   }

   if (IsWindowVisible(GetDlgItem(_hSelf, IDC_VIZPANEL_MCBS_OVERRIDE_IND)))
      setPanelMBCharIndicator(fileType);

   if (fileType.length() < 2) {
      syncListFileTypes();
      syncListThemes();
      return;
   }

   clearVisualize(FALSE);
   setDocFileType(hScintilla, fileType);
   if (syncFT) syncListFileTypes();

   setDocTheme(hScintilla, fileType, "");
   syncListThemes();

   loadUsedThemes();
   loadLexer();
   renderCurrentPage();
   setFocusOnEditor();
}

void VisualizerPanel::jumpToField(const string fileType, const int recordIndex, const int fieldIdx) {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   string currFileType{};
   if (!getDocFileType(hScintilla, currFileType) || (fileType != currFileType)) {
      MessageBox(_hSelf, VIZ_PANEL_JUMP_CHANGED_DOC, VIZ_PANEL_JUMP_FIELD_TITLE, MB_OK | MB_ICONSTOP);
      return;
   }

   if (recordIndex != caretRecordRegIndex) {
      MessageBox(_hSelf, VIZ_PANEL_JUMP_CHANGED_REC, VIZ_PANEL_JUMP_FIELD_TITLE, MB_OK | MB_ICONSTOP);
      return;
   }

   moveToFieldEdge(fileType, fieldIdx, TRUE, FALSE, TRUE);
}

void VisualizerPanel::fieldLeft() {
   moveToFieldEdge("", caretFieldIndex, FALSE, FALSE, FALSE);
}

void VisualizerPanel::fieldRight() {
   moveToFieldEdge("", caretFieldIndex, FALSE, TRUE, FALSE);
}

void VisualizerPanel::fieldCopy() {
   if (caretFieldIndex < 0) return;

   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   int leftPos{}, rightPos{};
   if (getFieldEdges("", caretFieldIndex, 0, leftPos, rightPos) < 0) return;

   int fieldLen{ rightPos - leftPos };
   if (fieldLen < 1) return;

   // if no trimming is required, copy to clipbard and return early
   if (!_configIO.getPreferenceBool(PREF_COPY_TRIM, FALSE)) {
      SendMessage(hScintilla, SCI_COPYRANGE, leftPos, rightPos);
      return;
   }

   string padText{};
   padText = Utils::WideToNarrow(_configIO.getPreference(leftAlign ? PREF_PASTE_RPAD : PREF_PASTE_LPAD));
   if (padText.length() < 1) padText = " ";

   int padLen{ static_cast<int>(padText.length()) };
   int leftTrimLen{}, rightTrimLen{};

   string colText(fieldLen + 1, '\0');
   Sci_TextRange sciTR{};

   sciTR.lpstrText = colText.data();
   sciTR.chrg.cpMin = static_cast<long>(leftPos);
   sciTR.chrg.cpMax = static_cast<long>(rightPos);
   SendMessage(hScintilla, SCI_GETTEXTRANGE, NULL, (LPARAM)&sciTR);

   colText = string{ colText.c_str() };

   if (leftAlign && fieldLen >= padLen) {
      // find right-most full/partial match
      int lastPadLen{ padLen };
      int matchStart{ fieldLen - padLen };
      int matchingPos{};

      while (lastPadLen > 0) {
         bool matchFailed{ FALSE };

         for (int i{}; i < lastPadLen; i++) {
            matchingPos = matchStart + i;
            if (colText.at(matchingPos) != padText.at(i)) {
               matchFailed = TRUE;
               lastPadLen--;
               matchStart++;
               break;
            }
         }
         if (!matchFailed && (matchingPos == fieldLen - 1)) break;
      }

#if FW_DEBUG_COPY_TRIM
      MessageBoxA(_hSelf, ("(" + to_string(colText.length()) + ", " + to_string(padText.length()) + ")").c_str(),
         "(ColLen, PadLen)", MB_OK);
      MessageBoxA(_hSelf, ("(" + to_string(lastPadLen) + ", " + to_string(matchStart) + ", " +
         to_string(matchingPos) + ")").c_str(), "(LastPadLen, MatchStart, MatchPos)", MB_OK);
#endif

      rightTrimLen = lastPadLen;

      // if right-most match found, find prior matches
      if (lastPadLen > 0) {
         while (fieldLen >= rightTrimLen + padLen) {
            if (colText.substr(fieldLen - rightTrimLen - padLen, padLen) == padText)
               rightTrimLen += padLen;
            else
               break;
         }
      }
   }
   else if (!leftAlign) {
      // trim LPADs for right align
      bool keepTrimming{ TRUE };
      while (keepTrimming && (leftTrimLen < fieldLen)) {
         for (int i{}; i < padLen; i++) {
            if (colText.at(leftTrimLen) == padText.at(i)) {
               leftTrimLen++;
            }
            else {
               keepTrimming = FALSE;
               break;
            }
         }
      }
   }

   if (leftPos + leftTrimLen < rightPos - rightTrimLen)
      SendMessage(hScintilla, SCI_COPYRANGE, leftPos + leftTrimLen, rightPos - rightTrimLen);

#if FW_DEBUG_COPY_TRIM
   colText = colText.substr(leftTrimLen, colText.length() - leftTrimLen - rightTrimLen);
   MessageBoxA(_hSelf, ("(" + to_string(leftTrimLen) + ", " + to_string(rightTrimLen) + ")").c_str(),
      "(LeftTrimLen, RightTrimLen)", MB_OK);
   MessageBox(_hSelf, Utils::NarrowToWide("<|" + colText + "|>").c_str(),
      Utils::NarrowToWide("<|" + padText + "|>").c_str(), MB_OK);
#endif
}

void VisualizerPanel::fieldPaste() {
   if (caretFieldIndex < 0) return;

   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   int leftPos{}, rightPos{};
   if (getFieldEdges("", caretFieldIndex, 0, leftPos, rightPos) < 0) return;

   int fieldCurrLen{ rightPos - leftPos };
   if (fieldCurrLen < 1) return;

   int fieldLength{ recInfoList[caretRecordRegIndex].fieldWidths[caretFieldIndex] };

   wstring clipText;
   Utils::getClipboardText(GetParent(_hSelf), clipText);

   int clipLength{ static_cast<int>(clipText.length()) };
   if (clipLength < 1) return;

   if (clipLength > fieldLength) {
      clipText = clipText.substr(leftAlign ? 0 : (clipLength - fieldLength), fieldLength);
   }
   else if (clipLength < fieldLength) {
      int gapLength{ fieldLength - clipLength };

      wstring padText{ _configIO.getPreference(leftAlign ? PREF_PASTE_RPAD : PREF_PASTE_LPAD) };
      if (padText.length() < 1) padText = L" ";

      wstring fillText{};
      while (static_cast<int>(fillText.length()) < gapLength) {
         fillText.append(padText);
      }
      fillText = fillText.substr(0, gapLength);

      clipText = leftAlign ? (clipText + fillText) : (fillText + clipText);
   }

   SendMessage(hScintilla, SCI_DELETERANGE, leftPos, fieldCurrLen);
   SendMessage(hScintilla, SCI_INSERTTEXT, leftPos, (LPARAM)(Utils::WideToNarrow(clipText).c_str()));
}

void VisualizerPanel::visualizeTheme() {
   if (!isVisible()) return;

   wchar_t fDesc[MAX_PATH]{};

   SendMessage(hThemesLB, WM_GETTEXT, MAX_PATH, (LPARAM)fDesc);
   wstring theme{ fDesc };

   if (theme.length() < 2) {
      clearVisualize();
      return;
   }

   setDocTheme(getCurrentScintilla(), "", Utils::WideToNarrow(theme));
   loadUsedThemes();
   renderCurrentPage();
}

void VisualizerPanel::clearVisualize(bool sync) {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   SendMessage(hScintilla, SCI_STARTSTYLING, 0, NULL);
   SendMessage(hScintilla, SCI_SETSTYLING,
      SendMessage(hScintilla, SCI_GETLENGTH, NULL, NULL), STYLE_DEFAULT);

   setDocFileType(hScintilla, "");
   setDocTheme(hScintilla, "", "");
   clearLexer();

   if (sync) {
      syncListFileTypes();
      syncListThemes();
   }
}

int VisualizerPanel::loadTheme(const wstring theme) {
   PSCIFUNC_T sciFunc;
   void* sciPtr;

   if (!getDirectScintillaFunc(sciFunc, sciPtr)) return -1;

   int styleCount{ Utils::StringtoInt(_configIO.getStyleValue(theme, "Count")) };

   // Do not load more than FW_STYLE_THEMES_MAX_ITEMS styles (including EOL styleInfo)
   styleCount = (loadedStyleCount + styleCount >= FW_STYLE_THEMES_MAX_ITEMS) ?
      (FW_STYLE_THEMES_MAX_ITEMS - loadedStyleCount - 1) : styleCount;
   if (styleCount < 1) return 0;

   int styleIndex{ FW_STYLE_THEMES_START_INDEX + loadedStyleCount };

   ThemeInfo TI{};
   TI.name = theme;
   TI.styleCount = styleCount;
   TI.rangeStartIndex = styleIndex + 1; // Offset by 1 to account for EOL Style
   themeSet.emplace_back(TI);

   StyleInfo styleInfo{};

   _configIO.getFullStyle(theme, "EOL", styleInfo);
   sciFunc(sciPtr, SCI_STYLESETBACK, (WPARAM)styleIndex, (LPARAM)styleInfo.backColor);
   sciFunc(sciPtr, SCI_STYLESETFORE, (WPARAM)styleIndex, (LPARAM)styleInfo.foreColor);
   sciFunc(sciPtr, SCI_STYLESETBOLD, (WPARAM)styleIndex, (LPARAM)styleInfo.bold);
   sciFunc(sciPtr, SCI_STYLESETITALIC, (WPARAM)styleIndex, (LPARAM)styleInfo.italics);
   styleIndex++;

   char bufKey[8];
   for (int i{}; i < styleCount; i++) {
      snprintf(bufKey, 8, "BFBI_%02i", i);
      _configIO.getFullStyle(theme, bufKey, styleInfo);

      sciFunc(sciPtr, SCI_STYLESETBACK, (WPARAM)styleIndex, (LPARAM)styleInfo.backColor);
      sciFunc(sciPtr, SCI_STYLESETFORE, (WPARAM)styleIndex, (LPARAM)styleInfo.foreColor);
      sciFunc(sciPtr, SCI_STYLESETBOLD, (WPARAM)styleIndex, (LPARAM)styleInfo.bold);
      sciFunc(sciPtr, SCI_STYLESETITALIC, (WPARAM)styleIndex, (LPARAM)styleInfo.italics);
      styleIndex++;
   }

#if FW_DEBUG_SET_STYLES
   wstring dbgMessage;
   int back, fore, bold, italics;

   for (int i{ styleIndex - styleCount }; i < styleIndex; i++) {
      back = sciFunc(sciPtr, SCI_STYLEGETBACK, (WPARAM)i, NULL);
      fore = sciFunc(sciPtr, SCI_STYLEGETFORE, (WPARAM)i, NULL);
      bold = sciFunc(sciPtr, SCI_STYLEGETBOLD, (WPARAM)i, NULL);
      italics = sciFunc(sciPtr, SCI_STYLEGETITALIC, (WPARAM)i, NULL);

      dbgMessage = L"C0" + to_wstring(i - styleIndex + styleCount) + L"_STYLES = " +
         to_wstring(back) + L", " + to_wstring(fore) + L", " +
         to_wstring(bold) + L", " + to_wstring(italics);
      MessageBox(_hSelf, dbgMessage.c_str(), L"Theme Styles", MB_OK);
   }
#endif


   return styleCount + 1;  // Add 1 to include EOL styleInfo
}

int VisualizerPanel::loadUsedThemes() {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return -1;

   loadedStyleCount = 0;
   themeSet.clear();

   string fileType;
   if (!getDocFileType(hScintilla, fileType)) return 0;
   _configIO.setVizConfig(fileType);

   wstring fileTheme;
   if (!getDocTheme(hScintilla, fileTheme)) return 0;

   loadedStyleCount += loadTheme(fileTheme);

   // Load Record Type themes different than File Type theme
   vector<string> recTypesList;
   int recTypeCount{ _configIO.getConfigValueList(recTypesList, fileType, "RecordTypes") };

   for (int i{}; i < recTypeCount; i++) {
      wstring recTheme{};
      recTheme = _configIO.getConfigWideChar(fileType, (recTypesList[i] + "_Theme"));
      if (recTheme == L"") continue;

      bool loaded{ FALSE };
      for (size_t j{}; j < themeSet.size(); j++) {
         if (recTheme == themeSet[j].name) {
            loaded = TRUE;
            break;
         }
      }

      if (!loaded)
         loadedStyleCount += loadTheme(recTheme);
   }

   return static_cast<int>(themeSet.size());
}

int VisualizerPanel::loadLexer() {
   if (unlexed && !isVisible()) return 0;

   PSCIFUNC_T sciFunc;
   void* sciPtr;

   if (!getDirectScintillaFunc(sciFunc, sciPtr)) return -1;

   string fileType;
   if (!getDocFileType(sciFunc, sciPtr, fileType)) {
      clearLexer();
      return 0;
   }

   if (fwVizRegexed.compare(fileType) != 0)
      clearLexer();

   if (recInfoList.size() > 0)
      return static_cast<int>(recInfoList.size());

   const std::wregex trimSpace{ std::wregex(L"(^( )+)|(( )+$)") };
   int styleIndex{ FW_STYLE_THEMES_START_INDEX + FW_STYLE_THEMES_MAX_ITEMS };
   struct loadedStyle {
      int index;
      string style;
   };
   vector<loadedStyle> loadedStyles{};

   vector<string> recTypes;
   int recTypeCount{ _configIO.getConfigValueList(recTypes, fileType, "RecordTypes") };

   recInfoList.resize(recTypeCount);

   for (int i{}; i < recTypeCount; i++) {
      string& recType = recTypes[i];
      RecordInfo& RT = recInfoList[i];

      RT.label = _configIO.getConfigWideChar(fileType, (recType + "_Label"), recType, "");

      RT.marker = _configIO.getConfigStringA(fileType, (recType + "_Marker"), ".", "");
      if (Utils::isInvalidRegex(RT.marker)) RT.marker = "";

      RT.regExpr = regex{ RT.marker + ".*" };
      RT.theme = _configIO.getConfigWideChar(fileType, (recType + "_Theme"));

      string fieldWidthList;
      int fieldCount;

      fieldWidthList = _configIO.getConfigStringA(fileType, (recType + "_FieldWidths"));
      fieldCount = _configIO.Tokenize(fieldWidthList, RT.fieldWidths);

      RT.fieldStarts.clear();
      RT.fieldStarts.resize(fieldCount);

      for (int fnum{}, startPos{}; fnum < fieldCount; fnum++) {
         RT.fieldStarts[fnum] = startPos;
         startPos += RT.fieldWidths[fnum];
      }

      wstring fieldLabelList, fieldType;
      size_t colonPos{};

      fieldLabelList = _configIO.getConfigWideChar(fileType, (recType + "_FieldLabels"));
      fieldCount = _configIO.Tokenize(fieldLabelList, RT.fieldLabels);

      RT.fieldStyles.clear();
      RT.fieldStyles.resize(fieldCount);

      for (int fnum{}; fnum < fieldCount; fnum++) {
         wstring& field{ RT.fieldLabels[fnum] };
         RT.fieldStyles[fnum] = -1;

         colonPos = field.find(':');
         if (colonPos != string::npos) {
            fieldType = field.substr(colonPos + 1);
            fieldType = regex_replace(fieldType, trimSpace, L"");

            string styleText{ _configIO.getFieldStyleText(fieldType) };
            if (styleText.length() != 16) continue;

            // If this styleInfo is already loaded, just map to that styleInfo slot
            bool styleMatched{ FALSE };
            for (size_t k{}; k < loadedStyles.size(); k++) {
               if (styleText == loadedStyles[k].style) {
                  RT.fieldStyles[fnum] = loadedStyles[k].index;
                  styleMatched = TRUE;
                  break;
               }
            }
            if (styleMatched) continue;

            if (styleIndex < FW_STYLE_FIELDS_MIN_INDEX) continue;

            StyleInfo fieldStyle;
            _configIO.parseFieldStyle(styleText, fieldStyle);

            sciFunc(sciPtr, SCI_STYLESETBACK, (WPARAM)styleIndex, (LPARAM)fieldStyle.backColor);
            sciFunc(sciPtr, SCI_STYLESETFORE, (WPARAM)styleIndex, (LPARAM)fieldStyle.foreColor);
            sciFunc(sciPtr, SCI_STYLESETBOLD, (WPARAM)styleIndex, (LPARAM)fieldStyle.bold);
            sciFunc(sciPtr, SCI_STYLESETITALIC, (WPARAM)styleIndex, (LPARAM)fieldStyle.italics);

            RT.fieldStyles[fnum] = styleIndex;
            loadedStyles.emplace_back(loadedStyle{ styleIndex, styleText });
            styleIndex--;
         }
      }
   }

   fwVizRegexed = fileType;
   unlexed = FALSE;

#if FW_DEBUG_LOAD_REGEX
   int fieldCount;

   for (int i{}; i < recTypeCount; i++) {
      wstring dbgMessage;
      wstring& recType = recTypes[i];
      RecordInfo& RT = recInfoList[i];

      dbgMessage = recType + L"\nRec_Label = " + RT.label +
         L"\nRec_Marker = " + _configIO.NarrowToWide(RT.marker) +
         L"\nRec_Theme = " + RT.theme +
         L"\nFieldWidths=\n";

      fieldCount = static_cast<int>(RT.fieldWidths.size());

      for (int j{}; j < fieldCount; j++) {
         dbgMessage += L" (" + to_wstring(RT.fieldStarts[j]) + L", " + to_wstring(RT.fieldWidths[j]) + L"),";
      }

      MessageBox(_hSelf, dbgMessage.c_str(), L"", MB_OK);
   }
#endif

   return recTypeCount;
}

void VisualizerPanel::applyLexer(const size_t startLine, const size_t endLine) {
   if (unlexed && !isVisible()) return;

   PSCIFUNC_T sciFunc;
   void* sciPtr;

   if (!getDirectScintillaFunc(sciFunc, sciPtr)) return;

   string fileType;
   if (!getDocFileType(sciFunc, sciPtr, fileType)) return;

   wstring fileTheme;
   if (!getDocTheme(sciFunc, sciPtr, fileTheme)) return;

   if (themeSet.size() < 1) return;

   int shiftPerRec{ themeSet[0].styleCount };
   if (shiftPerRec < 1) return;

   // set shiftPerRec to an integer just less than half of base theme's styleCount
   shiftPerRec = (shiftPerRec < 5) ? 1 : ((shiftPerRec + 1) >> 1) - 1;

   char lineTextCStr[FW_LINE_MAX_LENGTH]{};
   string recStartText{}, eolMarker{};
   size_t caretLine, eolMarkerLen, eolMarkerPos, recStartLine{},
      currentPos, startPos, endPos, recStartPos{};

   const size_t regexedCount{ recInfoList.size() };
   bool newRec{ TRUE };

   eolMarker = _configIO.getConfigStringA(fileType, "RecordTerminator");
   eolMarkerLen = eolMarker.length();

   bool byteCols{ !_configIO.getMultiByteLexing(fileType) };

   caretLine = sciFunc(sciPtr, SCI_LINEFROMPOSITION,
      sciFunc(sciPtr, SCI_GETCURRENTPOS, NULL, NULL), NULL);

   caretRecordRegIndex = -1;
   caretRecordStartPos = 0;
   caretRecordEndPos = 0;

#if FW_DEBUG_LEXER_COUNT
   ++lexCount;
#endif

   for (auto currentLine{ startLine }; currentLine < endLine; currentLine++) {
      if (sciFunc(sciPtr, SCI_LINELENGTH, currentLine, NULL) > FW_LINE_MAX_LENGTH)
         continue;

      sciFunc(sciPtr, SCI_GETLINE, (WPARAM)currentLine, (LPARAM)lineTextCStr);
      startPos = sciFunc(sciPtr, SCI_POSITIONFROMLINE, currentLine, NULL);
      endPos = sciFunc(sciPtr, SCI_GETLINEENDPOSITION, currentLine, NULL);
      string_view lineText{ lineTextCStr, endPos - startPos };

      if (newRec) {
         recStartLine = currentLine;
         recStartPos = startPos;
         recStartText = lineText;
      }

      if (newRec && lineText.length() == 0)
         continue;

      if (eolMarkerLen == 0) {
         newRec = TRUE;
         eolMarkerPos = endPos;
      }
      else if (lineText.length() > eolMarkerLen &&
         (lineText.substr(lineText.length() - eolMarkerLen) == eolMarker)) {
         newRec = TRUE;
         eolMarkerPos = endPos - eolMarkerLen;
      }
      else if (currentLine < endLine) {
         newRec = FALSE;
         continue;
      }
      else {
         eolMarkerPos = endPos;
      }

      currentPos = recStartPos;

      int colorOffset{};
      size_t regexIndex{};

      while (regexIndex < regexedCount) {
         if (regex_match(recStartText, recInfoList[regexIndex].regExpr)) {
            if (caretLine >= recStartLine && caretLine <= currentLine) {
               caretRecordRegIndex = static_cast<int>(regexIndex);
               caretRecordStartPos = static_cast<int>(recStartPos);
               caretRecordEndPos = static_cast<int>(endPos);
               caretEolMarkerPos = static_cast<int>(eolMarkerPos);
            }

            break;
         }

         regexIndex++;
         colorOffset += shiftPerRec;
      }

      if (regexIndex >= regexedCount) continue;

      const vector<int>& recFieldWidths{ recInfoList[regexIndex].fieldWidths };
      const size_t fieldCount{ recFieldWidths.size() };

      wstring recTheme{ recInfoList[regexIndex].theme };
      size_t themeIndex{};

      if ((recTheme != L"") && (recTheme != fileTheme)) {
         for (size_t i{ 0 }; i < themeSet.size(); i++) {
            if (recTheme == themeSet[i].name) {
               themeIndex = i;
               colorOffset = 0;
               break;
            }
         }
      }

      const int& styleRangeStart{ themeSet[themeIndex].rangeStartIndex };
      const int& styleCount{ themeSet[themeIndex].styleCount };
      if (styleCount < 1) continue;

#if FW_DEBUG_APPLY_LEXER
      wstring dbgMessage;
      size_t dbgPos{ currentPos };

      dbgMessage = L"FieldWidths[" + to_wstring(regexIndex) + L"] = " +
         to_wstring(fieldCount) + L"\n";

      for (int i{}; i < static_cast<int>(fieldCount); i++) {
         dbgMessage += L" (" + to_wstring(dbgPos) + L", " +
            to_wstring(recFieldWidths[i]) + L", " + to_wstring(eolMarkerPos - eolMarkerLen) + L"), ";
         dbgPos += recFieldWidths[i];
      }

      MessageBox(_hSelf, dbgMessage.c_str(), L"", MB_OK);
#endif

      size_t styleIndex{};
      const vector<int>& recFieldStyles{ recInfoList[regexIndex].fieldStyles };

      if (byteCols) {
         int unstyledLen{};
         for (size_t i{}; i < fieldCount; i++) {
            sciFunc(sciPtr, SCI_STARTSTYLING, (WPARAM)currentPos, NULL);
            unstyledLen = static_cast<int>(eolMarkerPos - currentPos);
            currentPos += recFieldWidths[i];

            styleIndex = (recFieldStyles[i] >= 0) ?
               recFieldStyles[i] : styleRangeStart + ((i + colorOffset) % styleCount);

            if (recFieldWidths[i] < unstyledLen) {
               sciFunc(sciPtr, SCI_SETSTYLING, (WPARAM)recFieldWidths[i], styleIndex);
            }
            else {
               sciFunc(sciPtr, SCI_SETSTYLING, (WPARAM)unstyledLen, styleIndex);
               unstyledLen = 0;

               sciFunc(sciPtr, SCI_STARTSTYLING, (WPARAM)eolMarkerPos, NULL);
               sciFunc(sciPtr, SCI_SETSTYLING, (WPARAM)eolMarkerLen, styleRangeStart - 1);
               break;
            }
         }

         if (fieldCount > 0 && unstyledLen > 0) {
            sciFunc(sciPtr, SCI_STARTSTYLING, (WPARAM)currentPos, NULL);
            sciFunc(sciPtr, SCI_SETSTYLING, (WPARAM)(endPos - currentPos), styleRangeStart - 1);
         }
      }
      else {
         size_t nextPos{};
         for (size_t i{}; i < fieldCount; i++) {
            sciFunc(sciPtr, SCI_STARTSTYLING, (WPARAM)currentPos, NULL);
            nextPos = static_cast<int>(sciFunc(sciPtr, SCI_POSITIONRELATIVE,
               (WPARAM)currentPos, (LPARAM)recFieldWidths[i]));

            styleIndex = (recFieldStyles[i] >= 0) ?
               recFieldStyles[i] : styleRangeStart + ((i + colorOffset) % styleCount);

            if (nextPos > 0 && nextPos <= eolMarkerPos) {
               sciFunc(sciPtr, SCI_SETSTYLING, (WPARAM)(nextPos - currentPos), styleIndex);
               currentPos = nextPos;
            }
            else {
               sciFunc(sciPtr, SCI_SETSTYLING, (WPARAM)(eolMarkerPos - currentPos), styleIndex);

               sciFunc(sciPtr, SCI_STARTSTYLING, (WPARAM)eolMarkerPos, NULL);
               sciFunc(sciPtr, SCI_SETSTYLING, (WPARAM)eolMarkerLen, styleRangeStart - 1);
               currentPos = 0;
               break;
            }
         }

         if (fieldCount > 0 && currentPos > 0 && eolMarkerPos >= currentPos) {
            sciFunc(sciPtr, SCI_STARTSTYLING, (WPARAM)currentPos, NULL);
            sciFunc(sciPtr, SCI_SETSTYLING, (WPARAM)(endPos - currentPos), styleRangeStart - 1);
         }
      }
   }
}

void VisualizerPanel::renderCurrentPage() {
   if (loadLexer() < 1) {
      clearCaretFieldInfo();
      return;
   }

   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   size_t lineCount, linesOnScreen, firstVisible, startLine, endLine;

   lineCount = static_cast<size_t>(SendMessage(hScintilla, SCI_GETLINECOUNT, NULL, NULL));
   linesOnScreen = static_cast<size_t>(SendMessage(hScintilla, SCI_LINESONSCREEN, NULL, NULL));
   firstVisible = static_cast<size_t>(SendMessage(hScintilla, SCI_GETFIRSTVISIBLELINE, NULL, NULL));
   startLine = static_cast<size_t>(SendMessage(hScintilla, SCI_DOCLINEFROMVISIBLE, firstVisible, NULL));
   endLine = static_cast<size_t>(SendMessage(hScintilla, SCI_DOCLINEFROMVISIBLE, firstVisible + linesOnScreen, NULL));
   if (endLine + 1 < lineCount) endLine++;

   applyLexer(startLine, endLine);
   displayCaretFieldInfo(startLine, endLine);
}

void VisualizerPanel::clearCaretFieldInfo() {
   enableFieldControls(FALSE);
   SetWindowText(hFieldInfo, L"");
}

void VisualizerPanel::resizeCaretFieldInfo(int width) {
   RECT rcInfo;
   GetWindowRect(hFieldInfo, &rcInfo);

   // Get fieldInfo top-leftEdge coordinates relative to dock panel
   POINT pt{ rcInfo.left, rcInfo.top };
   ScreenToClient(_hSelf, &pt);

   MoveWindow(hFieldInfo, pt.x, pt.y, (width - pt.x - 3), (rcInfo.bottom - rcInfo.top), TRUE);
}

int VisualizerPanel::getFieldEdges(const string fileType, const int fieldIdx, const int rightPullback,
   int& leftPos, int& rightPos) {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return -1;

   string currFileType{};

   if (fileType == "") {
      if (!getDocFileType(hScintilla, currFileType)) return -1;
   }
   else {
      currFileType = fileType;
   }

   RecordInfo& FLD{ recInfoList[caretRecordRegIndex] };

   if (fieldIdx < 0 || fieldIdx >= static_cast<int>(FLD.fieldStarts.size())) return -1;

   int leftOffset{ FLD.fieldStarts[fieldIdx] };
   int rightOffset{ leftOffset + FLD.fieldWidths[fieldIdx] - rightPullback };

   if (!_configIO.getMultiByteLexing(currFileType)) {
      leftPos = caretRecordStartPos + leftOffset;
      rightPos = caretRecordStartPos + rightOffset;
   }
   else {
      leftPos = static_cast<int>(SendMessage(hScintilla, SCI_POSITIONRELATIVE,
         (WPARAM)caretRecordStartPos, (LPARAM)leftOffset));
      rightPos = static_cast<int>(SendMessage(hScintilla, SCI_POSITIONRELATIVE,
         (WPARAM)caretRecordStartPos, (LPARAM)rightOffset));
   }

   if (leftPos >= caretEolMarkerPos)
      leftPos = caretEolMarkerPos - 1;

   if (rightPos >= caretEolMarkerPos)
      rightPos = caretEolMarkerPos - 1;

   return 0;
}

void VisualizerPanel::moveToFieldEdge(const string fileType, const int fieldIdx, bool jumpTo, bool rightEdge, bool hilite) {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   int caretPos{ caretPos = static_cast<int>(SendMessage(hScintilla, SCI_GETCURRENTPOS, NULL, NULL)) };

   if (fieldIdx < 0) {
      if (caretPos >= caretEolMarkerPos) {
         caretPos = caretEolMarkerPos - 1;
         SendMessage(hScintilla, SCI_GOTOPOS, caretPos, NULL);
      }
      return;
   }

   int leftPos{}, rightPos{};
   if (getFieldEdges(fileType, fieldIdx, 1, leftPos, rightPos) < 0) return;

   if (!jumpTo) {
      if (rightEdge) {
         if (caretPos == rightPos && caretPos < caretEolMarkerPos - 1)
            if (getFieldEdges(fileType, fieldIdx + 1, 1, leftPos, rightPos) < 0) return;
      }
      else {
         if (caretPos == leftPos && caretPos > caretRecordStartPos)
            if (getFieldEdges(fileType, fieldIdx - 1, 1, leftPos, rightPos) < 0) return;
      }
   }

   SendMessage(hScintilla, SCI_SETXCARETPOLICY, CARET_JUMPS | CARET_EVEN, (LPARAM)0);
   SendMessage(hScintilla, SCI_GOTOPOS, (rightEdge ? rightPos : leftPos), 0);

   // Flash caret
   if (hilite) {
      HANDLE hThread = CreateThread(NULL, 0, threadPositionHighlighter, 0, 0, NULL);
      if (hThread > 0) CloseHandle(hThread);
   }

   setFocusOnEditor();
}

void VisualizerPanel::setFieldAlign(bool left) {
   leftAlign = left;
   ShowWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_RPAD_INDIC), leftAlign ? SW_SHOW : SW_HIDE);
   ShowWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_LPAD_INDIC), leftAlign ? SW_HIDE : SW_SHOW);
}

void VisualizerPanel::displayCaretFieldInfo(const size_t startLine, const size_t endLine) {
   if (!isVisible()) return;

   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   string fileType;

   if (!getDocFileType(hScintilla, fileType)) return;

   wstring fieldInfoText{};
   int caretPos;
   size_t caretLine;
   bool byteCols{ !_configIO.getMultiByteLexing(fileType) };
   wstring newLine{ L"\r\n" };

   caretFieldIndex = -1;
   caretPos = static_cast<int>(SendMessage(hScintilla, SCI_GETCURRENTPOS, NULL, NULL));
   caretLine = SendMessage(hScintilla, SCI_LINEFROMPOSITION, caretPos, NULL);

   if (caretLine < startLine || caretLine > endLine) {
      clearCaretFieldInfo();
      return;
   }

   if (caretRecordRegIndex < 0) {
      if (SendMessage(hScintilla, SCI_POSITIONFROMLINE, caretLine, NULL) ==
         SendMessage(hScintilla, SCI_GETLINEENDPOSITION, caretLine, NULL)) {
         fieldInfoText = CUR_POS_DATA_BLANK_LINE;
      }
      else {
         fieldInfoText = CUR_POS_DATA_UNKOWN_REC;
      }
   }
   else if (caretPos == caretRecordEndPos) {
      fieldInfoText = CUR_POS_DATA_REC_END;
   }
   else if (caretPos >= caretEolMarkerPos) {
      fieldInfoText = CUR_POS_DATA_REC_TERM;
   }
   else {
      RecordInfo& FLD{ recInfoList[caretRecordRegIndex] };
      int caretColumn, fieldCount, fieldLabelCount, cumulativeWidth{}, matchedField{ -1 }, recLength;

      if (byteCols) {
         caretColumn = caretPos - caretRecordStartPos;
         recLength = caretEolMarkerPos - caretRecordStartPos;
      }
      else {
         caretColumn = static_cast<int>(SendMessage(hScintilla, SCI_COUNTCHARACTERS, caretRecordStartPos, caretPos));
         recLength = static_cast<int>(SendMessage(hScintilla, SCI_COUNTCHARACTERS, caretRecordStartPos, caretEolMarkerPos));
      }

      fieldInfoText = CUR_POS_DATA_REC_TYPE + FLD.label;
      fieldCount = static_cast<int>(FLD.fieldStarts.size());
      fieldLabelCount = static_cast<int>(FLD.fieldLabels.size());

      for (int i{}; i < fieldCount; i++) {
         cumulativeWidth += FLD.fieldWidths[i];
         if (caretColumn >= FLD.fieldStarts[i] && caretColumn < cumulativeWidth) {
            matchedField = i;
         }
      }

      fieldInfoText += newLine + CUR_POS_DATA_REC_LENGTH +
         to_wstring(recLength) + L"/" + to_wstring(cumulativeWidth) + CUR_POS_DATA_CURR_DEFINED;

      if (matchedField < 0) {
         fieldInfoText += newLine + CUR_POS_DATA_OVERFLOW;
      }
      else {
         caretFieldIndex = matchedField;
         fieldInfoText += newLine + CUR_POS_DATA_FIELD_LABEL;

         if (fieldLabelCount == 0 || matchedField >= fieldLabelCount)
            fieldInfoText += CUR_POS_DATA_FIELD_NUM + to_wstring(matchedField + 1);
         else
            fieldInfoText += FLD.fieldLabels[matchedField];

         int fieldBegin{ FLD.fieldStarts[matchedField] };
         int fieldLength{ FLD.fieldWidths[matchedField] };

         fieldInfoText += newLine + CUR_POS_DATA_FIELD_START + to_wstring(fieldBegin + 1);
         fieldInfoText += newLine + CUR_POS_DATA_FIELD_WIDTH + to_wstring(fieldLength);
         fieldInfoText += newLine + CUR_POS_DATA_FIELD_COL + to_wstring(caretColumn - fieldBegin + 1);
         setFieldAlign((caretColumn - fieldBegin) < (fieldBegin + fieldLength - caretColumn));
      }
   }

   wchar_t byteInfo[200];
   UCHAR startChar = static_cast<UCHAR>(SendMessage(hScintilla, SCI_GETCHARAT, caretPos, 0));

   if (!(startChar & 0x80)) {
      swprintf(byteInfo, 200, L"0x%X [%u]", startChar, startChar);
      fieldInfoText += newLine + CUR_POS_DATA_ANSI_BYTE + wstring(byteInfo);
   }
   else {
      swprintf(byteInfo, 200, L"%X", startChar);
      fieldInfoText += newLine + CUR_POS_DATA_UTF8_BYTES + wstring(byteInfo);

      int unicodeHead{ 0 }, unicodeTail{ 0 };
      UCHAR nextChar{};

      nextChar = static_cast<UCHAR>(SendMessage(hScintilla, SCI_GETCHARAT, caretPos + 1, 0));
      swprintf(byteInfo, 200, L" %X", nextChar);
      fieldInfoText += wstring(byteInfo);

      unicodeHead = (startChar & 31) << 6;
      unicodeTail = (nextChar & 63);

      if ((startChar & 0xE0) == 0xE0) {
         nextChar = static_cast<UCHAR>(SendMessage(hScintilla, SCI_GETCHARAT, caretPos + 2, 0));
         swprintf(byteInfo, 200, L" %X", nextChar);
         fieldInfoText += wstring(byteInfo);

         unicodeHead = (startChar & 15) << 12;
         unicodeTail = (unicodeTail << 6) + (nextChar & 63);

         if ((startChar & 0xF0) == 0xF0) {
            nextChar = static_cast<UCHAR>(SendMessage(hScintilla, SCI_GETCHARAT, caretPos + 3, 0));
            swprintf(byteInfo, 200, L" %X", nextChar);
            fieldInfoText += wstring(byteInfo);

            unicodeHead = (startChar & 7) << 18;
            unicodeTail = (unicodeTail << 6) + (nextChar & 63);
         }
      }

      swprintf(byteInfo, 200, L"  (U+%X)", (unicodeHead + unicodeTail));
      fieldInfoText += wstring(byteInfo);
   }

#if FW_DEBUG_LEXER_COUNT
   fieldInfoText = L"Lex Count: " + to_wstring(lexCount) + L"\r\n" + fieldInfoText;
#endif

   SetWindowText(hFieldInfo, fieldInfoText.c_str());
   enableFieldControls(TRUE);
}

void VisualizerPanel::showJumpDialog() {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   string fileType;
   if (!getDocFileType(hScintilla, fileType)) return;

   RecordInfo& FLD{ recInfoList[caretRecordRegIndex] };

   int fieldCount = static_cast<int>(FLD.fieldStarts.size());
   int fieldLabelCount = static_cast<int>(FLD.fieldLabels.size());

   vector<wstring> fieldLabels;
   fieldLabels.resize(fieldCount);

   for (int i{}; i < fieldCount; i++) {
      if (fieldLabelCount == 0 || i >= fieldLabelCount) {
         fieldLabels[i] = CUR_POS_DATA_FIELD_NUM + to_wstring(i + 1);
      }
      else {
         fieldLabels[i] = FLD.fieldLabels[i];
      }
   }

   _jumpDlg.doDialog((HINSTANCE)_gModule);
   _jumpDlg.initDialog(fileType, caretRecordRegIndex, caretFieldIndex, fieldLabels);
}

void VisualizerPanel::showExtractDialog() {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   string fileType;
   if (!getDocFileType(hScintilla, fileType)) return;

   _dataExtractDlg.doDialog((HINSTANCE)_gModule);
   _dataExtractDlg.initDialog(fileType, recInfoList);
}

bool VisualizerPanel::getDocFileType(HWND hScintilla, string& fileType) {
   char fType[MAX_PATH]{};

   SendMessage(hScintilla, SCI_GETPROPERTY, (WPARAM)FW_DOC_FILE_TYPE, (LPARAM)fType);
   fileType = fType;

   return (fileType.length() > 0);
}

bool VisualizerPanel::getDocFileType(PSCIFUNC_T sciFunc, void* sciPtr, string& fileType) {
   char fType[MAX_PATH]{};

   sciFunc(sciPtr, SCI_GETPROPERTY, (WPARAM)FW_DOC_FILE_TYPE, (LPARAM)fType);
   fileType = fType;

   return (fileType.length() > 0);
}

bool VisualizerPanel::detectFileType(HWND hScintilla, string& fileType) {
   if (!isVisible()) return FALSE;
   if (!_configIO.checkConfigFilesforUCS16()) return FALSE;

   char lineTextCStr[FW_LINE_MAX_LENGTH]{};
   size_t startPos, endPos;

   vector<string> fileTypes;
   _configIO.getConfigValueList(fileTypes, "Base", "FileTypes");

   for (string fType : fileTypes) {
      bool matched{ FALSE };

      for (int i{}; i < ADFT_MAX; i++) {
         char idx[5];
         snprintf(idx, 5, "%02d", i + 1);

         int line = _configIO.getConfigInt(fType, "ADFT_Line_" + string{ idx });
         if (line == 0) continue;

         string strRegex = _configIO.getConfigStringA(fType, "ADFT_Regex_" + string{ idx });
         if (strRegex.length() == 0) continue;
         if (Utils::isInvalidRegex(strRegex)) continue;

         int lineCount = static_cast<int>(SendMessage(hScintilla, SCI_GETLINECOUNT, NULL, NULL));

         line += (line < 0) ? lineCount : -1;
         if (line < 0 || line >= lineCount) continue;

         if (strRegex.substr(strRegex.length() - 1) != "$")
            strRegex += ".*";

         SendMessage(hScintilla, SCI_GETLINE, (WPARAM)line, (LPARAM)lineTextCStr);
         startPos = SendMessage(hScintilla, SCI_POSITIONFROMLINE, line, NULL);
         endPos = SendMessage(hScintilla, SCI_GETLINEENDPOSITION, line, NULL);
         string_view lineText{ lineTextCStr, endPos - startPos };

         if (regex_match(string{ lineText }, regex{ strRegex }))
            matched = TRUE;
         else {
            matched = FALSE;
            break;
         }
      }

      if (matched) {
         fileType = fType;
         setDocFileType(hScintilla, fileType);
         setDocTheme(hScintilla, fileType, "");
         break;
      }
   }

   return (fileType.length() > 0);
}

bool VisualizerPanel::getDocTheme(HWND hScintilla, wstring& theme) {
   char fTheme[MAX_PATH]{};

   SendMessage(hScintilla, SCI_GETPROPERTY, (WPARAM)FW_DOC_FILE_THEME, (LPARAM)fTheme);
   theme = Utils::NarrowToWide(fTheme);

   return (theme.length() > 0);
}

bool VisualizerPanel::getDocTheme(PSCIFUNC_T sciFunc, void* sciPtr, wstring& theme) {
   char fTheme[MAX_PATH]{};

   sciFunc(sciPtr, SCI_GETPROPERTY, (WPARAM)FW_DOC_FILE_THEME, (LPARAM)fTheme);
   theme = Utils::NarrowToWide(fTheme);

   return (theme.length() > 0);
}

void VisualizerPanel::setDocFileType(HWND hScintilla, string fileType) {
   if (!isVisible()) return;

   enableThemeList(fileType.length() > 0);
   SendMessage(hScintilla, SCI_SETPROPERTY, (WPARAM)FW_DOC_FILE_TYPE, (LPARAM)fileType.c_str());
}

void VisualizerPanel::setDocTheme(HWND hScintilla, string fileType, string theme) {
   if (fileType.length() > 0)
      theme = _configIO.getConfigStringA(fileType, "FileTheme");

   SendMessage(hScintilla, SCI_SETPROPERTY, (WPARAM)FW_DOC_FILE_THEME, (LPARAM)theme.c_str());
}

void VisualizerPanel::setADFTCheckbox() {
   bool checked{ IsDlgButtonChecked(_hSelf, IDC_VIZPANEL_AUTO_DETECT_FT) == BST_CHECKED };

   _configIO.setPreferenceBool(PREF_ADFT, checked);
   if (checked) visualizeFile("", FALSE, TRUE, TRUE);
}

void VisualizerPanel::setPanelMBCharState() {
   _configIO.setPanelMBCharState(IsDlgButtonChecked(_hSelf, IDC_VIZPANEL_MCBS_OVERRIDE));
   visualizeFile("", FALSE, (IsDlgButtonChecked(_hSelf, IDC_VIZPANEL_AUTO_DETECT_FT) == BST_CHECKED), TRUE);
}

void VisualizerPanel::setPanelMBCharIndicator(string fileType) {
   wstring indicator{};
   UINT state{ IsDlgButtonChecked(_hSelf, IDC_VIZPANEL_MCBS_OVERRIDE) };

   if (fileType.length() < 2 || state == BST_UNCHECKED)
      indicator = L"";
   else if (state == BST_CHECKED)
      indicator = L"*";
   else if (_configIO.getConfigStringA(fileType, "MultiByteChars", "N", "") == "Y")
      indicator = L"+";
   else
      indicator = L"-";

   SetDlgItemText(_hSelf, IDC_VIZPANEL_MCBS_OVERRIDE_IND, indicator.c_str());
}

void VisualizerPanel::onBufferActivate() {
   unlexed = TRUE;
   if (isVisible()) visualizeFile("", TRUE, TRUE, TRUE);
}

void VisualizerPanel::setFocusOnEditor() {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   SendMessage(hScintilla, SCI_GRABFOCUS, 0, 0);
}

void VisualizerPanel::clearLexer() {
   recInfoList.clear();
   fwVizRegexed = "";
}

void VisualizerPanel::popupSamplesMenu() {
   HMENU hPopupMenu = CreatePopupMenu();
   _submenu.initSamplesPopup(hPopupMenu);

   RECT rc;
   GetWindowRect(GetDlgItem(_hSelf, IDC_VIZPANEL_FILE_SAMPLES_BTN), &rc);

   int cmd = TrackPopupMenu(hPopupMenu, TPM_RIGHTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD,
      rc.right, rc.bottom, 0, _hSelf, NULL);

   if (cmd) nppMessage(NPPM_MENUCOMMAND, NULL, cmd);

   // Calling RemoveMenu is needed since the appended items are being referenced from the NPP Main menu.
   // In RemoveMenu, zero is used as the position since items shift down with each remove call.
   // 'i' is used only as a loopcounter.
   int itemCount = GetMenuItemCount(hPopupMenu);

   for (int i{}; i < itemCount; i++) {
      RemoveMenu(hPopupMenu, 0, MF_BYPOSITION);
   }
   DestroyMenu(hPopupMenu);
}

DWORD __stdcall VisualizerPanel::threadPositionHighlighter(void*) {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return FALSE;

   // Look for Idem Potency Hold
   if (idemPotentKey)
      //  Idem Potency check failed. Another thread is processing this same function. Return immediately.
      return FALSE;
   else
      // OK to continue. Set Idem Potency Hold
      idemPotentKey = TRUE;

   // Modify caret styleInfo briefly to highlight the new position
   int currCaret = static_cast<int>(SendMessage(hScintilla, SCI_GETCARETSTYLE, 0, 0));
   SendMessage(hScintilla, SCI_SETCARETSTYLE, CARETSTYLE_BLOCK, 0);
   Sleep(_configIO.getPreferenceInt(PREF_CARET_FLASH, 5) * 1000);
   SendMessage(hScintilla, SCI_SETCARETSTYLE, currCaret, 0);

   int pos = static_cast<int>(SendMessage(hScintilla, SCI_GETCURRENTPOS, 0, 0));
   SendMessage(hScintilla, SCI_BRACEHIGHLIGHT, pos, -1);

   // Clear Idem Potency Hold
   idemPotentKey = FALSE;

   return TRUE;
}
