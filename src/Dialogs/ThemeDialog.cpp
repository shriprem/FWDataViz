#include "ThemeDialog.h"
#include "EximFileTypeDialog.h"

//#define HEX_COL_LEN 6

extern HINSTANCE _gModule;
extern ThemeDialog _themeDlg;
extern EximFileTypeDialog _eximDlg;

LRESULT CALLBACK procStylesListBox(HWND hwnd, UINT messageId, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
   switch (messageId) {
      case WM_VSCROLL:
         _themeDlg.initPreviewSwatch();
         break;
   }

   return DefSubclassProc(hwnd, messageId, wParam, lParam);
}

LRESULT CALLBACK procHexColorEditControl(HWND hwnd, UINT messageId, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
   switch (messageId) {
      case WM_CHAR:
      {
         char ch{ static_cast<char>(wParam) };
         if (!((ch == VK_BACK) || (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'))) {
            showEditBalloonTip(hwnd, THEME_DIALOG_HEX_TITLE, THEME_DIALOG_HEX_CHARS_ONLY);
            return FALSE;
         }
         break;
      }

      case WM_PASTE:
      {
         wstring clipText;
         Utils::getClipboardText(GetParent(hwnd), clipText);
         if (!regex_match(clipText, std::wregex(L"^[0-9A-Fa-f]{0,6}$"))) {
            showEditBalloonTip(hwnd, THEME_DIALOG_HEX_TITLE, THEME_DIALOG_HEX_CHARS_ONLY);
            return FALSE;
         }
         break;
      }
   }

   return DefSubclassProc(hwnd, messageId, wParam, lParam);
}

ThemeDialog::~ThemeDialog() {
   if (hbr != NULL) DeleteObject(hbr);
};

void ThemeDialog::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_THEME_DEFINER_DIALOG);
   }

   hThemesLB = GetDlgItem(_hSelf, IDC_THEME_DEF_LIST_BOX);
   hStylesLB = GetDlgItem(_hSelf, IDC_THEME_STYLE_LIST_BOX);

   SendDlgItemMessage(_hSelf, IDC_THEME_DEF_DESC_EDIT, EM_LIMITTEXT, MAX_PATH, NULL);
   SendDlgItemMessage(_hSelf, IDC_THEME_STYLE_DEF_BACK_EDIT, EM_LIMITTEXT, 6, NULL);
   SendDlgItemMessage(_hSelf, IDC_THEME_STYLE_DEF_FORE_EDIT, EM_LIMITTEXT, 6, NULL);

   SetWindowSubclass(GetDlgItem(_hSelf, IDC_THEME_STYLE_LIST_BOX), procStylesListBox, NULL, NULL);
   SetWindowSubclass(GetDlgItem(_hSelf, IDC_THEME_STYLE_DEF_BACK_EDIT), procHexColorEditControl, NULL, NULL);
   SetWindowSubclass(GetDlgItem(_hSelf, IDC_THEME_STYLE_DEF_FORE_EDIT), procHexColorEditControl, NULL, NULL);

   Utils::loadBitmap(_hSelf, IDC_THEME_DEF_DOWN_BUTTON, IDB_VIZ_MOVE_DOWN_BITMAP);
   Utils::addTooltip(_hSelf, IDC_THEME_DEF_DOWN_BUTTON, NULL, THEME_DEF_MOVE_DOWN, FALSE);

   Utils::loadBitmap(_hSelf, IDC_THEME_DEF_UP_BUTTON, IDB_VIZ_MOVE_UP_BITMAP);
   Utils::addTooltip(_hSelf, IDC_THEME_DEF_UP_BUTTON, NULL, THEME_DEF_MOVE_UP, FALSE);

   Utils::loadBitmap(_hSelf, IDC_THEME_STYLE_DOWN_BUTTON, IDB_VIZ_MOVE_DOWN_BITMAP);
   Utils::addTooltip(_hSelf, IDC_THEME_STYLE_DOWN_BUTTON, NULL, THEME_STYLE_MOVE_DOWN, FALSE);

   Utils::loadBitmap(_hSelf, IDC_THEME_STYLE_UP_BUTTON, IDB_VIZ_MOVE_UP_BITMAP);
   Utils::addTooltip(_hSelf, IDC_THEME_STYLE_UP_BUTTON, NULL, THEME_STYLE_MOVE_UP, FALSE);

   Utils::loadBitmap(_hSelf, IDC_THEME_DEF_INFO_BUTTON, IDB_VIZ_INFO_BITMAP);
   Utils::addTooltip(_hSelf, IDC_THEME_DEF_INFO_BUTTON, NULL, VIZ_PANEL_INFO_TIP, FALSE);

   bool recentOS = Utils::checkBaseOS(WV_VISTA);
   wstring fontName = recentOS ? L"Consolas" : L"Courier New";
   int fontHeight = recentOS ? 8 : 7;

   Utils::setFont(_hSelf, IDC_THEME_STYLE_DEF_OUTPUT, fontName, fontHeight);

   if (_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
   SetFocus(_hSelf);

   loadConfigInfo();
   fillThemes();
   setPangram();
}

