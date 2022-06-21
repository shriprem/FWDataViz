#include "DataExtractDialog.h"

extern VisualizerPanel _vizPanel;
extern DataExtractDialog _dataExtractDlg;

LRESULT CALLBACK procKeyNavigation(HWND hwnd, UINT messageId, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
   switch (messageId) {
   case WM_KEYDOWN:
      if (_dataExtractDlg.processKey(hwnd, wParam)) return FALSE;
      break;

   case WM_SYSKEYDOWN:
      if (_dataExtractDlg.processSysKey(hwnd, wParam)) return FALSE;
      break;
   }

   return DefSubclassProc(hwnd, messageId, wParam, lParam);
}


LRESULT CALLBACK procTemplateName(HWND hwnd, UINT messageId, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
   switch (messageId) {
   case WM_CHAR:
      if (wParam == '[' || wParam == ']') {
         Utils::showEditBalloonTip(hwnd, DATA_EXTRACT_INVTEMPL_TITLE, DATA_EXTRACT_INVTEMPL_MSG);
         return FALSE;
      }
      break;

   case WM_PASTE:
   {
      wstring clipText;

      Utils::getClipboardText(GetParent(hwnd), clipText);

      if (!regex_match(clipText, std::wregex(L"[|]"))) {
         Utils::showEditBalloonTip(hwnd, DATA_EXTRACT_INVTEMPL_TITLE, DATA_EXTRACT_INVTEMPL_MSG);
         return FALSE;
      }
      break;
   }
   }

   return DefSubclassProc(hwnd, messageId, wParam, lParam);
}

void DataExtractDialog::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_DATA_EXTRACT_DIALOG);
   }

   hIndicator = GetDlgItem(_hSelf, IDC_DAT_EXT_CURRENT_LINE);
   hTemplatesList = GetDlgItem(_hSelf, IDC_DAT_EXT_TEMPLATE_LIST);
   hTemplateName = GetDlgItem(_hSelf, IDC_DAT_EXT_TEMPLATE_NAME);

   SendMessage(hTemplateName, EM_LIMITTEXT, (WPARAM)MAX_TEMPLATE_NAME, NULL);
   SetWindowSubclass(hTemplateName, procTemplateName, NULL, NULL);

   for (int i{}; i < LINES_PER_PAGE; ++i) {
      SetWindowSubclass(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + i), procKeyNavigation, NULL, NULL);
      SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + i, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);

      SetWindowSubclass(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + i), procKeyNavigation, NULL, NULL);
      SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + i, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);

      Utils::loadBitmap(_hSelf, IDC_DAT_EXT_ITEM_ADD_BTN_01 + i, IDB_DAT_EXT_PLUS_BITMAP);
      Utils::addTooltip(_hSelf, IDC_DAT_EXT_ITEM_ADD_BTN_01 + i, NULL, DATA_EXTRACT_ADD_LINE_ITEM, FALSE);

      Utils::loadBitmap(_hSelf, IDC_DAT_EXT_ITEM_DEL_BTN_01 + i, IDB_DAT_EXT_MINUS_BITMAP);
      Utils::addTooltip(_hSelf, IDC_DAT_EXT_ITEM_DEL_BTN_01 + i, NULL, DATA_EXTRACT_DEL_LINE_ITEM, FALSE);
   }

   Utils::loadBitmap(_hSelf, IDC_DAT_EXT_PAGE_PREV_BUTTON, IDB_DAT_EXT_PAGE_PREV_BITMAP);
   Utils::addTooltip(_hSelf, IDC_DAT_EXT_PAGE_PREV_BUTTON, NULL, DATA_EXTRACT_PAGE_PREV, FALSE);

   Utils::loadBitmap(_hSelf, IDC_DAT_EXT_PAGE_NEXT_BUTTON, IDB_DAT_EXT_PAGE_NEXT_BITMAP);
   Utils::addTooltip(_hSelf, IDC_DAT_EXT_PAGE_NEXT_BUTTON, NULL, DATA_EXTRACT_PAGE_NEXT, FALSE);

   Utils::loadBitmap(_hSelf, IDC_DAT_EXT_PAGE_ADD_BUTTON, IDB_DAT_EXT_PAGE_ADD_BITMAP);
   Utils::addTooltip(_hSelf, IDC_DAT_EXT_PAGE_ADD_BUTTON, NULL, DATA_EXTRACT_PAGE_ADD, FALSE);

   Utils::loadBitmap(_hSelf, IDC_DAT_EXT_PAGE_DEL_BUTTON, IDB_DAT_EXT_PAGE_DEL_BITMAP);
   Utils::addTooltip(_hSelf, IDC_DAT_EXT_PAGE_DEL_BUTTON, NULL, DATA_EXTRACT_PAGE_DEL, FALSE);

   Utils::loadBitmap(_hSelf, IDC_DAT_EXT_ITEM_DOWN_BUTTON, IDB_VIZ_MOVE_DOWN_BITMAP);
   Utils::addTooltip(_hSelf, IDC_DAT_EXT_ITEM_DOWN_BUTTON, NULL, DATA_EXTRACT_MOVE_DOWN, FALSE);

   Utils::loadBitmap(_hSelf, IDC_DAT_EXT_ITEM_UP_BUTTON, IDB_VIZ_MOVE_UP_BITMAP);
   Utils::addTooltip(_hSelf, IDC_DAT_EXT_ITEM_UP_BUTTON, NULL, DATA_EXTRACT_MOVE_UP, FALSE);

   CheckDlgButton(_hSelf, IDC_DAT_EXT_TEMPLATE_CURR_ONLY, BST_CHECKED);

   Utils::loadBitmap(_hSelf, IDC_DAT_EXT_INFO_BUTTON, IDB_VIZ_INFO_BITMAP);
   Utils::addTooltip(_hSelf, IDC_DAT_EXT_INFO_BUTTON, NULL, VIZ_PANEL_INFO_TIP, FALSE);

   if (_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
   SetFocus(_hSelf);
}

