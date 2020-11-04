#include "ConfigureDialog.h"
#include "EximFileTypeDialog.h"

extern HINSTANCE _gModule;
extern ConfigureDialog _configDlg;
EximFileTypeDialog _eximDlg;

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

   hFilesLB = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_LIST_BOX);
   hFileEOL = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_EOL_EDIT);
   hRecsLB = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_LIST_BOX);
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

   bool recentOS = Utils::checkBaseOS(WV_VISTA);
   wstring fontName = recentOS ? L"Consolas" : L"Courier New";
   int fontHeight = recentOS ? 8 : 7;

   Utils::setFont(_hSelf, IDC_FWVIZ_DEF_FILE_EOL_ANSI, fontName, fontHeight, FW_REGULAR, TRUE);
   Utils::setFont(_hSelf, IDC_FWVIZ_DEF_REC_START_ANSI, fontName, fontHeight, FW_REGULAR, TRUE);
   Utils::setFont(_hSelf, IDC_FWVIZ_DEF_REC_REGEX_ANSI, fontName, fontHeight, FW_REGULAR, TRUE);

   if (_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);

   if (vFileTypes.size() < 1) {
      loadConfigInfo();
      fillFileTypes();
   }
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

            case IDC_FWVIZ_DEF_FILE_DESC_EDIT:
               switch HIWORD(wParam) {
                  case EN_CHANGE:
                     if (!loadingEdits) {
                        cleanFileVals = FALSE;
                        enableFileSelection();
                     }
                     break;
                  }
               break;

            case IDC_FWVIZ_DEF_FILE_EOL_EDIT:
               switch HIWORD(wParam) {
                  case EN_CHANGE:
                     if (!loadingEdits) {
                        cleanFileVals = FALSE;
                        enableFileSelection();
                     }
                     break;
                  }
               break;

            case IDC_FWVIZ_DEF_FILE_THEME_LIST:
               switch HIWORD(wParam) {
                  case LBN_SELCHANGE:
                     if (!loadingEdits) {
                        cleanFileVals = FALSE;
                        enableFileSelection();
                     }
                     break;
                  }
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

            case IDC_FWVIZ_DEF_REC_DESC_EDIT:
               switch HIWORD(wParam) {
                  case EN_CHANGE:
                     if (!loadingEdits) {
                        cleanRecVals = FALSE;
                        enableRecSelection();
                     }
                     break;
               }
               break;

            case IDC_FWVIZ_DEF_REC_START_EDIT:
               switch HIWORD(wParam) {
                  case EN_CHANGE:
                     if (!loadingEdits) {
                        cleanRecVals = FALSE;
                        enableRecSelection();
                     }

                     if (GetFocus() == hRecStart) {
                        onRecStartEditChange();
                     }
                     break;
               }
               break;

            case IDC_FWVIZ_DEF_REC_REGEX_EDIT:
               switch HIWORD(wParam) {
                  case EN_CHANGE:
                     if (!loadingEdits) {
                        cleanRecVals = FALSE;
                        enableRecSelection();
                     }

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
                  case EN_CHANGE:
                     if (!loadingEdits) {
                        cleanFieldVals = FALSE;
                        enableRecSelection();
                     }
                     break;

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
                  case EN_CHANGE:
                     if (!loadingEdits) {
                        cleanFieldVals = FALSE;
                        enableRecSelection();
                     }
                     break;

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
               fillFieldTypes();
               break;

            case IDCANCEL:
            case IDCLOSE:
               if (promptDiscardChangesNo()) return TRUE;

               display(FALSE);
               return TRUE;

            case IDC_FWVIZ_DEF_SAVE_CONFIG_BTN:
               SetCursor(LoadCursor(NULL, IDC_WAIT));
               saveConfigInfo();
               SetCursor(LoadCursor(NULL, IDC_ARROW));
               return TRUE;

            case IDC_FWVIZ_DEF_RESET_BTN:
               if (!promptDiscardChangesNo()) {
                  configFile = L"";
                  loadConfigInfo();
                  fillFileTypes();
               }
               break;

            case IDC_FWVIZ_DEF_BACKUP_LOAD_BTN:
               if (!promptDiscardChangesNo()) {
                  wstring sBackupConfigFile;

                  if (_configIO.queryConfigFileName(_hSelf, TRUE, TRUE, sBackupConfigFile)) {
                     configFile = sBackupConfigFile;
                     loadConfigInfo();
                     fillFileTypes();
                     cleanConfigFile = FALSE;
                     enableFileSelection();
                  }
               }
               break;


            case IDC_FWVIZ_DEF_BACKUP_VIEW_BTN:
               _configIO.viewBackupFolder();
               break;

            case IDC_FWVIZ_DEF_EXTRACT_BTN:
               showEximDialog(TRUE);
               break;

            case IDC_FWVIZ_DEF_APPEND_BTN:
               showEximDialog(FALSE);
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
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_SAVE_CONFIG_BTN, FWVIZ_DIALOG_SAVE_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_RESET_BTN, FWVIZ_DIALOG_RESET_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_BACKUP_LOAD_BTN, FWVIZ_DIALOG_BKUP_LOAD_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_BACKUP_VIEW_BTN, FWVIZ_DIALOG_BKUP_VIEW_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_EXTRACT_BTN, FWVIZ_DIALOG_EXTRACT_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_APPEND_BTN, FWVIZ_DIALOG_APPEND_BTN);
   SetDlgItemText(_hSelf, IDCLOSE, FWVIZ_DIALOG_CLOSE_BTN);
}

