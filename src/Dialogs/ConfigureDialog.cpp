#include "ConfigureDialog.h"

extern ConfigureDialog _configDlg;

LRESULT CALLBACK procFieldEditMessages(HWND hwnd, UINT messageId, WPARAM wParam,
   LPARAM lParam, UINT_PTR, DWORD_PTR) {

   HWND hThis{ hwnd == _configDlg.hEditLabels ? _configDlg.hEditLabels : _configDlg.hEditWidths };
   HWND hThat{ hwnd == _configDlg.hEditLabels ? _configDlg.hEditWidths : _configDlg.hEditLabels };

   switch (messageId) {
      case WM_CHAR:
         if (wParam == ',' && hwnd == _configDlg.hEditLabels) {
            EDITBALLOONTIP tip;
            tip.cbStruct = sizeof(tip);
            tip.pszTitle = L"Commas are item separators";
            tip.pszText = L"The line will be split into multiple items when the 'Accept' button below is clicked.";
            tip.ttiIcon = TTI_ERROR;
            SendMessage(hwnd, EM_SHOWBALLOONTIP, NULL, (LPARAM)&tip);
            MessageBeep(MB_OK);
         }
         break;

      case WM_KEYDOWN:
      case WM_KEYUP:
      case WM_LBUTTONUP:
         _configDlg.hiliteFieldEditPairedItem(hThis, hThat);
         break;

      case WM_VSCROLL:
         _configDlg.syncFieldEditScrolling(hThis, hThat);
         break;
   }

   return DefSubclassProc(hwnd, messageId, wParam, lParam);
}


void ConfigureDialog::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_FWVIZ_DEFINER_DIALOG);
   }

   hListFiles = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_LIST_BOX);
   hListRecs = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_LIST_BOX);
   hEditLabels = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FIELD_LABELS_EDIT);
   hEditWidths = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FIELD_WIDTHS_EDIT);

   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_DESC_EDIT, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);
   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_TERM_EDIT, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);
   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_REC_DESC_EDIT, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);
   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_REC_START_EDIT, EM_LIMITTEXT, (WPARAM)(MAX_PATH - 1), NULL);
   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_REC_REGEX_EDIT, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);

   SendMessage(hEditLabels, EM_LIMITTEXT, (WPARAM)FW_LINE_MAX_LENGTH, NULL);
   SendMessage(hEditWidths, EM_LIMITTEXT, (WPARAM)FW_LINE_MAX_LENGTH, NULL);

   SetWindowSubclass(hEditLabels, procFieldEditMessages, NULL, NULL);
   SetWindowSubclass(hEditWidths, procFieldEditMessages, NULL, NULL);

   Utils::setFontBold(_hSelf, IDOK);

   if (_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);

   loadConfigInfo();
   fillFileTypes();
}

