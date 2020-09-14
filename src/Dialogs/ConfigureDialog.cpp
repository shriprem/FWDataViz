#include "ConfigureDialog.h"

void ConfigureDialog::doDialog(HINSTANCE hInst)
{
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_FWVIZ_DEFINER_DIALOG);
   }

   if (_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   ::SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
}

void ConfigureDialog::localize()
{
   ::SetWindowText(_hSelf, FWVIZ_DIALOG_TITLE);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_GROUP_BOX, FWVIZ_DEF_FILE_GROUP_BOX);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_DESC_LABEL, FWVIZ_DEF_FILE_DESC_LABEL);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_TERM_LABEL, FWVIZ_DEF_FILE_TERM_LABEL);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LABEL, FWVIZ_DEF_FILE_THEME_LABEL);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_NEW_BTN, FWVIZ_DEF_FILE_NEW_BTN);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_ACCEPT_BTN, FWVIZ_DEF_FILE_ACCEPT_BTN);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_DEL_BTN, FWVIZ_DEF_FILE_DEL_BTN);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_GROUP_BOX, FWVIZ_DEF_REC_GROUP_BOX);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_DESC_LABEL, FWVIZ_DEF_REC_DESC_LABEL);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_START_LABEL, FWVIZ_DEF_REC_START_LABEL);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_REGEX_LABEL, FWVIZ_DEF_REC_REGEX_LABEL);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_NEW_BTN, FWVIZ_DEF_REC_NEW_BTN);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_ACCEPT_BTN, FWVIZ_DEF_REC_ACCEPT_BTN);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_DEL_BTN, FWVIZ_DEF_REC_DEL_BTN);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_GROUP_BOX, FWVIZ_DEF_FIELD_GROUP_BOX);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_WIDTHS_TEXT, FWVIZ_DEF_FIELD_WIDTHS_TEXT);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_LABELS_TEXT, FWVIZ_DEF_FIELD_LABELS_TEXT);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_ACCEPT_BTN, FWVIZ_DEF_FIELD_ACCEPT_BTN);
   ::SetDlgItemText(_hSelf, IDOK, FWVIZ_DIALOG_SAVE_BTN);
   ::SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_RESET_BTN, FWVIZ_DIALOG_RESET_BTN);
   ::SetDlgItemText(_hSelf, IDCANCEL, FWVIZ_DIALOG_CLOSE_BTN);
}

INT_PTR CALLBACK ConfigureDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM) {
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
   }

   return FALSE;
}