void DataExtractDialog::initDialog(const string fileType, const vector<RecordInfo>& recInfoList) {
   initFileType = fileType;
   cRecInfoList = recInfoList;

   initFileTypeLabel = _configIO.getConfigWideChar(initFileType, "FileLabel");
   extractsConfigFile = Utils::WideToNarrow(_configIO.getConfigFile(_configIO.CONFIG_EXTRACTS));

   initRecTypeLists();
   loadTemplatesList();
   newTemplate();
   moveIndicators(0, TRUE);
}

void DataExtractDialog::refreshDarkMode() {
   NPPDM_AutoSubclassAndThemeChildControls(_hSelf);
   redraw();
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
            initLineItemFieldList(LOWORD(wParam) - IDC_DAT_EXT_ITEM_RECORD_01);
            break;

         case CBN_SETFOCUS:
            moveIndicators(LOWORD(wParam) - IDC_DAT_EXT_ITEM_RECORD_01, FALSE);
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
            moveIndicators(LOWORD(wParam) - IDC_DAT_EXT_ITEM_FIELD_01, FALSE);
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
            moveIndicators(LOWORD(wParam) - IDC_DAT_EXT_ITEM_PREFIX_01, FALSE);
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
            moveIndicators(LOWORD(wParam) - IDC_DAT_EXT_ITEM_SUFFIX_01, FALSE);
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

      case IDC_DAT_EXT_PAGE_PREV_BUTTON:
         previousPage();
         break;

      case IDC_DAT_EXT_PAGE_NEXT_BUTTON:
         nextPage();
         break;

      case IDC_DAT_EXT_PAGE_ADD_BUTTON:
         addPage();
         break;

      case IDC_DAT_EXT_PAGE_DEL_BUTTON:
         deletePage();
         break;

      case IDC_DAT_EXT_ITEM_DOWN_BUTTON:
         swapLineItems(currentLineItem, currentLineItem + 1);
         break;

      case IDC_DAT_EXT_ITEM_UP_BUTTON:
         swapLineItems(currentLineItem, currentLineItem - 1);
         break;

      case IDC_DAT_EXT_INFO_BUTTON:
         ShellExecute(NULL, L"open", DATA_EXTRACT_INFO_README, NULL, NULL, SW_SHOW);
         break;

      case IDC_DAT_EXT_EXTRACT_BTN:
         extractData();
         break;

      case IDCANCEL:
      case IDCLOSE:
         display(FALSE);
         return TRUE;

      case IDC_DAT_EXT_TEMPLATE_CURR_ONLY:
         loadTemplatesList();
         newTemplate();
         break;

      case IDC_DAT_EXT_TEMPLATE_LIST:
         switch HIWORD(wParam) {
         case CBN_SELCHANGE:
            loadTemplate();
            break;
         }
         break;

      case IDC_DAT_EXT_TEMPLATE_NAME:
         switch HIWORD(wParam) {
         case EN_CHANGE:
            enableSaveTemplate();
            break;
         }
         break;

      case IDC_DAT_EXT_TEMPLATE_SAVE_BTN:
         saveTemplate();
         break;

      case IDC_DAT_EXT_TEMPLATE_NEW_BTN:
         newTemplate();
         break;

      case IDC_DAT_EXT_TEMPLATE_DEL_BTN:
         deleteTemplate();
         break;

      }
      break;

   case WM_NOTIFY:
      switch (((LPNMHDR)lParam)->code) {
      case NM_CLICK:
      case NM_RETURN:
         ShellExecute(NULL, L"open", DATA_EXTRACT_KEYNAV_README, NULL, NULL, SW_SHOW);
         display(FALSE);
         return TRUE;
      }
      break;

   case WM_INITDIALOG:
      NPPDM_InitSysLink(GetDlgItem(_hSelf, IDC_DAT_EXT_NEW_KEYBOARD_TIP));
      NPPDM_AutoSubclassAndThemeChildControls(_hSelf);
      break;

   case WM_CTLCOLORSTATIC:
      switch (GetDlgCtrlID((HWND)lParam)) {
      case IDC_DAT_EXT_CURRENT_LINE:
      case IDC_DAT_EXT_NEW_KEYBOARD_TIP:
         return NPPDM_OnCtlColorSysLink(reinterpret_cast<HDC>(wParam));
         break;

      default:
         if (NPPDM_IsEnabled()) {
            return NPPDM_OnCtlColorDarker(reinterpret_cast<HDC>(wParam));
         }
      }
      break;

   case WM_CTLCOLORDLG:
      if (NPPDM_IsEnabled()) {
         return NPPDM_OnCtlColorDarker(reinterpret_cast<HDC>(wParam));
      }
      break;

   case WM_CTLCOLOREDIT:
      if (NPPDM_IsEnabled()) {
         return NPPDM_OnCtlColorSofter(reinterpret_cast<HDC>(wParam));
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

void DataExtractDialog::localize() {
   SetWindowText(_hSelf, DATA_EXTRACT_DIALOG_TITLE);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_PREFIX_LABEL, DATA_EXTRACT_PREFIX_LABEL);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_RECORD_LABEL, DATA_EXTRACT_RECORD_LABEL);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_FIELD_LABEL, DATA_EXTRACT_FIELD_LABEL);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_SUFFIX_LABEL, DATA_EXTRACT_SUFFIX_LABEL);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_NEW_KEYBOARD_TIP, DATA_EXTRACT_KEYBOARD_TIP);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_NEW_LINE_TAB_TIP, DATA_EXTRACT_NEW_LINE_TAB);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_EXTRACT_BTN, DATA_EXTRACT_EXTRACT_BTN);
   SetDlgItemText(_hSelf, IDCLOSE, DATA_EXTRACT_CLOSE_BTN);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_GROUP, DATA_EXTRACT_TEMPLATE_GROUP);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_CURR_ONLY, DATA_EXTRACT_TEMPLATE_CURR);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_LIST_LABEL, DATA_EXTRACT_TEMPLATE_LOAD);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_NAME_LABEL, DATA_EXTRACT_TEMPLATE_NAME);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_SAVE_BTN, DATA_EXTRACT_TEMPLATE_SAVE);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_NEW_BTN, DATA_EXTRACT_TEMPLATE_NEW);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_DEL_BTN, DATA_EXTRACT_TEMPLATE_DEL);
}

