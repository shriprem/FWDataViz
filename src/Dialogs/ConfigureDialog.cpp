#include "ConfigureDialog.h"

extern ConfigureDialog _configDlg;

LRESULT CALLBACK procANSIEditControl(HWND hwnd, UINT messageId, WPARAM wParam,
   LPARAM lParam, UINT_PTR, DWORD_PTR) {

   switch (messageId) {
      case WM_CHAR:
         if (static_cast<WCHAR>(wParam) > 255) {
            showEditBalloonTip(hwnd, FWVIZ_DIALOG_ANSI_TITLE, FWVIZ_DIALOG_ANSI_MESSAGE);
            return FALSE;
         }
         break;

      case WM_PASTE:
      {
         wstring clipText;

         Utils::getClipboardText(GetParent(hwnd), clipText);

         if (!regex_match(clipText, std::wregex(L"^[\x20-\xFF]*$"))) {
            showEditBalloonTip(hwnd, FWVIZ_DIALOG_ANSI_TITLE, FWVIZ_DIALOG_ANSI_MESSAGE);
            return FALSE;
         }
         break;
      }
   }

   return DefSubclassProc(hwnd, messageId, wParam, lParam);
}

LRESULT CALLBACK procFieldEditMessages(HWND hwnd, UINT messageId, WPARAM wParam,
   LPARAM lParam, UINT_PTR, DWORD_PTR) {

   HWND hThis{ hwnd == _configDlg.hFieldLabels ? _configDlg.hFieldLabels : _configDlg.hFieldWidths };
   HWND hThat{ hwnd == _configDlg.hFieldLabels ? _configDlg.hFieldWidths : _configDlg.hFieldLabels };

   switch (messageId) {
      case WM_CHAR:
         if (wParam == ',' && hwnd == _configDlg.hFieldLabels) {
            showEditBalloonTip(hwnd, FWVIZ_DIALOG_COMMAS_TITLE, FWVIZ_DIALOG_COMMAS_MESSAGE);
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

   hFilesList = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_LIST_BOX);
   hFileEOL = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_EOL_EDIT);
   hRecsList = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_LIST_BOX);
   hRecStart = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_START_EDIT);
   hRecRegex = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_REGEX_EDIT);
   hFieldLabels = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FIELD_LABELS_EDIT);
   hFieldWidths = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FIELD_WIDTHS_EDIT);

   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_DESC_EDIT, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);
   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_REC_DESC_EDIT, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);

   SendMessage(hFileEOL, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);
   SendMessage(hRecStart, EM_LIMITTEXT, (WPARAM)(MAX_PATH - 1), NULL);
   SendMessage(hRecRegex, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);
   SendMessage(hFieldLabels, EM_LIMITTEXT, (WPARAM)FW_LINE_MAX_LENGTH, NULL);
   SendMessage(hFieldWidths, EM_LIMITTEXT, (WPARAM)FW_LINE_MAX_LENGTH, NULL);

   SetWindowSubclass(hFileEOL, procANSIEditControl, NULL, NULL);
   SetWindowSubclass(hRecStart, procANSIEditControl, NULL, NULL);
   SetWindowSubclass(hRecRegex, procANSIEditControl, NULL, NULL);
   SetWindowSubclass(hFieldLabels, procFieldEditMessages, NULL, NULL);
   SetWindowSubclass(hFieldWidths, procFieldEditMessages, NULL, NULL);

   Utils::loadBitmap(_hSelf, IDC_FWVIZ_DEF_FILE_DOWN_BUTTON, IDC_FWVIZ_DEF_MOVE_DOWN_BITMAP);
   Utils::addTooltip(_hSelf, IDC_FWVIZ_DEF_FILE_DOWN_BUTTON, NULL, FWVIZ_DEF_FILE_MOVE_DOWN, FALSE);

   Utils::loadBitmap(_hSelf, IDC_FWVIZ_DEF_FILE_UP_BUTTON, IDC_FWVIZ_DEF_MOVE_UP_BITMAP);
   Utils::addTooltip(_hSelf, IDC_FWVIZ_DEF_FILE_UP_BUTTON, NULL, FWVIZ_DEF_FILE_MOVE_UP, FALSE);

   Utils::loadBitmap(_hSelf, IDC_FWVIZ_DEF_REC_DOWN_BUTTON, IDC_FWVIZ_DEF_MOVE_DOWN_BITMAP);
   Utils::addTooltip(_hSelf, IDC_FWVIZ_DEF_REC_DOWN_BUTTON, NULL, FWVIZ_DEF_REC_MOVE_DOWN, FALSE);

   Utils::loadBitmap(_hSelf, IDC_FWVIZ_DEF_REC_UP_BUTTON, IDC_FWVIZ_DEF_MOVE_UP_BITMAP);
   Utils::addTooltip(_hSelf, IDC_FWVIZ_DEF_REC_UP_BUTTON, NULL, FWVIZ_DEF_REC_MOVE_UP, FALSE);

   Utils::setFontBold(_hSelf, IDOK);

   bool recentOS = Utils::checkBaseOS(WV_VISTA);
   wstring fontName = recentOS ? L"Consolas" : L"Courier New";
   int fontHeight = recentOS ? 8 : 7;

   Utils::setFont(_hSelf, IDC_FWVIZ_DEF_FILE_EOL_ANSI, fontName, fontHeight, FW_REGULAR, TRUE);
   Utils::setFont(_hSelf, IDC_FWVIZ_DEF_REC_START_ANSI, fontName, fontHeight, FW_REGULAR, TRUE);
   Utils::setFont(_hSelf, IDC_FWVIZ_DEF_REC_REGEX_ANSI, fontName, fontHeight, FW_REGULAR, TRUE);

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
            case IDC_FWVIZ_DEF_FILE_LIST_BOX:
               switch HIWORD(wParam) {
                  case LBN_SELCHANGE:
                     onFileTypeSelect();
                     break;
               }
               break;

            case IDC_FWVIZ_DEF_FILE_DOWN_BUTTON:
               moveFileType(MOVE_DOWN);
               break;

            case IDC_FWVIZ_DEF_FILE_UP_BUTTON:
               moveFileType(MOVE_UP);
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

            case IDC_FWVIZ_DEF_REC_LIST_BOX:
               switch HIWORD(wParam) {
                  case LBN_SELCHANGE:
                     onRecTypeSelect();
                     break;
               }
               break;

            case IDC_FWVIZ_DEF_REC_DOWN_BUTTON:
               moveRecType(MOVE_DOWN);
               break;

            case IDC_FWVIZ_DEF_REC_UP_BUTTON:
               moveRecType(MOVE_UP);
               break;

            case IDC_FWVIZ_DEF_REC_START_EDIT:
               switch HIWORD(wParam) {
                  case EN_CHANGE:
                     if (GetFocus() == hRecStart) {
                        onRecStartEditChange();
                     }
                     break;
               }
               break;

            case IDC_FWVIZ_DEF_REC_REGEX_EDIT:
               switch HIWORD(wParam) {
                  case EN_CHANGE:
                     if (GetFocus() == hRecRegex) {
                        onRecRegexEditChange();
                     }
                     break;

                  case EN_KILLFOCUS:
                     if (GetWindowTextLength(hRecRegex) == 0) {
                        SetWindowTextA(hRecRegex, ".");
                     }
                     break;
               }
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

            case IDC_FWVIZ_DEF_FIELD_LABELS_EDIT:
               switch HIWORD(wParam) {
                  case EN_SETFOCUS:
                     setFieldEditCaretOnFocus(hFieldLabels);
                     break;

                  case EN_VSCROLL:
                     if (GetFocus() == hFieldLabels) {
                        syncFieldEditScrolling(hFieldLabels, hFieldWidths);
                     }
               }
               break;

            case IDC_FWVIZ_DEF_FIELD_WIDTHS_EDIT:
               switch HIWORD(wParam) {
                  case EN_SETFOCUS:
                     setFieldEditCaretOnFocus(hFieldWidths);
                     break;

                  case EN_VSCROLL:
                     if (GetFocus() == hFieldWidths) {
                        syncFieldEditScrolling(hFieldWidths, hFieldLabels);
                     }
               }
               break;

            case IDC_FWVIZ_DEF_FIELD_ACCEPT_BTN:
               fieldEditsAccept();
               break;

            case IDC_FWVIZ_DEF_FIELD_RESET_BTN:
               fieldEditsReset();
               break;

            case IDCANCEL:
            case IDOK:
               display(FALSE);
               return TRUE;

            case IDC_FWVIZ_DEF_RESET_BTN:
               configFile = L"";
               reloadConfigInfo();
               break;

            case IDC_FWVIZ_DEF_BACKUP_LOAD_BTN:
               TCHAR sConfigFile[MAX_PATH];

               PathCombine(sConfigFile, _configIO.getPluginConfigDir(), L"Visualizer_Bkup.ini");
               configFile = wstring{ sConfigFile };
               reloadConfigInfo();
               break;
         }
         break;
   }

   return FALSE;
}