INT_PTR CALLBACK ConfigureDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM) {
   switch (message) {
      case WM_COMMAND:
         switch LOWORD(wParam) {
            case IDCANCEL:
            case IDOK:
               display(FALSE);
               return TRUE;

            case IDC_FWVIZ_DEF_FILE_LIST_BOX:
               switch HIWORD(wParam) {
                  case LBN_SELCHANGE:
                     onFileTypeSelect();
                     break;
               }
               break;

            case IDC_FWVIZ_DEF_REC_LIST_BOX:
               switch HIWORD(wParam) {
                  case LBN_SELCHANGE:
                     onRecTypeSelect();
                     break;
               }
               break;

            case IDC_FWVIZ_DEF_FIELD_LABELS_EDIT:
               switch HIWORD(wParam) {
                  case EN_SETFOCUS:
                     setFieldEditCaretOnFocus(hEditLabels);
                     break;

                  case EN_VSCROLL:
                     if (GetFocus() == hEditLabels) {
                        syncFieldEditScrolling(hEditLabels, hEditWidths);
                     }
               }
               break;

            case IDC_FWVIZ_DEF_FIELD_WIDTHS_EDIT:
               switch HIWORD(wParam) {
                  case EN_SETFOCUS:
                     setFieldEditCaretOnFocus(hEditWidths);
                     break;

                  case EN_VSCROLL:
                     if (GetFocus() == hEditWidths) {
                        syncFieldEditScrolling(hEditWidths, hEditLabels);
                     }
               }
               break;

            case IDC_FWVIZ_DEF_FIELD_ACCEPT_BTN:
               fieldEditsAccept();
               break;

            case IDC_FWVIZ_DEF_FIELD_RESET_BTN:
               fieldEditsReset();
               break;

            case IDC_FWVIZ_DEF_REC_ACCEPT_BTN:
               recEditAccept();
               break;

            case IDC_FWVIZ_DEF_REC_NEW_BTN:
               recEditNew();
               break;

            case IDC_FWVIZ_DEF_REC_DEL_BTN:
               recEditDelete();
               break;

            case IDC_FWVIZ_DEF_FILE_ACCEPT_BTN:
               fileEditAccept();
               break;

            case IDC_FWVIZ_DEF_FILE_NEW_BTN:
               fileEditNew();
               break;

            case IDC_FWVIZ_DEF_FILE_DEL_BTN:
               fileEditDelete();
               break;
         }
   }

   return FALSE;
}

void ConfigureDialog::localize() {
   SetWindowText(_hSelf, FWVIZ_DIALOG_TITLE);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_GROUP_BOX, FWVIZ_DEF_FILE_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_DESC_LABEL, FWVIZ_DEF_FILE_DESC_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_TERM_LABEL, FWVIZ_DEF_FILE_TERM_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LABEL, FWVIZ_DEF_FILE_THEME_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_NEW_BTN, FWVIZ_DEF_FILE_NEW_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_ACCEPT_BTN, FWVIZ_DEF_FILE_ACCEPT_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_DEL_BTN, FWVIZ_DEF_FILE_DEL_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_GROUP_BOX, FWVIZ_DEF_REC_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_DESC_LABEL, FWVIZ_DEF_REC_DESC_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_START_LABEL, FWVIZ_DEF_REC_START_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_REGEX_LABEL, FWVIZ_DEF_REC_REGEX_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_NEW_BTN, FWVIZ_DEF_REC_NEW_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_ACCEPT_BTN, FWVIZ_DEF_REC_ACCEPT_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_DEL_BTN, FWVIZ_DEF_REC_DEL_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_GROUP_BOX, FWVIZ_DEF_FIELD_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_WIDTHS_TEXT, FWVIZ_DEF_FIELD_WIDTHS_TEXT);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_LABELS_TEXT, FWVIZ_DEF_FIELD_LABELS_TEXT);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_ACCEPT_BTN, FWVIZ_DEF_FIELD_ACCEPT_BTN);
   SetDlgItemText(_hSelf, IDOK, FWVIZ_DIALOG_SAVE_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_RESET_BTN, FWVIZ_DIALOG_RESET_BTN);
   SetDlgItemText(_hSelf, IDCANCEL, FWVIZ_DIALOG_CLOSE_BTN);
}