void ConfigureDialog::indicateCleanStatus() {
   if (cleanConfigFile) {
      SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_SAVE_CONFIG_BTN, FWVIZ_DIALOG_SAVE_BTN);
      Utils::setFontRegular(_hSelf, IDC_FWVIZ_DEF_SAVE_CONFIG_BTN);
   }
   else {
      SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_SAVE_CONFIG_BTN, (wstring(FWVIZ_DIALOG_SAVE_BTN) + L"*").c_str());
      Utils::setFontBold(_hSelf, IDC_FWVIZ_DEF_SAVE_CONFIG_BTN);
   }

   if (cleanFileVals) {
      SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_ACCEPT_BTN, FWVIZ_DEF_FILE_ACCEPT_BTN);
      Utils::setFontRegular(_hSelf, IDC_FWVIZ_DEF_FILE_ACCEPT_BTN);
   }
   else {
      SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_ACCEPT_BTN, (wstring(FWVIZ_DEF_FILE_ACCEPT_BTN) + L"*").c_str());
      Utils::setFontBold(_hSelf, IDC_FWVIZ_DEF_FILE_ACCEPT_BTN);
   }

   if (cleanRecVals) {
      SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_ACCEPT_BTN, FWVIZ_DEF_REC_ACCEPT_BTN);
      Utils::setFontRegular(_hSelf, IDC_FWVIZ_DEF_REC_ACCEPT_BTN);
   }
   else {
      SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_ACCEPT_BTN, (wstring(FWVIZ_DEF_REC_ACCEPT_BTN) + L"*").c_str());
      Utils::setFontBold(_hSelf, IDC_FWVIZ_DEF_REC_ACCEPT_BTN);
   }

   if (cleanFieldVals) {
      SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_ACCEPT_BTN, FWVIZ_DEF_FIELD_ACCEPT_BTN);
      Utils::setFontRegular(_hSelf, IDC_FWVIZ_DEF_FIELD_ACCEPT_BTN);
   }
   else {
      SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_ACCEPT_BTN, (wstring(FWVIZ_DEF_FIELD_ACCEPT_BTN) + L"*").c_str());
      Utils::setFontBold(_hSelf, IDC_FWVIZ_DEF_FIELD_ACCEPT_BTN);
   }
}

int ConfigureDialog::addConfigInfo(int vIndex, const wstring& fileType, const wstring& sConfigFile) {
   FileType& FT = vFileTypes[vIndex];

   FT.label = _configIO.getConfigString(fileType, L"FileLabel", L"", sConfigFile);
   FT.eol = _configIO.getConfigStringA(fileType, L"RecordTerminator", L"", sConfigFile);
   FT.theme = _configIO.getConfigString(fileType, L"FileTheme", L"", sConfigFile);

   vector<wstring> recTypesList;
   wstring recTypes;
   int recTypeCount;

   recTypes = _configIO.getConfigString(fileType, L"RecordTypes", L"", sConfigFile);
   recTypeCount = _configIO.Tokenize(recTypes, recTypesList);

   FT.vRecTypes.clear();
   FT.vRecTypes.resize(recTypeCount);

   for (int j{}; j < recTypeCount; j++) {
      wstring& recType = recTypesList[j];
      RecordType& RT = FT.vRecTypes[j];

      RT.label = _configIO.getConfigString(fileType, (recType + L"_Label"), L"", sConfigFile);
      RT.marker = _configIO.getConfigStringA(fileType, (recType + L"_Marker"), L"", sConfigFile);
      RT.fieldWidths = _configIO.getConfigString(fileType, (recType + L"_FieldWidths"), L"", sConfigFile);
      RT.fieldLabels = _configIO.getConfigString(fileType, (recType + L"_FieldLabels"), L"", sConfigFile);
   }

   return recTypeCount;
}

