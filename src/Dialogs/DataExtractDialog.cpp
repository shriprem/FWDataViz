#include "DataExtractDialog.h"

extern VisualizerPanel _vizPanel;

void DataExtractDialog::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_DATA_EXTRACT_DIALOG);
   }

   hIndicator = GetDlgItem(_hSelf, IDC_DAT_EXT_CURRENT_LINE);
   SendDlgItemMessage(_hSelf, IDC_DAT_EXT_TEMPLATE_NAME, EM_LIMITTEXT, (WPARAM)MAX_TEMPLATE_NAME, NULL);

   for (int i{}; i < LINE_ITEM_COUNT; i++) {
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
   extractsConfigFile = _configIO.getExtractTemplatesFile();

   initLineItems();
   loadTemplatesList();
   moveIndicators(0, TRUE);
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

            case IDC_DAT_EXT_DOWN_BUTTON:
               if (currentLineItem < LINE_ITEM_COUNT - 1)
                  swapLineItems(currentLineItem, currentLineItem + 1);
               break;

            case IDC_DAT_EXT_UP_BUTTON:
               if (currentLineItem > 0)
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

      case WM_CTLCOLORSTATIC:
         switch (GetDlgCtrlID((HWND)lParam)) {
            case IDC_DAT_EXT_CURRENT_LINE:
               SetTextColor((HDC)wParam, GetSysColor(COLOR_HIGHLIGHT));
               return (INT_PTR)GetSysColorBrush(COLOR_BTNFACE);

            default:
               return NULL;
         }
      }

   return FALSE;
}

void DataExtractDialog::localize() {
   SetWindowText(_hSelf, DATA_EXTRACT_DIALOG_TITLE);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_PREFIX_LABEL, DATA_EXTRACT_PREFIX_LABEL);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_RECORD_LABEL, DATA_EXTRACT_RECORD_LABEL);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_FIELD_LABEL, DATA_EXTRACT_FIELD_LABEL);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_SUFFIX_LABEL, DATA_EXTRACT_SUFFIX_LABEL);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_NEW_LINE_TAB_TIP, DATA_EXTRACT_NEW_LINE_TAB);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_EXTRACT_BTN, DATA_EXTRACT_EXTRACT_BTN);
   SetDlgItemText(_hSelf, IDCLOSE, DATA_EXTRACT_CLOSE_BTN);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_GROUP, DATA_EXTRACT_TEMPLATE_GROUP);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_LIST_LABEL, DATA_EXTRACT_TEMPLATE_LOAD);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_NAME_LABEL, DATA_EXTRACT_TEMPLATE_NAME);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_SAVE_BTN, DATA_EXTRACT_TEMPLATE_SAVE);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_NEW_BTN, DATA_EXTRACT_TEMPLATE_NEW);
   SetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_DEL_BTN, DATA_EXTRACT_TEMPLATE_DEL);
}

void DataExtractDialog::initLineItems() {
   const vector<RecordInfo> recInfoList{ *pRecInfoList };

   for (int i{}; i < LINE_ITEM_COUNT; i++) {
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

void DataExtractDialog::initLineItemFieldList(int line) {
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

void DataExtractDialog::moveIndicators(int line, bool focusPrefix) {
   currentLineItem = line;

   RECT rc;
   GetWindowRect(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_DEL_BTN_01 + line), &rc);

   POINT newPos{ rc.right + 2, rc.top + 2 };
   ScreenToClient(_hSelf, &newPos);

   SetWindowPos(hIndicator, HWND_TOP, newPos.x, newPos.y, NULL, NULL, SWP_NOSIZE | SWP_NOOWNERZORDER);

   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_DOWN_BUTTON), line < LINE_ITEM_COUNT - 1);
   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_UP_BUTTON), line > 0);

   if (focusPrefix)
      SetFocus(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + line));
}

void DataExtractDialog::resetDropDown(HWND hList) {
   SendMessage(hList, CB_RESETCONTENT, NULL, NULL);
   SendMessage(hList, CB_ADDSTRING, NULL, (LPARAM)L"-");
   SendMessage(hList, CB_SETCURSEL, (WPARAM)0, NULL);
}

void DataExtractDialog::addLineItem(int line) {
   for (int i{ LINE_ITEM_COUNT }; i > line; i--) {
      copyLineItem(i - 1, i);
   }
   clearLineItem(line);

   moveIndicators(line, TRUE);
}

void DataExtractDialog::delLineItem(int line) {
   for (int i{ line }; i < LINE_ITEM_COUNT; i++) {
      copyLineItem(i + 1, i);
   }
   clearLineItem(LINE_ITEM_COUNT);

   moveIndicators(line, TRUE);
}