int ConfigureDialog::loadConfigInfo() {
   vector<wstring> fileTypes;
   wstring fileTypeList;
   int fileTypeCount;

   fileTypeList = _configIO.getConfigString(L"Base", L"FileTypes");
   fileTypeCount = _configIO.Tokenize(fileTypeList, fileTypes);

   fileInfoList.clear();
   fileInfoList.resize(fileTypeCount);

   for (int i{}; i < fileTypeCount; i++) {
      wstring &fileType = fileTypes[i];
      FileInfo &FILE = fileInfoList[i];

      FILE.label = _configIO.getConfigString(fileType.c_str(), L"FileLabel");
      FILE.eol = _configIO.getConfigStringA(fileType.c_str(), L"RecordTerminator");
      FILE.theme = _configIO.getConfigString(fileType.c_str(), L"FileTheme");

      vector<wstring> recTypes;
      wstring recTypeList;
      int recTypeCount;

      recTypeList = _configIO.getConfigString(fileType.c_str(), L"RecordTypes");
      recTypeCount = _configIO.Tokenize(recTypeList, recTypes);

      FILE.records.clear();
      FILE.records.resize(recTypeCount);

      for (int j{}; j < recTypeCount; j++) {
         wstring &recType = recTypes[j];
         RecordInfo &REC = FILE.records[j];

         REC.label = _configIO.getConfigString(fileType.c_str(), (recType + L"_Label").c_str());
         REC.marker = _configIO.getConfigStringA(fileType.c_str(), (recType + L"_Marker").c_str());
         REC.fieldWidths = _configIO.getConfigString(fileType.c_str(), (recType + L"_FieldWidths").c_str());
         REC.fieldLabels = _configIO.getConfigString(fileType.c_str(), (recType + L"_FieldLabels").c_str());
      }
   }

   return static_cast<int>(fileInfoList.size());
}

void ConfigureDialog::fillFileTypes() {
   // Fill File Types Listbox
   size_t fTypes;

   SendMessage(hListFiles, LB_RESETCONTENT, NULL, NULL);

   fTypes = fileInfoList.size();
   for (size_t i{}; i < fTypes; i++) {
      SendMessage(hListFiles, LB_ADDSTRING, NULL, (LPARAM)fileInfoList[i].label.c_str());
   }

   // Fill Themes Droplist
   wstring themes;
   vector<wstring> themesList;
   int themesCount;

   themes = _configIO.getConfigString(L"Base", L"Themes");
   themesCount = _configIO.Tokenize(themes, themesList);

   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LIST, CB_RESETCONTENT, NULL, NULL);

   for (int i{}; i < themesCount; i++) {
      SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LIST, CB_ADDSTRING, NULL, (LPARAM)themesList[i].c_str());
   }

   // Select first item
   if (fTypes > 0) {
      SendMessage(hListFiles, LB_SETCURSEL, 0, NULL);
      onFileTypeSelect();
   }
}

int ConfigureDialog::getCurrentFileIndex() {
   int idxFile;

   idxFile = static_cast<int>(SendMessage(hListFiles, LB_GETCURSEL, NULL, NULL));
   if (idxFile == LB_ERR) return LB_ERR;

   return idxFile;
}

int ConfigureDialog::getCurrentRecIndex() {
   int idxRec;

   idxRec = static_cast<int>(SendMessage(hListRecs, LB_GETCURSEL, NULL, NULL));
   if (idxRec == LB_ERR) return LB_ERR;

   return idxRec;
}

bool ConfigureDialog::getCurrentFileInfo(FileInfo* &fileInfo) {
   int idxFile{ getCurrentFileIndex() };
   if (idxFile == LB_ERR) return FALSE;

   fileInfo = &fileInfoList[idxFile];
   return TRUE;
}

ConfigureDialog::FileInfo ConfigureDialog::getNewFile() {
   FileInfo newFile;

   newFile.label = L"";
   newFile.eol = "";
   newFile.theme = L"VT_Basic";
   newFile.records = vector<RecordInfo>{ getNewRec() };

   return newFile;
}

bool ConfigureDialog::getCurrentRecInfo(RecordInfo*& recInfo) {
   int idxFile{ getCurrentFileIndex() };
   if (idxFile == LB_ERR) return FALSE;

   int idxRec{ getCurrentRecIndex() };
   if (idxRec == LB_ERR) return FALSE;

   recInfo = &fileInfoList[idxFile].records[idxRec];
   return TRUE;
}

