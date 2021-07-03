#include "DataExtractDialog.h"

extern VisualizerPanel _vizPanel;

void DataExtractDialog::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_DATA_EXTRACT_DIALOG);
   }

   hIndicator = GetDlgItem(_hSelf, IDC_DAT_EXT_CURRENT_LINE);

   for (int i{}; i < DISPLAY_COUNT; i++) {
      SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + i, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);
      SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + i, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);

      Utils::loadBitmap(_hSelf, IDC_DAT_EXT_ITEM_ADD_BTN_01 + i, IDB_VIZ_PLUS_BITMAP);
      Utils::addTooltip(_hSelf, IDC_DAT_EXT_ITEM_ADD_BTN_01 + i, NULL, DATA_EXTRACT_ADD_LINE_ITEM, FALSE);

      Utils::loadBitmap(_hSelf, IDC_DAT_EXT_ITEM_DEL_BTN_01 + i, IDB_VIZ_MINUS_BITMAP);
      Utils::addTooltip(_hSelf, IDC_DAT_EXT_ITEM_DEL_BTN_01 + i, NULL, DATA_EXTRACT_DEL_LINE_ITEM, FALSE);
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
   moveIndicators(0);
}

INT_PTR CALLBACK DataExtractDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
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
                     setLineItemFields(LOWORD(wParam) - IDC_DAT_EXT_ITEM_RECORD_01);
                     break;

                  case CBN_SETFOCUS:
                     moveIndicators(LOWORD(wParam) - IDC_DAT_EXT_ITEM_RECORD_01);
                     break;
                  }
                  break;

            case IDC_DAT_EXT_ITEM_FIELD_01:
            case IDC_DAT_EXT_ITEM_FIELD_02:
            case IDC_DAT_EXT_ITEM_FIELD_03:
            case IDC_DAT_EXT_ITEM_FIELD_04:
            case IDC_DAT_EXT_ITEM_FIELD_05:
            case IDC_DAT_EXT_ITEM_FIELD_06:
            case IDC_DAT_EXT_ITEM_FIELD_07:
            case IDC_DAT_EXT_ITEM_FIELD_08:
            case IDC_DAT_EXT_ITEM_FIELD_09:
            case IDC_DAT_EXT_ITEM_FIELD_10:
               switch HIWORD(wParam) {
                  case CBN_SETFOCUS:
                     moveIndicators(LOWORD(wParam) - IDC_DAT_EXT_ITEM_FIELD_01);
                     break;
                  }
                  break;

            case IDC_DAT_EXT_ITEM_PREFIX_01:
            case IDC_DAT_EXT_ITEM_PREFIX_02:
            case IDC_DAT_EXT_ITEM_PREFIX_03:
            case IDC_DAT_EXT_ITEM_PREFIX_04:
            case IDC_DAT_EXT_ITEM_PREFIX_05:
            case IDC_DAT_EXT_ITEM_PREFIX_06:
            case IDC_DAT_EXT_ITEM_PREFIX_07:
            case IDC_DAT_EXT_ITEM_PREFIX_08:
            case IDC_DAT_EXT_ITEM_PREFIX_09:
            case IDC_DAT_EXT_ITEM_PREFIX_10:
               switch HIWORD(wParam) {
                  case EN_SETFOCUS:
                     moveIndicators(LOWORD(wParam) - IDC_DAT_EXT_ITEM_PREFIX_01);
                     break;
                  }
                  break;

            case IDC_DAT_EXT_ITEM_SUFFIX_01:
            case IDC_DAT_EXT_ITEM_SUFFIX_02:
            case IDC_DAT_EXT_ITEM_SUFFIX_03:
            case IDC_DAT_EXT_ITEM_SUFFIX_04:
            case IDC_DAT_EXT_ITEM_SUFFIX_05:
            case IDC_DAT_EXT_ITEM_SUFFIX_06:
            case IDC_DAT_EXT_ITEM_SUFFIX_07:
            case IDC_DAT_EXT_ITEM_SUFFIX_08:
            case IDC_DAT_EXT_ITEM_SUFFIX_09:
            case IDC_DAT_EXT_ITEM_SUFFIX_10:
               switch HIWORD(wParam) {
                  case EN_SETFOCUS:
                     moveIndicators(LOWORD(wParam) - IDC_DAT_EXT_ITEM_SUFFIX_01);
                     break;
                  }
                  break;

            case IDC_DAT_EXT_ITEM_ADD_BTN_01:
            case IDC_DAT_EXT_ITEM_ADD_BTN_02:
            case IDC_DAT_EXT_ITEM_ADD_BTN_03:
            case IDC_DAT_EXT_ITEM_ADD_BTN_04:
            case IDC_DAT_EXT_ITEM_ADD_BTN_05:
            case IDC_DAT_EXT_ITEM_ADD_BTN_06:
            case IDC_DAT_EXT_ITEM_ADD_BTN_07:
            case IDC_DAT_EXT_ITEM_ADD_BTN_08:
            case IDC_DAT_EXT_ITEM_ADD_BTN_09:
            case IDC_DAT_EXT_ITEM_ADD_BTN_10:
               addLineItem(LOWORD(wParam) - IDC_DAT_EXT_ITEM_ADD_BTN_01);
               break;

            case IDC_DAT_EXT_ITEM_DEL_BTN_01:
            case IDC_DAT_EXT_ITEM_DEL_BTN_02:
            case IDC_DAT_EXT_ITEM_DEL_BTN_03:
            case IDC_DAT_EXT_ITEM_DEL_BTN_04:
            case IDC_DAT_EXT_ITEM_DEL_BTN_05:
            case IDC_DAT_EXT_ITEM_DEL_BTN_06:
            case IDC_DAT_EXT_ITEM_DEL_BTN_07:
            case IDC_DAT_EXT_ITEM_DEL_BTN_08:
            case IDC_DAT_EXT_ITEM_DEL_BTN_09:
            case IDC_DAT_EXT_ITEM_DEL_BTN_10:
               delLineItem(LOWORD(wParam) - IDC_DAT_EXT_ITEM_DEL_BTN_01);
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

      case WM_CTLCOLORSTATIC:
         return colorStaticControl(wParam, lParam);
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

   for (int i{}; i < DISPLAY_COUNT; i++) {
      SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + i, L"");
      SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + i, L"");
      resetDropDown(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + i));

      // Record Type Dropdown list
      HWND hRecList = GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + i);
      resetDropDown(hRecList);
      for (size_t j{}; j < recInfoList.size(); j++) {
         SendMessage(hRecList, CB_ADDSTRING, NULL, (LPARAM)recInfoList[j].label.c_str());
      }
   }
}