void DataExtractDialog::initRecTypeLists() {
   const vector<RecordInfo> recInfoList{ cRecInfoList };

   for (int i{}; i < LINES_PER_PAGE; ++i) {
      // Load Record Type Dropdown lists
      HWND hRecList = GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + i);
      resetDropDown(hRecList);

      for (size_t j{}; j < recInfoList.size(); ++j) {
         SendMessage(hRecList, CB_ADDSTRING, NULL, (LPARAM)recInfoList[j].label.c_str());
      }
   }
}

void DataExtractDialog::initLineItemFieldList(int line) {
   HWND hFTList = GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + line);
   resetDropDown(hFTList);

   string fileType{};
   if (!_vizPanel.getDocFileType(fileType) || (initFileType != fileType)) return;

   size_t RTIndex = SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + line, CB_GETCURSEL, NULL, NULL);
   if (RTIndex == 0) return;
   --RTIndex;

   const vector<RecordInfo> recInfoList{ cRecInfoList };
   size_t FTCount{ recInfoList[RTIndex].fieldLabels.size() };

   for (size_t i{}; i < FTCount; ++i) {
      SendMessage(hFTList, CB_ADDSTRING, NULL, (LPARAM)recInfoList[RTIndex].fieldLabels[i].c_str());
   }
}

void DataExtractDialog::moveIndicators(int line, bool focusPrefix) {
   currentLineItem = line;

   RECT rc;
   GetWindowRect(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_DEL_BTN_01 + line), &rc);

   POINT newPos{ rc.right + 1, rc.top + 2 };
   ScreenToClient(_hSelf, &newPos);

   SetWindowPos(hIndicator, HWND_TOP, newPos.x, newPos.y, NULL, NULL, SWP_NOSIZE | SWP_NOOWNERZORDER);

   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_DOWN_BUTTON),
      line < LINES_PER_PAGE - 1 || currentPage < getPageCount() - 1);
   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_UP_BUTTON), line > 0 || currentPage > 0);

   if (focusPrefix)
      SetFocus(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + line));
}