INT_PTR CALLBACK ThemeDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
   switch (message) {
      case WM_COMMAND:
         switch LOWORD(wParam) {
            case IDC_THEME_DEF_LIST_BOX:
               switch HIWORD(wParam) {
                  case LBN_SELCHANGE:
                     onThemeSelect();
                     break;
               }
               break;

            case IDC_THEME_DEF_DOWN_BUTTON:
               moveThemeType(MOVE_DOWN);
               break;

            case IDC_THEME_DEF_UP_BUTTON:
               moveThemeType(MOVE_UP);
               break;

            case IDC_THEME_DEF_INFO_BUTTON:
               ShellExecute(NULL, L"open", THEME_DEF_INFO_README, NULL, NULL, SW_SHOW);
               break;

            case IDC_THEME_DEF_DESC_EDIT:
               switch HIWORD(wParam) {
                  case EN_CHANGE:
                     if (!loadingEdits) {
                        cleanThemeVals = FALSE;
                        enableThemeSelection();
                     }
                     break;
                  }
               break;

            case IDC_THEME_DEF_ACCEPT_BTN:
               themeEditAccept();
               break;

            case IDC_THEME_DEF_NEW_BTN:
               themeEditNew();
               break;

            case IDC_THEME_DEF_CLONE_BTN:
               themeEditClone();
               break;

            case IDC_THEME_DEF_DEL_BTN:
               themeEditDelete();
               break;

            case IDC_THEME_STYLE_LIST_BOX:
               switch HIWORD(wParam) {
                  case LBN_SELCHANGE:
                     onStyleSelect();
                     if (swatchTopIndex !=
                        static_cast<int>(SendDlgItemMessage(_hSelf, IDC_THEME_STYLE_LIST_BOX, LB_GETTOPINDEX, NULL, NULL)))
                        initPreviewSwatch();
                     break;
               }
               break;

            case IDC_THEME_STYLE_DOWN_BUTTON:
               moveStyleType(MOVE_DOWN);
               break;

            case IDC_THEME_STYLE_UP_BUTTON:
               moveStyleType(MOVE_UP);
               break;

            case IDC_THEME_STYLE_NEW_BTN:
               styleEditNew(FALSE);
               break;

            case IDC_THEME_STYLE_CLONE_BTN:
               styleEditNew(TRUE);
               break;

            case IDC_THEME_STYLE_DEL_BTN:
               styleEditDelete();
               break;

            case IDC_THEME_STYLE_DEF_BACK_EDIT:
            case IDC_THEME_STYLE_DEF_FORE_EDIT:
               switch (HIWORD(wParam)) {
                  case EN_CHANGE:
                     bool back = (LOWORD(wParam) == IDC_THEME_STYLE_DEF_BACK_EDIT);
                     setStyleDefColor(FALSE, getStyleDefColor(back), back);
                     if (!loadingEdits) {
                        cleanStyleDefs = FALSE;
                        enableStyleSelection();
                     }
                     break;
               }
               break;

            case IDC_THEME_STYLE_DEF_BACKCOLOR:
               chooseStyleDefColor(TRUE);
               break;

            case IDC_THEME_STYLE_DEF_FORECOLOR:
               chooseStyleDefColor(FALSE);
               break;

            case IDC_THEME_STYLE_DEF_BOLD:
            case IDC_THEME_STYLE_DEF_ITALICS:
               setOutputFontStyle();
               cleanStyleDefs = FALSE;
               enableStyleSelection();
               break;

            case IDC_THEME_STYLE_DEF_OUTPUT:
               setPangram();
               break;

            case IDC_THEME_STYLE_DEF_ACCEPT_BTN:
               styleDefsAccept();
               break;

            case IDC_THEME_STYLE_DEF_RESET_BTN:
               fillStyleDefs();
               break;

            case IDCANCEL:
            case IDCLOSE:
               if (promptDiscardChangesNo()) return TRUE;
               display(FALSE);
               return TRUE;

            case IDC_THEME_DEF_SAVE_CONFIG_BTN:
               SetCursor(LoadCursor(NULL, IDC_WAIT));
               saveConfigInfo();
               SetCursor(LoadCursor(NULL, IDC_ARROW));
               return TRUE;

            case IDC_THEME_DEF_RESET_BTN:
               if (!promptDiscardChangesNo()) {
                  themeFile = L"";
                  loadConfigInfo();
                  fillThemes();
               }
               break;

            case IDC_THEME_DEF_BACKUP_LOAD_BTN:
               if (!promptDiscardChangesNo()) {
                  wstring backupThemeFile;

                  if (_configIO.queryConfigFileName(_hSelf, TRUE, TRUE, FALSE, backupThemeFile)) {
                     themeFile = backupThemeFile;
                     loadConfigInfo();
                     fillThemes();
                     cleanConfigFile = FALSE;
                     enableThemeSelection();
                  }
               }
               break;

            case IDC_THEME_DEF_BACKUP_VIEW_BTN:
               _configIO.viewBackupFolder();
               break;

            case IDC_THEME_DEF_EXTRACT_BTN:
               showEximDialog(TRUE);
               break;

            case IDC_THEME_DEF_APPEND_BTN:
               showEximDialog(FALSE);
               break;

            default:
               if (cleanStyleDefs)
                  processSwatchClick(LOWORD(wParam));
         }
         break;

      case WM_CTLCOLORSTATIC:
         if (styleDefColor) {
            INT_PTR ptr = colorStaticControl(wParam, lParam);
            if (ptr != NULL) return ptr;

            ptr = colorPreviewSwatch(wParam, lParam);
            if (ptr != NULL) return ptr;
         }

         if (NppDarkMode::isEnabled()) {
            return NppDarkMode::onCtlColorDarker(reinterpret_cast<HDC>(wParam));
         }

         break;

      case WM_INITDIALOG:
         if (NppDarkMode::isEnabled()) {
            NppDarkMode::autoSubclassAndThemeChildControls(_hSelf);
         }
         break;

      case WM_CTLCOLORDLG:
      case WM_CTLCOLORLISTBOX:
         if (NppDarkMode::isEnabled()) {
            return NppDarkMode::onCtlColorDarker(reinterpret_cast<HDC>(wParam));
         }
         break;

      case WM_CTLCOLOREDIT:
         if (NppDarkMode::isEnabled()) {
            return NppDarkMode::onCtlColorSofter(reinterpret_cast<HDC>(wParam));
         }
         break;

      case WM_PRINTCLIENT:
         if (NppDarkMode::isEnabled()) {
            return TRUE;
         }
         break;

      case NPPM_INTERNAL_REFRESHDARKMODE:
         NppDarkMode::autoThemeChildControls(_hSelf);
         return TRUE;
   }

   return FALSE;
}