ConfigureDialog::RecordInfo ConfigureDialog::getNewRec() {
   RecordInfo newRec;

   newRec.label = L"";
   newRec.marker = "";
   newRec.fieldLabels = L"";
   newRec.fieldWidths = L"";
   return newRec;
}

void ConfigureDialog::onFileTypeSelect() {
   FileInfo* fileInfo;

   if (!getCurrentFileInfo(fileInfo)) {
      FileInfo newFile{ getNewFile() };
      fileInfo = &newFile;
   }

   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_DESC_EDIT, fileInfo->label.c_str());
   SetDlgItemTextA(_hSelf, IDC_FWVIZ_DEF_FILE_TERM_EDIT, fileInfo->eol.c_str());

   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LIST, CB_SETCURSEL, (WPARAM)
      SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LIST, CB_FINDSTRING, (WPARAM)-1,
         (LPARAM)fileInfo->theme.c_str()), NULL);

   fillRecTypes();
}

void ConfigureDialog::fillRecTypes() {
   FileInfo* fileInfo;

   if (!getCurrentFileInfo(fileInfo)) {
      FileInfo newFile{ getNewFile() };
      fileInfo = &newFile;
   }

   vector <RecordInfo> &recInfoList = fileInfo->records;

   // Fill Rec Types Listbox
   size_t recTypes;

   SendMessage(hListRecs, LB_RESETCONTENT, NULL, NULL);

   recTypes = recInfoList.size();

   for (size_t i{}; i < recTypes; i++) {
      SendMessage(hListRecs, LB_ADDSTRING, NULL, (LPARAM)recInfoList[i].label.c_str());
   }

   // Select first item
   if (recTypes > 0) {
      SendMessage(hListRecs, LB_SETCURSEL, 0, NULL);
   }

   onRecTypeSelect();
}

void ConfigureDialog::onRecTypeSelect() {
   RecordInfo *recInfo;

   if (!getCurrentRecInfo(recInfo)) {
      RecordInfo newRec{ getNewRec() };
      recInfo = &newRec;
   }

   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_DESC_EDIT, recInfo->label.c_str());

   string regExpr = recInfo->marker;

   SetDlgItemTextA(_hSelf, IDC_FWVIZ_DEF_REC_REGEX_EDIT, regExpr.c_str());
   SetDlgItemTextA(_hSelf, IDC_FWVIZ_DEF_REC_START_EDIT,
      (regExpr.substr(0, 1) == "^") ? regExpr.substr(1).c_str() : "");

   fillFieldTypes();
}

void ConfigureDialog::fillFieldTypes() {
   RecordInfo *recInfo;

   if (!getCurrentRecInfo(recInfo)) {
      RecordInfo newRec{ getNewRec() };
      recInfo = &newRec;
   }

   // Field Labels
   wstring fieldLabels{ regex_replace(recInfo->fieldLabels, wregex(L","), L"\r\n") };
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_LABELS_EDIT, fieldLabels.c_str());

   // Field Widths
   wstring fieldWidths{ regex_replace(recInfo->fieldWidths, wregex(L","), L"\r\n") };
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_WIDTHS_EDIT, fieldWidths.c_str());
}

void ConfigureDialog::setFieldEditCaretOnFocus(HWND hEdit) {
   DWORD startPos{}, endPos{};
   SendMessage(hEdit, EM_GETSEL, (WPARAM)&startPos, (LPARAM)&endPos);

   if (GetWindowTextLength(hEdit) == static_cast<int>(endPos) - static_cast<int>(startPos)) {
      int caretPos = (hEdit == hEditLabels) ? editLabelsCaret : editWidthsCaret;
      SendMessage(hEdit, EM_SETSEL, (WPARAM)caretPos, (LPARAM)caretPos);
      SendMessage(hEdit, EM_SCROLLCARET, NULL, NULL);
   }

   hiliteFieldEditPairedItem(hEdit, (hEdit == hEditLabels) ? hEditWidths : hEditLabels);
}