void DataExtractDialog::resetDropDown(HWND hList) {
   SendMessage(hList, CB_RESETCONTENT, NULL, NULL);
   SendMessage(hList, CB_ADDSTRING, NULL, (LPARAM)L"-");
   Utils::setComboBoxSelection(hList, 0);
}

bool DataExtractDialog::isBlankLineItem(const LineItemInfo& lineItem) {
   return (lineItem.prefix.length() + lineItem.suffix.length() + lineItem.recType + lineItem.fieldType == 0);
}

void DataExtractDialog::addLineItem(int line) {
   int idx{ (currentPage * 10) + line };

   readPage();

   if (isBlankLineItem(liBuffer.back()))
      liBuffer.erase(liBuffer.end() - 1);

   LineItemInfo liEmpty{};
   liBuffer.insert(liBuffer.begin() + idx, liEmpty);
   if (liBuffer.size() > MAX_BUFFER_LINES)
      liBuffer.resize(MAX_BUFFER_LINES);

   loadPage(currentPage);
   moveIndicators(line, TRUE);
}

void DataExtractDialog::delLineItem(int line) {
   int idx{ (currentPage * 10) + line };

   readPage();
   liBuffer.erase(liBuffer.begin() + idx);

   loadPage(currentPage);
   moveIndicators(line, TRUE);
}

void DataExtractDialog::clearLineItem(int line) {
   SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + line, L"");
   SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + line, L"");
   Utils::setComboBoxSelection(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + line), 0);
   resetDropDown(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + line));
}

void DataExtractDialog::getLineItem(int line, LineItemInfo& lineItem) {
   TCHAR prefix[MAX_PATH + 1];
   GetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + line, prefix, MAX_PATH);
   lineItem.prefix = wstring{ prefix };

   TCHAR suffix[MAX_PATH + 1];
   GetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + line, suffix, MAX_PATH);
   lineItem.suffix = wstring{ suffix };

   lineItem.recType = static_cast<int>(
      SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + line, CB_GETCURSEL, NULL, NULL));

   lineItem.fieldType = static_cast<int>(
      SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + line, CB_GETCURSEL, NULL, NULL));
}

void DataExtractDialog::setLineItem(int line, LineItemInfo& lineItem) {
   SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + line, lineItem.prefix.c_str());
   SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + line, lineItem.suffix.c_str());
   Utils::setComboBoxSelection(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + line), lineItem.recType);
   initLineItemFieldList(line);
   Utils::setComboBoxSelection(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + line), lineItem.fieldType);
}

void DataExtractDialog::swapLineItems(int lineFrom, int lineTo) {
   int idxFrom{ (currentPage * 10) + lineFrom };
   int idxTo{ (currentPage * 10) + lineTo };
   int page{ currentPage };
   int line{ lineTo };

   int ctrlID{ GetDlgCtrlID(GetFocus()) };
   int ctrlTop{ (ctrlID - currentLineItem == IDC_DAT_EXT_ITEM_SUFFIX_01) ?
      IDC_DAT_EXT_ITEM_SUFFIX_01 : IDC_DAT_EXT_ITEM_PREFIX_01 };

   if (idxTo < 0 || idxTo >= static_cast<int>(liBuffer.size())) return;

   readPage();

   if (lineTo >= 0 && lineTo < LINES_PER_PAGE) {
      getLineItem(lineFrom, liBuffer[idxTo]);
      getLineItem(lineTo, liBuffer[idxFrom]);
   }
   else {
      LineItemInfo& liFrom{ liBuffer[idxFrom] };
      LineItemInfo liTo{ liBuffer[idxTo] };

      getLineItem(lineFrom, liBuffer[idxTo]);

      liFrom.prefix = liTo.prefix;
      liFrom.recType = liTo.recType;
      liFrom.fieldType = liTo.fieldType;
      liFrom.suffix = liTo.suffix;

      if (lineTo < 0) {
         line = LINES_PER_PAGE - 1;
         --page;
      }
      else if (lineTo >= LINES_PER_PAGE) {
         line = 0;
         ++page;
      }
   }

   loadPage(page);
   SetFocus(GetDlgItem(_hSelf, ctrlTop + line));
}

void DataExtractDialog::gotoLine(int ctrlID, int lineTo) {
   int idxTo{ (currentPage * 10) + lineTo };
   int page{ currentPage };
   int line{ lineTo };
   int ctrlTop{ ctrlID - currentLineItem };

   if (idxTo < 0 || idxTo >= static_cast<int>(liBuffer.size())) return;

   if (lineTo < 0 || lineTo >= LINES_PER_PAGE) {
      readPage();

      if (lineTo < 0) {
         line = LINES_PER_PAGE - 1;
         --page;
      }
      else if (lineTo >= LINES_PER_PAGE) {
         line = 0;
         ++page;
      }

      loadPage(page);
   }

   SetFocus(GetDlgItem(_hSelf, ctrlTop + line));
}