void ThemeDialog::localize() {
   SetWindowText(_hSelf, THEME_DIALOG_TITLE);
   SetDlgItemText(_hSelf, IDC_THEME_DEF_GROUP_BOX, THEME_DEF_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_THEME_DEF_DESC_LABEL, THEME_DEF_DESC_LABEL);
   SetDlgItemText(_hSelf, IDC_THEME_DEF_ACCEPT_BTN, THEME_DEF_ACCEPT_BTN);
   SetDlgItemText(_hSelf, IDC_THEME_DEF_NEW_BTN, THEME_DEF_NEW_BTN);
   SetDlgItemText(_hSelf, IDC_THEME_DEF_CLONE_BTN, THEME_DEF_CLONE_BTN);
   SetDlgItemText(_hSelf, IDC_THEME_DEF_DEL_BTN, THEME_DEF_DEL_BTN);
   SetDlgItemText(_hSelf, IDC_THEME_STYLE_GROUP_BOX, THEME_STYLE_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_THEME_STYLE_NEW_BTN, THEME_STYLE_NEW_BTN);
   SetDlgItemText(_hSelf, IDC_THEME_STYLE_CLONE_BTN, THEME_STYLE_CLONE_BTN);
   SetDlgItemText(_hSelf, IDC_THEME_STYLE_DEL_BTN, THEME_STYLE_DEL_BTN);
   SetDlgItemText(_hSelf, IDC_THEME_STYLE_DEF_GROUP_BOX, THEME_STYLE_DEF_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_THEME_STYLE_DEF_BACK_LABEL, THEME_STYLE_DEF_BACKCOLOR);
   SetDlgItemText(_hSelf, IDC_THEME_STYLE_DEF_FORE_LABEL, THEME_STYLE_DEF_FORECOLOR);
   SetDlgItemText(_hSelf, IDC_THEME_STYLE_DEF_BOLD, THEME_STYLE_DEF_BOLD);
   SetDlgItemText(_hSelf, IDC_THEME_STYLE_DEF_ITALICS, THEME_STYLE_DEF_ITALICS);
   SetDlgItemText(_hSelf, IDC_THEME_STYLE_DEF_OUT_LABEL, THEME_STYLE_DEF_OUT_LABEL);
   SetDlgItemText(_hSelf, IDC_THEME_STYLE_DEF_ACCEPT_BTN, THEME_STYLE_DEF_ACCEPT_BTN);
   SetDlgItemText(_hSelf, IDC_THEME_STYLE_DEF_RESET_BTN, THEME_STYLE_DEF_RESET_BTN);
   SetDlgItemText(_hSelf, IDC_THEME_DEF_SAVE_CONFIG_BTN, THEME_DIALOG_SAVE_BTN);
   SetDlgItemText(_hSelf, IDC_THEME_DEF_RESET_BTN, THEME_DIALOG_RESET_BTN);
   SetDlgItemText(_hSelf, IDC_THEME_DEF_BACKUP_LOAD_BTN, THEME_DIALOG_BKUP_LOAD_BTN);
   SetDlgItemText(_hSelf, IDC_THEME_DEF_BACKUP_VIEW_BTN, THEME_DIALOG_BKUP_VIEW_BTN);
   SetDlgItemText(_hSelf, IDC_THEME_DEF_EXTRACT_BTN, THEME_DIALOG_EXTRACT_BTN);
   SetDlgItemText(_hSelf, IDC_THEME_DEF_APPEND_BTN, THEME_DIALOG_APPEND_BTN);
   SetDlgItemText(_hSelf, IDCLOSE, THEME_DIALOG_CLOSE_BTN);
}

void ThemeDialog::indicateCleanStatus() {
   if (cleanConfigFile) {
      SetDlgItemText(_hSelf, IDC_THEME_DEF_SAVE_CONFIG_BTN, THEME_DIALOG_SAVE_BTN);
      Utils::setFontRegular(_hSelf, IDC_THEME_DEF_SAVE_CONFIG_BTN);
   }
   else {
      SetDlgItemText(_hSelf, IDC_THEME_DEF_SAVE_CONFIG_BTN, (wstring(THEME_DIALOG_SAVE_BTN) + L"*").c_str());
      Utils::setFontBold(_hSelf, IDC_THEME_DEF_SAVE_CONFIG_BTN);
   }

   if (cleanThemeVals) {
      SetDlgItemText(_hSelf, IDC_THEME_DEF_ACCEPT_BTN, THEME_DEF_ACCEPT_BTN);
      Utils::setFontRegular(_hSelf, IDC_THEME_DEF_ACCEPT_BTN);
   }
   else {
      SetDlgItemText(_hSelf, IDC_THEME_DEF_ACCEPT_BTN, (wstring(THEME_DEF_ACCEPT_BTN) + L"*").c_str());
      Utils::setFontBold(_hSelf, IDC_THEME_DEF_ACCEPT_BTN);
   }

   if (cleanStyleDefs) {
      SetDlgItemText(_hSelf, IDC_THEME_STYLE_DEF_ACCEPT_BTN, THEME_STYLE_DEF_ACCEPT_BTN);
      Utils::setFontRegular(_hSelf, IDC_THEME_STYLE_DEF_ACCEPT_BTN);
   }
   else {
      SetDlgItemText(_hSelf, IDC_THEME_STYLE_DEF_ACCEPT_BTN, (wstring(THEME_STYLE_DEF_ACCEPT_BTN) + L"*").c_str());
      Utils::setFontBold(_hSelf, IDC_THEME_STYLE_DEF_ACCEPT_BTN);
   }
}

int ThemeDialog::loadConfigInfo() {
   vector<wstring> themesList;
   wstring sThemes;
   int themesCount;

   sThemes = _configIO.getStyleValue(L"Base", L"Themes", themeFile);
   themesCount = _configIO.Tokenize(sThemes, themesList);

   vThemeTypes.clear();
   vThemeTypes.resize(themesCount);

   for (int i{}; i < themesCount; i++) {
      wstring& themeType = themesList[i];
      loadThemeInfo(i, themeType, themeFile);
   }

   return static_cast<int>(vThemeTypes.size());
}

int ThemeDialog::loadThemeInfo(int vIndex, const wstring& themeType, const wstring& sThemeFile) {
   ThemeType& TT = vThemeTypes[vIndex];

   TT.label = themeType;
   _configIO.getFullStyle(themeType, L"EOL", TT.eolStyle, sThemeFile);

   wchar_t buf[10];
   int styleCount;

   styleCount = _configIO.StringtoInt(_configIO.getStyleValue(themeType, L"Count", sThemeFile));

   TT.vStyleInfo.clear();
   TT.vStyleInfo.resize(styleCount);

   for (int i{}; i < styleCount; i++) {
      swprintf(buf, 8, L"BFBI_%02i", i);
      _configIO.getFullStyle(themeType, buf, TT.vStyleInfo[i], sThemeFile);
   }

   return styleCount;
}

void ThemeDialog::fillThemes() {
   SendMessage(hThemesLB, LB_RESETCONTENT, NULL, NULL);

   for (const ThemeType TT : vThemeTypes) {
      SendMessage(hThemesLB, LB_ADDSTRING, NULL, (LPARAM)TT.label.c_str());
   }

   if (vThemeTypes.size() > 0)
      SendMessage(hThemesLB, LB_SETCURSEL, 0, NULL);

   cleanConfigFile = TRUE;
   cleanThemeVals = TRUE;
   onThemeSelect();
}