int ConfigureDialog::loadConfigInfo() {
   vector<wstring> fileTypeList;
   wstring sFileTypes;
   int fileTypeCount;

   sFileTypes = _configIO.getConfigString(L"Base", L"FileTypes", L"", configFile);
   fileTypeCount = _configIO.Tokenize(sFileTypes, fileTypeList);

   vFileTypes.clear();
   vFileTypes.resize(fileTypeCount);

   for (int i{}; i < fileTypeCount; i++) {
      wstring &fileType = fileTypeList[i];
      addConfigInfo(i, fileType, configFile);
   }

   return static_cast<int>(vFileTypes.size());
}

void ConfigureDialog::fillFileTypes() {
   // Fill File Types Listbox
   size_t fTypes;

   SendMessage(hFilesLB, LB_RESETCONTENT, NULL, NULL);

   fTypes = vFileTypes.size();
   for (size_t i{}; i < fTypes; i++) {
      SendMessage(hFilesLB, LB_ADDSTRING, NULL, (LPARAM)vFileTypes[i].label.c_str());
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
      SendMessage(hFilesLB, LB_SETCURSEL, 0, NULL);
   }

   cleanConfigFile = TRUE;
   cleanFileVals = TRUE;
   onFileTypeSelect();
}

int ConfigureDialog::getCurrentFileTypeIndex() {
   int idxFT;

   idxFT = static_cast<int>(SendMessage(hFilesLB, LB_GETCURSEL, NULL, NULL));
   if (idxFT == LB_ERR) return LB_ERR;

   return idxFT;
}

int ConfigureDialog::getCurrentRecIndex() {
   int idxRec;

   idxRec = static_cast<int>(SendMessage(hRecsLB, LB_GETCURSEL, NULL, NULL));
   if (idxRec == LB_ERR) return LB_ERR;

   return idxRec;
}

bool ConfigureDialog::getCurrentFileTypeInfo(FileType* &fileInfo) {
   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return FALSE;

   fileInfo = &vFileTypes[idxFT];
   return TRUE;
}

ConfigureDialog::FileType ConfigureDialog::getNewFileType() {
   FileType newFile;

   newFile.label = L"";
   newFile.eol = "";
   newFile.theme = L"VT_Basic";
   newFile.vRecTypes = vector<RecordType>{ getNewRec() };

   return newFile;
}

void ConfigureDialog::getFileTypeConfig(size_t idxFT, bool cr_lf, wstring &ftCode, wstring &ftConfig) {
   size_t recTypeCount;
   wchar_t fileTypeCode[60], recTypeCode[10];
   wstring new_line, rawCode, recTypes{}, rtConfig{}, recTypePrefix;

   FileType& FT = vFileTypes[idxFT];
   new_line = cr_lf ? L"\r\n" : L"\n";

   rawCode = regex_replace(FT.label, wregex(L"( |,|=|\\[|\\])"), L"_").substr(0, 50);
   swprintf(fileTypeCode, 60, L"FT%03d_%s", static_cast<int>(idxFT + 1), rawCode.c_str());
   _configIO.ToUpper(fileTypeCode);

   recTypeCount = (FT.vRecTypes.size() > 999) ? 999 : FT.vRecTypes.size();

   for (size_t j{}; j < recTypeCount; j++) {
      RecordType& RT = FT.vRecTypes[j];

      swprintf(recTypeCode, 10, L"REC%03d", static_cast<int>(j + 1));
      recTypePrefix = wstring{ recTypeCode };
      recTypes += (j == 0 ? L"RecordTypes=" : L",") + recTypePrefix;

      rtConfig +=
         recTypePrefix + L"_Label=" + RT.label + new_line +
         recTypePrefix + L"_Marker=" + _configIO.NarrowToWide(RT.marker) + new_line +
         recTypePrefix + L"_FieldLabels=" + RT.fieldLabels + new_line +
         recTypePrefix + L"_FieldWidths=" + RT.fieldWidths + new_line;
   }

   ftCode = wstring{ fileTypeCode };
   ftConfig = L"[" + ftCode + L"]" + new_line +
      L"FileLabel=" + FT.label + new_line +
      L"FileTheme=" + FT.theme + new_line +
      L"RecordTerminator=" + _configIO.NarrowToWide(FT.eol) + new_line +
      recTypes + new_line + rtConfig;
}