void ConfigureDialog::hiliteFieldEditPairedItem(HWND hThis, HWND hThat) {
   int thisLine = static_cast<int>(SendMessage(hThis, EM_LINEFROMCHAR,
      (WPARAM) SendMessage(hThis, EM_LINEINDEX, (WPARAM)-1, NULL), NULL));

   int thatLineCount = static_cast<int>(SendMessage(hThat, EM_GETLINECOUNT, NULL, NULL));
   if (thisLine >= thatLineCount) return;

   int lineStart = static_cast<int>(SendMessage(hThat, EM_LINEINDEX, (WPARAM)thisLine, NULL));
   int lineLength = static_cast<int>(SendMessage(hThat, EM_LINELENGTH, (WPARAM)lineStart, NULL));

   ((hThis == hEditLabels) ? editWidthsCaret : editLabelsCaret) = lineStart;

   SendMessage(hThat, EM_SETSEL, (WPARAM)lineStart, (LPARAM)(lineStart + lineLength));
   SendMessage(hThat, EM_SCROLLCARET, NULL, NULL);
}

void ConfigureDialog::syncFieldEditScrolling(HWND hThis, HWND hThat) {
   int thisLine = static_cast<int>(SendMessage(hThis, EM_GETFIRSTVISIBLELINE, NULL, NULL));

   int thatLineCount = static_cast<int>(SendMessage(hThat, EM_GETLINECOUNT, NULL, NULL));
   if (thisLine >= thatLineCount) return;

   int thatLine = static_cast<int>(SendMessage(hThat, EM_GETFIRSTVISIBLELINE, NULL, NULL));

   SendMessage(hThat, EM_LINESCROLL, NULL, thisLine - thatLine);
}


void ConfigureDialog::fieldEditsAccept() {
   RecordInfo *recInfo;

   if (!getCurrentRecInfo(recInfo)) return;

   wchar_t fieldValues[FW_LINE_MAX_LENGTH + 1];
   wstring vals{};

   // Field Labels
   GetWindowText(hEditLabels, fieldValues, (FW_LINE_MAX_LENGTH + 1));

   // Replace any trailing spaces + newlines with commas
   vals = regex_replace(fieldValues, wregex(L" *\r\n"), L",");

   // Replace any leading spaces + commas with commas
   vals = regex_replace(vals, wregex(L", +([^,]*)"), L",$1");

   // Trim any leading & trailing spaces + commas for the entire string
   vals = regex_replace(vals, wregex(L"^ +(.*[^ ]) +$"), L"$1");

   recInfo->fieldLabels = vals;
   SetWindowText(hEditLabels, regex_replace(vals, wregex(L","), L"\r\n").c_str());

   // Field Widths
   GetWindowText(hEditWidths, fieldValues, (FW_LINE_MAX_LENGTH + 1));

   // Replace any newlines with commas.
   // No processing needed for leading & trailing spaces since this is a numeric edit control
   vals = regex_replace(fieldValues, wregex(L"\r\n"), L",");

   recInfo->fieldWidths = vals;
}

void ConfigureDialog::fieldEditsReset() {
   fillFieldTypes();
}

void ConfigureDialog::recEditAccept() {
   int idxFile{ getCurrentFileIndex() };
   if (idxFile == LB_ERR) return;

   int idxRec{ getCurrentRecIndex() };
   if (idxRec == LB_ERR) return;

   RecordInfo &recInfo = fileInfoList[idxFile].records[idxRec];

   wchar_t recDesc[MAX_PATH + 1];

   GetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_DESC_EDIT, recDesc, MAX_PATH + 1);
   recInfo.label = recDesc;

   char regexVal[MAX_PATH + 1];

   GetDlgItemTextA(_hSelf, IDC_FWVIZ_DEF_REC_REGEX_EDIT, regexVal, MAX_PATH + 1);
   recInfo.marker = regexVal;

   SendMessage(hListRecs, LB_DELETESTRING, (WPARAM)idxRec, NULL);
   SendMessage(hListRecs, LB_INSERTSTRING, (WPARAM)idxRec, (LPARAM)recDesc);
   SendMessage(hListRecs, LB_SETCURSEL, idxRec, NULL);
}

