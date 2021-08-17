#include "AboutDialog.h"

void AboutDialog::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_ABOUT_DIALOG);
   }

   localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
}

void AboutDialog::localize() {
#ifdef _WIN64
   wstring buildBit{ L" (64-bit)" };
#else
   wstring buildBit{ L" (32-bit)" };
#endif // _WIN64

   SetWindowText(_hSelf, ABOUT_DIALOG_TITLE);

   SetDlgItemText(_hSelf, IDC_ABOUT_NAME, getVersionInfo(L"FileDescription").c_str());

   SetDlgItemText(_hSelf, IDC_ABOUT_VERSION,
      (L"Version: " + getVersionInfo(L"FileVersion") + buildBit).c_str());

   SetDlgItemTextA(_hSelf, IDC_ABOUT_BUILD_TIME,
      ("Build time: " + string(__DATE__) + " - " + string(__TIME__)).c_str());

   SetDlgItemText(_hSelf, IDC_ABOUT_ATTRIBUTION, getVersionInfo(L"LegalCopyright").c_str());
   SetDlgItemText(_hSelf, IDOK, ABOUT_BTN_LABEL_OK);
}

INT_PTR CALLBACK AboutDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
   switch (message) {
      case WM_INITDIALOG:
         if (NppDarkMode::isEnabled()) {
            LITEM item = { 0 };
            item.iLink = 0;
            item.mask = LIF_ITEMINDEX | LIF_STATE;
            item.state = LIS_DEFAULTCOLORS;
            item.stateMask = LIS_DEFAULTCOLORS;
            SendMessage(GetDlgItem(_hSelf, IDC_ABOUT_PROD_URL), LM_SETITEM, 0, (LPARAM)&item);

            NppDarkMode::autoSubclassAndThemeChildControls(_hSelf);
         }

         break;

      case WM_COMMAND:
         switch LOWORD(wParam) {
            case IDCANCEL:
            case IDOK:
               display(FALSE);
               return TRUE;
         }
         break;

      case WM_NOTIFY:
         switch (((LPNMHDR)lParam)->code) {
            case NM_CLICK:
            case NM_RETURN:
               ShellExecute(NULL, L"open", getVersionInfo(L"CompanyName").c_str(), NULL, NULL, SW_SHOW);
               display(FALSE);
               return TRUE;
         }
         break;

      case WM_CTLCOLORDLG:
         if (NppDarkMode::isEnabled()) {
            return NppDarkMode::onCtlColorDarker(reinterpret_cast<HDC>(wParam));
         }
         break;

      case WM_CTLCOLORSTATIC:
         if (GetDlgCtrlID((HWND)lParam) == IDC_ABOUT_PROD_URL) {
            if (NppDarkMode::isEnabled()) {
               return NppDarkMode::onCtlColorSysLink(reinterpret_cast<HDC>(wParam));
            }
         }

         if (NppDarkMode::isEnabled()) {
            return NppDarkMode::onCtlColorDarker((HDC)wParam);
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