int ThemeDialog::getCurrentThemeIndex() {
   int idxTT;

   idxTT = static_cast<int>(SendMessage(hThemesLB, LB_GETCURSEL, NULL, NULL));
   if (idxTT == LB_ERR) return LB_ERR;

   return idxTT;
}

int ThemeDialog::getCurrentStyleIndex() {
   int idxStyle;

   idxStyle = static_cast<int>(SendMessage(hStylesLB, LB_GETCURSEL, NULL, NULL));
   if (idxStyle == LB_ERR) return LB_ERR;

   return idxStyle;
}

bool ThemeDialog::getCurrentThemeInfo(ThemeType*& themeInfo) {
   int idxTT{ getCurrentThemeIndex() };
   if (idxTT == LB_ERR) return FALSE;

   themeInfo = &vThemeTypes[idxTT];
   return TRUE;
}

ThemeDialog::ThemeType ThemeDialog::getNewTheme() {
   ThemeType newTheme;

   newTheme.label = L"";
   newTheme.vStyleInfo = vector<StyleInfo>{ getNewStyle() };

   return newTheme;
}

wstring ThemeDialog::getStyleConfig(int idx, StyleInfo& style) {
   wchar_t styleDef[25];

   swprintf(styleDef, 25, L"BFBI_%02i=%06X %06X ", idx, style.backColor, style.foreColor);
   return wstring{ styleDef } + (style.bold ? L"1" : L"0") + (style.italics ? L"1" : L"0");
}

void ThemeDialog::getThemeConfig(size_t idxTh, bool cr_lf, wstring& themeLabel, wstring& ttConfig) {
   wstring new_line = cr_lf ? L"\r\n" : L"\n";
   ThemeType& TT = vThemeTypes[idxTh];
   size_t styleCount = (TT.vStyleInfo.size() > 99) ? 99 : TT.vStyleInfo.size();

   themeLabel = TT.label;
   ttConfig = L"[" + themeLabel + L"]" + new_line +
      L"Count=" + to_wstring(styleCount) + new_line +
      L"EOL=" + getStyleConfig(0, TT.eolStyle).substr(8) + new_line;

   for (size_t j{}; j < styleCount; j++) {
      ttConfig += getStyleConfig(static_cast<int>(j), TT.vStyleInfo[j]) + new_line;
   }
}

bool ThemeDialog::getCurrentStyleInfo(StyleInfo*& recInfo) {
   int idxTT{ getCurrentThemeIndex() };
   if (idxTT == LB_ERR) return FALSE;

   int idxStyle{ getCurrentStyleIndex() };
   if (idxStyle == LB_ERR) return FALSE;

   recInfo = (idxStyle < static_cast<int>(vThemeTypes[idxTT].vStyleInfo.size())) ?
      &vThemeTypes[idxTT].vStyleInfo[idxStyle] : &vThemeTypes[idxTT].eolStyle;

   return TRUE;
}

StyleInfo ThemeDialog::getNewStyle() {
   StyleInfo newStyle{};

   newStyle.backColor = static_cast<int>(nppMessage(NPPM_GETEDITORDEFAULTBACKGROUNDCOLOR, NULL, NULL));
   newStyle.foreColor = static_cast<int>(nppMessage(NPPM_GETEDITORDEFAULTFOREGROUNDCOLOR, NULL, NULL));
   newStyle.bold = 0;
   newStyle.italics = 0;

   return newStyle;
}

void ThemeDialog::onThemeSelect() {
   ThemeType* themeInfo;

   if (!getCurrentThemeInfo(themeInfo)) {
      ThemeType newTheme{ getNewTheme() };
      themeInfo = &newTheme;
   }

   loadingEdits = TRUE;
   SetDlgItemText(_hSelf, IDC_THEME_DEF_DESC_EDIT, themeInfo->label.c_str());
   loadingEdits = FALSE;

   enableMoveThemeButtons();
   fillStyles();
}

void ThemeDialog::enableMoveThemeButtons() {
   int idxFT{ getCurrentThemeIndex() };
   if (idxFT == LB_ERR) return;

   EnableWindow(GetDlgItem(_hSelf, IDC_THEME_DEF_DOWN_BUTTON),
      (idxFT < static_cast<int>(vThemeTypes.size()) - 1));
   EnableWindow(GetDlgItem(_hSelf, IDC_THEME_DEF_UP_BUTTON), (idxFT > 0));
}

void ThemeDialog::enableThemeSelection() {
   bool enable{ cleanThemeVals && cleanStyleDefs };
   EnableWindow(GetDlgItem(_hSelf, IDC_THEME_DEF_LIST_BOX), enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_THEME_DEF_NEW_BTN), enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_THEME_DEF_CLONE_BTN), enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_THEME_DEF_EXTRACT_BTN), enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_THEME_DEF_APPEND_BTN), enable);

   if (enable) {
      enableMoveThemeButtons();
   }
   else {
      EnableWindow(GetDlgItem(_hSelf, IDC_THEME_DEF_DOWN_BUTTON), FALSE);
      EnableWindow(GetDlgItem(_hSelf, IDC_THEME_DEF_UP_BUTTON), FALSE);
   }

   indicateCleanStatus();
}

int ThemeDialog::moveThemeType(move_dir dir) {
   const int idxTheme{ getCurrentThemeIndex() };
   if (idxTheme == LB_ERR) return LB_ERR;

   switch(dir) {
      case MOVE_DOWN:
         if (idxTheme >= static_cast<int>(vThemeTypes.size()) - 1) return LB_ERR;
         break;

      case MOVE_UP:
         if (idxTheme == 0) return LB_ERR;
         break;

      default:
         return LB_ERR;
   }

   ThemeType currType = vThemeTypes[idxTheme];
   ThemeType& adjType = vThemeTypes[idxTheme + dir];

   vThemeTypes[idxTheme] = adjType;
   vThemeTypes[idxTheme + dir] = currType;

   SendMessage(hThemesLB, LB_DELETESTRING, (WPARAM)idxTheme, NULL);
   SendMessage(hThemesLB, LB_INSERTSTRING, (WPARAM)(idxTheme + dir),
      (LPARAM)vThemeTypes[idxTheme + dir].label.c_str());
   SendMessage(hThemesLB, LB_SETCURSEL, idxTheme + dir, NULL);

   cleanConfigFile = FALSE;
   indicateCleanStatus();
   enableMoveThemeButtons();

   return idxTheme + dir;
}

