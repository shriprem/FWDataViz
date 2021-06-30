#include "DataExtractDialog.h"

#include "DataExtractDialog.h"
#include "VisualizerPanel.h"

extern VisualizerPanel _vizPanel;

void DataExtractDialog::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_DATA_EXTRACTOR_DIALOG);
   }

   for (int i{}; i < listedItemCount; i++) {
      Utils::loadBitmap(_hSelf, IDC_DAT_EXT_ITEM_DEL_BTN_01 + i, IDB_VIZ_DELETE_BITMAP);
      Utils::addTooltip(_hSelf, IDC_DAT_EXT_ITEM_DEL_BTN_01 + i, NULL, DATA_EXTRACT_DELETE_ITEM, FALSE);
   }

   Utils::loadBitmap(_hSelf, IDC_DAT_EXT_DOWN_BUTTON, IDB_VIZ_MOVE_DOWN_BITMAP);
   Utils::addTooltip(_hSelf, IDC_DAT_EXT_DOWN_BUTTON, NULL, DATA_EXTRACT_MOVE_DOWN, FALSE);

   Utils::loadBitmap(_hSelf, IDC_DAT_EXT_UP_BUTTON, IDB_VIZ_MOVE_UP_BITMAP);
   Utils::addTooltip(_hSelf, IDC_DAT_EXT_UP_BUTTON, NULL, DATA_EXTRACT_MOVE_UP, FALSE);

   Utils::loadBitmap(_hSelf, IDC_DAT_EXT_INFO_BUTTON, IDB_VIZ_INFO_BITMAP);
   Utils::addTooltip(_hSelf, IDC_DAT_EXT_INFO_BUTTON, NULL, VIZ_PANEL_INFO_TIP, FALSE);

   if (_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
   SetFocus(_hSelf);
}

void DataExtractDialog::initDialog(const wstring fileType) {
   initDocFileType = fileType;
   SetFocus(GetDlgItem(_hSelf, IDC_DAT_EXT_TEMPLATE_LIST));
}

INT_PTR CALLBACK DataExtractDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM) {
   switch (message) {
      case WM_COMMAND:
         switch LOWORD(wParam) {
            case IDC_DAT_EXT_INFO_BUTTON:
               ShellExecute(NULL, L"open", DATA_EXTRACT_INFO_README, NULL, NULL, SW_SHOW);
               break;

            case IDCANCEL:
               case IDCLOSE:
                  display(FALSE);
                  return TRUE;
         }
         break;
      }

   return FALSE;
}

void DataExtractDialog::localize() {
   //SetWindowText(_hSelf, JUMP_FIELD_DIALOG_TITLE);
   //SetDlgItemText(_hSelf, IDC_JUMP_FIELD_LABEL, JUMP_FIELD_SELECT_LABEL);
   //SetDlgItemText(_hSelf, IDC_JUMP_FIELD_GO_BTN, JUMP_FIELD_GO_BTN);
   //SetDlgItemText(_hSelf, IDCLOSE, JUMP_FIELD_CLOSE_BTN);
}

