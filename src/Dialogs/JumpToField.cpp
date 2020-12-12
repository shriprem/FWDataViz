#include "JumpToField.h"
#include "VisualizerPanel.h"

extern VisualizerPanel _vizPanel;

void JumpToField::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_JUMP_FIELD_DIALOG);
   }

   if (_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
}

void JumpToField::initDialog(int recordIndex, int fieldIndex, const vector<wstring>& fieldLabels) {
   caretRecordRegIndex = recordIndex;

   hFieldList = GetDlgItem(_hSelf, IDC_JUMP_FIELD_LIST);

   SendMessage(hFieldList, CB_RESETCONTENT, NULL, NULL);

   for (const auto label : fieldLabels) {
      SendMessage(hFieldList, CB_ADDSTRING, NULL, (LPARAM)label.c_str());
   }

   if (fieldLabels.size() > 0)
      SendMessage(hFieldList, CB_SETCURSEL, fieldIndex < 0 ? 0 : fieldIndex, NULL);

   SetFocus(hFieldList);
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
                     break;
                  }
               break;

            case IDC_JUMP_FIELD_GO_BTN:
               onJumpBtnClick();
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

void JumpToField::onJumpBtnClick() {
   display(FALSE);

   _vizPanel.jumpToField(caretRecordRegIndex,
      static_cast<int>(SendMessage(hFieldList, CB_GETCURSEL, NULL, NULL)));
}