bool ConfigureDialog::getCurrentRecInfo(RecordType*& recInfo) {
   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return FALSE;

   int idxRec{ getCurrentRecIndex() };
   if (idxRec == LB_ERR) return FALSE;

   recInfo = &vFileTypes[idxFT].vRecTypes[idxRec];
   return TRUE;
}

ConfigureDialog::RecordType ConfigureDialog::getNewRec() {
   RecordType newRec;

   newRec.label = L"";
   newRec.marker = "";
   newRec.fieldLabels = L"";
   newRec.fieldWidths = L"";
   return newRec;
}

void ConfigureDialog::onFileTypeSelect() {
   FileType* fileInfo;

   if (!getCurrentFileTypeInfo(fileInfo)) {
      FileType newFile{ getNewFileType() };
      fileInfo = &newFile;
   }

   loadingEdits = TRUE;
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_DESC_EDIT, fileInfo->label.c_str());
   SetDlgItemTextA(_hSelf, IDC_FWVIZ_DEF_FILE_EOL_EDIT, fileInfo->eol.c_str());
   loadingEdits = FALSE;

   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LIST, CB_SETCURSEL, (WPARAM)
      SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LIST, CB_FINDSTRING, (WPARAM)-1,
         (LPARAM)fileInfo->theme.c_str()), NULL);

   enableMoveFileButtons();
   fillRecTypes();
}

void ConfigureDialog::enableMoveFileButtons() {
   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return;

   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_DOWN_BUTTON),
      (idxFT < static_cast<int>(vFileTypes.size()) - 1));
   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_UP_BUTTON), (idxFT > 0));
}

void ConfigureDialog::enableFileSelection() {
   bool enable{ cleanFileVals && cleanRecVals && cleanFieldVals };
   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_LIST_BOX), enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_NEW_BTN), enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_EXTRACT_BTN), enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_APPEND_BTN), enable);

   if (enable) {
      enableMoveFileButtons();
   }
   else {
      EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_DOWN_BUTTON), FALSE);
      EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_UP_BUTTON), FALSE);
   }

   indicateCleanStatus();
}

int ConfigureDialog::moveFileType(move_dir dir) {
   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return LB_ERR;

   switch(dir) {
      case MOVE_DOWN:
         if (idxFT >= static_cast<int>(vFileTypes.size()) - 1) return LB_ERR;
         break;

      case MOVE_UP:
         if (idxFT == 0) return LB_ERR;
         break;

      default:
         return LB_ERR;
   }

   FileType currType = vFileTypes[idxFT];
   FileType &adjType = vFileTypes[idxFT + dir];

   vFileTypes[idxFT] = adjType;
   vFileTypes[idxFT + dir] = currType;

   SendMessage(hFilesLB, LB_DELETESTRING, (WPARAM)idxFT, NULL);
   SendMessage(hFilesLB, LB_INSERTSTRING, (WPARAM)(idxFT + dir),
      (LPARAM)vFileTypes[idxFT + dir].label.c_str());
   SendMessage(hFilesLB, LB_SETCURSEL, idxFT + dir, NULL);

   cleanConfigFile = FALSE;
   indicateCleanStatus();
   enableMoveFileButtons();

   return idxFT + dir;
}

void ConfigureDialog::fillRecTypes() {
   FileType* fileInfo;

   if (!getCurrentFileTypeInfo(fileInfo)) {
      FileType newFile{ getNewFileType() };
      fileInfo = &newFile;
   }

   vector <RecordType> &recInfoList = fileInfo->vRecTypes;

   // Fill Rec Types Listbox
   size_t recTypes;

   SendMessage(hRecsLB, LB_RESETCONTENT, NULL, NULL);

   recTypes = recInfoList.size();

   for (size_t i{}; i < recTypes; i++) {
      SendMessage(hRecsLB, LB_ADDSTRING, NULL, (LPARAM)recInfoList[i].label.c_str());
   }

   // Select first item
   if (recTypes > 0) {
      SendMessage(hRecsLB, LB_SETCURSEL, 0, NULL);
   }

   cleanRecVals = TRUE;
   onRecTypeSelect();
}