size_t DataExtractDialog::getValidLineItems(vector<LineItemInfo>& validLIs, bool validFieldType, bool activateNLT) {
   readPage();
   validLIs.clear();

   for (size_t i{}; i < liBuffer.size(); ++i) {
      LineItemInfo lineInfo{ liBuffer[i] };

      if (isBlankLineItem(lineInfo)) continue;
      if (validFieldType && lineInfo.fieldType == 0) continue;

      // Decrement both recType & fieldType by one to account for the first "-" item in their dropdowns
      lineInfo.recType--;
      lineInfo.fieldType--;

      if (activateNLT) {
         _configIO.ActivateNewLineTabs(lineInfo.prefix);
         _configIO.ActivateNewLineTabs(lineInfo.suffix);
      }

      validLIs.emplace_back(lineInfo);
   }

   return validLIs.size();
}

void DataExtractDialog::extractData() {
   const vector<RecordInfo> recInfoList{ cRecInfoList };
   vector<LineItemInfo> validLIs{};

   PSCIFUNC_T sciFunc;
   void* sciPtr;

   if (getValidLineItems(validLIs, TRUE, TRUE) < 1) return;
   if (!getDirectScintillaFunc(sciFunc, sciPtr)) return;

   string fileType{};
   if (!_vizPanel.getDocFileType(fileType) || (initFileType != fileType)) {
      MessageBox(_hSelf, DATA_EXTRACT_CHANGED_DOC, DATA_EXTRACT_DIALOG_TITLE, MB_OK | MB_ICONSTOP);
      return;
   }

   const size_t lineCount{ static_cast<size_t>(sciFunc(sciPtr, SCI_GETLINECOUNT, NULL, NULL)) };
   if (lineCount < 1) return;

   const size_t regexedCount{ recInfoList.size() };

   string lineTextCStr(FW_LINE_MAX_LENGTH, '\0');
   string recStartText{}, eolMarker{};
   size_t eolMarkerLen, eolMarkerPos, recStartLine{}, startPos, endPos, recStartPos{};
   bool newRec{ TRUE };

   bool recMatch{};
   wstring extract{};

   string fieldText(FW_LINE_MAX_LENGTH, '\0');
   Sci_TextRange sciTR{};
   sciTR.lpstrText = fieldText.data();

   eolMarker = _configIO.getConfigStringA(fileType, "RecordTerminator");
   eolMarkerLen = eolMarker.length();

   bool byteCols{ !_configIO.getMultiByteLexing(fileType) };

   const size_t endLine{ lineCount };
   for (size_t currentLine{}; currentLine < endLine; ++currentLine) {
      if (sciFunc(sciPtr, SCI_LINELENGTH, currentLine, NULL) > FW_LINE_MAX_LENGTH) {
         continue;
      }

      sciFunc(sciPtr, SCI_GETLINE, (WPARAM)currentLine, (LPARAM)lineTextCStr.c_str());
      startPos = sciFunc(sciPtr, SCI_POSITIONFROMLINE, currentLine, NULL);
      endPos = sciFunc(sciPtr, SCI_GETLINEENDPOSITION, currentLine, NULL);
      string_view lineText{ lineTextCStr.c_str(), endPos - startPos};

      if (newRec) {
         recStartLine = currentLine;
         recStartPos = startPos;
         recStartText = lineText;
      }

      if (newRec && lineText.empty()) {
         continue;
      }

      if (eolMarkerLen == 0) {
         newRec = TRUE;
         eolMarkerPos = endPos;
      }
      else if (lineText.length() > eolMarkerLen &&
         (lineText.substr(lineText.length() - eolMarkerLen) == eolMarker)) {
         newRec = TRUE;
         eolMarkerPos = endPos - eolMarkerLen;
      }
      else if (currentLine < endLine) {
         newRec = FALSE;
         continue;
      }
      else {
         eolMarkerPos = endPos;
      }

      size_t regexIndex{};

      while (regexIndex < regexedCount) {
         if (regex_match(recStartText, recInfoList[regexIndex].regExpr)) break;
         ++regexIndex;
      }

      if (regexIndex >= regexedCount) continue;

      recMatch = FALSE;

      for (size_t j{}; j < validLIs.size(); ++j) {
         const LineItemInfo& LI{ validLIs[j] };
         const RecordInfo RI{ recInfoList[LI.recType] };
         if (static_cast<int>(regexIndex) != LI.recType) continue;

         if (byteCols) {
            sciTR.chrg.cpMin = static_cast<long>(recStartPos + RI.fieldStarts[LI.fieldType]);
            sciTR.chrg.cpMax = sciTR.chrg.cpMin + RI.fieldWidths[LI.fieldType];
         }
         else {
            sciTR.chrg.cpMin = static_cast<long>(sciFunc(sciPtr, SCI_POSITIONRELATIVE,
               (WPARAM)recStartPos, (LPARAM)RI.fieldStarts[LI.fieldType]));
            sciTR.chrg.cpMax = static_cast<long>(sciFunc(sciPtr, SCI_POSITIONRELATIVE,
               (WPARAM)sciTR.chrg.cpMin, (LPARAM)RI.fieldWidths[LI.fieldType]));
         }

         if (sciTR.chrg.cpMax > static_cast<long>(eolMarkerPos) || sciTR.chrg.cpMax == 0)
            sciTR.chrg.cpMax = static_cast<long>(eolMarkerPos);

         if (sciTR.chrg.cpMin < static_cast<long>(eolMarkerPos) &&
            (recStartPos + RI.fieldStarts[LI.fieldType] == 0 || sciTR.chrg.cpMin > 0)) {
            sciFunc(sciPtr, SCI_GETTEXTRANGE, NULL, (LPARAM)&sciTR);

            extract += validLIs[j].prefix + Utils::NarrowToWide(fieldText.c_str()) + validLIs[j].suffix;
            recMatch = TRUE;
         }
      }

      if (recMatch) extract += L"\n";
   }


   nppMessage(NPPM_MENUCOMMAND, NULL, IDM_FILE_NEW);
   sciFunc(sciPtr, SCI_SETTEXT, NULL, (LPARAM)(Utils::WideToNarrow(extract)).c_str());
}