void ThemeDialog::fillStyles() {
   ThemeType* themeInfo;

   if (!getCurrentThemeInfo(themeInfo)) {
      ThemeType newTheme{ getNewTheme() };
      themeInfo = &newTheme;
   }

   wchar_t styleLabel[10];
   vector <StyleInfo>& styleList{ themeInfo->vStyleInfo };
   int styleCount{ static_cast<int>(styleList.size()) };

   SendMessage(hStylesLB, LB_RESETCONTENT, NULL, NULL);

   for (int i{}; i < styleCount; i++) {
      swprintf(styleLabel, 10, L"Style #%02i", i);
      SendMessage(hStylesLB, LB_ADDSTRING, NULL, (LPARAM)styleLabel);
   }

   SendMessage(hStylesLB, LB_ADDSTRING, NULL, (LPARAM)L"EOL Marker Style");
   SendMessage(hStylesLB, LB_SETCURSEL, 0, NULL);

   onStyleSelect();
   initPreviewSwatch();
}

void ThemeDialog::onStyleSelect() {
   int idxTheme{ getCurrentThemeIndex() };
   if (idxTheme == LB_ERR) return;

   int idxStyle{ getCurrentStyleIndex() };
   if (idxStyle == LB_ERR) return;

   EnableWindow(GetDlgItem(_hSelf, IDC_THEME_STYLE_DEL_BTN),
      (idxStyle < static_cast<int>(vThemeTypes[idxTheme].vStyleInfo.size())));
   enableMoveStyleButtons();
   fillStyleDefs();
}

void ThemeDialog::enableMoveStyleButtons() {
   int idxTheme{ getCurrentThemeIndex() };
   if (idxTheme == LB_ERR) return;

   int idxStyle{ getCurrentStyleIndex() };
   if (idxStyle == LB_ERR) return;

   int styleCount{ static_cast<int>(vThemeTypes[idxTheme].vStyleInfo.size()) };

   EnableWindow(GetDlgItem(_hSelf, IDC_THEME_STYLE_DOWN_BUTTON), (idxStyle < styleCount - 1));
   EnableWindow(GetDlgItem(_hSelf, IDC_THEME_STYLE_UP_BUTTON), (idxStyle > 0 && idxStyle < styleCount));
}

void ThemeDialog::enableStyleSelection() {
   EnableWindow(GetDlgItem(_hSelf, IDC_THEME_STYLE_LIST_BOX), cleanStyleDefs);
   EnableWindow(GetDlgItem(_hSelf, IDC_THEME_STYLE_CLONE_BTN), cleanStyleDefs);
   EnableWindow(GetDlgItem(_hSelf, IDC_THEME_STYLE_NEW_BTN), cleanStyleDefs);

   if (cleanStyleDefs) {
      enableMoveStyleButtons();
   }
   else {
      EnableWindow(GetDlgItem(_hSelf, IDC_THEME_STYLE_DOWN_BUTTON), cleanStyleDefs);
      EnableWindow(GetDlgItem(_hSelf, IDC_THEME_STYLE_UP_BUTTON), cleanStyleDefs);
   }

   enableThemeSelection();
}

int ThemeDialog::moveStyleType(move_dir dir) {
   const int idxTheme{ getCurrentThemeIndex() };
   if (idxTheme == LB_ERR) return LB_ERR;

   const int idxStyle{ getCurrentStyleIndex() };
   if (idxStyle == LB_ERR) return LB_ERR;

   vector<StyleInfo>& styleList = vThemeTypes[idxTheme].vStyleInfo;

   switch (dir) {
   case MOVE_DOWN:
      if (idxStyle >= static_cast<int>(styleList.size()) - 1) return LB_ERR;
      break;

   case MOVE_UP:
      if (idxStyle == 0) return LB_ERR;
      break;

   default:
      return LB_ERR;
   }

   StyleInfo currType = styleList[idxStyle];
   StyleInfo& adjType = styleList[idxStyle + dir];

   styleList[idxStyle] = adjType;
   styleList[idxStyle + dir] = currType;

   TCHAR itemText[100]{};

   SendMessage(hStylesLB, LB_GETTEXT, (WPARAM)idxStyle, (LPARAM)itemText);
   SendMessage(hStylesLB, LB_DELETESTRING, (WPARAM)idxStyle, NULL);
   SendMessage(hStylesLB, LB_INSERTSTRING, (WPARAM)(idxStyle + dir), (LPARAM)itemText);
   SendMessage(hStylesLB, LB_SETCURSEL, idxStyle + dir, NULL);

   cleanConfigFile = FALSE;
   indicateCleanStatus();
   enableMoveStyleButtons();
   initPreviewSwatch();

   return idxStyle + dir;
}

int ThemeDialog::getStyleDefColor(bool back) {
   TCHAR buf[10];

   GetDlgItemText(_hSelf, back ? IDC_THEME_STYLE_DEF_BACK_EDIT : IDC_THEME_STYLE_DEF_FORE_EDIT, buf, 7);

   return _configIO.StringtoInt(buf, 16);
}

void ThemeDialog::setStyleDefColor(bool setEdit, int color, bool back) {
   if (setEdit) {
      TCHAR buf[10];
      swprintf(buf, 7, L"%06X", color);

      loadingEdits = TRUE;
      SetDlgItemText(_hSelf, back ? IDC_THEME_STYLE_DEF_BACK_EDIT : IDC_THEME_STYLE_DEF_FORE_EDIT, buf);
      loadingEdits = FALSE;
   }

   // Set styleBack | styleFore here. Will be used in WM_CTLCOLORSTATIC, triggered by the setFontRegular() calls
   styleDefColor = TRUE;
   (back ? styleBack : styleFore) = Utils::intToRGB(color);
   Utils::setFontRegular(_hSelf, back ? IDC_THEME_STYLE_DEF_BACKCOLOR : IDC_THEME_STYLE_DEF_FORECOLOR);
   setOutputFontStyle();
}

