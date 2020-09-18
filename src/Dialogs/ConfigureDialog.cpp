#include "ConfigureDialog.h"

void ConfigureDialog::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_FWVIZ_DEFINER_DIALOG);
   }

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

   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_LIST_BOX, LB_RESETCONTENT, NULL, NULL);

   fTypes = fileInfoList.size();
   for (size_t i{}; i < fTypes; i++) {
      SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_LIST_BOX, LB_ADDSTRING, NULL,
         (LPARAM)fileInfoList[i].label.c_str());
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
      SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_LIST_BOX, LB_SETCURSEL, 0, NULL);
      onFileTypeSelect();
   }
}

bool ConfigureDialog::getCurrentFileInfo(FileInfo &fileInfo) {
   int idxFile;

   idxFile = static_cast<int>(SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_LIST_BOX, LB_GETCURSEL, NULL, NULL));
   if (idxFile == LB_ERR) return FALSE;

   fileInfo = fileInfoList[idxFile];
   return TRUE;
}

void ConfigureDialog::onFileTypeSelect() {
   FileInfo fileInfo;

   if (!getCurrentFileInfo(fileInfo)) return;

   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_DESC_EDIT, fileInfo.label.c_str());
   SetDlgItemTextA(_hSelf, IDC_FWVIZ_DEF_FILE_TERM_EDIT, fileInfo.eol.c_str());

   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LIST, CB_SETCURSEL, (WPARAM)
      SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LIST, CB_FINDSTRING, (WPARAM)-1,
         (LPARAM)fileInfo.theme.c_str()), NULL);

   fillRecTypes();
}

void ConfigureDialog::fillRecTypes() {
   FileInfo fileInfo;

   if (!getCurrentFileInfo(fileInfo)) return;

   vector <RecordInfo> &recInfoList = fileInfo.records;

   // Fill Rec Types Listbox
   size_t recTypes;

   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_REC_LIST_BOX, LB_RESETCONTENT, NULL, NULL);

   recTypes = recInfoList.size();

   for (size_t i{}; i < recTypes; i++) {
      SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_REC_LIST_BOX, LB_ADDSTRING, NULL,
         (LPARAM)recInfoList[i].label.c_str());
   }

   // Select first item
   if (recTypes > 0) {
      SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_REC_LIST_BOX, LB_SETCURSEL, 0, NULL);
      onRecTypeSelect();
   }
}

bool ConfigureDialog::getCurrentRecInfo(RecordInfo &recInfo) {
   int idxFile, idxRec;

   idxFile = static_cast<int>(SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_LIST_BOX, LB_GETCURSEL, NULL, NULL));
   if (idxFile == LB_ERR) return FALSE;

   idxRec = static_cast<int>(SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_REC_LIST_BOX, LB_GETCURSEL, NULL, NULL));
   if (idxRec == LB_ERR) return FALSE;

   recInfo = fileInfoList[idxFile].records[idxRec];
   return TRUE;
}

void ConfigureDialog::onRecTypeSelect() {
   RecordInfo recInfo;

   if (!getCurrentRecInfo(recInfo)) return;

   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_DESC_EDIT, recInfo.label.c_str());

   string regExpr = recInfo.marker;

   SetDlgItemTextA(_hSelf, IDC_FWVIZ_DEF_REC_REGEX_EDIT, regExpr.c_str());
   SetDlgItemTextA(_hSelf, IDC_FWVIZ_DEF_REC_START_EDIT,
      (regExpr.substr(0, 1) == "^") ? regExpr.substr(1).c_str() : "");

   fillFieldTypes();
}

void ConfigureDialog::fillFieldTypes() {
   RecordInfo recInfo;

   if (!getCurrentRecInfo(recInfo)) return;

   // Field Widths
   wstring fieldWidths{};

   fieldWidths += regex_replace(recInfo.fieldWidths, wregex(L","), L"\r\n");
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_WIDTHS_EDIT, fieldWidths.c_str());

   // Field Labels
   wstring fieldLabels{};

   fieldLabels += regex_replace(recInfo.fieldLabels, wregex(L","), L"\r\n");
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_LABELS_EDIT, fieldLabels.c_str());
}

