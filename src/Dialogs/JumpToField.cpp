#include "JumpToField.h"
#include "VisualizerPanel.h"

extern VisualizerPanel _vizPanel;

void JumpToField::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_JUMP_FIELD_DIALOG);
   }

   hCaretFlash = GetDlgItem(_hSelf, IDC_JUMP_CARET_FLASH_SLIDER);
   SendMessage(hCaretFlash, TBM_SETRANGEMIN, FALSE, 1);
   SendMessage(hCaretFlash, TBM_SETRANGEMAX, FALSE, 10);

   Utils::addTooltip(_hSelf, IDC_JUMP_CARET_FLASH_LABEL, NULL, JUMP_TIP_CARET_FLASH, TRUE);
   Utils::addTooltip(_hSelf, IDC_JUMP_CARET_FLASH_SLIDER, NULL, JUMP_TIP_CARET_FLASH, TRUE);
   Utils::addTooltip(_hSelf, IDC_JUMP_CARET_FLASH_VALUE, NULL, JUMP_TIP_CARET_FLASH, TRUE);

   if (_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
}

void JumpToField::initDialog(const string fileType, int recordIndex,
   int fieldIndex, const vector<wstring>& fieldLabels) {
   initFileType = fileType;
   initRecordRegIndex = recordIndex;

   hFieldList = GetDlgItem(_hSelf, IDC_JUMP_FIELD_LIST);

   SendMessage(hFieldList, CB_RESETCONTENT, NULL, NULL);

   for (const wstring label : fieldLabels) {
      SendMessage(hFieldList, CB_ADDSTRING, NULL, (LPARAM)label.c_str());
   }

   if (fieldLabels.size() > 0)
      SendMessage(hFieldList, CB_SETCURSEL, fieldIndex < 0 ? 0 : fieldIndex, NULL);

   int flashSecs{ _configIO.getPreferenceInt(PREF_CARET_FLASH, 5) };
   SendMessage(hCaretFlash, TBM_SETPOS, TRUE, flashSecs);
   syncTbarToText(flashSecs);

   SetFocus(hFieldList);
}

void JumpToField::setFileTypeData(const wstring& ftConfig) {
   SetDlgItemText(_hSelf, IDC_JUMP_FIELD_LIST, ftConfig.c_str());
}

INT_PTR CALLBACK JumpToField::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
   switch (message) {
   case WM_COMMAND:
      switch LOWORD(wParam) {
      case IDCANCEL:
      case IDCLOSE:
         display(FALSE);
         return TRUE;

      case IDC_JUMP_FIELD_LIST:
         switch HIWORD(wParam) {
         case CBN_SELCHANGE:
            break;
         }
         break;

      case IDC_JUMP_FIELD_GO_BTN:
         onJumpBtnClick();
         break;
      }
      break;

   case WM_HSCROLL:
      if (lParam == (LPARAM)hCaretFlash) syncTbarToText();
      break;

   case WM_INITDIALOG:
      NPPDM_AutoSubclassAndThemeChildControls(_hSelf);
      break;

   case WM_CTLCOLORDLG:
   case WM_CTLCOLORBTN:
   case WM_CTLCOLORLISTBOX:
   case WM_CTLCOLORSTATIC:
      if (NPPDM_IsEnabled()) {
         return NPPDM_OnCtlColorDarker(reinterpret_cast<HDC>(wParam));
      }
      break;
   }

   return FALSE;
}

void JumpToField::localize() {
   SetWindowText(_hSelf, JUMP_FIELD_DIALOG_TITLE);
   SetDlgItemText(_hSelf, IDC_JUMP_FIELD_LABEL, JUMP_FIELD_SELECT_LABEL);
   SetDlgItemText(_hSelf, IDC_JUMP_CARET_FLASH_LABEL, JUMP_LABEL_CARET_FLASH);
   SetDlgItemText(_hSelf, IDC_JUMP_FIELD_GO_BTN, JUMP_FIELD_GO_BTN);
   SetDlgItemText(_hSelf, IDCLOSE, JUMP_FIELD_CLOSE_BTN);
}

void JumpToField::onJumpBtnClick() {
   _configIO.setPreferenceInt(PREF_CARET_FLASH, getTbarPosition());
   display(FALSE);

   _vizPanel.jumpToField(initFileType, initRecordRegIndex,
      static_cast<int>(SendMessage(hFieldList, CB_GETCURSEL, NULL, NULL)));
}

int JumpToField::getTbarPosition() {
   return static_cast<int>(SendMessage(hCaretFlash, TBM_GETPOS, 0, 0));
}

void JumpToField::syncTbarToText(int optVal) {
   SetDlgItemInt(_hSelf, IDC_JUMP_CARET_FLASH_VALUE, (optVal ? optVal : getTbarPosition()), FALSE);
}