int DataExtractDialog::loadTemplatesList() {
   resetDropDown(hTemplatesList);
   SetWindowText(hTemplateName, L"");

   bool currentOnly{ IsDlgButtonChecked(_hSelf, IDC_DAT_EXT_TEMPLATE_CURR_ONLY) == BST_CHECKED };

   int sectionCount{};
   vector<string> sectionList{};

   sectionCount = _configIO.getConfigAllSectionsList(sectionList, extractsConfigFile);

   for (int i{}; i < sectionCount; ++i) {
      string templateName{};
      bool matching{};

      matching = (initFileType == _configIO.getConfigStringA(sectionList[i], "FileType", "", extractsConfigFile));

      templateName = currentOnly ?
         (matching ? sectionList[i] : "") :
         (string{ matching ? "" : DATA_EXTRACT_TEMPLATE_OTHER } + sectionList[i]);

      if (!templateName.empty())
         SendMessage(hTemplatesList, CB_ADDSTRING, NULL, (LPARAM)Utils::NarrowToWide(templateName).c_str());
   }
   return sectionCount;
}

void DataExtractDialog::loadTemplate() {
   string templateName{ getSelectedTemplate() };

   if (templateName.length() <= 1) {
      newTemplate();
      return;
   }

   SetWindowText(hTemplateName, Utils::NarrowToWide(templateName).c_str());
   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_TEMPLATE_SAVE_BTN), TRUE);
   enableDeleteTemplate();

   int liCount{ _configIO.getConfigInt(templateName, "LineItemCount", 0, extractsConfigFile) };
   if (liCount < 1) {
      newTemplate();
      return;
   }

   int loadCount{ liCount <= MAX_BUFFER_LINES ? liCount : MAX_BUFFER_LINES };

   liBuffer.clear();
   liBuffer.resize(static_cast<size_t>(getPageCount(loadCount)) * 10);

   const vector<RecordInfo> recInfoList{ cRecInfoList };

   for (int i{}; i < loadCount; ++i) {
      LineItemInfo& LI{ liBuffer[i] };

      char tNum[4];
      snprintf(tNum, 4, "%02d_", i);
      string numSuffix{ tNum };

      LI.prefix = _configIO.getConfigWideChar(templateName, (numSuffix + "Prefix").c_str(), "", extractsConfigFile);
      LI.suffix = _configIO.getConfigWideChar(templateName, (numSuffix + "Suffix").c_str(), "", extractsConfigFile);

      LI.recType = _configIO.getConfigInt(templateName, (numSuffix + "Record").c_str(), 0, extractsConfigFile);
      if (LI.recType > static_cast<int>(recInfoList.size())) LI.recType = -1;

      if (LI.recType >= 0) {
         LI.fieldType = _configIO.getConfigInt(templateName, (numSuffix + "Field").c_str(), 0, extractsConfigFile);
         if (LI.fieldType > static_cast<int>(recInfoList[LI.recType].fieldLabels.size())) LI.fieldType = -1;
      }

      LI.recType++;
      LI.fieldType++;
   }

   loadPage(0);
}

