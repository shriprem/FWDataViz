#include "EximFileTypeDialog.h"

void EximFileTypeDialog::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_FILE_TYPE_EXIM_DIALOG);
   }

   goToCenter();

   Utils::loadBitmap(_hSelf, IDC_FTEXIM_INFO_BUTTON, IDB_VIZ_INFO_BITMAP);
   Utils::addTooltip(_hSelf, IDC_FTEXIM_INFO_BUTTON, NULL, VIZ_PANEL_INFO_TIP, FALSE);

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
}

void EximFileTypeDialog::initDialog(bool bExtract, bool bViz) {
   extractMode = bExtract;
   vizMode = bViz;
   localize();

   ShowWindow(GetDlgItem(_hSelf, IDC_FTEXIM_SAVE_FILE), extractMode ? SW_SHOW : SW_HIDE);
   ShowWindow(GetDlgItem(_hSelf, IDC_FTEXIM_LOAD_FILE), extractMode ? SW_HIDE : SW_SHOW);
   ShowWindow(GetDlgItem(_hSelf, IDC_FTEXIM_APPEND), extractMode ? SW_HIDE : SW_SHOW);

   bool recentOS = Utils::checkBaseOS(WV_VISTA);
   wstring fontName = recentOS ? L"Consolas" : L"Courier New";
   int fontHeight = recentOS ? 10 : 8;

   Utils::setFont(_hSelf, IDC_FTEXIM_EDIT_CNTRL, fontName, fontHeight);
   SetFocus(GetDlgItem(_hSelf, IDC_FTEXIM_EDIT_CNTRL));
}

void EximFileTypeDialog::setFileTypeData(const wstring& ftConfig) {
   SetDlgItemText(_hSelf, IDC_FTEXIM_EDIT_CNTRL, ftConfig.c_str());
}

void EximFileTypeDialog::localize() {
   SetWindowText(_hSelf, vizMode ?
      extractMode ? FT_EXIM_EXTRACT_FT_TITLE : FT_EXIM_APPEND_FT_TITLE :
      extractMode ? FT_EXIM_EXTRACT_THEME_TITLE : FT_EXIM_APPEND_THEME_TITLE);
   SetDlgItemText(_hSelf, IDC_FTEXIM_EDIT_LABEL, vizMode ? FT_EXIM_EDIT_FT_LABEL : FT_EXIM_EDIT_THEME_LABEL);
   SetDlgItemText(_hSelf, IDC_FTEXIM_EDIT_CNTRL, NULL);
   SetDlgItemText(_hSelf, IDCLOSE, FT_EXIM_CLOSE_BTN);

   if (extractMode) {
      SetDlgItemText(_hSelf, IDC_FTEXIM_SAVE_FILE, FT_EXIM_SAVE_FILE_BTN);
   }
   else {
      SetDlgItemText(_hSelf, IDC_FTEXIM_LOAD_FILE, FT_EXIM_LOAD_FILE_BTN);
      SetDlgItemText(_hSelf, IDC_FTEXIM_APPEND, FT_EXIM_APPEND_BTN);
   }
}

INT_PTR CALLBACK EximFileTypeDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM) {
   switch (message) {
      case WM_COMMAND:
         switch LOWORD(wParam) {
            case IDC_FTEXIM_INFO_BUTTON:
               ShellExecute(NULL, L"open", vizMode ?
                  extractMode ? FILE_EXTRACT_INFO_README : FILE_APPEND_INFO_README:
                  extractMode ? THEME_EXTRACT_INFO_README : THEME_APPEND_INFO_README,
                  NULL, NULL, SW_SHOW);
               break;

            case IDCANCEL:
               case IDCLOSE:
                  display(FALSE);
                  return TRUE;

            case IDC_FTEXIM_LOAD_FILE:
               loadExtractFile();
               break;

            case IDC_FTEXIM_SAVE_FILE:
               saveExtractFile();
               break;

            case IDC_FTEXIM_APPEND:
               appendExtractFile();
               break;
         }
         break;

      case WM_INITDIALOG:
         if (NppDarkMode::isEnabled()) {
            NppDarkMode::autoSubclassAndThemeChildControls(_hSelf);
         }
         break;

      case WM_CTLCOLORDLG:
      case WM_CTLCOLORLISTBOX:
      case WM_CTLCOLORSTATIC:
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
   }

   return FALSE;
}

void EximFileTypeDialog::appendExtractFile() {
   wstring tmpFile{};

   _configIO.getBackupTempFileName(tmpFile);
   _configIO.saveConfigFile(getEditControlText(), vizMode, tmpFile);
   display(FALSE);

   if (vizMode) {
      _configDlg.appendFileTypeConfigs(tmpFile);
      SetFocus(_configDlg.getHSelf());
   }
   else {
      _themeDlg.appendThemeConfigs(tmpFile);
      SetFocus(_themeDlg.getHSelf());
   }

   DeleteFile(tmpFile.c_str());
}

void EximFileTypeDialog::loadExtractFile() {
   wstring sExtractFile{};

   if (_configIO.queryConfigFileName(_hSelf, TRUE, FALSE, vizMode, sExtractFile)) {
      if (_configIO.isUCS16File(sExtractFile))
         _configIO.convertUCS16FiletoUTF8(sExtractFile);

      if (_configIO.isUCS16File(sExtractFile)) return;

      string sExtractData{ _configIO.readConfigFile(sExtractFile) };
      SetDlgItemText(_hSelf, IDC_FTEXIM_EDIT_CNTRL, Utils::MultiByteToWide(sExtractData).c_str());
   }
}

void EximFileTypeDialog::saveExtractFile() {
   wstring sExtractFile{};

   if (_configIO.queryConfigFileName(_hSelf, FALSE, FALSE, vizMode, sExtractFile)) {
      _configIO.saveConfigFile(getEditControlText(), vizMode, sExtractFile);
   }
}

wstring EximFileTypeDialog::getEditControlText() {
      TCHAR sExtractData[FW_LINE_MAX_LENGTH];
      GetDlgItemText(_hSelf, IDC_FTEXIM_EDIT_CNTRL, sExtractData, FW_LINE_MAX_LENGTH);
      return wstring{ sExtractData };
}