void DataExtractDialog::setLineItemFields(int line) {
   HWND hFTList = GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + line);
   resetDropDown(hFTList);

   size_t RTIndex = SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + line, CB_GETCURSEL, NULL, NULL);
   if (RTIndex == 0) return;
   RTIndex--;

   const vector<RecordInfo> recInfoList{ *pRecInfoList };
   size_t FTCount{ recInfoList[RTIndex].fieldLabels.size() };

   for (size_t i{}; i < FTCount; i++) {
      SendMessage(hFTList, CB_ADDSTRING, NULL, (LPARAM)recInfoList[RTIndex].fieldLabels[i].c_str());
   }
}

void DataExtractDialog::resetDropDown(HWND hList) {
   SendMessage(hList, CB_RESETCONTENT, NULL, NULL);
   SendMessage(hList, CB_ADDSTRING, NULL, (LPARAM)L"-");
   SendMessage(hList, CB_SETCURSEL, (WPARAM)0, NULL);
}

void DataExtractDialog::clearLineItem(int line) {
   SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + line, L"");
   SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + line, L"");
   SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + line, CB_SETCURSEL, (WPARAM)0, NULL);
   resetDropDown(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + line));
}

void DataExtractDialog::copyLineItem(int fromLine, int toLine) {
   wchar_t prefix[MAX_PATH + 1];
   GetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + fromLine, prefix, MAX_PATH);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + toLine, prefix);

   wchar_t suffix[MAX_PATH + 1];
   GetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + fromLine, suffix, MAX_PATH);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + toLine, suffix);

   SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + toLine, CB_SETCURSEL,
      SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + fromLine, CB_GETCURSEL, NULL, NULL), NULL);

   setLineItemFields(toLine);
   SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + toLine, CB_SETCURSEL,
      SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + fromLine, CB_GETCURSEL, NULL, NULL), NULL);
}
void DataExtractDialog::addLineItem(int line) {
   for (int i{ DISPLAY_COUNT }; i > line; i--) {
      copyLineItem(i - 1, i);
   }
   clearLineItem(line);

   SetFocus(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + line));
   moveIndicators(line);
}

void DataExtractDialog::delLineItem(int line) {
   for (int i{ line }; i < DISPLAY_COUNT; i++) {
      copyLineItem(i + 1, i);
   }
   clearLineItem(DISPLAY_COUNT);

   SetFocus(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + line));
   moveIndicators(line);
}

void DataExtractDialog::moveIndicators(int line) {
   RECT rectLine;
   GetWindowRect(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_DEL_BTN_01 + line), &rectLine);

   POINT newPos{ rectLine.right + 3, rectLine.top + 2 };
   ScreenToClient(_hSelf, &newPos);

   SetWindowPos(hIndicator, HWND_TOP, newPos.x, newPos.y, NULL, NULL, SWP_NOSIZE | SWP_NOOWNERZORDER);

   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_DOWN_BUTTON), line < DISPLAY_COUNT - 1);
   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_UP_BUTTON), line > 0);
}

INT_PTR DataExtractDialog::colorStaticControl(WPARAM wParam, LPARAM lParam) {
   HDC hdc = (HDC)wParam;
   DWORD ctrlID = GetDlgCtrlID((HWND)lParam);

   if (hbr != NULL) DeleteObject(hbr);

   switch (ctrlID) {
      case IDC_DAT_EXT_CURRENT_LINE:
         SetBkColor(hdc, Utils::intToRGB(0x00FFFF));
         SetTextColor(hdc, Utils::intToRGB(0x0000FF));
         hbr = CreateSolidBrush(Utils::intToRGB(0x00FFFF));
         return (INT_PTR)hbr;

      default:
         return NULL;
   }
}