void ConfigureDialog::recEditNew() {
   int idxFile{ getCurrentFileIndex() };
   if (idxFile == LB_ERR) return;

   RecordInfo newRec{ getNewRec() };
   vector<RecordInfo> &records = fileInfoList[idxFile].records;

   records.push_back(newRec);

   size_t moveTo = records.size() - 1;

   SendMessage(hListRecs, LB_ADDSTRING, NULL, (LPARAM)newRec.label.c_str());
   SendMessage(hListRecs, LB_SETCURSEL, (WPARAM)moveTo, NULL);
   onRecTypeSelect();
}

int ConfigureDialog::recEditDelete() {
   int idxFile{ getCurrentFileIndex() };
   if (idxFile == LB_ERR) return LB_ERR;

   int idxRec{ getCurrentRecIndex() };
   if (idxRec == LB_ERR) return LB_ERR;

   vector<RecordInfo> &records = fileInfoList[idxFile].records;
   records.erase(records.begin() + idxRec);

   int lastRec = static_cast<int>(records.size()) - 1;
   int moveTo = (idxRec <= lastRec - 1) ? idxRec : lastRec;

   SendMessage(hListRecs, LB_DELETESTRING, (WPARAM)idxRec, NULL);
   SendMessage(hListRecs, LB_SETCURSEL, (WPARAM)moveTo, NULL);
   onRecTypeSelect();

   return moveTo;
}

void ConfigureDialog::fileEditAccept() {
   int idxFile{ getCurrentFileIndex() };
   if (idxFile == LB_ERR) return;

   FileInfo &fileInfo = fileInfoList[idxFile];

   wchar_t fileVal[MAX_PATH + 1];

   GetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_DESC_EDIT, fileVal, MAX_PATH + 1);
   fileInfo.label = fileVal;

   char eolVal[MAX_PATH + 1];

   GetDlgItemTextA(_hSelf, IDC_FWVIZ_DEF_FILE_TERM_EDIT, eolVal, MAX_PATH + 1);
   fileInfo.eol = eolVal;

   GetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LIST, fileVal, MAX_PATH + 1);
   fileInfo.theme = fileVal;

   SendMessage(hListFiles, LB_DELETESTRING, (WPARAM)idxFile, NULL);
   SendMessage(hListFiles, LB_INSERTSTRING, (WPARAM)idxFile, (LPARAM)fileInfo.label.c_str());
   SendMessage(hListFiles, LB_SETCURSEL, idxFile, NULL);
}

void ConfigureDialog::fileEditNew() {
   FileInfo newFile{ getNewFile() };

   fileInfoList.push_back(newFile);

   size_t moveTo = fileInfoList.size() - 1;

   SendMessage(hListFiles, LB_ADDSTRING, NULL, (LPARAM)newFile.label.c_str());
   SendMessage(hListFiles, LB_SETCURSEL, (WPARAM)moveTo, NULL);
   onFileTypeSelect();
}

int ConfigureDialog::fileEditDelete() {
   int idxFile{ getCurrentFileIndex() };
   if (idxFile == LB_ERR) return LB_ERR;

   fileInfoList.erase(fileInfoList.begin() + idxFile);

   int lastFile = static_cast<int>(fileInfoList.size()) - 1;
   int moveTo = (idxFile <= lastFile - 1) ? idxFile : lastFile;

   SendMessage(hListFiles, LB_DELETESTRING, (WPARAM)idxFile, NULL);
   SendMessage(hListFiles, LB_SETCURSEL, (WPARAM)moveTo, NULL);
   onFileTypeSelect();

   return moveTo;
}