void ConfigureDialog::onRecTypeSelect() {
   RecordType *recInfo;

   if (!getCurrentRecInfo(recInfo)) {
      RecordType newRec{ getNewRec() };
      recInfo = &newRec;
   }

   loadingEdits = TRUE;

   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_DESC_EDIT, recInfo->label.c_str());

   string regExpr = recInfo->marker;

   SetWindowTextA(hRecRegex, regExpr.c_str());
   SetWindowTextA(hRecStart, (regExpr.substr(0, 1) == "^") ? regExpr.substr(1).c_str() : "");

   loadingEdits = FALSE;

   enableMoveRecButtons();
   fillFieldTypes();
}

void ConfigureDialog::enableMoveRecButtons() {
   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return;

   int idxRec{ getCurrentRecIndex() };
   if (idxRec == LB_ERR) return;

   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_DOWN_BUTTON),
      (idxRec < static_cast<int>(vFileTypes[idxFT].vRecTypes.size()) - 1));
   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_UP_BUTTON), (idxRec > 0));
}

void ConfigureDialog::enableRecSelection() {
   bool enable{ cleanRecVals && cleanFieldVals };
   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_LIST_BOX), enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_NEW_BTN), enable);

   if (enable) {
      enableMoveRecButtons();
   }
   else {
      EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_DOWN_BUTTON), FALSE);
      EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_UP_BUTTON), FALSE);
   }

   enableFileSelection();
}

int ConfigureDialog::moveRecType(move_dir dir) {
   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return LB_ERR;

   int idxRec{ getCurrentRecIndex() };
   if (idxRec == LB_ERR) return LB_ERR;

   vector<RecordType>& recList = vFileTypes[idxFT].vRecTypes;

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

   RecordType currType = recList[idxRec];
   RecordType& adjType = recList[idxRec + dir];

   recList[idxRec] = adjType;
   recList[idxRec + dir] = currType;

   SendMessage(hRecsLB, LB_DELETESTRING, (WPARAM)idxRec, NULL);
   SendMessage(hRecsLB, LB_INSERTSTRING, (WPARAM)(idxRec + dir),
      (LPARAM)recList[idxRec + dir].label.c_str());
   SendMessage(hRecsLB, LB_SETCURSEL, idxRec + dir, NULL);

   enableMoveRecButtons();

   return idxRec + dir;
}

