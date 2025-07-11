#include "VisualizerPanel.h"
#include "ConfigureDialog.h"
#include "ThemeDialog.h"
#include "PreferencesDialog.h"
#include "JumpToField.h"
#include "DataExtractDialog.h"
#include "FoldStructDialog.h"
#include "AboutDialog.h"
#include <WindowsX.h>

extern HINSTANCE _gModule;
extern SubmenuManager _submenu;
extern FuncItem pluginMenuItems[MI_COUNT];

ConfigureDialog _configDlg;
ThemeDialog _themeDlg;
PreferencesDialog _prefsDlg;
JumpToField _jumpDlg;
DataExtractDialog _dataExtractDlg;
FoldStructDialog _foldStructDlg;
AboutDialog _aboutDlg;

INT_PTR CALLBACK VisualizerPanel::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
   switch (message) {
   case WM_COMMAND:
      switch LOWORD(wParam) {
      case IDC_VIZPANEL_FILETYPE_SELECT:
         switch HIWORD(wParam) {
         case CBN_SELCHANGE:
            if (SendMessage(hFTList, CB_GETCURSEL, 0, 0) > 0)
               visualizeFile("", FALSE, FALSE, FALSE);
            else
               clearVisualize(FALSE);

            break;
         }
         break;

      case IDC_VIZPANEL_INFO_BUTTON:
      {
      #if FW_DEBUG_DOC_INFO
         wstring docInfoStr{};

         for (const DocInfo DI : docInfoList) {
            docInfoStr = docInfoStr + L"File Path: " + DI.fileName +
               L"\r\nFileType: " + Utils::NarrowToWide(DI.docType) +
               L"\r\nTheme: " + Utils::NarrowToWide(DI.docTheme) +
               L"\r\n==================================\r\n";
         }

         MessageBox(_hSelf, docInfoStr.c_str(), L"", MB_OK);
      #else
         ShellExecute(NULL, L"open", VIZPANEL_INFO_README, NULL, NULL, SW_SHOW);
      #endif // FW_DEBUG_DOC_INFO
      }
         break;

      case IDC_VIZPANEL_FILE_SAMPLES_BTN:
         popupSamplesMenu();
         break;

      case IDC_VIZPANEL_FILETYPE_CONFIG:
         showConfigDialog();
         break;

      case IDC_VIZPANEL_THEME_SELECT:
         switch HIWORD(wParam) {
         case CBN_SELCHANGE:
            visualizeTheme();
            break;
         }
         break;

      case IDC_VIZPANEL_THEME_CONFIG:
         showThemeDialog();
         break;

      case IDC_VIZPANEL_CLEAR_BTN:
         if (_configIO.getPreferenceBool(PREF_CLEARVIZ_AUTO, FALSE)) {
            CheckDlgButton(_hSelf, IDC_VIZPANEL_AUTO_DETECT_FT, BST_UNCHECKED);
            setADFTCheckbox();
         }
         SendMessage(hFTList, CB_SETCURSEL, (WPARAM)-1, 0);
         clearVisualize(FALSE);
         break;

      case IDCANCEL:
      case IDCLOSE:
         setFocusOnEditor();
         display(FALSE);
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

      case IDC_VIZPANEL_DEFAULT_BACKGROUND:
         setDefaultBackground();
         break;

      case IDC_VIZPANEL_SHOW_CALLTIP:
         setShowCalltip();
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
         setFocusOnEditor();
         break;

      case IDC_VIZPANEL_FIELD_RIGHT_BUTTON:
         fieldRight();
         setFocusOnEditor();
         break;

      case IDC_VIZPANEL_FIELD_COPY_BUTTON:
         fieldCopy();
         setFocusOnEditor();
         break;

      case IDC_VIZPANEL_FIELD_PASTE_BUTTON:
         fieldPaste();
         setFocusOnEditor();
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
         if (_configIO.fixIfNotUTF8File(_configIO.CONFIG_EXTRACTS))
            showExtractDialog();
         break;

      case IDC_VIZPANEL_FOLD_INFO_BUTTON:
         ShellExecute(NULL, L"open", VIZPANEL_FOLD_INFO_README, NULL, NULL, SW_SHOW);
         break;

      case IDC_VIZPANEL_FOLDING_APPLY_BTN:
         if (_configIO.fixIfNotUTF8File(_configIO.CONFIG_FOLDSTRUCTS)) {
            setDocFolded(FALSE);
            applyFolding("");
         }
         setFocusOnEditor();
         break;

      case IDC_VIZPANEL_FOLDING_REMOVE_BTN:
         removeFolding();
         setFocusOnEditor();
         break;

      case IDC_VIZPANEL_FOLDING_DEFINE_BTN:
         showFoldStructDialog();
         break;

      case IDC_VIZPANEL_FOLDING_FOLD_BTN:
         foldLevelMenu();
         break;

      case IDC_VIZPANEL_FOLDING_TOGGLE_BTN:
         toggleFolding();
         setFocusOnEditor();
         break;

      case IDC_VIZPANEL_FOLDING_UNFOLD_BTN:
         unfoldLevelMenu();
         break;

      case IDC_VIZPANEL_ABOUT_BUTTON:
         showAboutDialog();
         break;

      case IDC_VIZPANEL_FILE_INFO_BUTTON:
      {
         wstring fileList = _configIO.getActiveConfigFile(_configIO.CONFIG_VIZ) + L"\n" +
            _configIO.getActiveConfigFile(_configIO.CONFIG_THEMES) + L"\n" +
            _configIO.getActiveConfigFile(_configIO.CONFIG_FOLDSTRUCTS) + L"\n" +
            wstring(80, '-') + VIZ_PANEL_FILE_INFO_TIP;

         Utils::updateTooltip(_hSelf, IDC_VIZPANEL_FILE_INFO_BUTTON, hTipIniFiles, fileList.data());
         break;
      }

      }

      break;

   case WM_LBUTTONDOWN:
   case WM_MBUTTONDOWN:
   case WM_RBUTTONDOWN:
      SetFocus(hFTList);
      break;

   case WM_SHOWWINDOW:
      Utils::checkMenuItem(MI_FWVIZ_PANEL, wParam);
      if (wParam) visualizeFile("", TRUE, TRUE, TRUE);
      break;

   case WM_NOTIFY:
   {
      LPNMHDR pnmh = reinterpret_cast<LPNMHDR>(lParam);
      if (pnmh->hwndFrom == _hParent && LOWORD(pnmh->code) == DMN_CLOSE) {
         display(FALSE);

         unlexed = (_configIO.getPreferenceBool(PREF_CLEARVIZ_PANEL, FALSE));
         if (unlexed) clearVisualize(FALSE);
      }
      break;
   }

   case WM_SIZE:
      onPanelResize(lParam);
      break;

   case WM_INITDIALOG:
      NPPDM_AutoSubclassAndThemeChildControls(_hSelf);
      break;

   case WM_CTLCOLORDLG:
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

      case IDC_VIZPANEL_PASTE_LEFT_LABEL:
      case IDC_VIZPANEL_PASTE_RIGHT_LABEL:
      case IDC_VIZPANEL_PASTE_RPAD_LABEL:
      case IDC_VIZPANEL_PASTE_LPAD_LABEL:
         return NPPDM_OnCtlColorIfEnabled(reinterpret_cast<HDC>(wParam), fieldEnabled);

      case IDC_VIZPANEL_PASTE_RPAD_INDIC:
      case IDC_VIZPANEL_PASTE_LPAD_INDIC:
         return NPPDM_OnCtlHiliteIfEnabled(reinterpret_cast<HDC>(wParam), fieldEnabled);

      default:
         if (NPPDM_IsEnabled()) {
            return NPPDM_OnCtlColorDarker(reinterpret_cast<HDC>(wParam));
         }
      }
      break;

   case WM_CTLCOLORLISTBOX:
      if (NPPDM_IsEnabled()) {
         return NPPDM_OnCtlColorListbox(wParam, lParam);
      }
      break;

   case WM_PRINTCLIENT:
      if (NPPDM_IsEnabled()) return TRUE;
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

   utf8Config = _configIO.checkConfigFilesforUTF8();
   if (!utf8Config) return;

   PreferencesDialog::applyFoldLineColorAlpha();

   setFont(_hSelf, IDC_VIZPANEL_FIELD_LABEL, fontName, fontHeight, FW_BOLD, FALSE, TRUE);
   setFont(_hSelf, IDC_VIZPANEL_FIELD_INFO, fontName, fontHeight);

   loadBitmap(_hSelf, IDC_VIZPANEL_INFO_BUTTON, IDB_VIZ_INFO_BITMAP);
   addTooltip(_hSelf, IDC_VIZPANEL_INFO_BUTTON, L"", VIZ_PANEL_INFO_TIP, FALSE);

   loadBitmap(_hSelf, IDC_VIZPANEL_FILE_SAMPLES_BTN, IDB_VIZ_FILE_SAMPLES_BITMAP);
   addTooltip(_hSelf, IDC_VIZPANEL_FILE_SAMPLES_BTN, L"", VIZ_PANEL_FILE_SAMPLES_TIP, FALSE);

   loadBitmap(_hSelf, IDC_VIZPANEL_FILETYPE_CONFIG, IDB_VIZ_FILE_CONFIG_BITMAP);
   addTooltip(_hSelf, IDC_VIZPANEL_FILETYPE_CONFIG, L"", VIZ_PANEL_FILE_CONFIG_TIP, FALSE);

   loadBitmap(_hSelf, IDC_VIZPANEL_THEME_CONFIG, IDB_VIZ_COLOR_CONFIG_BITMAP);
   addTooltip(_hSelf, IDC_VIZPANEL_THEME_CONFIG, L"", VIZ_PANEL_THEME_CONFIG_TIP, FALSE);

   SetWindowText(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_RPAD_FIELD), _configIO.getPreference(PREF_PASTE_RPAD).c_str());
   SetWindowText(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_LPAD_FIELD), _configIO.getPreference(PREF_PASTE_LPAD).c_str());

   addTooltip(_hSelf, IDC_VIZPANEL_CLEAR_BTN, L"", VIZ_PANEL_CLEAR_BTN_TIP, FW_TIP_MEDIUM, TRUE);
   addTooltip(_hSelf, IDC_VIZPANEL_FIELD_COPY_TRIM, L"", VIZ_PANEL_FIELD_TRIM_TIP, FW_TIP_SHORT, TRUE);

   addTooltip(_hSelf, IDC_VIZPANEL_FIELD_LEFT_BUTTON, L"", VIZ_PANEL_FIELD_LEFT_TIP, FW_TIP_SHORT, TRUE);
   hTipHopRight = addTooltip(_hSelf, IDC_VIZPANEL_FIELD_RIGHT_BUTTON, L"",
      _configIO.getPreferenceBool(PREF_HOP_RT_LEFT_EDGE, FALSE) ? VIZ_PANEL_FLD_ALT_RIGHT_TIP : VIZ_PANEL_FIELD_RIGHT_TIP,
      FW_TIP_SHORT, TRUE);

   addTooltip(_hSelf, IDC_VIZPANEL_FIELD_COPY_BUTTON, L"", VIZ_PANEL_FIELD_COPY_TIP, FW_TIP_MEDIUM, TRUE);
   addTooltip(_hSelf, IDC_VIZPANEL_FIELD_PASTE_BUTTON, L"", VIZ_PANEL_FIELD_PASTE_TIP, FW_TIP_LONG, TRUE);

   addTooltip(_hSelf, IDC_VIZPANEL_PASTE_LEFT_LABEL, L"", VIZ_PANEL_FIELD_RPAD_TIP, FW_TIP_LONG, TRUE);
   addTooltip(_hSelf, IDC_VIZPANEL_PASTE_RPAD_LABEL, L"", VIZ_PANEL_FIELD_RPAD_TIP, FW_TIP_LONG, TRUE);
   addTooltip(_hSelf, IDC_VIZPANEL_PASTE_RPAD_INDIC, L"", VIZ_PANEL_FIELD_RPAD_TIP, FW_TIP_LONG, TRUE);

   addTooltip(_hSelf, IDC_VIZPANEL_PASTE_RIGHT_LABEL, L"", VIZ_PANEL_FIELD_LPAD_TIP, FW_TIP_LONG, TRUE);
   addTooltip(_hSelf, IDC_VIZPANEL_PASTE_LPAD_LABEL, L"", VIZ_PANEL_FIELD_LPAD_TIP, FW_TIP_LONG, TRUE);
   addTooltip(_hSelf, IDC_VIZPANEL_PASTE_LPAD_INDIC, L"", VIZ_PANEL_FIELD_LPAD_TIP, FW_TIP_LONG, TRUE);

   loadBitmap(_hSelf, IDC_VIZPANEL_FOLD_INFO_BUTTON, IDB_VIZ_INFO_BITMAP);
   addTooltip(_hSelf, IDC_VIZPANEL_FOLD_INFO_BUTTON, L"", VIZ_PANEL_INFO_TIP, FALSE);

   loadBitmap(_hSelf, IDC_VIZPANEL_FILE_INFO_BUTTON, IDB_VIZ_INI_FILES_BITMAP);
   hTipIniFiles = addTooltip(_hSelf, IDC_VIZPANEL_FILE_INFO_BUTTON, VIZ_PANEL_FILE_INFO_TITLE, VIZ_PANEL_FILE_INFO_TIP, FW_TIP_MEDIUM, TRUE);

   loadBitmap(_hSelf, IDC_VIZPANEL_ABOUT_BUTTON, IDB_VIZ_ABOUT_BITMAP);
   addTooltip(_hSelf, IDC_VIZPANEL_ABOUT_BUTTON, L"", ABOUT_DIALOG_TITLE, TRUE);

   setFont(_hSelf, IDC_VIZPANEL_MCBS_OVERRIDE_IND, fontName, 9);

   if constexpr(_gLanguage != LANG_ENGLISH) localize();
}