void ThemeDialog::fillStyleDefs() {
   StyleInfo* style;

   if (!getCurrentStyleInfo(style)) {
      StyleInfo newStyle{ getNewStyle() };
      style = &newStyle;
   }

   CheckDlgButton(_hSelf, IDC_THEME_STYLE_DEF_BOLD, style->bold ? BST_CHECKED : BST_UNCHECKED);
   CheckDlgButton(_hSelf, IDC_THEME_STYLE_DEF_ITALICS, style->italics ? BST_CHECKED : BST_UNCHECKED);

   setStyleDefColor(TRUE, style->backColor, TRUE);
   setStyleDefColor(TRUE, style->foreColor, FALSE);

   cleanStyleDefs = TRUE;
   enableStyleSelection();
}

void ThemeDialog::styleDefsAccept() {
   if (cleanStyleDefs) return;

   StyleInfo* style;
   if (!getCurrentStyleInfo(style)) return;

   int idxStyle = getCurrentStyleIndex();

   style->backColor = getStyleDefColor(TRUE);
   style->foreColor = getStyleDefColor(FALSE);
   style->bold = (IsDlgButtonChecked(_hSelf, IDC_THEME_STYLE_DEF_BOLD) == BST_CHECKED);
   style->italics = (IsDlgButtonChecked(_hSelf, IDC_THEME_STYLE_DEF_ITALICS) == BST_CHECKED);
   initPreviewSwatch(idxStyle, idxStyle);

   cleanConfigFile = FALSE;
   cleanStyleDefs = TRUE;
   enableStyleSelection();
}

INT_PTR ThemeDialog::colorStaticControl(WPARAM wParam, LPARAM lParam) {
   HDC hdc = (HDC)wParam;
   DWORD ctrlID = GetDlgCtrlID((HWND)lParam);

   if (hbr != NULL) DeleteObject(hbr);

   switch (ctrlID)
   {
      case IDC_THEME_STYLE_DEF_BACKCOLOR:
         SetBkColor(hdc, styleBack);
         hbr = CreateSolidBrush(styleBack);
         return (INT_PTR)hbr;

      case IDC_THEME_STYLE_DEF_FORECOLOR:
         SetBkColor(hdc, styleFore);
         hbr = CreateSolidBrush(styleFore);
         return (INT_PTR)hbr;

      case IDC_THEME_STYLE_DEF_OUTPUT:
         SetBkColor(hdc, styleBack);
         SetTextColor(hdc, styleFore);
         hbr = CreateSolidBrush(styleBack);
         return (INT_PTR)hbr;

      default:
         return NULL;
   }
}

INT_PTR ThemeDialog::colorPreviewSwatch(WPARAM wParam, LPARAM lParam) {
   const int idxTheme{ getCurrentThemeIndex() };
   if (idxTheme == LB_ERR) return NULL;
   ThemeType& TT = vThemeTypes[idxTheme];

   int topIdx = static_cast<int>(SendDlgItemMessage(_hSelf, IDC_THEME_STYLE_LIST_BOX, LB_GETTOPINDEX, NULL, NULL));
   int styleCount = static_cast<int>(TT.vStyleInfo.size());
   int ctrlIDOffset, brushColor;

   HDC hdc = (HDC)wParam;
   DWORD ctrlID = GetDlgCtrlID((HWND)lParam);

   if (hbr != NULL) DeleteObject(hbr);

   // Back color swatches
   ctrlIDOffset = static_cast<int>(ctrlID) - IDC_THEME_SWATCH_BACK_00;
   if (ctrlIDOffset >= 0 && ctrlIDOffset < SWATCH_ITEM_COUNT) {
      if (ctrlIDOffset + topIdx < styleCount)
         brushColor = Utils::intToRGB(TT.vStyleInfo[ctrlIDOffset + topIdx].backColor);
      else if (ctrlIDOffset + topIdx == styleCount)
         brushColor = Utils::intToRGB(TT.eolStyle.backColor);
      else
         return NULL;

      SetBkColor(hdc, brushColor);
      hbr = CreateSolidBrush(brushColor);
      return (INT_PTR)hbr;
   }

   // Fore color swatches
   ctrlIDOffset = static_cast<int>(ctrlID) - IDC_THEME_SWATCH_FORE_00;
   if (ctrlIDOffset >= 0 && ctrlIDOffset < SWATCH_ITEM_COUNT) {
      if (ctrlIDOffset + topIdx < styleCount)
         brushColor = Utils::intToRGB(TT.vStyleInfo[ctrlIDOffset + topIdx].foreColor);
      else if (ctrlIDOffset + topIdx == styleCount)
         brushColor = Utils::intToRGB(TT.eolStyle.foreColor);
      else
         return NULL;

      SetBkColor(hdc, brushColor);
      hbr = CreateSolidBrush(brushColor);
      return (INT_PTR)hbr;
   }

   return NULL;
}

void ThemeDialog::initPreviewSwatch(int idxStart, int idxEnd) {
   const int idxTheme{ getCurrentThemeIndex() };
   if (idxTheme == LB_ERR) return;
   ThemeType& TT = vThemeTypes[idxTheme];

   swatchTopIndex = static_cast<int>(SendDlgItemMessage(_hSelf, IDC_THEME_STYLE_LIST_BOX, LB_GETTOPINDEX, NULL, NULL));
   int styleCount = static_cast<int>(TT.vStyleInfo.size());

   for (int i{idxStart}; i <= idxEnd; i++) {
      ShowWindow(GetDlgItem(_hSelf, IDC_THEME_SWATCH_BACK_00 + i), (i + swatchTopIndex <= styleCount));
      Utils::setFontRegular(_hSelf, IDC_THEME_SWATCH_BACK_00 + i);

      ShowWindow(GetDlgItem(_hSelf, IDC_THEME_SWATCH_FORE_00 + i), (i + swatchTopIndex <= styleCount));
      Utils::setFontRegular(_hSelf, IDC_THEME_SWATCH_FORE_00 + i);
   }
}

void ThemeDialog::processSwatchClick(int ctrlID) {
   int topIdx = static_cast<int>(SendDlgItemMessage(_hSelf, IDC_THEME_STYLE_LIST_BOX, LB_GETTOPINDEX, NULL, NULL));

   if (ctrlID >= IDC_THEME_SWATCH_BACK_00 && ctrlID <= IDC_THEME_SWATCH_BACK_00 + SWATCH_ITEM_COUNT) {
      SendDlgItemMessage(_hSelf, IDC_THEME_STYLE_LIST_BOX, LB_SETCURSEL,
         ctrlID - IDC_THEME_SWATCH_BACK_00 + topIdx, NULL);
      onStyleSelect();
   }

   if (ctrlID >= IDC_THEME_SWATCH_FORE_00 && ctrlID <= IDC_THEME_SWATCH_FORE_00 + SWATCH_ITEM_COUNT) {
      SendDlgItemMessage(_hSelf, IDC_THEME_STYLE_LIST_BOX, LB_SETCURSEL,
         ctrlID - IDC_THEME_SWATCH_FORE_00 + topIdx, NULL);
      onStyleSelect();
   }
}