string DataExtractDialog::getSelectedTemplate() {
   bool validTemplate{ SendMessage(hTemplatesList, CB_GETCURSEL, NULL, NULL) > 0 };
   string templateName{};

   if (validTemplate) {
      wchar_t tName[MAX_TEMPLATE_NAME + 1];
      GetWindowText(hTemplatesList, tName, MAX_TEMPLATE_NAME);

      templateName = Utils::WideToNarrow(tName);

      size_t otherLen = string{ DATA_EXTRACT_TEMPLATE_OTHER }.length();

      if (templateName.substr(0, otherLen) == DATA_EXTRACT_TEMPLATE_OTHER)
         templateName = templateName.substr(otherLen);
   }

   return templateName;
}

string DataExtractDialog::getTemplateName() {
   wchar_t tName[MAX_TEMPLATE_NAME + 1];
   GetWindowText(hTemplateName, tName, MAX_TEMPLATE_NAME);
   return Utils::WideToNarrow(tName);
}

void DataExtractDialog::enableSaveTemplate() {
   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_TEMPLATE_SAVE_BTN), getTemplateName().length() > 2);
}

void DataExtractDialog::enableDeleteTemplate() {
   int selected = static_cast<int>(SendMessage(hTemplatesList, CB_GETCURSEL, NULL, NULL));
   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_TEMPLATE_DEL_BTN), selected > 0);
}

void DataExtractDialog::saveTemplate() {
   vector<LineItemInfo> validLIs{};
   getValidLineItems(validLIs, FALSE, FALSE);

   string templateName{ getTemplateName() };
   if (templateName.length() < 3) return;

   // First, delete any existing section with the same name
   _configIO.deleteSection(templateName, extractsConfigFile);
   _configIO.setConfigStringA(templateName, "FileType", initFileType, extractsConfigFile);
   _configIO.setConfigMultiByte(templateName, "FileLabel", initFileTypeLabel, extractsConfigFile);
   _configIO.setConfigStringA(templateName, "LineItemCount", to_string(validLIs.size()), extractsConfigFile);

   for (size_t i{}; i < validLIs.size(); ++i) {
      LineItemInfo& LI = validLIs[i];

      char tNum[4];
      snprintf(tNum, 4, "%02d_", static_cast<int>(i));
      string numSuffix{ tNum };

      _configIO.setConfigMultiByte(templateName, numSuffix + "Prefix", LI.prefix, extractsConfigFile);
      _configIO.setConfigStringA(templateName, numSuffix + "Record", to_string(LI.recType), extractsConfigFile);
      _configIO.setConfigStringA(templateName, numSuffix + "Field", to_string(LI.fieldType), extractsConfigFile);
      _configIO.setConfigMultiByte(templateName, numSuffix + "Suffix", LI.suffix, extractsConfigFile);
   }

   wstring lbName{ Utils::NarrowToWide(templateName) };
   if (SendMessage(hTemplatesList, CB_FINDSTRING, (WPARAM)-1, (LPARAM)lbName.c_str()) == CB_ERR) {
      Utils::setComboBoxSelection(hTemplatesList,
         static_cast<int>(SendMessage(hTemplatesList, CB_ADDSTRING, NULL, (LPARAM)lbName.c_str())));
   }

   loadTemplate();
}

void DataExtractDialog::newTemplate() {
   liBuffer.clear();
   liBuffer.resize(LINES_PER_PAGE);
   loadPage(0);

   Utils::setComboBoxSelection(hTemplatesList, 0);
   SetWindowText(hTemplateName, L"");
   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_TEMPLATE_SAVE_BTN), FALSE);
   enableDeleteTemplate();
}

