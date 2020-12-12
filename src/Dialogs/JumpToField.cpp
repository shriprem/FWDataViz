#include "JumpToField.h"

void JumpToField::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_JUMP_FIELD_DIALOG);
   }

   if (_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
}

void JumpToField::initDialog() {
   SetFocus(GetDlgItem(_hSelf, IDC_JUMP_FIELD_LIST));
}

void JumpToField::setFileTypeData(const wstring& ftConfig) {
   SetDlgItemText(_hSelf, IDC_JUMP_FIELD_LIST, ftConfig.c_str());
}

INT_PTR CALLBACK JumpToField::run_dlgProc(UINT message, WPARAM wParam, LPARAM) {
   switch (message) {
      case WM_COMMAND:
         switch LOWORD(wParam) {
            case IDCANCEL:
            case IDCLOSE:
               display(FALSE);
               return TRUE;

            case IDC_JUMP_FIELD_LIST:
               switch HIWORD(wParam) {
                  case LBN_SELCHANGE:
                     //visualizeTheme();
                     break;
                  }
               break;

            case IDC_JUMP_FIELD_GO_BTN:
               break;
         }
         break;
   }

   return FALSE;
}

void JumpToField::localize() {
   SetWindowText(_hSelf, JUMP_FIELD_DIALOG_TITLE);
   SetDlgItemText(_hSelf, IDC_JUMP_FIELD_LABEL, JUMP_FIELD_SELECT_LABEL);
   SetDlgItemText(_hSelf, IDC_JUMP_FIELD_GO_BTN, JUMP_FIELD_GO_BTN);
   SetDlgItemText(_hSelf, IDCLOSE, JUMP_FIELD_CLOSE_BTN);
}