void ThemeDialog::setOutputFontStyle() {
   Utils::setFontRegular(_hSelf, IDC_THEME_STYLE_DEF_OUTPUT);

   if (IsDlgButtonChecked(_hSelf, IDC_THEME_STYLE_DEF_BOLD) == BST_CHECKED)
      Utils::setFontBold(_hSelf, IDC_THEME_STYLE_DEF_OUTPUT);

   if (IsDlgButtonChecked(_hSelf, IDC_THEME_STYLE_DEF_ITALICS) == BST_CHECKED)
      Utils::setFontItalic(_hSelf, IDC_THEME_STYLE_DEF_OUTPUT);
}

void ThemeDialog::chooseStyleDefColor(bool back) {
   StyleInfo* style;
   if (!getCurrentStyleInfo(style)) return;

   int color = getStyleDefColor(back);

   CHOOSECOLOR cc;
   ZeroMemory(&cc, sizeof(cc));

   cc.lStructSize = sizeof(cc);
   cc.hwndOwner = _hSelf;
   cc.rgbResult = Utils::intToRGB(color);
   cc.lpCustColors = (LPDWORD) customColors;
   cc.Flags = CC_FULLOPEN | CC_RGBINIT;

   if (!ChooseColor(&cc)) return;

   color = static_cast<int>(cc.rgbResult);
   setStyleDefColor(TRUE, color, back);

   cleanConfigFile = FALSE;
   cleanStyleDefs = FALSE;
   enableStyleSelection();
}

void ThemeDialog::setPangram() {
   constexpr int count{ 10 };

   wstring pangrams[count] = {
      L"SPHINX OF BLACK QUARTZ,\r\nJUDGE MY VOW.",
      L"A QUART JAR OF OIL\r\nMIXED WITH ZINC OXIDE\r\nMAKES A VERY BRIGHT PAINT.",
      L"A WIZARD'S JOB IS TO\r\nVEX CHUMPS QUICKLY IN FOG.",
      L"AMAZINGLY FEW DISCOTHEQUES\r\nPROVIDE JUKEBOXES.",
      L"PACK MY BOX WITH\r\nFIVE DOZEN LIQUOR JUGS.",
      L"THE LAZY MAJOR WAS FIXING\r\nCUPID'S BROKEN QUIVER.",
      L"MY FAXED JOKE WON A PAGER\r\nIN THE CABLE TV QUIZ SHOW.",
      L"THE FIVE BOXING WIZARDS\r\nJUMP QUICKLY.",
      L"FEW BLACK TAXIS\r\nDRIVE UP MAJOR ROADS\r\nON QUIET HAZY NIGHTS.",
      L"WE PROMPTLY JUDGED\r\nANTIQUE IVORY BUCKLES\r\nFOR THE NEXT PRIZE."
   };

   SetDlgItemText(_hSelf, IDC_THEME_STYLE_DEF_OUTPUT, (pangrams[rand() % count]).c_str());
}

void ThemeDialog::styleEditNew(bool clone) {
   int idxFT{ getCurrentThemeIndex() };
   if (idxFT == LB_ERR) return;

   int idxST{ getCurrentStyleIndex() };
   if (clone && idxST == LB_ERR) return;

   StyleInfo newStyle{ getNewStyle() };
   vector<StyleInfo>& styles = vThemeTypes[idxFT].vStyleInfo;
   int newIdx{ static_cast<int>(styles.size()) };

   if (newIdx >= STYLE_ITEM_LIMIT) {
      TCHAR buf[100];
      swprintf(buf, 100, THEME_STYLE_LIMIT_ERROR, STYLE_ITEM_LIMIT);
      MessageBox(_hSelf, buf, clone ? THEME_STYLE_CLONE_ACTION : THEME_STYLE_NEW_ACTION, MB_OK | MB_ICONSTOP);
      return;
   }

   wchar_t styleLabel[10];
   swprintf(styleLabel, 10, L"Style #%02i", newIdx);

   if (clone) {
      newStyle.backColor = styles[idxST].backColor;
      newStyle.foreColor = styles[idxST].foreColor;
      newStyle.bold = styles[idxST].bold;
      newStyle.italics = styles[idxST].italics;
   }

   styles.push_back(newStyle);

   SendMessage(hStylesLB, LB_INSERTSTRING, newIdx, (LPARAM)styleLabel);
   SendMessage(hStylesLB, LB_SETCURSEL, (WPARAM)newIdx, NULL);
   onStyleSelect();
   initPreviewSwatch();

   cleanConfigFile = FALSE;
   enableStyleSelection();
}

int ThemeDialog::styleEditDelete() {
   int idxFT{ getCurrentThemeIndex() };
   if (idxFT == LB_ERR) return LB_ERR;

   int idxRec{ getCurrentStyleIndex() };
   if (idxRec == LB_ERR) return LB_ERR;

   vector<StyleInfo>& records = vThemeTypes[idxFT].vStyleInfo;
   records.erase(records.begin() + idxRec);

   int lastRec = static_cast<int>(records.size()) - 1;
   int moveTo = (idxRec <= lastRec - 1) ? idxRec : lastRec;

   SendMessage(hStylesLB, LB_DELETESTRING, (WPARAM)idxRec, NULL);
   SendMessage(hStylesLB, LB_SETCURSEL, (WPARAM)moveTo, NULL);

   cleanConfigFile = FALSE;
   onStyleSelect();
   initPreviewSwatch();

   return moveTo;
}

void ThemeDialog::themeEditAccept() {
   if (cleanThemeVals) return;

   int idxFT{ getCurrentThemeIndex() };
   if (idxFT == LB_ERR) return;

   ThemeType& fileInfo = vThemeTypes[idxFT];

   wchar_t fileVal[MAX_PATH + 1];

   GetDlgItemText(_hSelf, IDC_THEME_DEF_DESC_EDIT, fileVal, MAX_PATH + 1);
   fileInfo.label = fileVal;

   SendMessage(hThemesLB, LB_DELETESTRING, (WPARAM)idxFT, NULL);
   SendMessage(hThemesLB, LB_INSERTSTRING, (WPARAM)idxFT, (LPARAM)fileInfo.label.c_str());
   SendMessage(hThemesLB, LB_SETCURSEL, idxFT, NULL);

   cleanConfigFile = FALSE;
   cleanThemeVals = TRUE;
   enableThemeSelection();
}