void DataExtractDialog::deleteTemplate() {
   if (MessageBox(_hSelf, DATA_EXTRACT_DELETE_PROMPT, DATA_EXTRACT_DIALOG_TITLE,
      MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
      return;

   _configIO.deleteSection(getTemplateName(), extractsConfigFile);

   SendMessage(hTemplatesList, CB_DELETESTRING,
      (WPARAM)SendMessage(hTemplatesList, CB_GETCURSEL, (WPARAM)0, NULL), NULL);

   newTemplate();
}

int DataExtractDialog::getPageCount(int items) {
   if (items == 0)
      items = static_cast<int>(liBuffer.size());

   return ((items - 1) / 10) + 1;
}

void DataExtractDialog::loadPage(int pageNum) {
   size_t seq{ static_cast<size_t>(pageNum * LINES_PER_PAGE) };

   if (liBuffer.size() <= seq) return;

   if (liBuffer.size() < (seq + LINES_PER_PAGE))
      liBuffer.resize(seq + LINES_PER_PAGE);

   currentPage = pageNum;
   enablePageButtons();

   for (int i{}; i < LINES_PER_PAGE; ++i) {
      setLineItem(i, liBuffer[seq]);
      SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_SEQ_01 + i, to_wstring(++seq).c_str());
   }

   moveIndicators(0, TRUE);
}

void DataExtractDialog::readPage() {
   size_t seq{ static_cast<size_t>(currentPage * LINES_PER_PAGE) };

   if (liBuffer.size() < (seq + LINES_PER_PAGE))
      liBuffer.resize(seq + LINES_PER_PAGE);

   for (int i{}; i < LINES_PER_PAGE; ++i) {
      getLineItem(i, liBuffer[seq + i]);
   }
}

void DataExtractDialog::previousPage() {
   if (currentPage > 0)
      readPage();
   loadPage(currentPage - 1);
}

void DataExtractDialog::nextPage() {
   if (currentPage < getPageCount() - 1) {
      readPage();
      loadPage(currentPage + 1);
   }
}

void DataExtractDialog::addPage() {
   const int pageCount{ getPageCount() };

   if (pageCount < MAX_PAGES) {
      readPage();
      liBuffer.resize(liBuffer.size() + LINES_PER_PAGE);
      loadPage(pageCount);
   }
}

void DataExtractDialog::deletePage() {
   if (currentPage > 0) {
      int start{ currentPage * 10 };
      liBuffer.erase(liBuffer.begin() + start, liBuffer.begin() + start + LINES_PER_PAGE);
      loadPage(currentPage - 1);
   }
}

void DataExtractDialog::enablePageButtons() {
   const int pageCount{ getPageCount() };

   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_ADD_BTN_10), pageCount < MAX_PAGES);
   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_PAGE_PREV_BUTTON), currentPage > 0);
   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_PAGE_NEXT_BUTTON), currentPage < pageCount - 1);
   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_PAGE_ADD_BUTTON), pageCount < MAX_PAGES);
   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_PAGE_DEL_BUTTON), currentPage > 0);
}

bool DataExtractDialog::processKey(HWND hCtrl, WPARAM wParam) {
   int ctrlID{ GetDlgCtrlID(hCtrl) };

   switch (wParam) {
   case VK_DOWN:
      if (Utils::checkKeyHeldDown(VK_CONTROL)) {
         swapLineItems(currentLineItem, currentLineItem + 1);
         return TRUE;
      }
      else {
         gotoLine(ctrlID, currentLineItem + 1);
         return TRUE;
      }
      break;

   case VK_UP:
      if (Utils::checkKeyHeldDown(VK_CONTROL)) {
         swapLineItems(currentLineItem, currentLineItem - 1);
         return TRUE;
      }
      else {
         gotoLine(ctrlID, currentLineItem - 1);
         return TRUE;
      }
      break;

   case VK_HOME:
      if (Utils::checkKeyHeldDown(VK_CONTROL)) {
         SetFocus(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01));
         return TRUE;
      }
      break;

   case VK_END:
      if (Utils::checkKeyHeldDown(VK_CONTROL)) {
         SetFocus(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + LINES_PER_PAGE - 1));
         return TRUE;
      }
      return TRUE;
      break;

   case VK_PRIOR:
      if (Utils::checkKeyHeldDown(VK_CONTROL)) {
         previousPage();
      }
      else {
         SetFocus(GetDlgItem(_hSelf, ctrlID - currentLineItem));
      }
      return TRUE;
      break;

   case VK_NEXT:
      if (Utils::checkKeyHeldDown(VK_CONTROL)) {
         nextPage();
      }
      else {
         SetFocus(GetDlgItem(_hSelf, ctrlID - currentLineItem + LINES_PER_PAGE - 1));
      }
      return TRUE;
      break;

   case VK_ADD:
   case VK_OEM_PLUS:
   case VK_INSERT:
      if (Utils::checkKeyHeldDown(VK_CONTROL) && Utils::checkKeyHeldDown(VK_SHIFT)) {
         addPage();
         return TRUE;
      }
      else if (Utils::checkKeyHeldDown(VK_CONTROL)) {
         addLineItem(currentLineItem);
         return TRUE;
      }
      break;

   case VK_SUBTRACT:
   case VK_OEM_MINUS:
   case VK_DELETE:
      if (Utils::checkKeyHeldDown(VK_CONTROL) && Utils::checkKeyHeldDown(VK_SHIFT)) {
         deletePage();
         return TRUE;
      }
      else if (Utils::checkKeyHeldDown(VK_CONTROL)) {
         delLineItem(currentLineItem);
         return TRUE;
      }
      break;
   }

   return FALSE;
}

bool DataExtractDialog::processSysKey(HWND, WPARAM wParam) {
   switch (wParam) {
   case VK_HOME:
      if (Utils::checkKeyHeldDown(VK_MENU)) {
         SetFocus(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + currentLineItem));
         return TRUE;
      }
      break;

   case VK_END:
      if (Utils::checkKeyHeldDown(VK_MENU)) {
         SetFocus(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + currentLineItem));
         return TRUE;
      }
      break;
   }

   return FALSE;
}