void ConfigureDialog::localize() {
   SetWindowText(_hSelf, FWVIZ_DIALOG_TITLE);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_GROUP_BOX, FWVIZ_DEF_FILE_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_DESC_LABEL, FWVIZ_DEF_FILE_DESC_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_EOL_LABEL, FWVIZ_DEF_FILE_EOL_LABEL);
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

   fileTypeList = _configIO.getConfigString(L"Base", L"FileTypes", L"", configFile.c_str());
   fileTypeCount = _configIO.Tokenize(fileTypeList, fileTypes);

   fileInfoList.clear();
   fileInfoList.resize(fileTypeCount);

   for (int i{}; i < fileTypeCount; i++) {
      wstring &fileType = fileTypes[i];
      FileInfo &FILE = fileInfoList[i];

      FILE.label = _configIO.getConfigString(fileType.c_str(), L"FileLabel", L"", configFile.c_str());
      FILE.eol = _configIO.getConfigStringA(fileType.c_str(), L"RecordTerminator", L"", configFile.c_str());
      FILE.theme = _configIO.getConfigString(fileType.c_str(), L"FileTheme", L"", configFile.c_str());

      vector<wstring> recTypes;
      wstring recTypeList;
      int recTypeCount;

      recTypeList = _configIO.getConfigString(fileType.c_str(), L"RecordTypes", L"", configFile.c_str());
      recTypeCount = _configIO.Tokenize(recTypeList, recTypes);

      FILE.records.clear();
      FILE.records.resize(recTypeCount);

      for (int j{}; j < recTypeCount; j++) {
         wstring &recType = recTypes[j];
         RecordInfo &REC = FILE.records[j];

         REC.label = _configIO.getConfigString(fileType.c_str(),
            (recType + L"_Label").c_str(), L"", configFile.c_str());
         REC.marker = _configIO.getConfigStringA(fileType.c_str(),
            (recType + L"_Marker").c_str(), L"", configFile.c_str());
         REC.fieldWidths = _configIO.getConfigString(fileType.c_str(),
            (recType + L"_FieldWidths").c_str(), L"", configFile.c_str());
         REC.fieldLabels = _configIO.getConfigString(fileType.c_str(),
            (recType + L"_FieldLabels").c_str(), L"", configFile.c_str());
      }
   }

   return static_cast<int>(fileInfoList.size());
}