void ConfigureDialog::fillFieldTypes() {
   RecordType *recInfo;

   if (!getCurrentRecInfo(recInfo)) {
      RecordType newRec{ getNewRec() };
      recInfo = &newRec;
   }

   // Field Labels
   wstring fieldLabels{ regex_replace(recInfo->fieldLabels, wregex(L","), L"\r\n") };
   SetWindowText(hFieldLabels, fieldLabels.c_str());

   // Field Widths
   wstring fieldWidths{ regex_replace(recInfo->fieldWidths, wregex(L","), L"\r\n") };
   SetWindowText(hFieldWidths, fieldWidths.c_str());

   cleanFieldVals = TRUE;
   enableRecSelection();
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
   if (cleanFieldVals) return;

   RecordType *recInfo;
   if (!getCurrentRecInfo(recInfo)) return;

   wchar_t fieldValues[FW_LINE_MAX_LENGTH + 1];
   wstring vals{};

   // Field Labels
   GetWindowText(hFieldLabels, fieldValues, (FW_LINE_MAX_LENGTH + 1));

   // Replace any trailing spaces + newlines with commas
   vals = regex_replace(fieldValues, wregex(L" *\r\n"), L",");

   // Replace any commas + leading spaces with commas
   vals = regex_replace(vals, wregex(L", +([^,]*)"), L",$1");

   // Trim any leading & trailing spaces for the entire string
   vals = regex_replace(vals, wregex(L"^ +(.*[^ ]) +$"), L"$1");

   recInfo->fieldLabels = vals;
   SetWindowText(hFieldLabels, regex_replace(vals, wregex(L","), L"\r\n").c_str());

   // Field Widths
   GetWindowText(hFieldWidths, fieldValues, (FW_LINE_MAX_LENGTH + 1));

   // Replace any newlines with commas.
   // No processing needed for leading & trailing spaces since this is a numeric edit control
   vals = regex_replace(fieldValues, wregex(L"\r\n"), L",");

   recInfo->fieldWidths = vals;

   cleanConfigFile = FALSE;
   cleanFieldVals = TRUE;
   enableRecSelection();
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
   if (cleanRecVals) return;

   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return;

   int idxRec{ getCurrentRecIndex() };
   if (idxRec == LB_ERR) return;

   RecordType &recInfo = vFileTypes[idxFT].vRecTypes[idxRec];

   wchar_t recDesc[MAX_PATH + 1];

   GetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_DESC_EDIT, recDesc, MAX_PATH + 1);
   recInfo.label = recDesc;

   char regexVal[MAX_PATH + 1];

   GetDlgItemTextA(_hSelf, IDC_FWVIZ_DEF_REC_REGEX_EDIT, regexVal, MAX_PATH + 1);
   recInfo.marker = regexVal;

   SendMessage(hRecsLB, LB_DELETESTRING, (WPARAM)idxRec, NULL);
   SendMessage(hRecsLB, LB_INSERTSTRING, (WPARAM)idxRec, (LPARAM)recDesc);
   SendMessage(hRecsLB, LB_SETCURSEL, idxRec, NULL);

   cleanConfigFile = FALSE;
   cleanRecVals = TRUE;
   enableRecSelection();
}

void ConfigureDialog::recEditNew() {
   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return;

   RecordType newRec{ getNewRec() };
   vector<RecordType> &records = vFileTypes[idxFT].vRecTypes;

   records.push_back(newRec);

   size_t moveTo = records.size() - 1;

   SendMessage(hRecsLB, LB_ADDSTRING, NULL, (LPARAM)newRec.label.c_str());
   SendMessage(hRecsLB, LB_SETCURSEL, (WPARAM)moveTo, NULL);
   onRecTypeSelect();

   cleanConfigFile = FALSE;
   cleanRecVals = FALSE;
   enableRecSelection();
}

int ConfigureDialog::recEditDelete() {
   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return LB_ERR;

   int idxRec{ getCurrentRecIndex() };
   if (idxRec == LB_ERR) return LB_ERR;

   vector<RecordType> &records = vFileTypes[idxFT].vRecTypes;
   records.erase(records.begin() + idxRec);

   int lastRec = static_cast<int>(records.size()) - 1;
   int moveTo = (idxRec <= lastRec - 1) ? idxRec : lastRec;

   SendMessage(hRecsLB, LB_DELETESTRING, (WPARAM)idxRec, NULL);
   SendMessage(hRecsLB, LB_SETCURSEL, (WPARAM)moveTo, NULL);

   cleanConfigFile = FALSE;
   cleanRecVals = TRUE;
   onRecTypeSelect();

   return moveTo;
}

void ConfigureDialog::fileEditAccept() {
   if (cleanFileVals) return;

   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return;

   FileType &fileInfo = vFileTypes[idxFT];

   wchar_t fileVal[MAX_PATH + 1];

   GetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_DESC_EDIT, fileVal, MAX_PATH + 1);
   fileInfo.label = fileVal;

   char eolVal[MAX_PATH + 1];

   GetDlgItemTextA(_hSelf, IDC_FWVIZ_DEF_FILE_EOL_EDIT, eolVal, MAX_PATH + 1);
   fileInfo.eol = eolVal;

   GetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LIST, fileVal, MAX_PATH + 1);
   fileInfo.theme = fileVal;

   SendMessage(hFilesLB, LB_DELETESTRING, (WPARAM)idxFT, NULL);
   SendMessage(hFilesLB, LB_INSERTSTRING, (WPARAM)idxFT, (LPARAM)fileInfo.label.c_str());
   SendMessage(hFilesLB, LB_SETCURSEL, idxFT, NULL);

   cleanConfigFile = FALSE;
   cleanFileVals = TRUE;
   enableFileSelection();
}

