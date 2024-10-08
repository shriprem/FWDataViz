#include "JumpToField.h"
#include "VisualizerPanel.h"

extern VisualizerPanel _vizPanel;

void JumpToField::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_JUMP_FIELD_DIALOG);
   }

   CheckDlgButton(_hSelf, IDC_JUMP_FIELD_PREF_SEQ_NUM, _configIO.getPreferenceBool(PREF_JUMP_FIELD_SEQ, FALSE) ? BST_CHECKED : BST_UNCHECKED);

   hCaretFlash = GetDlgItem(_hSelf, IDC_JUMP_CARET_FLASH_SLIDER);
   SendMessage(hCaretFlash, TBM_SETRANGEMIN, FALSE, 1);
   SendMessage(hCaretFlash, TBM_SETRANGEMAX, FALSE, 10);

   using Utils::addTooltip;

   addTooltip(_hSelf, IDC_JUMP_CARET_FLASH_LABEL, L"", JUMP_TIP_CARET_FLASH, TRUE);
   addTooltip(_hSelf, IDC_JUMP_CARET_FLASH_SLIDER, L"", JUMP_TIP_CARET_FLASH, TRUE);
   addTooltip(_hSelf, IDC_JUMP_CARET_FLASH_VALUE, L"", JUMP_TIP_CARET_FLASH, TRUE);

   if constexpr(_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
}

void JumpToField::refreshDarkMode() {
   NPPDM_AutoThemeChildControls(_hSelf);
   redraw();
   SendMessage(hCaretFlash, TBM_SETRANGEMIN, FALSE, 1);
}

void JumpToField::initDialog(const string fileType, int recordIndex, int fieldIndex, const vector<wstring>& fieldLabels) {
   initFileType = fileType;
   initRecordRegIndex = recordIndex;
   pFieldLabels = &fieldLabels;

   hFieldList = GetDlgItem(_hSelf, IDC_JUMP_FIELD_LIST);
   loadJumpList(fieldIndex);

   int flashSecs{ _configIO.getPreferenceInt(PREF_CARET_FLASH, 5) };
   SendMessage(hCaretFlash, TBM_SETPOS, TRUE, flashSecs);
   setTbarPosition(flashSecs, FALSE);

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

      case IDC_JUMP_FIELD_PREF_SEQ_NUM:
         onJumpSeqNumPref();
         break;

      case IDC_JUMP_FIELD_GO_BTN:
         onJumpBtnClick();
         break;
      }
      break;

   case WM_HSCROLL:
      if (lParam == (LPARAM)hCaretFlash) setTbarPosition(getTbarPosition(), TRUE);
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

void JumpToField::localize() {
   SetWindowText(_hSelf, JUMP_FIELD_DIALOG_TITLE);
   SetDlgItemText(_hSelf, IDC_JUMP_FIELD_LABEL, JUMP_FIELD_SELECT_LABEL);
   SetDlgItemText(_hSelf, IDC_JUMP_FIELD_PREF_SEQ_NUM, JUMP_FIELD_PREF_SEQ_NUM);
   SetDlgItemText(_hSelf, IDC_JUMP_CARET_FLASH_LABEL, JUMP_LABEL_CARET_FLASH);
   SetDlgItemText(_hSelf, IDC_JUMP_FIELD_GO_BTN, JUMP_FIELD_GO_BTN);
   SetDlgItemText(_hSelf, IDCLOSE, JUMP_FIELD_CLOSE_BTN);
}

void JumpToField::loadJumpList(int fieldIndex) {
   if (fieldIndex < 0)
      fieldIndex = static_cast<int>(SendMessage(hFieldList, CB_GETCURSEL, 0, 0));

   SendMessage(hFieldList, CB_RESETCONTENT, NULL, NULL);

   wchar_t seqNo[5];
   bool showSeqNo{ (_configIO.getPreferenceBool(PREF_JUMP_FIELD_SEQ, FALSE)) };

   int fieldCount{ static_cast<int>(pFieldLabels->size()) };

   for (int i{}; i < fieldCount; ++i) {
      swprintf(seqNo, 5, L"%02d. ", (i + 1));
      SendMessage(hFieldList, CB_ADDSTRING, NULL, (LPARAM)((showSeqNo ? seqNo : L"") + pFieldLabels->at(i)).c_str());
   }

   if (fieldCount > 0)
      Utils::setComboBoxSelection(hFieldList, fieldIndex < 0 ? 0 : fieldIndex);
}

void JumpToField::onJumpSeqNumPref() {
   _configIO.setPreferenceBool(PREF_JUMP_FIELD_SEQ, (IsDlgButtonChecked(_hSelf, IDC_JUMP_FIELD_PREF_SEQ_NUM) == BST_CHECKED));
   loadJumpList();
}

void JumpToField::onJumpBtnClick() {
   display(FALSE);

   _vizPanel.jumpToField(initFileType, initRecordRegIndex,
      static_cast<int>(SendMessage(hFieldList, CB_GETCURSEL, NULL, NULL)));
}

int JumpToField::getTbarPosition() const {
   return static_cast<int>(SendMessage(hCaretFlash, TBM_GETPOS, 0, 0));
}

void JumpToField::setTbarPosition(int val, bool savePref) {
   SetDlgItemInt(_hSelf, IDC_JUMP_CARET_FLASH_VALUE, val, FALSE);
   if (savePref) _configIO.setPreferenceInt(PREF_CARET_FLASH, val);
}