void VisualizerPanel::localize() {
   SetWindowText(_hSelf, MENU_PANEL_NAME);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_FILETYPE_LABEL, VIZ_PANEL_FILETYPE_LABEL);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_THEME_LABEL, VIZ_PANEL_THEME_LABEL);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_CLEAR_BTN, VIZ_PANEL_CLEAR_BUTTON);
   SetDlgItemText(_hSelf, IDCLOSE, VIZ_PANEL_CLOSE);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_PREFERENCES_BTN, VIZ_PANEL_PREFERENCES);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_AUTO_DETECT_FT, VIZ_PANEL_AUTO_DETECT_FT);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_MCBS_OVERRIDE, VIZ_PANEL_MCBS_OVERRIDE);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_DEFAULT_BACKGROUND, VIZPANEL_DEFAULT_BACKGROUND);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_SHOW_CALLTIP, VIZPANEL_SHOW_CALLTIP);
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
   SetDlgItemText(_hSelf, IDC_VIZPANEL_FOLDING_GROUP_BOX, VIZPANEL_FOLD_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_FOLDING_APPLY_BTN, VIZPANEL_FOLD_APPLY_BTN);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_FOLDING_REMOVE_BTN, VIZPANEL_FOLD_REMOVE_BTN);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_FOLDING_DEFINE_BTN, VIZPANEL_FOLD_DEFINE_BTN);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_FOLDING_FOLD_BTN, VIZPANEL_FOLD_FOLD_BTN);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_FOLDING_TOGGLE_BTN, VIZPANEL_FOLD_TOGGLE_BTN);
   SetDlgItemText(_hSelf, IDC_VIZPANEL_FOLDING_UNFOLD_BTN, VIZPANEL_FOLD_UNFOLD_BTN);
}

void VisualizerPanel::display(bool toShow) {
   DockingDlgInterface::display(toShow);

   if (!utf8Config) return;

   panelMounted = toShow;

   if (!toShow) {
      if (_configDlg.isCreated() && _configDlg.isVisible())
         _configDlg.display(FALSE);

      if (_themeDlg.isCreated() && _themeDlg.isVisible())
         _themeDlg.display(FALSE);

      if (_prefsDlg.isCreated() && _prefsDlg.isVisible())
         _prefsDlg.display(FALSE);

      if (_jumpDlg.isCreated() && _jumpDlg.isVisible())
         _jumpDlg.display(FALSE);

      if (_dataExtractDlg.isCreated() && _dataExtractDlg.isVisible())
         _dataExtractDlg.display(FALSE);

      if (_foldStructDlg.isCreated() && _foldStructDlg.isVisible())
         _foldStructDlg.display(FALSE);

      if (_aboutDlg.isCreated() && _aboutDlg.isVisible())
         _aboutDlg.display(FALSE);

      unlexed = (_configIO.getPreferenceBool(PREF_CLEARVIZ_PANEL, FALSE));
      if (unlexed) clearVisualize(FALSE);
      return;
   }

   hFTList = GetDlgItem(_hSelf, IDC_VIZPANEL_FILETYPE_SELECT);
   hThemesLB = GetDlgItem(_hSelf, IDC_VIZPANEL_THEME_SELECT);
   hFieldInfo = GetDlgItem(_hSelf, IDC_VIZPANEL_FIELD_INFO);

   CheckDlgButton(_hSelf, IDC_VIZPANEL_AUTO_DETECT_FT,
      _configIO.getPreferenceBool(PREF_ADFT) ? BST_CHECKED : BST_UNCHECKED);

   initMBCharsCheckbox();

   CheckDlgButton(_hSelf, IDC_VIZPANEL_DEFAULT_BACKGROUND,
      _configIO.getPreferenceBool(PREF_DEF_BACKGROUND, FALSE) ? BST_CHECKED : BST_UNCHECKED);

   CheckDlgButton(_hSelf, IDC_VIZPANEL_SHOW_CALLTIP,
      _configIO.getPreferenceBool(PREF_SHOW_CALLTIP, FALSE) ? BST_CHECKED : BST_UNCHECKED);

   CheckDlgButton(_hSelf, IDC_VIZPANEL_FIELD_COPY_TRIM,
      _configIO.getPreferenceBool(PREF_COPY_TRIM, FALSE) ? BST_CHECKED : BST_UNCHECKED);

   unlexed = TRUE;
   visualizeFile("", TRUE, TRUE, TRUE);

   if (unlexed)
      SetFocus(hFTList);
   else
      setFocusOnEditor();
}

