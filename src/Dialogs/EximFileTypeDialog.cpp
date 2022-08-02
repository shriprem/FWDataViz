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

void EximFileTypeDialog::initDialog(HWND hWnd, exim_clients mode, bool bExtract) {
   hClientDlg = hWnd;
   exim_mode = mode;
   extractMode = bExtract;
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

void EximFileTypeDialog::refreshDarkMode() {
   NPPDM_AutoThemeChildControls(_hSelf);
   redraw();
}

void EximFileTypeDialog::setFileTypeData(const wstring& ftConfig) {
   SetDlgItemText(_hSelf, IDC_FTEXIM_EDIT_CNTRL, ftConfig.c_str());
}

void EximFileTypeDialog::localize() {
   SetWindowText(_hSelf, extractMode ? EXTRACT_TITLE[exim_mode] : APPEND_TITLE[exim_mode]);
   SetDlgItemText(_hSelf, IDC_FTEXIM_EDIT_LABEL, EDIT_LABEL[exim_mode]);
   SetDlgItemText(_hSelf, IDC_FTEXIM_EDIT_CNTRL, L"");
   SetDlgItemText(_hSelf, IDCLOSE, EXIM_CLOSE_BTN);

   if (extractMode) {
      SetDlgItemText(_hSelf, IDC_FTEXIM_SAVE_FILE, EXIM_SAVE_FILE_BTN);
      SetDlgItemText(_hSelf, IDCLOSE, EXIM_CLOSE_BTN);
   }
   else {
      SetDlgItemText(_hSelf, IDC_FTEXIM_LOAD_FILE, EXIM_LOAD_FILE_BTN);
      SetDlgItemText(_hSelf, IDC_FTEXIM_APPEND, APPEND_BTN[exim_mode]);
      SetDlgItemText(_hSelf, IDCLOSE, EXIM_CANCEL_BTN);
   }
}

INT_PTR CALLBACK EximFileTypeDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
   switch (message) {
   case WM_COMMAND:
      switch LOWORD(wParam) {
      case IDC_FTEXIM_INFO_BUTTON:
         ShellExecute(NULL, L"open", extractMode ? CONFIG_EXTRACT_INFO_README : CONFIG_APPEND_INFO_README, NULL, NULL, SW_SHOW);
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
      NPPDM_AutoSubclassAndThemeChildControls(_hSelf);
      break;

   case WM_CTLCOLORDLG:
   case WM_CTLCOLORSTATIC:
      if (NPPDM_IsEnabled()) {
         return NPPDM_OnCtlColorDarker(reinterpret_cast<HDC>(wParam));
      }
      break;

   case WM_CTLCOLOREDIT:
      if (NPPDM_IsEnabled()) {
         return NPPDM_OnCtlColorSofter(reinterpret_cast<HDC>(wParam));
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
   }

   return FALSE;
}

void EximFileTypeDialog::appendExtractFile() {
   wstring tmpFile{};

   _configIO.getBackupTempFileName(tmpFile);
   _configIO.saveConfigFile(getEditControlText(), tmpFile);
   display(FALSE);

   SendMessage(hClientDlg, FWVIZMSG_APPEND_EXIM_DATA, 0, (LPARAM)tmpFile.c_str());
   DeleteFile(tmpFile.c_str());
}

void EximFileTypeDialog::loadExtractFile() {
   wstring sExtractFile{};

   if (_configIO.queryConfigFileName(_hSelf, TRUE, FALSE, sExtractFile)) {
      if (_configIO.fixIfNotUTF8File(sExtractFile)) {
         string sExtractData{ _configIO.readConfigFile(sExtractFile) };
         SetDlgItemText(_hSelf, IDC_FTEXIM_EDIT_CNTRL, Utils::NarrowToWide(sExtractData).c_str());
      }
   }
}

void EximFileTypeDialog::saveExtractFile() {
   wstring sExtractFile{};

   if (_configIO.queryConfigFileName(_hSelf, FALSE, FALSE, sExtractFile)) {
      _configIO.saveConfigFile(getEditControlText(), sExtractFile);
   }
}

wstring EximFileTypeDialog::getEditControlText() {
   wstring sExtractData(FW_LINE_MAX_LENGTH, '\0');
   GetDlgItemText(_hSelf, IDC_FTEXIM_EDIT_CNTRL, sExtractData.data(), FW_LINE_MAX_LENGTH);
   return wstring{ sExtractData.c_str()};
}