int ConfigureDialog::appendFileTypeConfigs(const wstring& sConfigFile) {
   wstring sections{};
   int sectionCount{};

   sectionCount = _configIO.getConfigSectionList(sections, sConfigFile);

   vector<wstring> sectionList{};
   sectionCount = _configIO.Tokenize(sections, sectionList);

   wstring sectionLabel{};
   int validCount{};
   for (int i{}; i < sectionCount; i++) {
      sectionLabel = _configIO.getConfigString(sectionList[i], L"FileLabel", L"", sConfigFile);
      if (sectionLabel.length() > 0) {
         FileType newFile{ getNewFileType() };

         vFileTypes.push_back(newFile);
         addConfigInfo(static_cast<int>(vFileTypes.size() - 1), sectionList[i], sConfigFile);
         SendMessage(hFilesLB, LB_ADDSTRING, NULL, (LPARAM)sectionLabel.c_str());
         validCount++;
      }
   }

   SendMessage(hFilesLB, LB_SETCURSEL, (WPARAM)(vFileTypes.size() - 1), NULL);
   onFileTypeSelect();

   cleanConfigFile = FALSE;
   enableFileSelection();

   return validCount;
}

void ConfigureDialog::fileEditNew() {
   FileType newFile{ getNewFileType() };

   vFileTypes.push_back(newFile);

   size_t moveTo = vFileTypes.size() - 1;

   SendMessage(hFilesLB, LB_ADDSTRING, NULL, (LPARAM)newFile.label.c_str());
   SendMessage(hFilesLB, LB_SETCURSEL, (WPARAM)moveTo, NULL);
   onFileTypeSelect();

   cleanConfigFile = FALSE;
   cleanFileVals = FALSE;
   enableFileSelection();
}

int ConfigureDialog::fileEditDelete() {
   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return LB_ERR;

   vFileTypes.erase(vFileTypes.begin() + idxFT);

   int lastFile = static_cast<int>(vFileTypes.size()) - 1;
   int moveTo = (idxFT <= lastFile - 1) ? idxFT : lastFile;

   SendMessage(hFilesLB, LB_DELETESTRING, (WPARAM)idxFT, NULL);
   SendMessage(hFilesLB, LB_SETCURSEL, (WPARAM)moveTo, NULL);

   cleanConfigFile = FALSE;
   cleanFileVals = TRUE;
   onFileTypeSelect();

   return moveTo;
}

bool ConfigureDialog::promptDiscardChangesNo() {
   if (!(cleanConfigFile && cleanFileVals && cleanRecVals && cleanFieldVals)) {
      if (MessageBox(_hSelf, FWVIZ_DISCARD_CHANGES, FWVIZ_DIALOG_TITLE,
         MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
         return TRUE;
   }

   return false;
}

void ConfigureDialog::saveConfigInfo() {
   if (!cleanFieldVals) fieldEditsAccept();
   if (!cleanRecVals) recEditAccept();
   if (!cleanFileVals) fileEditAccept();

   size_t fileTypeCount;
   wstring fileData{}, fileTypes{}, themes{}, ftCode{}, ftConfig{};

   themes = _configIO.getConfigString(L"Base", L"Themes");
   themes = themes.length() > 0 ? themes : L"VT_Basic";

   _configIO.backupMoveConfigFile();

   fileTypeCount = (vFileTypes.size() > 999) ? 999 : vFileTypes.size();

   for (size_t i{}; i < fileTypeCount; i++) {
      getFileTypeConfig(i, TRUE, ftCode, ftConfig);
      fileTypes += (i == 0 ? L"" : L",") + ftCode;
      fileData += ftConfig + L"\r\n";
   }

   fileData = L"[Base]\r\nFileTypes=" + fileTypes + L"\r\nThemes=" + themes + L"\r\n\r\n" + fileData;
   _configIO.saveConfigFile(fileData);

   cleanConfigFile = TRUE;
   indicateCleanStatus();
   RefreshVisualizerPanel();
}

void ConfigureDialog::showEximDialog(bool bExtract) {
   _eximDlg.doDialog((HINSTANCE)_gModule);
   _eximDlg.initDialog(bExtract);

   if (bExtract) {
      int idxFT{ getCurrentFileTypeIndex() };
      if (idxFT == LB_ERR) return;

      wstring ftCode{}, ftConfig{};
      getFileTypeConfig(idxFT, TRUE, ftCode, ftConfig);
      _eximDlg.setFileTypeData(ftConfig);
   }
}