void ConfigureDialog::fillFileTypes() {
   // Fill File Types Listbox
   size_t fTypes;

   SendMessage(hFilesList, LB_RESETCONTENT, NULL, NULL);

   fTypes = fileInfoList.size();
   for (size_t i{}; i < fTypes; i++) {
      SendMessage(hFilesList, LB_ADDSTRING, NULL, (LPARAM)fileInfoList[i].label.c_str());
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
      SendMessage(hFilesList, LB_SETCURSEL, 0, NULL);
      onFileTypeSelect();
   }
}

int ConfigureDialog::getCurrentFileIndex() {
   int idxFile;

   idxFile = static_cast<int>(SendMessage(hFilesList, LB_GETCURSEL, NULL, NULL));
   if (idxFile == LB_ERR) return LB_ERR;

   return idxFile;
}

int ConfigureDialog::getCurrentRecIndex() {
   int idxRec;

   idxRec = static_cast<int>(SendMessage(hRecsList, LB_GETCURSEL, NULL, NULL));
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
   SetDlgItemTextA(_hSelf, IDC_FWVIZ_DEF_FILE_EOL_EDIT, fileInfo->eol.c_str());

   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LIST, CB_SETCURSEL, (WPARAM)
      SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LIST, CB_FINDSTRING, (WPARAM)-1,
         (LPARAM)fileInfo->theme.c_str()), NULL);

   enableMoveFileButtons();
   fillRecTypes();
}