void DataExtractDialog::clearLineItem(int line) {
   SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + line, L"");
   SetDlgItemText(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + line, L"");
   SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + line, CB_SETCURSEL, (WPARAM)0, NULL);
   resetDropDown(GetDlgItem(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + line));
}

void DataExtractDialog::copyLineItem(int fromLine, int toLine) {
   char prefix[MAX_PATH + 1];
   GetDlgItemTextA(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + fromLine, prefix, MAX_PATH);
   SetDlgItemTextA(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + toLine, prefix);

   char suffix[MAX_PATH + 1];
   GetDlgItemTextA(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + fromLine, suffix, MAX_PATH);
   SetDlgItemTextA(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + toLine, suffix);

   SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + toLine, CB_SETCURSEL,
      SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + fromLine, CB_GETCURSEL, NULL, NULL), NULL);

   initLineItemFieldList(toLine);
   SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + toLine, CB_SETCURSEL,
      SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + fromLine, CB_GETCURSEL, NULL, NULL), NULL);
}

bool DataExtractDialog::getLineItem(int line, LineItemInfo& lineItem) {
   char prefix[MAX_PATH + 1];
   GetDlgItemTextA(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + line, prefix, MAX_PATH);
   lineItem.prefix = string{ prefix };

   char suffix[MAX_PATH + 1];
   GetDlgItemTextA(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + line, suffix, MAX_PATH);
   lineItem.suffix = string{ suffix };

   lineItem.recType = static_cast<int>(
      SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + line, CB_GETCURSEL, NULL, NULL));

   lineItem.fieldType = static_cast<int>(
      SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + line, CB_GETCURSEL, NULL, NULL));

   return lineItem.fieldType > 0;
}

void DataExtractDialog::setLineItem(int line, LineItemInfo& lineItem) {
   SetDlgItemTextA(_hSelf, IDC_DAT_EXT_ITEM_PREFIX_01 + line, lineItem.prefix.c_str());
   SetDlgItemTextA(_hSelf, IDC_DAT_EXT_ITEM_SUFFIX_01 + line, lineItem.suffix.c_str());
   SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_RECORD_01 + line, CB_SETCURSEL, lineItem.recType, NULL);
   initLineItemFieldList(line);
   SendDlgItemMessage(_hSelf, IDC_DAT_EXT_ITEM_FIELD_01 + line, CB_SETCURSEL, lineItem.fieldType, NULL);
}

void DataExtractDialog::swapLineItems(int lineFrom, int lineTo) {
   LineItemInfo toItem;

   getLineItem(lineTo, toItem);
   copyLineItem(lineFrom, lineTo);
   setLineItem(lineFrom, toItem);
   moveIndicators(lineTo, TRUE);
}

size_t DataExtractDialog::getReconciledLineItems(bool activateNLT) {
   LineItemInfo lineInfo;

   validLineItems.clear();

   for (int i{}; i < LINE_ITEM_COUNT; i++) {
      if (getLineItem(i, lineInfo)) {
         // Decrement both recType & fieldType by one to account for the first "-" item in their dropdowns
         lineInfo.recType--;
         lineInfo.fieldType--;

         if (activateNLT) {
            _configIO.ActivateNewLineTabs(lineInfo.prefix);
            _configIO.ActivateNewLineTabs(lineInfo.suffix);
         }

         validLineItems.emplace_back(lineInfo);
      }
   }

   return validLineItems.size();
}