int ThemeDialog::appendThemeConfigs(const wstring& sThemeFile) {
   int sectionCount{}, validCount{};
   wstring sections{};
   vector<wstring> sectionList{};

   sectionCount = _configIO.getConfigSectionList(sections, sThemeFile);
   sectionCount = _configIO.Tokenize(sections, sectionList);

   for (int i{}; i < sectionCount; i++) {
      if (_configIO.getConfigString(sectionList[i], L"Count", L"", sThemeFile).length() > 0) {
         if (!checkThemeLimit(FALSE)) break;

         ThemeType newTheme{ getNewTheme() };

         vThemeTypes.push_back(newTheme);
         loadThemeInfo(static_cast<int>(vThemeTypes.size() - 1), sectionList[i], sThemeFile);
         SendMessage(hThemesLB, LB_ADDSTRING, NULL, (LPARAM)sectionList[i].c_str());
         validCount++;
      }
   }

   SendMessage(hThemesLB, LB_SETCURSEL, (WPARAM)(vThemeTypes.size() - 1), NULL);
   onThemeSelect();

   cleanConfigFile = FALSE;
   enableThemeSelection();

   return validCount;
}

void ThemeDialog::themeEditNew() {
   if (!checkThemeLimit(FALSE)) return;

   ThemeType newFile{ getNewTheme() };

   vThemeTypes.push_back(newFile);

   size_t moveTo = vThemeTypes.size() - 1;

   SendMessage(hThemesLB, LB_ADDSTRING, NULL, (LPARAM)newFile.label.c_str());
   SendMessage(hThemesLB, LB_SETCURSEL, (WPARAM)moveTo, NULL);
   onThemeSelect();

   cleanConfigFile = FALSE;
   cleanThemeVals = FALSE;
   enableThemeSelection();
}

void ThemeDialog::themeEditClone() {
   if (!checkThemeLimit(TRUE)) return;

   int idxTT{ getCurrentThemeIndex() };
   if (idxTT == LB_ERR) return;

   ThemeType& TT = vThemeTypes[idxTT];
   ThemeType NT{};

   NT.label = TT.label + L"_clone";

   size_t recCount = TT.vStyleInfo.size();
   NT.vStyleInfo.resize(recCount);

   for (size_t i{}; i < recCount; i++) {
      NT.vStyleInfo[i].backColor = TT.vStyleInfo[i].backColor;
      NT.vStyleInfo[i].foreColor = TT.vStyleInfo[i].foreColor;
      NT.vStyleInfo[i].bold = TT.vStyleInfo[i].bold;
      NT.vStyleInfo[i].italics = TT.vStyleInfo[i].italics;
   }

   vThemeTypes.push_back(NT);

   SendMessage(hThemesLB, LB_ADDSTRING, NULL, (LPARAM)NT.label.c_str());
   SendMessage(hThemesLB, LB_SETCURSEL, (WPARAM)(vThemeTypes.size() - 1), NULL);

   onThemeSelect();
   cleanConfigFile = FALSE;
   enableThemeSelection();
}

int ThemeDialog::themeEditDelete() {
   int idxFT{ getCurrentThemeIndex() };
   if (idxFT == LB_ERR) return LB_ERR;

   vThemeTypes.erase(vThemeTypes.begin() + idxFT);

   int lastFile = static_cast<int>(vThemeTypes.size()) - 1;
   int moveTo = (idxFT <= lastFile - 1) ? idxFT : lastFile;

   SendMessage(hThemesLB, LB_DELETESTRING, (WPARAM)idxFT, NULL);
   SendMessage(hThemesLB, LB_SETCURSEL, (WPARAM)moveTo, NULL);

   cleanConfigFile = FALSE;
   cleanThemeVals = TRUE;
   onThemeSelect();

   return moveTo;
}

bool ThemeDialog::checkThemeLimit(bool clone) {
   if (vThemeTypes.size() < THEME_ITEM_LIMIT) return TRUE;

   TCHAR buf[100];
   swprintf(buf, 100, THEME_DEF_LIMIT_ERROR, THEME_ITEM_LIMIT);
   MessageBox(_hSelf, buf, clone ? THEME_DEF_CLONE_ACTION : THEME_DEF_NEW_ACTION, MB_OK | MB_ICONSTOP);
   return FALSE;
}

bool ThemeDialog::promptDiscardChangesNo() {
   if (!(cleanConfigFile && cleanThemeVals && cleanStyleDefs)) {
      if (MessageBox(_hSelf, THEME_DISCARD_CHANGES, THEME_DIALOG_TITLE,
         MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
         return TRUE;
   }

   return false;
}

void ThemeDialog::saveConfigInfo() {
   if (!cleanStyleDefs) styleDefsAccept();
   if (!cleanThemeVals) themeEditAccept();

   size_t themeCount;
   wstring fileData{}, themes{}, ttCode{}, ttConfig{};

   themeCount = (vThemeTypes.size() > 999) ? 999 : vThemeTypes.size();

   for (size_t i{}; i < themeCount; i++) {
      getThemeConfig(i, TRUE, ttCode, ttConfig);
      themes += (i == 0 ? L"" : L",") + ttCode;
      fileData += ttConfig + L"\r\n";
   }

   fileData = L"[Base]\r\nThemes=" + themes + L"\r\n\r\n" + fileData;

   _configIO.backupConfigFile(FALSE);
   _configIO.saveConfigFile(fileData, FALSE);

   cleanConfigFile = TRUE;
   indicateCleanStatus();
   RefreshVisualizerPanel();
}

void ThemeDialog::showEximDialog(bool bExtract) {
   _eximDlg.doDialog((HINSTANCE)_gModule);
   _eximDlg.initDialog(bExtract, FALSE);

   if (bExtract) {
      int idxTT{ getCurrentThemeIndex() };
      if (idxTT == LB_ERR) return;

      wstring ttCode{}, ttConfig{};
      getThemeConfig(idxTT, TRUE, ttCode, ttConfig);
      _eximDlg.setFileTypeData(ttConfig);
   }
}