void ConfigureDialog::enableMoveFileButtons() {
   int idxFile{ getCurrentFileIndex() };
   if (idxFile == LB_ERR) return;

   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_DOWN_BUTTON),
      (idxFile < static_cast<int>(fileInfoList.size()) - 1));
   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_UP_BUTTON), (idxFile > 0));
}

int ConfigureDialog::moveFileType(move_dir dir) {
   int idxFile{ getCurrentFileIndex() };
   if (idxFile == LB_ERR) return LB_ERR;

   switch(dir) {
      case MOVE_DOWN:
         if (idxFile >= static_cast<int>(fileInfoList.size()) - 1) return LB_ERR;
         break;

      case MOVE_UP:
         if (idxFile == 0) return LB_ERR;
         break;

      default:
         return LB_ERR;
   }

   FileInfo currType = fileInfoList[idxFile];
   FileInfo &adjType = fileInfoList[idxFile + dir];

   fileInfoList[idxFile] = adjType;
   fileInfoList[idxFile + dir] = currType;

   SendMessage(hFilesList, LB_DELETESTRING, (WPARAM)idxFile, NULL);
   SendMessage(hFilesList, LB_INSERTSTRING, (WPARAM)(idxFile + dir),
      (LPARAM)fileInfoList[idxFile + dir].label.c_str());
   SendMessage(hFilesList, LB_SETCURSEL, idxFile + dir, NULL);

   enableMoveFileButtons();

   return idxFile + dir;
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

   SendMessage(hRecsList, LB_RESETCONTENT, NULL, NULL);

   recTypes = recInfoList.size();

   for (size_t i{}; i < recTypes; i++) {
      SendMessage(hRecsList, LB_ADDSTRING, NULL, (LPARAM)recInfoList[i].label.c_str());
   }

   // Select first item
   if (recTypes > 0) {
      SendMessage(hRecsList, LB_SETCURSEL, 0, NULL);
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

   SetWindowTextA(hRecRegex, regExpr.c_str());
   SetWindowTextA(hRecStart, (regExpr.substr(0, 1) == "^") ? regExpr.substr(1).c_str() : "");

   enableMoveRecButtons();
   fillFieldTypes();
}

void ConfigureDialog::enableMoveRecButtons() {
   int idxFile{ getCurrentFileIndex() };
   if (idxFile == LB_ERR) return;

   int idxRec{ getCurrentRecIndex() };
   if (idxRec == LB_ERR) return;

   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_DOWN_BUTTON),
      (idxRec < static_cast<int>(fileInfoList[idxFile].records.size()) - 1));
   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_UP_BUTTON), (idxRec > 0));
}