void VisualizerPanel::refreshDarkMode() {
   NPPDM_AutoThemeChildControls(_hSelf);
   RegisterDockPanelIcon();
   redraw();
   SendMessage(GetDlgItem(_hSelf, IDC_PREF_FOLD_LINE_ALPHA_SLIDER), TBM_SETRANGEMIN, FALSE, 0);

   if (_configDlg.isCreated())
      _configDlg.refreshDarkMode();

   if (_themeDlg.isCreated())
      _themeDlg.refreshDarkMode();

   if (_prefsDlg.isCreated())
      _prefsDlg.refreshDarkMode();

   if (_jumpDlg.isCreated())
      _jumpDlg.refreshDarkMode();

   if (_dataExtractDlg.isCreated())
      _dataExtractDlg.refreshDarkMode();

   if (_foldStructDlg.isCreated())
      _foldStructDlg.refreshDarkMode();

   if (_aboutDlg.isCreated())
      _aboutDlg.refreshDarkMode();
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

void VisualizerPanel::updateHopRightTip() {
   Utils::updateTooltip(_hSelf, IDC_VIZPANEL_FIELD_RIGHT_BUTTON, hTipHopRight,
      (_configIO.getPreferenceBool(PREF_HOP_RT_LEFT_EDGE, FALSE)) ? VIZ_PANEL_FLD_ALT_RIGHT_TIP : VIZ_PANEL_FIELD_RIGHT_TIP);
}

void VisualizerPanel::setParent(HWND parent2set) {
   _hParent = parent2set;
}

void VisualizerPanel::loadListFileTypes() {
   SendMessage(hFTList, CB_RESETCONTENT, NULL, NULL);

   if (!utf8Config) return;

   vector<string> fileTypes;
   int ftCount{ _configIO.getConfigValueList(fileTypes, "Base", "FileTypes") };

   vFileTypes.clear();
   vFileTypes.resize(ftCount);
   SendMessage(hFTList, CB_ADDSTRING, NULL, (LPARAM)L"-");

   for (int i{}; i < ftCount; ++i) {
      wstring fileLabel{ _configIO.getConfigWideChar(fileTypes[i], "FileLabel") };

      vFileTypes[i].fileType = fileTypes[i];
      vFileTypes[i].fileLabel = fileLabel;
      SendMessage(hFTList, CB_ADDSTRING, NULL, (LPARAM)fileLabel.c_str());
   }
}

void VisualizerPanel::loadListThemes() const {
   SendMessage(hThemesLB, CB_RESETCONTENT, NULL, NULL);

   if (!utf8Config) return;

   vector<wstring> themesList;
   _configIO.getThemesList(themesList);

   for (const wstring theme : themesList) {
      SendMessage(hThemesLB, CB_ADDSTRING, NULL, (LPARAM)theme.c_str());
   }
}

void VisualizerPanel::syncListFileTypes() {
   if (!panelMounted) return;

   HWND hScintilla{ GetCurrentScintilla() };
   if (!hScintilla) return;

   string fileType;
   getDocFileType(fileType);

   if (fileType.empty()) {
      if (_configIO.getPreferenceBool(PREF_ADFT))
         detectFileType(hScintilla, fileType);
   }
   else
      _configIO.setVizConfig(fileType);

   loadListFileTypes();

   if (!fileType.empty()) {
      Utils::setComboBoxSelection(hFTList, static_cast<int>(
         SendMessage(hFTList, CB_FINDSTRING, (WPARAM)-1, (LPARAM)_configIO.getConfigWideChar(fileType, "FileLabel").c_str())));
   }

   enableThemeList(!fileType.empty());
}

void VisualizerPanel::syncListThemes() {
   wstring theme;

   getDocTheme(theme);
   loadListThemes();

   Utils::setComboBoxSelection(hThemesLB, theme.empty() ?
      -1 : static_cast<int>(SendMessage(hThemesLB, CB_FINDSTRING, (WPARAM)-1, (LPARAM)theme.c_str())));
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
   EnableWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_LPAD_FIELD), fieldEnabled);

   InvalidateRect(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_RPAD_INDIC), nullptr, TRUE);
   InvalidateRect(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_LPAD_INDIC), nullptr, TRUE);
   InvalidateRect(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_LEFT_LABEL), nullptr, TRUE);
   InvalidateRect(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_RIGHT_LABEL), nullptr, TRUE);
   InvalidateRect(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_RPAD_LABEL), nullptr, TRUE);
   InvalidateRect(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_LPAD_LABEL), nullptr, TRUE);

   HMENU hPluginMenu = (HMENU)NppMessage(NPPM_GETMENUHANDLE);

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
   if (!panelMounted) return;

   themeEnabled = enable;
   InvalidateRect(GetDlgItem(_hSelf, IDC_VIZPANEL_THEME_LABEL), nullptr, TRUE);
   EnableWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_THEME_SELECT), enable);
}

void VisualizerPanel::visualizeFile(string fileType, bool bCachedFT, bool bAutoFT, bool bSyncFT) {
   HWND hScintilla{ GetCurrentScintilla() };
   if (!hScintilla) return;

   if (bCachedFT) {
      getDocFileType(fileType);
      _configIO.setVizConfig(fileType);
   }

   if (fileType.empty()) {
      if (bAutoFT) {
         if (_configIO.getPreferenceBool(PREF_ADFT))
            detectFileType(hScintilla, fileType);
      }
      else {
         int index{ static_cast<int>(SendMessage(hFTList, CB_GETCURSEL, 0, 0)) };
         if (index > 0)
            fileType = vFileTypes[index - 1].fileType;
      }
   }

   if (IsWindowVisible(GetDlgItem(_hSelf, IDC_VIZPANEL_MCBS_OVERRIDE_IND)))
      setPanelMBCharIndicator(fileType);

   if (fileType.empty()) {
      syncListFileTypes();
      syncListThemes();
      return;
   }

   wstring theme{};
   getDocTheme(theme);

   clearVisualize(FALSE);
   setDocFileType(fileType);
   if (bSyncFT) syncListFileTypes();

   setDocTheme(Utils::WideToNarrow(theme), theme.empty() ? fileType : "");
   syncListThemes();

   loadUsedThemes();
   loadLexer();
   renderCurrentPage();

   string fsType{ detectFoldStructType(fileType) };

   if (!fsType.empty() && (_configIO.getFoldStructValueA(fsType, "FoldLevelAuto") == "Y"))
      applyFolding(fsType);

   enableFoldableControls(!fsType.empty());
   setFocusOnEditor();
}

void VisualizerPanel::delDocInfo(intptr_t bufferID) {
   wstring filePath(MAX_PATH, '\0');
   NppMessage(NPPM_GETFULLPATHFROMBUFFERID, bufferID, (LPARAM)filePath.c_str());
   filePath = filePath.c_str();

   for (size_t i{}; i < vDocInfo.size(); ++i) {
      if (vDocInfo[i].fileName == filePath) {
         vDocInfo.erase(vDocInfo.begin() + i);
         return;
      }
   }
}

void VisualizerPanel::showConfigDialog() {
   if (_configIO.fixIfNotUTF8File(_configIO.CONFIG_VIZ))
      _configDlg.doDialog((HINSTANCE)_gModule);
}

void VisualizerPanel::showThemeDialog() {
   if (_configIO.fixIfNotUTF8File(_configIO.CONFIG_THEMES))
      _themeDlg.doDialog((HINSTANCE)_gModule);
}