void DataExtractDialog::extractData() {
   const vector<RecordInfo> recInfoList{ *pRecInfoList };
   PSCIFUNC_T sciFunc;
   void* sciPtr;

   if (getReconciledLineItems(TRUE) < 1) return;
   if (!getDirectScintillaFunc(sciFunc, sciPtr)) return;

   wstring fileType{};
   if (!_vizPanel.getDocFileType(sciFunc, sciPtr, fileType) || (initDocFileType != fileType)) {
      MessageBox(_hSelf, DATA_EXTRACT_CHANGED_DOC, DATA_EXTRACT_DIALOG_TITLE, MB_OK | MB_ICONSTOP);
      return;
   }

   const size_t lineCount{ static_cast<size_t>(sciFunc(sciPtr, SCI_GETLINECOUNT, NULL, NULL)) };
   if (lineCount < 1) return;

   const size_t regexedCount{ recInfoList.size() };

   char lineTextCStr[FW_LINE_MAX_LENGTH]{};
   string extract{}, recStartText{}, eolMarker;

   char fieldText[FW_LINE_MAX_LENGTH]{};
   Sci_TextRange sciTR{};
   sciTR.lpstrText = fieldText;

   size_t eolMarkerLen, eolMarkerPos, recStartLine{}, currentPos, startPos, endPos, recStartPos{};
   bool newRec{ TRUE };
   bool recMatch{};

   eolMarker = _configIO.getConfigStringA(fileType, L"RecordTerminator");
   eolMarkerLen = eolMarker.length();

   const size_t endLine{ lineCount };
   for (size_t currentLine{}; currentLine < endLine; currentLine++) {
      if (sciFunc(sciPtr, SCI_LINELENGTH, currentLine, NULL) > FW_LINE_MAX_LENGTH) {
         continue;
      }

      sciFunc(sciPtr, SCI_GETLINE, (WPARAM)currentLine, (LPARAM)lineTextCStr);
      startPos = sciFunc(sciPtr, SCI_POSITIONFROMLINE, currentLine, NULL);
      endPos = sciFunc(sciPtr, SCI_GETLINEENDPOSITION, currentLine, NULL);
      string_view lineText{ lineTextCStr, endPos - startPos };

      if (newRec) {
         recStartLine = currentLine;
         recStartPos = startPos;
         recStartText = lineText;
      }

      if (newRec && lineText.length() == 0) {
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

      currentPos = recStartPos;

      size_t regexIndex{};

      while (regexIndex < regexedCount) {
         if (regex_match(recStartText, recInfoList[regexIndex].regExpr)) break;
         regexIndex++;
      }

      if (regexIndex >= regexedCount) continue;

      recMatch = FALSE;

      for (size_t j{}; j < validLineItems.size(); j++) {
         LineItemInfo& LI = validLineItems[j];
         if (regexIndex != LI.recType) continue;

         sciTR.chrg.cpMin = static_cast<long>(recStartPos + recInfoList[LI.recType].fieldStarts[LI.fieldType]);
         sciTR.chrg.cpMax = sciTR.chrg.cpMin + static_cast<long>(recInfoList[LI.recType].fieldWidths[LI.fieldType]);
         sciFunc(sciPtr, SCI_GETTEXTRANGE, NULL, (LPARAM)&sciTR);

         extract += validLineItems[j].prefix + fieldText + validLineItems[j].suffix;
         recMatch = TRUE;
      }

      if (recMatch) extract += "\n";
   }

   nppMessage(NPPM_MENUCOMMAND, NULL, IDM_FILE_NEW);
   sciFunc(sciPtr, SCI_SETTEXT, NULL, (LPARAM)extract.c_str());
}

int DataExtractDialog::loadTemplatesList(){
   const vector<RecordInfo> recInfoList{ *pRecInfoList };

   HWND hTemplatesList = GetDlgItem(_hSelf, IDC_DAT_EXT_TEMPLATE_LIST);
   resetDropDown(hTemplatesList);
   for (size_t j{}; j < recInfoList.size(); j++) {
      SendMessage(hTemplatesList, CB_ADDSTRING, NULL, (LPARAM)recInfoList[j].label.c_str());
   }
   return 0;
}

void DataExtractDialog::loadTemplate() {
   enableDeleteTemplate();
}

void DataExtractDialog::enableSaveTemplate() {
   wchar_t name[MAX_TEMPLATE_NAME + 1];
   GetDlgItemText(_hSelf, IDC_DAT_EXT_TEMPLATE_NAME, name, MAX_TEMPLATE_NAME);
   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_TEMPLATE_SAVE_BTN), wstring(name).length() > 2);
}

void DataExtractDialog::enableDeleteTemplate() {
   int selected = static_cast<int>(SendDlgItemMessage(_hSelf, IDC_DAT_EXT_TEMPLATE_LIST, CB_GETCURSEL, NULL, NULL));
   EnableWindow(GetDlgItem(_hSelf, IDC_DAT_EXT_TEMPLATE_DEL_BTN), selected > 0);
}

void DataExtractDialog::saveTemplate() {
   MessageBox(_hSelf, extractsConfigFile.c_str(), L"Saving Template", 0);
}

void DataExtractDialog::newTemplate() {
   MessageBox(_hSelf, L"New Template", L"", 0);
}

void DataExtractDialog::deleteTemplate() {
   if (MessageBox(_hSelf, DATA_EXTRACT_DELETE_PROMPT, DATA_EXTRACT_DIALOG_TITLE,
      MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
      return;

   MessageBox(_hSelf, L"Deleting Template", L"", 0);
}