int ConfigureDialog::moveRecType(move_dir dir) {
   int idxFile{ getCurrentFileIndex() };
   if (idxFile == LB_ERR) return LB_ERR;

   int idxRec{ getCurrentRecIndex() };
   if (idxRec == LB_ERR) return LB_ERR;

   vector<RecordInfo>& recList = fileInfoList[idxFile].records;

   switch (dir) {
   case MOVE_DOWN:
      if (idxRec >= static_cast<int>(recList.size()) - 1) return LB_ERR;
      break;

   case MOVE_UP:
      if (idxRec == 0) return LB_ERR;
      break;

   default:
      return LB_ERR;
   }

   RecordInfo currType = recList[idxRec];
   RecordInfo& adjType = recList[idxRec + dir];

   recList[idxRec] = adjType;
   recList[idxRec + dir] = currType;

   SendMessage(hRecsList, LB_DELETESTRING, (WPARAM)idxRec, NULL);
   SendMessage(hRecsList, LB_INSERTSTRING, (WPARAM)(idxRec + dir),
      (LPARAM)recList[idxRec + dir].label.c_str());
   SendMessage(hRecsList, LB_SETCURSEL, idxRec + dir, NULL);

   enableMoveRecButtons();

   return idxRec + dir;
}

void ConfigureDialog::fillFieldTypes() {
   RecordInfo *recInfo;

   if (!getCurrentRecInfo(recInfo)) {
      RecordInfo newRec{ getNewRec() };
      recInfo = &newRec;
   }

   // Field Labels
   wstring fieldLabels{ regex_replace(recInfo->fieldLabels, wregex(L","), L"\r\n") };
   SetWindowText(hFieldLabels, fieldLabels.c_str());

   // Field Widths
   wstring fieldWidths{ regex_replace(recInfo->fieldWidths, wregex(L","), L"\r\n") };
   SetWindowText(hFieldWidths, fieldWidths.c_str());
}

void ConfigureDialog::setFieldEditCaretOnFocus(HWND hEdit) {
   DWORD startPos{}, endPos{};
   SendMessage(hEdit, EM_GETSEL, (WPARAM)&startPos, (LPARAM)&endPos);

   if (GetWindowTextLength(hEdit) == static_cast<int>(endPos) - static_cast<int>(startPos)) {
      int caretPos = (hEdit == hFieldLabels) ? editLabelsCaret : editWidthsCaret;
      SendMessage(hEdit, EM_SETSEL, (WPARAM)caretPos, (LPARAM)caretPos);
      SendMessage(hEdit, EM_SCROLLCARET, NULL, NULL);
   }

   hiliteFieldEditPairedItem(hEdit, (hEdit == hFieldLabels) ? hFieldWidths : hFieldLabels);
}

void ConfigureDialog::hiliteFieldEditPairedItem(HWND hThis, HWND hThat) {
   int thisLine = static_cast<int>(SendMessage(hThis, EM_LINEFROMCHAR,
      (WPARAM) SendMessage(hThis, EM_LINEINDEX, (WPARAM)-1, NULL), NULL));

   int thatLineCount = static_cast<int>(SendMessage(hThat, EM_GETLINECOUNT, NULL, NULL));
   if (thisLine >= thatLineCount) return;

   int lineStart = static_cast<int>(SendMessage(hThat, EM_LINEINDEX, (WPARAM)thisLine, NULL));
   int lineLength = static_cast<int>(SendMessage(hThat, EM_LINELENGTH, (WPARAM)lineStart, NULL));

   ((hThis == hFieldLabels) ? editWidthsCaret : editLabelsCaret) = lineStart;

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
   GetWindowText(hFieldLabels, fieldValues, (FW_LINE_MAX_LENGTH + 1));

   // Replace any trailing spaces + newlines with commas
   vals = regex_replace(fieldValues, wregex(L" *\r\n"), L",");

   // Replace any leading spaces + commas with commas
   vals = regex_replace(vals, wregex(L", +([^,]*)"), L",$1");

   // Trim any leading & trailing spaces + commas for the entire string
   vals = regex_replace(vals, wregex(L"^ +(.*[^ ]) +$"), L"$1");

   recInfo->fieldLabels = vals;
   SetWindowText(hFieldLabels, regex_replace(vals, wregex(L","), L"\r\n").c_str());

   // Field Widths
   GetWindowText(hFieldWidths, fieldValues, (FW_LINE_MAX_LENGTH + 1));

   // Replace any newlines with commas.
   // No processing needed for leading & trailing spaces since this is a numeric edit control
   vals = regex_replace(fieldValues, wregex(L"\r\n"), L",");

   recInfo->fieldWidths = vals;
}

