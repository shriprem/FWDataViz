#include "DataExtractDialog.h"

extern VisualizerPanel _vizPanel;

void DataExtractDialog::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_DATA_EXTRACT_DIALOG);
   }

   for (int i{}; i < DISPLAY_COUNT; i++) {
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

void DataExtractDialog::initDialog(const wstring fileType, const vector<RecordInfo>& recInfoList) {
   initDocFileType = fileType;
   pRecInfoList = &recInfoList;

   initDisplayFields();
   SetFocus(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01));
}

INT_PTR CALLBACK DataExtractDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM) {
   switch (message) {
      case WM_COMMAND:
         switch LOWORD(wParam) {
            case IDC_DAT_EXT_ITEM_RECORD_01:
            case IDC_DAT_EXT_ITEM_RECORD_02:
            case IDC_DAT_EXT_ITEM_RECORD_03:
            case IDC_DAT_EXT_ITEM_RECORD_04:
            case IDC_DAT_EXT_ITEM_RECORD_05:
            case IDC_DAT_EXT_ITEM_RECORD_06:
            case IDC_DAT_EXT_ITEM_RECORD_07:
            case IDC_DAT_EXT_ITEM_RECORD_08:
            case IDC_DAT_EXT_ITEM_RECORD_09:
            case IDC_DAT_EXT_ITEM_RECORD_10:
               switch HIWORD(wParam) {
               case CBN_SELCHANGE:
                  onRecTypeChange(LOWORD(wParam));
                  break;
               }
               break;

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
   SetWindowText(_hSelf, DATA_EXTRACT_DIALOG_TITLE);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_PREFIX_LABEL, DATA_EXTRACT_PREFIX_LABEL);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_RECORD_LABEL, DATA_EXTRACT_RECORD_LABEL);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_FIELD_LABEL, DATA_EXTRACT_FIELD_LABEL);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_SUFFIX_LABEL, DATA_EXTRACT_SUFFIX_LABEL);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_EXTRACT_BTN, DATA_EXTRACT_EXTRACT_BTN);
   SetDlgItemText(_hSelf, IDCLOSE, DATA_EXTRACT_CLOSE_BTN);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_GROUP, DATA_EXTRACT_TEMPLATE_GROUP);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_LIST_LABEL, DATA_EXTRACT_TEMPLATE_LOAD);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_NAME_LABEL, DATA_EXTRACT_TEMPLATE_NAME);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_SAVE_BTN, DATA_EXTRACT_TEMPLATE_SAVE);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_NEW_BTN, DATA_EXTRACT_TEMPLATE_NEW);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_DEL_BTN, DATA_EXTRACT_TEMPLATE_DEL);
}

void DataExtractDialog::initDisplayFields() {
   const vector<RecordInfo> recInfoList{ *pRecInfoList };
   int recTypeCount{ static_cast<int>(recInfoList.size()) };

   for (int i{}; i < DISPLAY_COUNT; i++) {
      SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + i, L"");
      SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + i, L"");
      SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + i, CB_RESETCONTENT, NULL, NULL);

      // Initialize Record Type Dropdown Lists
      HWND hRecList = GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + i);
      SendMessage(hRecList, CB_RESETCONTENT, NULL, NULL);
      SendMessage(hRecList, CB_ADDSTRING, NULL, (LPARAM)L"-");

      for (int j{}; j < recTypeCount; j++) {
         SendMessage(hRecList, CB_ADDSTRING, NULL, (LPARAM)recInfoList[j].label.c_str());
      }

      SendMessage(hRecList, CB_SETCURSEL, (WPARAM)0, NULL);
   }
}

void DataExtractDialog::onRecTypeChange(int ctrlID) {
   const vector<RecordInfo> recInfoList{ *pRecInfoList };

   HWND hFTList = GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + (ctrlID - IDC_DAT_EXT_ITEM_RECORD_01));
   int RTIndex = static_cast<int>(SendDlgItemMessage(_hSelf, ctrlID, CB_GETCURSEL, NULL, NULL)) - 1;

   if (RTIndex < 0) {
      SendMessage(hFTList, CB_RESETCONTENT, NULL, NULL);
      return;
   }

   int FTCount{ static_cast<int>(recInfoList[RTIndex].fieldLabels.size()) };
   SendMessage(hFTList, CB_RESETCONTENT, NULL, NULL);
   SendMessage(hFTList, CB_ADDSTRING, NULL, (LPARAM)L"-");

   for (int i{}; i < FTCount; i++) {
      SendMessage(hFTList, CB_ADDSTRING, NULL, (LPARAM)recInfoList[RTIndex].fieldLabels[i].c_str());
   }

   SendMessage(hFTList, CB_SETCURSEL, (WPARAM)0, NULL);
}