void VisualizerPanel::jumpToField(const string fileType, const int recordIndex, const int fieldIdx) {
   string currFileType{};
   if (!getDocFileType(currFileType) || (fileType != currFileType)) {
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
   bool hopRight_LeftEdge{ _configIO.getPreferenceBool(PREF_HOP_RT_LEFT_EDGE, FALSE) };
   moveToFieldEdge("", caretFieldIndex + (hopRight_LeftEdge ? 1 : 0), FALSE, !hopRight_LeftEdge, FALSE);
}

void VisualizerPanel::fieldCopy() {
   if (caretFieldIndex < 0) return;

   HWND hScintilla{ GetCurrentScintilla() };
   if (!hScintilla) return;

   intptr_t leftPos{}, rightPos{};
   if (getFieldEdges("", caretFieldIndex, 0, leftPos, rightPos) < 0) return;

   intptr_t fieldLen{ rightPos - leftPos };
   if (fieldLen < 1) return;

   // if no trimming is required, copy to clipbard and return early
   if (!_configIO.getPreferenceBool(PREF_COPY_TRIM, FALSE)) {
      SendMessage(hScintilla, SCI_COPYRANGE, leftPos, rightPos);
      return;
   }

   string padText{};
   padText = Utils::WideToNarrow(_configIO.getPreference(leftAlign ? PREF_PASTE_RPAD : PREF_PASTE_LPAD));
   if (padText.empty()) padText = " ";

   intptr_t padLen{ static_cast<intptr_t>(padText.length()) };
   intptr_t leftTrimLen{}, rightTrimLen{};

   string colText(fieldLen + 1, '\0');
   Sci_TextRangeFull sciTR{};

   sciTR.lpstrText = colText.data();
   sciTR.chrg.cpMin = leftPos;
   sciTR.chrg.cpMax = rightPos;
   SendMessage(hScintilla, SCI_GETTEXTRANGEFULL, NULL, (LPARAM)&sciTR);

   colText = string{ colText.c_str() };

   if (leftAlign && fieldLen >= padLen) {
      // find right-most full/partial match
      intptr_t lastPadLen{ padLen };
      intptr_t matchStart{ fieldLen - padLen };
      intptr_t matchingPos{};

      while (lastPadLen > 0) {
         bool matchFailed{ FALSE };

         for (intptr_t i{}; i < lastPadLen; ++i) {
            matchingPos = matchStart + i;
            if (colText.at(matchingPos) != padText.at(i)) {
               matchFailed = TRUE;
               --lastPadLen;
               ++matchStart;
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
         for (int i{}; i < padLen; ++i) {
            if (colText.at(leftTrimLen) == padText.at(i)) {
               ++leftTrimLen;
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

   HWND hScintilla{ GetCurrentScintilla() };
   if (!hScintilla) return;

   intptr_t leftPos{}, rightPos{};
   if (getFieldEdges("", caretFieldIndex, 0, leftPos, rightPos) < 0) return;

   intptr_t fieldCurrLen{ rightPos - leftPos };
   if (fieldCurrLen < 1) return;

   int fieldLength{ vRecInfo[caretRecordRegIndex].fieldWidths[caretFieldIndex] };

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
      if (padText.empty()) padText = L" ";

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
   wchar_t fDesc[MAX_PATH]{};

   SendMessage(hThemesLB, WM_GETTEXT, MAX_PATH, (LPARAM)fDesc);
   wstring theme{ fDesc };

   if (theme.length() < 2) {
      clearVisualize();
      return;
   }

   setDocTheme(Utils::WideToNarrow(theme));
   loadUsedThemes();
   renderCurrentPage();
}

void VisualizerPanel::clearVisualize(bool sync) {
   if (!fwVizRegexed.empty()) {
      HWND hScintilla{ GetCurrentScintilla() };
      if (!hScintilla) return;

      SendMessage(hScintilla, SCI_STARTSTYLING, 0, NULL);
      SendMessage(hScintilla, SCI_SETSTYLING,
         SendMessage(hScintilla, SCI_GETLENGTH, NULL, NULL), STYLE_DEFAULT);
   }

   setDocFileType("");
   setDocTheme("");
   clearLexer();

   if (sync) {
      syncListFileTypes();
      syncListThemes();
   }
}

int VisualizerPanel::loadTheme(const wstring theme) {
   PSCIFUNC_T sciFunc;
   void* sciPtr;

   if (!GetDirectScintillaFunc(sciFunc, sciPtr)) return -1;

   bool useDefaultBackColor{ _configIO.getPreferenceBool(PREF_DEF_BACKGROUND, FALSE) };
   int defaultBackColor{ static_cast<int>(NppMessage(NPPM_GETEDITORDEFAULTBACKGROUNDCOLOR)) };
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
   vThemes.emplace_back(TI);

   StyleInfo styleInfo{};

   _configIO.getFullStyle(theme, "EOL", styleInfo);
   sciFunc(sciPtr, SCI_STYLESETBACK, styleIndex, (useDefaultBackColor ? defaultBackColor : styleInfo.backColor));
   sciFunc(sciPtr, SCI_STYLESETFORE, styleIndex, styleInfo.foreColor);
   sciFunc(sciPtr, SCI_STYLESETBOLD, styleIndex, styleInfo.bold);
   sciFunc(sciPtr, SCI_STYLESETITALIC, styleIndex, styleInfo.italics);
   ++styleIndex;

   char bufKey[8];
   for (int i{}; i < styleCount; ++i) {
      snprintf(bufKey, 8, "BFBI_%02i", i);
      _configIO.getFullStyle(theme, bufKey, styleInfo);

      sciFunc(sciPtr, SCI_STYLESETBACK, styleIndex, (useDefaultBackColor ? defaultBackColor : styleInfo.backColor));
      sciFunc(sciPtr, SCI_STYLESETFORE, styleIndex, styleInfo.foreColor);
      sciFunc(sciPtr, SCI_STYLESETBOLD, styleIndex, styleInfo.bold);
      sciFunc(sciPtr, SCI_STYLESETITALIC, styleIndex, styleInfo.italics);
      ++styleIndex;
   }

#if FW_DEBUG_SET_STYLES
   wstring dbgMessage;
   int back, fore, bold, italics;

   for (int i{ styleIndex - styleCount }; i < styleIndex; ++i) {
      back = static_cast<int>(sciFunc(sciPtr, SCI_STYLEGETBACK, i, NULL));
      fore = static_cast<int>(sciFunc(sciPtr, SCI_STYLEGETFORE, i, NULL));
      bold = static_cast<int>(sciFunc(sciPtr, SCI_STYLEGETBOLD, i, NULL));
      italics = static_cast<int>(sciFunc(sciPtr, SCI_STYLEGETITALIC, i, NULL));

      dbgMessage = L"C0" + to_wstring(i - styleIndex + styleCount) + L"_STYLES = " +
         to_wstring(back) + L", " + to_wstring(fore) + L", " +
         to_wstring(bold) + L", " + to_wstring(italics);
      MessageBox(_hSelf, dbgMessage.c_str(), L"Theme Styles", MB_OK);
   }
#endif


   return styleCount + 1;  // Add 1 to include EOL styleInfo
}

int VisualizerPanel::loadUsedThemes() {
   loadedStyleCount = 0;
   vThemes.clear();
   initCalltipStyle();

   string fileType;
   if (!getDocFileType(fileType)) return 0;
   _configIO.setVizConfig(fileType);

   wstring fileTheme;
   if (!getDocTheme(fileTheme)) return 0;

   loadedStyleCount += loadTheme(fileTheme);

   // Load Record Type themes different than File Type theme
   vector<string> recTypesList;
   int recTypeCount{ _configIO.getConfigValueList(recTypesList, fileType, "RecordTypes") };

   for (int i{}; i < recTypeCount; ++i) {
      wstring recTheme{};
      recTheme = _configIO.getConfigWideChar(fileType, (recTypesList[i] + "_Theme"));
      if (recTheme == L"") continue;

      bool loaded{ FALSE };
      for (size_t j{}; j < vThemes.size(); ++j) {
         if (recTheme == vThemes[j].name) {
            loaded = TRUE;
            break;
         }
      }

      if (!loaded)
         loadedStyleCount += loadTheme(recTheme);
   }

   return static_cast<int>(vThemes.size());
}

int VisualizerPanel::loadLexer() {
   if (unlexed && !panelMounted) return 0;

   PSCIFUNC_T sciFunc;
   void* sciPtr;

   if (!GetDirectScintillaFunc(sciFunc, sciPtr)) return -1;

   string fileType;
   if (!getDocFileType(fileType)) {
      clearLexer();
      return 0;
   }

   if (fwVizRegexed.compare(fileType) != 0)
      clearLexer();

   if (vRecInfo.size() > 0)
      return static_cast<int>(vRecInfo.size());

   const std::wregex trimSpace{ std::wregex(L"(^( )+)|(( )+$)") };
   int styleIndex{ FW_STYLE_THEMES_START_INDEX + FW_STYLE_THEMES_MAX_ITEMS };
   struct loadedStyle {
      int index;
      string style;
   };
   vector<loadedStyle> loadedStyles{};

   vector<string> recTypes;
   int recTypeCount{ _configIO.getConfigValueList(recTypes, fileType, "RecordTypes") };

   vRecInfo.resize(recTypeCount);

   for (int i{}; i < recTypeCount; ++i) {
      string& recType = recTypes[i];
      RecordInfo& RT = vRecInfo[i];

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

      for (int fnum{}, startPos{}; fnum < fieldCount; ++fnum) {
         RT.fieldStarts[fnum] = startPos;
         startPos += RT.fieldWidths[fnum];
      }

      wstring fieldLabelList, fieldType;
      size_t colonPos{};

      fieldLabelList = _configIO.getConfigWideChar(fileType, (recType + "_FieldLabels"));
      fieldCount = _configIO.Tokenize(fieldLabelList, RT.fieldLabels);

      RT.fieldStyles.clear();
      RT.fieldStyles.resize(fieldCount);

      for (int fnum{}; fnum < fieldCount; ++fnum) {
         wstring& field{ RT.fieldLabels[fnum] };
         RT.fieldStyles[fnum] = -1;

         colonPos = field.find(':');
         if (colonPos != string::npos) {
            fieldType = field.substr(colonPos + 1);
            fieldType = regex_replace(fieldType, trimSpace, L"");

            string styleText{ _configIO.getFieldStyleText(fieldType) };
            if (styleText.length() != 16) {
               RT.fieldStyles[fnum] = STYLE_DEFAULT;
               continue;
            }

            // If this styleInfo is already loaded, just map to that styleInfo slot
            bool styleMatched{ FALSE };
            for (size_t k{}; k < loadedStyles.size(); ++k) {
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

            sciFunc(sciPtr, SCI_STYLESETBACK, styleIndex, fieldStyle.backColor);
            sciFunc(sciPtr, SCI_STYLESETFORE, styleIndex, fieldStyle.foreColor);
            sciFunc(sciPtr, SCI_STYLESETBOLD, styleIndex, fieldStyle.bold);
            sciFunc(sciPtr, SCI_STYLESETITALIC, styleIndex, fieldStyle.italics);

            RT.fieldStyles[fnum] = styleIndex;
            loadedStyles.emplace_back(loadedStyle{ styleIndex, styleText });
            --styleIndex;
         }
      }
   }

   fwVizRegexed = fileType;
   unlexed = FALSE;

#if FW_DEBUG_LOAD_REGEX
   int fieldCount;

   for (int i{}; i < recTypeCount; ++i) {
      wstring dbgMessage;
      wstring& recType = recTypes[i];
      RecordInfo& RT = recInfoList[i];

      dbgMessage = recType + L"\nRec_Label = " + RT.label +
         L"\nRec_Marker = " + _configIO.NarrowToWide(RT.marker) +
         L"\nRec_Theme = " + RT.theme +
         L"\nFieldWidths=\n";

      fieldCount = static_cast<int>(RT.fieldWidths.size());

      for (int j{}; j < fieldCount; ++j) {
         dbgMessage += L" (" + to_wstring(RT.fieldStarts[j]) + L", " + to_wstring(RT.fieldWidths[j]) + L"),";
      }

      MessageBox(_hSelf, dbgMessage.c_str(), L"", MB_OK);
   }
#endif

   return recTypeCount;
}

void VisualizerPanel::applyLexer(const intptr_t startLine, intptr_t endLine) {
   if (unlexed && !panelMounted) return;

   PSCIFUNC_T sciFunc;
   void* sciPtr;

   if (!GetDirectScintillaFunc(sciFunc, sciPtr)) return;

   string fileType;
   if (!getDocFileType(fileType)) return;

   wstring fileTheme;
   if (!getDocTheme(fileTheme)) return;

   if (vThemes.size() < 1) return;

   int shiftPerRec{ vThemes[0].styleCount };
   if (shiftPerRec < 1) return;

   intptr_t lineCount;
   lineCount = sciFunc(sciPtr, SCI_GETLINECOUNT, NULL, NULL);
   if (endLine > lineCount) endLine = lineCount;

   // set shiftPerRec to an integer just less than half of base theme's styleCount
   shiftPerRec = (shiftPerRec < 5) ? 1 : ((shiftPerRec + 1) >> 1) - 1;

   string lineTextCStr(FW_LINE_MAX_LENGTH, '\0');
   string recStartText{}, eolMarker{};
   intptr_t caretLine, eolMarkerPos, recStartLine{}, currentPos, startPos, endPos, recStartPos{};
   size_t eolMarkerLen;
   const size_t regexedCount{ vRecInfo.size() };
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

   for (auto currentLine{ startLine }; currentLine < endLine; ++currentLine) {
      if (sciFunc(sciPtr, SCI_LINELENGTH, currentLine, NULL) > FW_LINE_MAX_LENGTH)
         continue;

      sciFunc(sciPtr, SCI_GETLINE, currentLine, (LPARAM)lineTextCStr.c_str());
      startPos = sciFunc(sciPtr, SCI_POSITIONFROMLINE, currentLine, NULL);
      endPos = sciFunc(sciPtr, SCI_GETLINEENDPOSITION, currentLine, NULL);
      string_view lineText{ lineTextCStr.c_str(), static_cast<size_t>(endPos - startPos) };

      if (newRec) {
         recStartLine = currentLine;
         recStartPos = startPos;
         recStartText = lineText;
      }

      if (newRec && lineText.empty())
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
         if (regex_match(recStartText, vRecInfo[regexIndex].regExpr)) {
            if (caretLine >= recStartLine && caretLine <= currentLine) {
               caretRecordRegIndex = static_cast<int>(regexIndex);
               caretRecordStartPos = static_cast<int>(recStartPos);
               caretRecordEndPos = static_cast<int>(endPos);
               caretEolMarkerPos = static_cast<int>(eolMarkerPos);
            }

            break;
         }

         ++regexIndex;
         colorOffset += shiftPerRec;
      }

      if (regexIndex >= regexedCount) continue;

      const vector<int>& recFieldWidths{ vRecInfo[regexIndex].fieldWidths };
      const size_t fieldCount{ recFieldWidths.size() };

      wstring recTheme{ vRecInfo[regexIndex].theme };
      size_t themeIndex{};

      if ((recTheme != L"") && (recTheme != fileTheme)) {
         for (size_t i{ 0 }; i < vThemes.size(); ++i) {
            if (recTheme == vThemes[i].name) {
               themeIndex = i;
               colorOffset = 0;
               break;
            }
         }
      }

      const int& styleRangeStart{ vThemes[themeIndex].rangeStartIndex };
      const int& styleCount{ vThemes[themeIndex].styleCount };
      if (styleCount < 1) continue;

#if FW_DEBUG_APPLY_LEXER
      wstring dbgMessage;
      size_t dbgPos{ currentPos };

      dbgMessage = L"FieldWidths[" + to_wstring(regexIndex) + L"] = " +
         to_wstring(fieldCount) + L"\n";

      for (int i{}; i < static_cast<int>(fieldCount); ++i) {
         dbgMessage += L" (" + to_wstring(dbgPos) + L", " +
            to_wstring(recFieldWidths[i]) + L", " + to_wstring(eolMarkerPos - eolMarkerLen) + L"), ";
         dbgPos += recFieldWidths[i];
      }

      MessageBox(_hSelf, dbgMessage.c_str(), L"", MB_OK);
#endif

      size_t styleIndex{};
      const vector<int>& recFieldStyles{ vRecInfo[regexIndex].fieldStyles };

      if (byteCols) {
         intptr_t unstyledLen{};

         for (size_t i{}; i < fieldCount; ++i) {
            sciFunc(sciPtr, SCI_STARTSTYLING, currentPos, NULL);
            unstyledLen = eolMarkerPos - currentPos;
            currentPos += recFieldWidths[i];

            styleIndex = (recFieldStyles[i] >= 0) ?
               recFieldStyles[i] : styleRangeStart + ((i + colorOffset) % styleCount);

            if (recFieldWidths[i] < unstyledLen) {
               sciFunc(sciPtr, SCI_SETSTYLING, recFieldWidths[i], styleIndex);
            }
            else {
               sciFunc(sciPtr, SCI_SETSTYLING, unstyledLen, styleIndex);
               unstyledLen = 0;

               sciFunc(sciPtr, SCI_STARTSTYLING, eolMarkerPos, NULL);
               sciFunc(sciPtr, SCI_SETSTYLING, eolMarkerLen, styleRangeStart - 1);
               break;
            }
         }

         if (fieldCount > 0 && unstyledLen > 0) {
            sciFunc(sciPtr, SCI_STARTSTYLING, currentPos, NULL);
            sciFunc(sciPtr, SCI_SETSTYLING, (endPos - currentPos), styleRangeStart - 1);
         }

#if FW_DEBUG_APPLIED_STYLES
         if (currentLine == caretLine) {
            size_t dbgStyleIndex{};
            wstring dbgMessage{}, dbgPre{ L", " }, dbgNoPre{};
            intptr_t dbgPos{};

            dbgPos = recStartPos;
            dbgMessage = L"Input Styles:\n";

            for (size_t i{}; (i < fieldCount) && (dbgPos < eolMarkerPos); ++i) {
               dbgStyleIndex = (recFieldStyles[i] >= 0) ?
                  recFieldStyles[i] : styleRangeStart + ((i + colorOffset) % styleCount);
               dbgMessage += (i == 0 ? dbgNoPre : dbgPre) + L"(" + to_wstring(dbgPos) + L", " +
                  to_wstring(recFieldWidths[i]) + L", " + to_wstring(dbgStyleIndex) + L")";
               dbgPos += recFieldWidths[i];
            }

            dbgPos = recStartPos;
            dbgMessage += L"\n\nApplied Styles:\n";

            for (size_t i{}; (i < fieldCount) && (dbgPos < eolMarkerPos); ++i) {
               dbgMessage += (i == 0 ? dbgNoPre : dbgPre) + L"(" + to_wstring(dbgPos) + L", " +
                  to_wstring(recFieldWidths[i]) + L", " + to_wstring(sciFunc(sciPtr, SCI_GETSTYLEAT, dbgPos, NULL)) + L")";
               dbgPos += recFieldWidths[i];
            }

            dbgMessage += L"\n\nDocument Length: " + to_wstring(sciFunc(sciPtr, SCI_GETLENGTH, NULL, NULL));
            dbgMessage += L"\tEnd Styled: " + to_wstring(sciFunc(sciPtr, SCI_GETENDSTYLED, NULL, NULL));
            MessageBox(_hSelf, dbgMessage.c_str(), L"", MB_OK);
         }
#endif
      }
      else {
         intptr_t nextPos{};
         for (size_t i{}; i < fieldCount; ++i) {
            sciFunc(sciPtr, SCI_STARTSTYLING, currentPos, NULL);
            nextPos = sciFunc(sciPtr, SCI_POSITIONRELATIVE, currentPos, recFieldWidths[i]);

            styleIndex = (recFieldStyles[i] >= 0) ?
               recFieldStyles[i] : styleRangeStart + ((i + colorOffset) % styleCount);

            if (nextPos > 0 && nextPos <= eolMarkerPos) {
               sciFunc(sciPtr, SCI_SETSTYLING, (nextPos - currentPos), styleIndex);
               currentPos = nextPos;
            }
            else {
               sciFunc(sciPtr, SCI_SETSTYLING, (eolMarkerPos - currentPos), styleIndex);

               sciFunc(sciPtr, SCI_STARTSTYLING, eolMarkerPos, NULL);
               sciFunc(sciPtr, SCI_SETSTYLING, eolMarkerLen, styleRangeStart - 1);
               currentPos = 0;
               break;
            }
         }

         if (fieldCount > 0 && currentPos > 0 && eolMarkerPos >= currentPos) {
            sciFunc(sciPtr, SCI_STARTSTYLING, currentPos, NULL);
            sciFunc(sciPtr, SCI_SETSTYLING, (endPos - currentPos), styleRangeStart - 1);
         }
      }
   }
}

void VisualizerPanel::renderScrolledPage(void* view) {
   bool otherView{ (view != GetCurrentScintilla()) };

   if (otherView)
      NppMessage(NPPM_MENUCOMMAND, 0, (LPARAM)IDM_VIEW_SWITCHTO_OTHER_VIEW);

   renderCurrentPage();

   if (otherView)
      NppMessage(NPPM_MENUCOMMAND, 0, (LPARAM)IDM_VIEW_SWITCHTO_OTHER_VIEW);
}

void VisualizerPanel::renderCurrentPage() {
   if (loadLexer() < 1) {
      clearCaretFieldInfo();
      return;
   }

   HWND hScintilla{ GetCurrentScintilla() };
   if (!hScintilla) return;

   intptr_t linesOnScreen, firstVisible, startLine, endLine;

   linesOnScreen = SendMessage(hScintilla, SCI_LINESONSCREEN, NULL, NULL);
   firstVisible = SendMessage(hScintilla, SCI_GETFIRSTVISIBLELINE, NULL, NULL);
   startLine = SendMessage(hScintilla, SCI_DOCLINEFROMVISIBLE, firstVisible, NULL);
   endLine = SendMessage(hScintilla, SCI_DOCLINEFROMVISIBLE, firstVisible + linesOnScreen, NULL);

   applyLexer(startLine, endLine + 1);
   displayCaretFieldInfo(startLine, endLine);
}

void VisualizerPanel::clearCaretFieldInfo() {
   enableFieldControls(FALSE);
   SetWindowText(hFieldInfo, L"");
}

void VisualizerPanel::onPanelResize(LPARAM lParam) {
   RECT rcInfo;
   GetWindowRect(hFieldInfo, &rcInfo);

   // Get fieldInfo top-leftEdge coordinates relative to dock panel
   POINT pt{ rcInfo.left, rcInfo.top };
   ScreenToClient(_hSelf, &pt);

   MoveWindow(hFieldInfo, pt.x, pt.y, (LOWORD(lParam) - pt.x - 3), (rcInfo.bottom - rcInfo.top), TRUE);

   // About button
   HWND hAboutBtn{GetDlgItem(_hSelf, IDC_VIZPANEL_ABOUT_BUTTON)};
   RECT rcAboutBtn;
   GetWindowRect(hAboutBtn, &rcAboutBtn);

   int aboutBtnWidth{rcAboutBtn.right - rcAboutBtn.left};
   int aboutBtnHeight{rcAboutBtn.bottom - rcAboutBtn.top};

   MoveWindow(hAboutBtn, (LOWORD(lParam) - aboutBtnWidth - 3), (HIWORD(lParam) - aboutBtnHeight - 3), aboutBtnWidth, aboutBtnHeight, TRUE);

   // Ini Files Info button
   HWND hIniBtn{GetDlgItem(_hSelf, IDC_VIZPANEL_FILE_INFO_BUTTON)};
   RECT rcIniBtn;
   GetWindowRect(hIniBtn, &rcIniBtn);

   int iniBtnWidth{rcIniBtn.right - rcIniBtn.left};
   int iniBtnHeight{rcIniBtn.bottom - rcIniBtn.top};

   MoveWindow(hIniBtn, (LOWORD(lParam) - aboutBtnWidth - iniBtnWidth - 4), (HIWORD(lParam) - iniBtnHeight - 3), iniBtnWidth, iniBtnHeight, TRUE);
}

int VisualizerPanel::getFieldEdges(const string fileType, const int fieldIdx, const int rightPullback,
   intptr_t& leftPos, intptr_t& rightPos) {
   HWND hScintilla{ GetCurrentScintilla() };
   if (!hScintilla) return -1;

   string currFileType{};

   if (fileType == "") {
      if (!getDocFileType(currFileType)) return -1;
   }
   else {
      currFileType = fileType;
   }

   RecordInfo& FLD{ vRecInfo[caretRecordRegIndex] };

   if (fieldIdx < 0 || fieldIdx >= static_cast<int>(FLD.fieldStarts.size())) return -1;

   int leftOffset{ FLD.fieldStarts[fieldIdx] };
   int rightOffset{ leftOffset + FLD.fieldWidths[fieldIdx] - rightPullback };

   if (!_configIO.getMultiByteLexing(currFileType)) {
      leftPos = caretRecordStartPos + leftOffset;
      rightPos = caretRecordStartPos + rightOffset;
   }
   else {
      leftPos = SendMessage(hScintilla, SCI_POSITIONRELATIVE, caretRecordStartPos, leftOffset);
      rightPos = SendMessage(hScintilla, SCI_POSITIONRELATIVE, caretRecordStartPos, rightOffset);
   }

   if (leftPos >= caretEolMarkerPos)
      leftPos = caretEolMarkerPos - 1;

   if (rightPos >= caretEolMarkerPos)
      rightPos = caretEolMarkerPos - 1;

   return 0;
}

void VisualizerPanel::moveToFieldEdge(const string fileType, const int fieldIdx, bool jumpTo, bool rightEdge, bool hilite) {
   HWND hScintilla{ GetCurrentScintilla() };
   if (!hScintilla) return;

   intptr_t caretPos{ SendMessage(hScintilla, SCI_GETCURRENTPOS, NULL, NULL) };

   if (fieldIdx < 0) {
      if (caretPos >= caretEolMarkerPos) {
         caretPos = caretEolMarkerPos - 1;
         SendMessage(hScintilla, SCI_GOTOPOS, caretPos, NULL);
      }
      return;
   }

   intptr_t leftPos{}, rightPos{};
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

   SendMessage(hScintilla, SCI_SETXCARETPOLICY, CARET_JUMPS | CARET_EVEN, 0);
   SendMessage(hScintilla, SCI_GOTOPOS, (rightEdge ? rightPos : leftPos), 0);

   // Flash caret
   if (hilite) {
      HANDLE hThread = CreateThread(NULL, 0, threadPositionHighlighter, 0, 0, NULL);
      if (hThread) CloseHandle(hThread);
   }

   setFocusOnEditor();
}

void VisualizerPanel::setFieldAlign(bool left) {
   leftAlign = left;
   ShowWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_RPAD_INDIC), leftAlign ? SW_SHOW : SW_HIDE);
   ShowWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_PASTE_LPAD_INDIC), leftAlign ? SW_HIDE : SW_SHOW);
}

void VisualizerPanel::displayCaretFieldInfo(const intptr_t startLine, const intptr_t endLine) {
   if (!isVisible()) return;

   HWND hScintilla{ GetCurrentScintilla() };
   if (!hScintilla) return;

   string fileType;

   if (!getDocFileType(fileType)) return;

   wstring fieldInfoText{};
   intptr_t caretPos;
   intptr_t caretLine;
   bool byteCols{ !_configIO.getMultiByteLexing(fileType) };
   wstring newLine{ L"\r\n" };

   caretFieldIndex = -1;
   caretPos = SendMessage(hScintilla, SCI_GETCURRENTPOS, NULL, NULL);
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
      RecordInfo& FLD{ vRecInfo[caretRecordRegIndex] };
      intptr_t caretColumn, recLength;
      int fieldCount, fieldLabelCount, cumulativeWidth{}, matchedField{ -1 };

      if (byteCols) {
         caretColumn = caretPos - caretRecordStartPos;
         recLength = caretEolMarkerPos - caretRecordStartPos;
      }
      else {
         caretColumn = SendMessage(hScintilla, SCI_COUNTCHARACTERS, caretRecordStartPos, caretPos);
         recLength = SendMessage(hScintilla, SCI_COUNTCHARACTERS, caretRecordStartPos, caretEolMarkerPos);
      }

      fieldInfoText = CUR_POS_DATA_REC_TYPE + FLD.label;
      fieldCount = static_cast<int>(FLD.fieldStarts.size());
      fieldLabelCount = static_cast<int>(FLD.fieldLabels.size());

      for (int i{}; i < fieldCount; ++i) {
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
         setFieldAlign(static_cast<int>(caretColumn - fieldBegin) < (fieldBegin + fieldLength - caretColumn));
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

   if (_configIO.getPreferenceBool(PREF_SHOW_CALLTIP, FALSE)) {
      int foldLevel{ static_cast<int>(SendMessage(hScintilla, SCI_GETFOLDLEVEL, caretLine, 0)) };

      if (foldLevel > 0) {
         int level{ (foldLevel - SC_FOLDLEVELBASE) & SC_FOLDLEVELNUMBERMASK };
         fieldInfoText += L"\n" + wstring(40, '-') +
            ((foldLevel & SC_FOLDLEVELHEADERFLAG) ?
               L"\n  Fold Header: " + to_wstring(level + 1) :
               L"\n   Fold Level: " + to_wstring(level));
      }

      calltipText = Utils::WideToNarrow(fieldInfoText);
      PostMessage(hScintilla, SCI_CALLTIPSHOW, caretPos, (LPARAM)calltipText.c_str());
   }
}

void VisualizerPanel::showJumpDialog() {
   string fileType;
   if (!getDocFileType(fileType)) return;

   RecordInfo& FLD{ vRecInfo[caretRecordRegIndex] };

   int fieldCount = static_cast<int>(FLD.fieldStarts.size());
   int fieldLabelCount = static_cast<int>(FLD.fieldLabels.size());

   vector<wstring> fieldLabels;
   fieldLabels.resize(fieldCount);

   for (int i{}; i < fieldCount; ++i) {
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

void VisualizerPanel::showAboutDialog() {
   _aboutDlg.doDialog((HINSTANCE)_gModule);
}

void VisualizerPanel::showExtractDialog() {
   string fileType;
   if (!getDocFileType(fileType)) return;

   _dataExtractDlg.doDialog((HINSTANCE)_gModule);
   _dataExtractDlg.initDialog(fileType, vRecInfo);
}

bool VisualizerPanel::detectFileType(HWND hScintilla, string& fileType) {
   if (!panelMounted) return FALSE;

   bool detected{ detectFileTypeByVizConfig(hScintilla, fileType, FALSE) };

   if (!detected)
      detected = detectFileTypeByVizConfig(hScintilla, fileType, TRUE);

   return detected;
}

bool VisualizerPanel::detectFileTypeByVizConfig(HWND hScintilla, string& fileType, bool defaultVizConfig) {
   if (defaultVizConfig) {
      _configIO.defaultVizConfig();
   }
   else {
      _configIO.userVizConfig();
      if (!_configIO.checkConfigFilesforUTF8()) return FALSE;
   }

   string lineTextCStr(FW_LINE_MAX_LENGTH, '\0');
   intptr_t startPos, endPos;

   vector<string> fileTypes;
   _configIO.getConfigValueList(fileTypes, "Base", "FileTypes");

   for (string fType : fileTypes) {
      bool matched{ FALSE };

      for (int i{}; i < ADFT_MAX; ++i) {
         char idx[5];
         snprintf(idx, 5, "%02d", i + 1);

         intptr_t line = _configIO.getConfigInt(fType, "ADFT_Line_" + string{ idx });
         if (line == 0) continue;

         string strRegex = _configIO.getConfigStringA(fType, "ADFT_Regex_" + string{ idx });
         if (strRegex.empty()) continue;
         if (Utils::isInvalidRegex(strRegex)) continue;

         intptr_t lineCount = SendMessage(hScintilla, SCI_GETLINECOUNT, NULL, NULL);

         line += (line < 0) ? lineCount : -1;
         if (line < 0 || line >= lineCount) continue;

         if (strRegex.substr(strRegex.length() - 1) != "$")
            strRegex += ".*";

         if (SendMessage(hScintilla, SCI_LINELENGTH, line, NULL) > FW_LINE_MAX_LENGTH)
            continue;

         SendMessage(hScintilla, SCI_GETLINE, line, (LPARAM)lineTextCStr.c_str());
         startPos = SendMessage(hScintilla, SCI_POSITIONFROMLINE, line, NULL);
         endPos = SendMessage(hScintilla, SCI_GETLINEENDPOSITION, line, NULL);
         string_view lineText{ lineTextCStr.c_str(), static_cast<size_t>(endPos - startPos) };

         if (regex_match(string{ lineText }, regex{ strRegex }))
            matched = TRUE;
         else {
            matched = FALSE;
            break;
         }
      }

      if (matched) {
         fileType = fType;
         setDocFileType(fileType);

         wstring theme{};
         if (!getDocTheme(theme)) setDocTheme("", fileType);
         break;
      }
   }

   if (defaultVizConfig && fileType.empty())
      _configIO.userVizConfig();

   return (!fileType.empty());
}

const wstring VisualizerPanel::getCurrentFileName() {
   wstring fileName(MAX_PATH, '\0');
   NppMessage(NPPM_GETFULLCURRENTPATH, MAX_PATH, (LPARAM)fileName.c_str());
   return wstring{ fileName.c_str() };
}

void VisualizerPanel::setDocInfo(bool bDocType, string val) {
   const wstring fileName{ getCurrentFileName() };

   for (DocInfo& DI : vDocInfo) {
      if (fileName == DI.fileName) {
         if (bDocType) DI.docType = val;
         if (!bDocType) DI.docTheme = val;
         return;
      }
   }

   DocInfo fi{fileName, (bDocType ? val : ""), (!bDocType ? val : "")};

   vDocInfo.emplace_back(fi);
}

bool VisualizerPanel::getDocFileType(string& fileType) {
   const wstring fileName{ getCurrentFileName() };
   fileType = "";

   for (const DocInfo& DI : vDocInfo) {
      if (fileName == DI.fileName) {
         fileType = DI.docType;
         break;
      }
   }

   return (!fileType.empty());
}

bool VisualizerPanel::getDocTheme(wstring& theme) {
   const wstring fileName{ getCurrentFileName() };
   theme = L"";

   for (const DocInfo& DI : vDocInfo) {
      if (fileName == DI.fileName) {
         theme = Utils::NarrowToWide(DI.docTheme);
         break;
      }
   }

   return (!theme.empty());
}

string VisualizerPanel::getDocFoldStructType() {
   const wstring fileName{ getCurrentFileName() };

   for (const DocInfo& DI : vDocInfo) {
      if (fileName == DI.fileName) return DI.foldStructType;
   }
   return "";
}

bool VisualizerPanel::getDocFolded() {
   const wstring fileName{ getCurrentFileName() };

   for (const DocInfo& DI : vDocInfo) {
      if (fileName == DI.fileName) return DI.folded;
   }
   return false;
}

void VisualizerPanel::setDocFileType(string fileType) {
   if (!panelMounted) return;

   enableThemeList(!fileType.empty());
   setDocInfo(true, fileType);
}

void VisualizerPanel::setDocTheme(string theme, string fileType) {
   if (theme.empty() && (!fileType.empty()))
      theme = _configIO.getConfigStringA(fileType, "FileTheme");

   setDocInfo(false, theme);
}

void VisualizerPanel::setDocFoldStructType(string foldStructType) {
   const wstring fileName{ getCurrentFileName() };

   for (DocInfo& DI : vDocInfo) {
      if (fileName == DI.fileName) {
         DI.foldStructType = foldStructType;
         return;
      }
   }
}

void VisualizerPanel::setDocFolded(bool bFolding) {
   const wstring fileName{ getCurrentFileName() };

   for (DocInfo& DI : vDocInfo) {
      if (fileName == DI.fileName) {
         DI.folded = bFolding;
         return;
      }
   }
}

void VisualizerPanel::setADFTCheckbox() {
   bool checked{ IsDlgButtonChecked(_hSelf, IDC_VIZPANEL_AUTO_DETECT_FT) == BST_CHECKED };

   _configIO.setPreferenceBool(PREF_ADFT, checked);
   if (checked) visualizeFile("", FALSE, TRUE, TRUE);
}

void VisualizerPanel::setPanelMBCharState() {
   _configIO.setPanelMBCharState(IsDlgButtonChecked(_hSelf, IDC_VIZPANEL_MCBS_OVERRIDE));
   visualizeFile("", TRUE, (IsDlgButtonChecked(_hSelf, IDC_VIZPANEL_AUTO_DETECT_FT) == BST_CHECKED), TRUE);
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

void VisualizerPanel::setDefaultBackground() {
   _configIO.setPreferenceBool(PREF_DEF_BACKGROUND, (IsDlgButtonChecked(_hSelf, IDC_VIZPANEL_DEFAULT_BACKGROUND) == BST_CHECKED));
   visualizeFile("", TRUE, (IsDlgButtonChecked(_hSelf, IDC_VIZPANEL_AUTO_DETECT_FT) == BST_CHECKED), TRUE);
}

void VisualizerPanel::setShowCalltip() {
   _configIO.setPreferenceBool(PREF_SHOW_CALLTIP, (IsDlgButtonChecked(_hSelf, IDC_VIZPANEL_SHOW_CALLTIP) == BST_CHECKED));
   visualizeFile("", TRUE, (IsDlgButtonChecked(_hSelf, IDC_VIZPANEL_AUTO_DETECT_FT) == BST_CHECKED), TRUE);
}

void VisualizerPanel::initCalltipStyle() {
   HWND hScintilla{ GetCurrentScintilla() };
   if (!hScintilla) return;

   SendMessage(hScintilla, SCI_STYLESETFONT, STYLE_CALLTIP, (LPARAM)"Consolas");
   SendMessage(hScintilla, SCI_STYLESETWEIGHT, STYLE_CALLTIP, 550);
   SendMessage(hScintilla, SCI_STYLESETFORE, STYLE_CALLTIP, RGB(175, 95, 63));
   SendMessage(hScintilla, SCI_STYLESETBACK, STYLE_CALLTIP, RGB(255, 255, 255));
   SendMessage(hScintilla, SCI_CALLTIPUSESTYLE, 3, 0);
}

void VisualizerPanel::onBufferActivate() {
   unlexed = TRUE;
   enableFoldableControls(FALSE);

   if (panelMounted) visualizeFile("", TRUE, TRUE, TRUE);

   enableFoldedControls(getDocFolded());
}

void VisualizerPanel::setFocusOnEditor() {
   HWND hScintilla{ GetCurrentScintilla() };
   if (!hScintilla) return;

   SendMessage(hScintilla, SCI_GRABFOCUS, 0, 0);
}

void VisualizerPanel::clearLexer() {
   vRecInfo.clear();
   fwVizRegexed = "";
}

void VisualizerPanel::popupSamplesMenu() {
   HMENU hPopupMenu = CreatePopupMenu();
   _submenu.initSamplesPopup(hPopupMenu);

   RECT rc;
   GetWindowRect(GetDlgItem(_hSelf, IDC_VIZPANEL_FILE_SAMPLES_BTN), &rc);

   int cmd = TrackPopupMenu(hPopupMenu, TPM_RIGHTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD,
      rc.right, rc.bottom, 0, _hSelf, NULL);

   if (cmd) NppMessage(NPPM_MENUCOMMAND, 0, cmd);

   // Calling RemoveMenu is needed since the appended items are being referenced from the NPP Main menu.
   // In RemoveMenu, zero is used as the position since items shift down with each remove call.
   // 'i' is used only as a loopcounter.
   int itemCount = GetMenuItemCount(hPopupMenu);

   for (int i{}; i < itemCount; ++i) {
      RemoveMenu(hPopupMenu, 0, MF_BYPOSITION);
   }
   DestroyMenu(hPopupMenu);
}

string VisualizerPanel::detectFoldStructType(string fileType) {
   if (fileType.empty()) return "";

   int foldStructCount{ _configIO.getFoldStructCount() };
   string fsType(6, '\0');

   for (int i{}; i < foldStructCount; ++i) {
      snprintf(fsType.data(), 6, "FS%03d", i + 1);
      if (_configIO.getFoldStructValueA(fsType, "FileType") == fileType) {
         setDocFoldStructType(fsType);
         return fsType.c_str();
      }
   }
   return "";
}

void VisualizerPanel::applyFolding(string fsType) {
   if (getDocFolded()) return;

   string fileType{};
   getDocFileType(fileType);
   if (fileType.empty()) return;

   PSCIFUNC_T sciFunc;
   void* sciPtr;

   if (fsType.empty())
      fsType = getDocFoldStructType();
   if (fsType.empty()) return;

   vector<FoldingInfo> foldingInfoList{};
   _configIO.getFoldStructFoldingInfo(Utils::NarrowToWide(fsType), foldingInfoList);

   if (!GetDirectScintillaFunc(sciFunc, sciPtr)) return;

   const size_t regexedCount{ vRecInfo.size() };

   string lineTextCStr(FW_LINE_MAX_LENGTH, '\0');
   string recStartText{}, eolMarker{};

   size_t eolMarkerLen, eolMarkerPos, recStartLine{}, startPos, endPos, recStartPos{};
   bool newRec{ TRUE };

   vector<FoldingInfo> foldStack{ {} };
   int currentLevel{ 0 };
   bool bFoldExists{};

   eolMarker = _configIO.getConfigStringA(fileType, "RecordTerminator");
   eolMarkerLen = eolMarker.length();

#if FW_DEBUG_FOLD_INFO
   wstring info{ L"Line\tRec. Type\tLevel\n" };
#endif // FW_DEBUG_FOLD_INFO

   SetCursor(LoadCursor(NULL, IDC_WAIT));
   const intptr_t lineCount{ sciFunc(sciPtr, SCI_GETLINECOUNT, NULL, NULL) };
   const intptr_t endLine{ lineCount };

   for (intptr_t currentLine{}; currentLine < endLine; ++currentLine) {
      if (sciFunc(sciPtr, SCI_LINELENGTH, currentLine, NULL) > FW_LINE_MAX_LENGTH) {
         continue;
      }

      sciFunc(sciPtr, SCI_GETLINE, currentLine, (LPARAM)lineTextCStr.c_str());
      startPos = sciFunc(sciPtr, SCI_POSITIONFROMLINE, currentLine, NULL);
      endPos = sciFunc(sciPtr, SCI_GETLINEENDPOSITION, currentLine, NULL);
      string_view lineText{ lineTextCStr.c_str(), static_cast<size_t>(endPos - startPos) };

      if (newRec) {
         recStartLine = currentLine;
         recStartPos = startPos;
         recStartText = lineText;
      }

      if (newRec && lineText.empty()) {
         continue;
      }

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

      size_t regexIndex{};

      while (regexIndex < regexedCount) {
         if (regex_match(recStartText, vRecInfo[regexIndex].regExpr)) break;
         ++regexIndex;
      }

      if (regexIndex >= regexedCount) {
         sciFunc(sciPtr, SCI_SETFOLDLEVEL, currentLine, SC_FOLDLEVELBASE | currentLevel);
         continue;
      }

      FoldingInfo* pLevelFI{ &foldStack.back() };
      wchar_t recTypeCode[7];
      swprintf(recTypeCode, 7, L"REC%03d", static_cast<int>(regexIndex + 1));

      bool bFoldLevelRec{};
      FoldingInfo* pLineFI{};

      for (size_t j{}; j < foldingInfoList.size(); ++j) {
         pLineFI = &foldingInfoList[j];

         if (static_cast<int>(regexIndex + 1) == pLineFI->recTypeIndex) {
            bFoldLevelRec = TRUE;
            break;
         }
      }

      if (bFoldLevelRec) {
         while (currentLevel > 0 &&
            (pLevelFI->priority > pLineFI->priority ||
               (pLevelFI->priority == pLineFI->priority && !pLineFI->recursive) ||
               (!pLevelFI->endRecords.empty() && pLevelFI->endRecords.find(recTypeCode) != string::npos)))
         {
            foldStack.pop_back();
            pLevelFI = &foldStack.back();
            --currentLevel;

            if (pLevelFI->recursive && pLevelFI->priority <= pLineFI->priority) break;
         }

#if FW_DEBUG_FOLD_INFO
         //sciFunc(sciPtr, SCI_SETLINEINDENTATION, currentLine, (currentLevel * 2));
         if (currentLine < 50)
            info += to_wstring(currentLine + 1) + L"\t" + recTypeCode + L"\t" + to_wstring(currentLevel + 1) + L"\n";
#endif // FW_DEBUG_FOLD_INFO

         sciFunc(sciPtr, SCI_SETFOLDLEVEL, currentLine, SC_FOLDLEVELHEADERFLAG | SC_FOLDLEVELBASE | currentLevel);
         foldStack.emplace_back(*pLineFI);
         ++currentLevel;
         bFoldExists = true;
      }
      else {
         if (currentLevel > 0 &&
            !pLevelFI->endRecords.empty() &&
            pLevelFI->endRecords.find(recTypeCode) != string::npos) {
            foldStack.pop_back();
            pLevelFI = &foldStack.back();
            sciFunc(sciPtr, SCI_SETFOLDLEVEL, currentLine, SC_FOLDLEVELBASE | currentLevel--);
         }
         else
            sciFunc(sciPtr, SCI_SETFOLDLEVEL, currentLine, SC_FOLDLEVELBASE | currentLevel);

#if FW_DEBUG_FOLD_INFO
         //sciFunc(sciPtr, SCI_SETLINEINDENTATION, currentLine, (currentLevel * 2));
         if (currentLine < 50)
            info += to_wstring(currentLine + 1) + L"\t" + recTypeCode + L"\t" + to_wstring(currentLevel + 1) + L"\n";
#endif // FW_DEBUG_FOLD_INFO

      }
   }

   SetCursor(LoadCursor(NULL, IDC_ARROW));
   enableFoldedControls(bFoldExists);
   setDocFolded(bFoldExists);

#if FW_DEBUG_FOLD_INFO
   MessageBox(_hSelf, (_configIO.getActiveConfigFile(_configIO.CONFIG_FOLDSTRUCTS) + L"\n\n" + info).c_str(),
      L"Fold Levels Info", MB_OK);
#endif // FW_DEBUG_FOLD_INFO
}

void VisualizerPanel::removeFolding() {
   // First, unfold all levels
   expandFoldLevel(TRUE, MAXBYTE);

   PSCIFUNC_T sciFunc;
   void* sciPtr;

   if (!GetDirectScintillaFunc(sciFunc, sciPtr)) return;

   SetCursor(LoadCursor(NULL, IDC_WAIT));
   const intptr_t lineCount{ sciFunc(sciPtr, SCI_GETLINECOUNT, NULL, NULL) };

   const intptr_t endLine{ lineCount };
   for (intptr_t currentLine{}; currentLine < endLine; ++currentLine) {
      sciFunc(sciPtr, SCI_SETFOLDLEVEL, currentLine, SC_FOLDLEVELBASE);
   }

   SetCursor(LoadCursor(NULL, IDC_ARROW));
   enableFoldedControls(FALSE);
   setDocFolded(FALSE);
}

void VisualizerPanel::enableFoldableControls(bool bFoldable) {
   EnableWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_FOLDING_APPLY_BTN), bFoldable);
}

void VisualizerPanel::enableFoldedControls(bool bFolded) {
   SetWindowText(GetDlgItem(_hSelf, IDC_VIZPANEL_FOLDING_APPLY_BTN), bFolded ? VIZPANEL_FOLD_REAPPLY_BTN : VIZPANEL_FOLD_APPLY_BTN);
   EnableWindow(GetDlgItem(_hSelf, IDC_VIZPANEL_FOLDING_REMOVE_BTN), bFolded);

   if (bFolded) {
      HWND hScintilla{ GetCurrentScintilla() };
      if (!hScintilla) return;

      SendMessage(hScintilla, SCI_SETMARGINTYPEN, FOLDING_MARGIN, SC_MARGIN_SYMBOL);
      SendMessage(hScintilla, SCI_SETMARGINMASKN, FOLDING_MARGIN, SC_MASK_FOLDERS);
      SendMessage(hScintilla, SCI_SETMARGINSENSITIVEN, FOLDING_MARGIN, TRUE);
      SendMessage(hScintilla, SCI_SETMARGINWIDTHN, FOLDING_MARGIN, Utils::scaleDPIX(14));
      SendMessage(hScintilla, SCI_SETFOLDFLAGS, SC_FOLDFLAG_LINEBEFORE_CONTRACTED | SC_FOLDFLAG_LINEAFTER_CONTRACTED, NULL);

      if (SendMessage(hScintilla, SCI_MARKERSYMBOLDEFINED, SC_MARKNUM_FOLDEROPEN, 0) < SC_MARK_ARROWDOWN) {
         SendMessage(hScintilla, SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS);
         SendMessage(hScintilla, SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS);
         SendMessage(hScintilla, SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
         SendMessage(hScintilla, SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER);
         SendMessage(hScintilla, SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER);
         SendMessage(hScintilla, SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED);
         SendMessage(hScintilla, SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_BOXPLUSCONNECTED);
      }
   }
}

void VisualizerPanel::toggleFolding() {
   HWND hScintilla{ GetCurrentScintilla() };
   if (!hScintilla) return;

   intptr_t currentLine, headerLine;

   currentLine = SendMessage(hScintilla, SCI_LINEFROMPOSITION, SendMessage(hScintilla, SCI_GETCURRENTPOS, 0, 0), 0);

   if (SendMessage(hScintilla, SCI_GETFOLDLEVEL, currentLine, 0) & SC_FOLDLEVELHEADERFLAG)
      headerLine = currentLine;
   else {
      headerLine = SendMessage(hScintilla, SCI_GETFOLDPARENT, currentLine, 0);
      if (headerLine == -1) return;
   }

   SendMessage(hScintilla, SCI_TOGGLEFOLD, headerLine, 0);
   renderCurrentPage();
}

int VisualizerPanel::foldLevelFromPopup(bool bFold) {
   constexpr int itemCount{ 10 };

   HMENU hPopupMenu = CreatePopupMenu();
   AppendMenu(hPopupMenu, MF_STRING, MAXBYTE, L"All Levels");
   AppendMenu(hPopupMenu, MF_SEPARATOR, NULL, NULL);

   for (int i{1}; i <= itemCount; ++i) {
      AppendMenu(hPopupMenu, MF_STRING, i, (L"Level " + to_wstring(i)).c_str());
   }

   RECT rc;
   GetWindowRect(GetDlgItem(_hSelf, bFold ? IDC_VIZPANEL_FOLDING_FOLD_BTN : IDC_VIZPANEL_FOLDING_UNFOLD_BTN), &rc);

   int option = TrackPopupMenu(hPopupMenu,
      (bFold ? TPM_LEFTALIGN : TPM_RIGHTALIGN) | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_NONOTIFY,
      (bFold ? rc.left : rc.right), rc.bottom, 0, _hSelf, NULL);

   for (int i{}; i < itemCount; ++i) {
      RemoveMenu(hPopupMenu, 0, MF_BYPOSITION);
   }
   DestroyMenu(hPopupMenu);

   return (option == MAXBYTE ? MAXBYTE : option - 1);
}

void VisualizerPanel::expandFoldLevel(bool bExpand, int foldLevel) {
   if (foldLevel < 0) return;

   HWND hScintilla{ GetCurrentScintilla() };
   if (!hScintilla) return;

   SetCursor(LoadCursor(NULL, IDC_WAIT));
   intptr_t lineCount{ SendMessage(hScintilla, SCI_GETLINECOUNT, 0, 0) };

   for (intptr_t line{ 0 }; line < lineCount; ++line) {
      int level{ static_cast<int>(SendMessage(hScintilla, SCI_GETFOLDLEVEL, line, 0)) };
      if (!(level & SC_FOLDLEVELHEADERFLAG)) continue;

      level -= SC_FOLDLEVELBASE;
      if (foldLevel < MAXBYTE && foldLevel != (level & SC_FOLDLEVELNUMBERMASK)) continue;

      if (static_cast<bool>(SendMessage(hScintilla, SCI_GETFOLDEXPANDED, line, 0)) != bExpand)
         SendMessage(hScintilla, SCI_TOGGLEFOLD, line, 0);
   }

   SetCursor(LoadCursor(NULL, IDC_ARROW));
   renderCurrentPage();
}

void VisualizerPanel::foldLevelMenu() {
   expandFoldLevel(FALSE, foldLevelFromPopup(TRUE));
}

void VisualizerPanel::unfoldLevelMenu() {
   expandFoldLevel(TRUE, foldLevelFromPopup(FALSE));
}

void VisualizerPanel::showFoldStructDialog() {
   _foldStructDlg.doDialog((HINSTANCE)_gModule);
}

DWORD __stdcall VisualizerPanel::threadPositionHighlighter(void*) {
   HWND hScintilla{ GetCurrentScintilla() };
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

   SendMessage(hScintilla, SCI_BRACEHIGHLIGHT, SendMessage(hScintilla, SCI_GETCURRENTPOS, 0, 0), -1);

   // Clear Idem Potency Hold
   idemPotentKey = FALSE;

   return TRUE;
}
