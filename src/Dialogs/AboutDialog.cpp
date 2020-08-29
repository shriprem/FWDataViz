#include "AboutDialog.h"

void AboutDialog::doDialog(HINSTANCE hInst)
{
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_ABOUT_DIALOG);
   }

   localize();
   goToCenter();

   ::SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
}

void AboutDialog::localize()
{
   ::SetWindowText(_hSelf, ABOUT_DIALOG_TITLE);

   ::SetDlgItemText(_hSelf, IDC_ABOUT_NAME, ABOUT_PLUGIN_NAME);
   ::SetDlgItemText(_hSelf, IDC_ABOUT_VERSION, PLUGIN_VERSION);
   ::SetDlgItemText(_hSelf, IDC_ABOUT_ATTRIBUTION, PLUGIN_ATTRIBUTION);
   ::SetDlgItemText(_hSelf, IDOK, ABOUT_BTN_LABEL_OK);
}

INT_PTR CALLBACK AboutDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)
   {
      case WM_COMMAND:
      {
         switch LOWORD(wParam)
         {
         case IDCANCEL:
         case IDOK:
            display(FALSE);
            return TRUE;
         }
         return FALSE;
      }

      case WM_NOTIFY:
      {
         switch (((LPNMHDR)lParam)->code)
         {
         case NM_CLICK:
         case NM_RETURN:
            ::ShellExecute(NULL, L"open", PLUGIN_GITHUB_PAGE, NULL, NULL, SW_SHOW);
            display(FALSE);
            return TRUE;
         }
         return FALSE;
      }

      default:
         return FALSE;
      }
}
