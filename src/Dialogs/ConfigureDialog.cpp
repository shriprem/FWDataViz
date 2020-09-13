#include "ConfigureDialog.h"

void ConfigureDialog::doDialog(HINSTANCE hInst)
{
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_FWVIZ_DEFINER_DIALOG);
   }

   localize();
   goToCenter();

   ::SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
}

void ConfigureDialog::localize()
{
   //::SetWindowText(_hSelf, IDD_FWVIZ_DEFINER_TITLE);
}

INT_PTR CALLBACK ConfigureDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)
   {
   case WM_COMMAND:
      switch LOWORD(wParam)
      {
         case IDCANCEL:
         case IDOK:
            display(FALSE);
            return TRUE;
      }

   case NULL:
      switch (lParam)
      {
         case NULL:
            return FALSE;
      }

   }

   return FALSE;
}