void ConfigureDialog::fieldEditsReset() {
   fillFieldTypes();
}

void ConfigureDialog::onRecStartEditChange() {
   char startChars[MAX_PATH];
   GetWindowTextA(hRecStart, startChars, MAX_PATH);

   string startText = string{ startChars };
   string regexText = (startText.length() > 0) ? "^" + startText : ".";

   SetWindowTextA(hRecRegex, regexText.c_str());
}

void ConfigureDialog::onRecRegexEditChange() {
   char regexChars[MAX_PATH];
   int cCount = GetWindowTextA(hRecRegex, regexChars, MAX_PATH);

   string startText{};

   if (cCount > 0 && regexChars[0] == '^') {
      startText = string{ regexChars }.substr(1);
   }

   SetWindowTextA(hRecStart, startText.c_str());
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

   SendMessage(hRecsList, LB_DELETESTRING, (WPARAM)idxRec, NULL);
   SendMessage(hRecsList, LB_INSERTSTRING, (WPARAM)idxRec, (LPARAM)recDesc);
   SendMessage(hRecsList, LB_SETCURSEL, idxRec, NULL);
}

void ConfigureDialog::recEditNew() {
   int idxFile{ getCurrentFileIndex() };
   if (idxFile == LB_ERR) return;

   RecordInfo newRec{ getNewRec() };
   vector<RecordInfo> &records = fileInfoList[idxFile].records;

   records.push_back(newRec);

   size_t moveTo = records.size() - 1;

   SendMessage(hRecsList, LB_ADDSTRING, NULL, (LPARAM)newRec.label.c_str());
   SendMessage(hRecsList, LB_SETCURSEL, (WPARAM)moveTo, NULL);
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

   SendMessage(hRecsList, LB_DELETESTRING, (WPARAM)idxRec, NULL);
   SendMessage(hRecsList, LB_SETCURSEL, (WPARAM)moveTo, NULL);
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

   GetDlgItemTextA(_hSelf, IDC_FWVIZ_DEF_FILE_EOL_EDIT, eolVal, MAX_PATH + 1);
   fileInfo.eol = eolVal;

   GetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LIST, fileVal, MAX_PATH + 1);
   fileInfo.theme = fileVal;

   SendMessage(hFilesList, LB_DELETESTRING, (WPARAM)idxFile, NULL);
   SendMessage(hFilesList, LB_INSERTSTRING, (WPARAM)idxFile, (LPARAM)fileInfo.label.c_str());
   SendMessage(hFilesList, LB_SETCURSEL, idxFile, NULL);
}

void ConfigureDialog::fileEditNew() {
   FileInfo newFile{ getNewFile() };

   fileInfoList.push_back(newFile);

   size_t moveTo = fileInfoList.size() - 1;

   SendMessage(hFilesList, LB_ADDSTRING, NULL, (LPARAM)newFile.label.c_str());
   SendMessage(hFilesList, LB_SETCURSEL, (WPARAM)moveTo, NULL);
   onFileTypeSelect();
}

int ConfigureDialog::fileEditDelete() {
   int idxFile{ getCurrentFileIndex() };
   if (idxFile == LB_ERR) return LB_ERR;

   fileInfoList.erase(fileInfoList.begin() + idxFile);

   int lastFile = static_cast<int>(fileInfoList.size()) - 1;
   int moveTo = (idxFile <= lastFile - 1) ? idxFile : lastFile;

   SendMessage(hFilesList, LB_DELETESTRING, (WPARAM)idxFile, NULL);
   SendMessage(hFilesList, LB_SETCURSEL, (WPARAM)moveTo, NULL);
   onFileTypeSelect();

   return moveTo;
}

void ConfigureDialog::reloadConfigInfo() {
   loadConfigInfo();
   fillFileTypes();
}
