#include "FoldStructDialog.h"
#include "EximFileTypeDialog.h"

#pragma comment(lib, "comctl32.lib")

extern HINSTANCE _gModule;

extern EximFileTypeDialog _eximDlg;

void FoldStructDialog::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_FOLD_STRUCT_DEFINER_DIALOG);
   }

   hFoldStructs = GetDlgItem(_hSelf, IDC_FOLD_DEF_FILE_LIST_BOX);
   hFTList = GetDlgItem(_hSelf, IDC_FOLD_DEF_FILE_TYPE_LIST);
   hFoldBlocks = GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_LIST_BOX);
   hHdrRTList = GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_TYPE_LIST);
   hImplRecs = GetDlgItem(_hSelf, IDC_FOLD_IMPLICIT_TRMNTRS_LIST);
   hExplRecs = GetDlgItem(_hSelf, IDC_FOLD_EXPLICIT_TRMNTRS_LIST);
   hExplRTList = GetDlgItem(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_LIST);

   Utils::loadBitmap(_hSelf, IDC_FOLD_DEF_FILE_DOWN_BUTTON, IDB_VIZ_MOVE_DOWN_BITMAP);
   Utils::addTooltip(_hSelf, IDC_FOLD_DEF_FILE_DOWN_BUTTON, NULL, FOLD_DEF_FILE_MOVE_DOWN, FALSE);

   Utils::loadBitmap(_hSelf, IDC_FOLD_DEF_FILE_UP_BUTTON, IDB_VIZ_MOVE_UP_BITMAP);
   Utils::addTooltip(_hSelf, IDC_FOLD_DEF_FILE_UP_BUTTON, NULL, FOLD_DEF_FILE_MOVE_UP, FALSE);

   Utils::loadBitmap(_hSelf, IDC_FOLD_DEF_HDR_REC_DOWN_BTN, IDB_VIZ_MOVE_DOWN_BITMAP);
   Utils::addTooltip(_hSelf, IDC_FOLD_DEF_HDR_REC_DOWN_BTN, NULL, FOLD_DEF_HDR_REC_MOVE_DOWN, FALSE);

   Utils::loadBitmap(_hSelf, IDC_FOLD_DEF_HDR_REC_UP_BTN, IDB_VIZ_MOVE_UP_BITMAP);
   Utils::addTooltip(_hSelf, IDC_FOLD_DEF_HDR_REC_UP_BTN, NULL, FOLD_DEF_HDR_REC_MOVE_UP, FALSE);

   Utils::loadBitmap(_hSelf, IDC_FOLD_DEF_INFO_BUTTON, IDB_VIZ_INFO_BITMAP);
   Utils::addTooltip(_hSelf, IDC_FOLD_DEF_INFO_BUTTON, NULL, VIZ_PANEL_INFO_TIP, FALSE);

   if (_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   // Dynamically adjust the x position for the Fold Block Priority edit control
   HWND hPriorityLabel{ GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_PRIORITY_LABEL) };

   wchar_t priorityText[MAX_PATH + 1];
   GetWindowText(hPriorityLabel, priorityText, MAX_PATH);
   int textWidth{ Utils::getTextPixelWidth(_hSelf, priorityText) };

   RECT rcLabel{};
   GetClientRect(hPriorityLabel, &rcLabel);
   MapWindowPoints(hPriorityLabel, _hSelf, (LPPOINT)&rcLabel, 2);

   HWND hPriorityEdit{ GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_PRIORITY_EDIT) };
   RECT rcEdit{};
   GetClientRect(hPriorityEdit, &rcEdit);
   MapWindowPoints(hPriorityEdit, _hSelf, (LPPOINT)&rcEdit, 2);

   SetWindowPos(hPriorityEdit, NULL, rcLabel.left + textWidth, rcEdit.top, NULL, NULL, SWP_NOSIZE | SWP_NOZORDER);

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);

   loadFileTypesList();
   loadStructsInfo();
   fillFoldStructs();
}

void FoldStructDialog::refreshDarkMode() {
   NPPDM_AutoThemeChildControls(_hSelf);
   redraw();

   if (_eximDlg.isCreated())
      _eximDlg.refreshDarkMode();
}

INT_PTR CALLBACK FoldStructDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
   switch (message) {
   case WM_COMMAND:
      switch LOWORD(wParam) {
      case IDC_FOLD_DEF_FILE_LIST_BOX:
         switch HIWORD(wParam) {
         case LBN_SELCHANGE:
            onFoldStructSelect();
            break;
         }
         break;

      case IDC_FOLD_DEF_FILE_DOWN_BUTTON:
         moveStructType(MOVE_DOWN);
         break;

      case IDC_FOLD_DEF_FILE_UP_BUTTON:
         moveStructType(MOVE_UP);
         break;

      case IDC_FOLD_DEF_FILE_TYPE_LIST:
      case IDC_FOLD_DEF_AUTO_FOLD_APPLY:
         switch HIWORD(wParam) {
         case BN_CLICKED:
         case CBN_SELCHANGE:
            if (!loadingEdits) {
               cleanStructVals = FALSE;
               enableStructSelection();
            }
            break;
         }
         break;

      case IDC_FOLD_DEF_FILE_ACCEPT_BTN:
      case IDC_FOLD_DEF_FILE_RESET_BTN:
         structEditAccept(LOWORD(wParam) == IDC_FOLD_DEF_FILE_ACCEPT_BTN);
         break;

      case IDC_FOLD_DEF_FILE_NEW_BTN:
         structEditNew();
         break;

      case IDC_FOLD_DEF_FILE_CLONE_BTN:
         structEditClone();
         break;

      case IDC_FOLD_DEF_FILE_DEL_BTN:
         structEditDelete();
         break;

      case IDC_FOLD_DEF_HDR_REC_LIST_BOX:
         switch HIWORD(wParam) {
         case LBN_SELCHANGE:
            onFoldBlockSelect();
            break;
         }
         break;

      case IDC_FOLD_DEF_HDR_REC_DOWN_BTN:
         moveBlockType(MOVE_DOWN);
         break;

      case IDC_FOLD_DEF_HDR_REC_UP_BTN:
         moveBlockType(MOVE_UP);
         break;

      case IDC_FOLD_DEF_HDR_REC_TYPE_LIST:
         switch HIWORD(wParam) {
         case CBN_SELCHANGE:
            if (!loadingEdits) {
               cleanBlockVals = FALSE;
               enableBlockSelection();
            }
            break;
         }
         break;

      case IDC_FOLD_DEF_HDR_PRIORITY_EDIT:
      case IDC_FOLD_DEF_HDR_REC_RECURSIVE:
         switch HIWORD(wParam) {
         case BN_CLICKED:
         case EN_CHANGE:
            if (!loadingEdits) {
               cleanBlockVals = FALSE;
               fillImplicitEndRecs();
               enableBlockSelection();
            }
            break;
         }
         break;

      case IDC_FOLD_DEF_HDR_REC_ACCEPT_BTN:
      case IDC_FOLD_DEF_HDR_REC_RESET_BTN:
         blockEditAccept(LOWORD(wParam) == IDC_FOLD_DEF_HDR_REC_ACCEPT_BTN);
         fillImplicitEndRecs();
         break;

      case IDC_FOLD_DEF_HDR_REC_NEW_BTN:
         blockEditNew(FALSE);
         break;

      case IDC_FOLD_DEF_HDR_REC_CLONE_BTN:
         blockEditNew(TRUE);
         break;

      case IDC_FOLD_DEF_HDR_REC_DEL_BTN:
         blockEditDelete();
         break;

      case IDC_FOLD_EXPLICIT_TRMNTRS_LIST:
         switch HIWORD(wParam) {
         case LBN_SELCHANGE:
            onEndRecSelect();
            break;
         }
         break;

      case IDC_FOLD_EXPLICIT_ENDREC_LIST:
         switch HIWORD(wParam) {
         case CBN_SELCHANGE:
            if (!loadingEdits) {
               cleanEndRecVals = FALSE;
               enableEndRecSelection();
            }
            break;
         }
         break;

      case IDC_FOLD_EXPLICIT_ENDREC_ACCEPT:
      case IDC_FOLD_EXPLICIT_ENDREC_RESET:
         endRecEditAccept(LOWORD(wParam) == IDC_FOLD_EXPLICIT_ENDREC_ACCEPT);
         break;

      case IDC_FOLD_EXPLICIT_ENDREC_NEW:
         endRecEditNew();
         break;

      case IDC_FOLD_EXPLICIT_ENDREC_DEL:
         endRecEditDelete();
         break;

      case IDCANCEL:
      case IDCLOSE:
         if (promptDiscardChangesNo()) return TRUE;

         display(FALSE);
         return TRUE;

      case IDC_FOLD_DEF_SAVE_BTN:
         SetCursor(LoadCursor(NULL, IDC_WAIT));
         saveFoldStructInfo();
         SetCursor(LoadCursor(NULL, IDC_ARROW));
         return TRUE;

      case IDC_FOLD_DEF_RESET_BTN:
         if (!promptDiscardChangesNo()) {
            structsFile = L"";
            loadStructsInfo();
            fillFoldStructs();
         }
         break;

      case IDC_FOLD_DEF_BACKUP_LOAD_BTN:
         if (!promptDiscardChangesNo()) {
            wstring backupStructsFile;

            if (_configIO.queryConfigFileName(_hSelf, TRUE, TRUE, backupStructsFile)) {
               if (_configIO.fixIfNotUTF8File(backupStructsFile)) {
                  structsFile = backupStructsFile;
                  loadStructsInfo();
                  fillFoldStructs();
                  cleanStructsFile = FALSE;
                  enableStructSelection();
               }
            }
         }
         break;

      case IDC_FOLD_DEF_BACKUP_VIEW_BTN:
         _configIO.viewBackupFolder();
         break;

      case IDC_FOLD_DEF_EXTRACT_BTN:
         showEximDialog(TRUE);
         break;

      case IDC_FOLD_DEF_APPEND_BTN:
         showEximDialog(FALSE);
         break;
      }
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

void FoldStructDialog::localize() {
   SetWindowText(_hSelf, FOLD_STRUCT_DIALOG_TITLE);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_FILE_GROUP_BOX, FOLD_DEF_FILE_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_FILE_TYPE_LABEL, FOLD_DEF_FILE_TYPE_LABEL);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_AUTO_FOLD_APPLY, FOLD_DEF_AUTO_FOLD_APPLY);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_FILE_ACCEPT_BTN, FOLD_DEF_FILE_ACCEPT_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_FILE_NEW_BTN, FOLD_DEF_FILE_NEW_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_FILE_RESET_BTN, FOLD_DEF_FILE_RESET_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_FILE_CLONE_BTN, FOLD_DEF_FILE_CLONE_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_FILE_DEL_BTN, FOLD_DEF_FILE_DEL_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_GROUP_BOX, FOLD_DEF_HDR_REC_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_TYPE_LABEL, FOLD_DEF_HDR_REC_TYPE_LABEL);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_PRIORITY_LABEL, FOLD_DEF_HDR_PRIORITY_LABEL);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_RECURSIVE, FOLD_DEF_HDR_REC_RECURSIVE);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_ACCEPT_BTN, FOLD_DEF_HDR_REC_ACCEPT_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_NEW_BTN, FOLD_DEF_HDR_REC_NEW_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_RESET_BTN, FOLD_DEF_HDR_REC_RESET_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_CLONE_BTN, FOLD_DEF_HDR_REC_CLONE_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_DEL_BTN, FOLD_DEF_HDR_REC_DEL_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_IMPLICIT_TRMNTRS_GROUP, FOLD_IMPLICIT_TRMNTRS_GROUP);
   SetDlgItemText(_hSelf, IDC_FOLD_EXPLICIT_TRMNTRS_GROUP, FOLD_EXPLICIT_TRMNTRS_GROUP);
   SetDlgItemText(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_LABEL, FOLD_EXPLICIT_ENDREC_LABEL);
   SetDlgItemText(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_ACCEPT, FOLD_EXPLICIT_ENDREC_ACCEPT);
   SetDlgItemText(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_NEW, FOLD_EXPLICIT_ENDREC_NEW);
   SetDlgItemText(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_RESET, FOLD_EXPLICIT_ENDREC_RESET);
   SetDlgItemText(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_DEL, FOLD_EXPLICIT_ENDREC_DEL);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_SAVE_BTN, FOLD_DEF_SAVE_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_RESET_BTN, FOLD_DEF_RESET_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_BACKUP_LOAD_BTN, FOLD_DEF_BACKUP_LOAD_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_BACKUP_VIEW_BTN, FOLD_DEF_BACKUP_VIEW_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_EXTRACT_BTN, FOLD_DEF_EXTRACT_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_APPEND_BTN, FOLD_DEF_APPEND_BTN);
}

void FoldStructDialog::indicateCleanStatus() {
   if (cleanStructsFile) {
      SetDlgItemText(_hSelf, IDC_FOLD_DEF_SAVE_BTN, FOLD_DEF_SAVE_BTN);
      Utils::setFontRegular(_hSelf, IDC_FOLD_DEF_SAVE_BTN);
   }
   else {
      SetDlgItemText(_hSelf, IDC_FOLD_DEF_SAVE_BTN, (wstring(FOLD_DEF_SAVE_BTN) + L"*").c_str());
      Utils::setFontBold(_hSelf, IDC_FOLD_DEF_SAVE_BTN);
   }

   if (cleanStructVals) {
      SetDlgItemText(_hSelf, IDC_FOLD_DEF_FILE_ACCEPT_BTN, FOLD_DEF_FILE_ACCEPT_BTN);
      Utils::setFontRegular(_hSelf, IDC_FOLD_DEF_FILE_ACCEPT_BTN);
   }
   else {
      SetDlgItemText(_hSelf, IDC_FOLD_DEF_FILE_ACCEPT_BTN, (wstring(FOLD_DEF_FILE_ACCEPT_BTN) + L"*").c_str());
      Utils::setFontBold(_hSelf, IDC_FOLD_DEF_FILE_ACCEPT_BTN);
   }

   if (cleanBlockVals) {
      SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_ACCEPT_BTN, FOLD_DEF_HDR_REC_ACCEPT_BTN);
      Utils::setFontRegular(_hSelf, IDC_FOLD_DEF_HDR_REC_ACCEPT_BTN);
   }
   else {
      SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_ACCEPT_BTN, (wstring(FOLD_DEF_HDR_REC_ACCEPT_BTN) + L"*").c_str());
      Utils::setFontBold(_hSelf, IDC_FOLD_DEF_HDR_REC_ACCEPT_BTN);
   }

   if (cleanEndRecVals) {
      SetDlgItemText(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_ACCEPT, FOLD_EXPLICIT_ENDREC_ACCEPT);
      Utils::setFontRegular(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_ACCEPT);
   }
   else {
      SetDlgItemText(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_ACCEPT, (wstring(FOLD_EXPLICIT_ENDREC_ACCEPT) + L"*").c_str());
      Utils::setFontBold(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_ACCEPT);
   }
}

void FoldStructDialog::fillFoldStructs() {
   SendMessage(hFoldStructs, LB_RESETCONTENT, NULL, NULL);

   for (size_t i{}; i < vFoldStructs.size(); ++i) {
      SendMessage(hFoldStructs, LB_ADDSTRING, NULL, (LPARAM)vFoldStructs[i].fileType.label.c_str());
   }

   if (vFoldStructs.size())
      SendMessage(hFoldStructs, LB_SETCURSEL, 0, 0);

   cleanStructsFile = TRUE;
   cleanStructVals = TRUE;
   onFoldStructSelect();
}

int FoldStructDialog::loadFileTypesList() {
   vector<string> fileTypes;
   int ftCount{ _configIO.getConfigValueList(fileTypes, "Base", "FileTypes") };

   vFileTypes.clear();
   vFileTypes.resize(ftCount);
   SendMessage(hFTList, CB_RESETCONTENT, NULL, NULL);

   for (int i{}; i < ftCount; ++i) {
      wstring fileLabel{ _configIO.getConfigWideChar(fileTypes[i], "FileLabel") };

      vFileTypes[i].type = fileTypes[i];
      vFileTypes[i].label = fileLabel;
      SendMessage(hFTList, CB_ADDSTRING, NULL, (LPARAM)fileLabel.c_str());
   }

   return ftCount;
}

int FoldStructDialog::loadRecTypesList(string fileType) {
   vector<string> recTypesList;
   int recTypeCount{ _configIO.getConfigValueList(recTypesList, fileType, "RecordTypes") };

   vRecTypes.clear();
   vRecTypes.resize(recTypeCount);

   for (int i{}; i < recTypeCount; ++i) {
      vRecTypes[i].type = recTypesList[i];
      vRecTypes[i].label = _configIO.getConfigWideChar(fileType, (recTypesList[i] + "_Label"));
   }

   return recTypeCount;
}

int FoldStructDialog::loadStructsInfo() {
   int foldStructCount{ _configIO.getFoldStructCount() };

   vFoldStructs.clear();
   vFoldStructs.resize(foldStructCount);

   for (int i{}; i < foldStructCount; ++i) {
      loadFoldStructInfo(i, structsFile);
   }

   return static_cast<int>(vFoldStructs.size());
}

int FoldStructDialog::loadFoldStructInfo(int vIndex, const wstring& sStructsFile) {
   string fsType(6, '\0');
   snprintf(fsType.data(), 6, "FS%03d", vIndex + 1);

   FoldStructInfo& FS{ vFoldStructs[vIndex] };
   TypeInfo& FT{ FS.fileType };

   FT.type = _configIO.getFoldStructValueA(fsType, "FileType", sStructsFile);

   FT.label = _configIO.getConfigWideChar(FT.type, "FileLabel");
   if (FT.label.empty()) FT.label = Utils::NarrowToWide(FT.type);

   FS.autoFold = (_configIO.getFoldStructValueA(fsType, "FoldLevelAuto", sStructsFile) == "Y");

   loadRecTypesList(FT.type);

   string headerRecs{ _configIO.getFoldStructValueA(fsType, "HeaderRecords", sStructsFile) };
   vector<string> headerRecList{};
   int headerCount{ _configIO.Tokenize(headerRecs, headerRecList) };
   int recTypeCount{ static_cast<int>(vRecTypes.size()) };

   vector<BlockInfo>& BI{ FS.vBlocks };
   BI.clear();
   BI.resize(headerCount);

   for (int i{}; i < headerCount; ++i) {
      int hdrIndex{ Utils::StringtoInt(headerRecList[i].substr(3)) - 1 };
      BI[i].hdrRec.type = headerRecList[i];
      BI[i].hdrRec.label = (hdrIndex >= recTypeCount) ? Utils::NarrowToWide(headerRecList[i]) : vRecTypes[hdrIndex].label;

      BI[i].priority = Utils::StringtoInt(_configIO.getFoldStructValueA(fsType, headerRecList[i] + "_Priority", sStructsFile));
      BI[i].recursive = (_configIO.getFoldStructValueA(fsType, headerRecList[i] + "_Recursive", sStructsFile) == "Y");

      string endRecs{ _configIO.getFoldStructValueA(fsType, headerRecList[i] + "_EndRecords", sStructsFile) };
      vector<string> endRecList{};
      int endRecsCount{ _configIO.Tokenize(endRecs, endRecList) };

      vector<TypeInfo>& ER{ BI[i].vEndRecs };
      ER.clear();
      ER.resize(endRecsCount);

      for (int j{}; j < endRecsCount; ++j) {
         int endRecIndex{ Utils::StringtoInt(endRecList[j].substr(3)) - 1 };
         ER[j].type = endRecList[j];
         ER[j].label = (endRecIndex >= recTypeCount) ? Utils::NarrowToWide(endRecList[j]) : vRecTypes[endRecIndex].label;
      }
   }

   return headerCount;
}

void FoldStructDialog::onFoldStructSelect() {
   FoldStructInfo* fsInfo;

   if (!getCurrentFoldStructInfo(fsInfo)) {
      FoldStructInfo newFile{};
      fsInfo = &newFile;
   }

   loadingEdits = TRUE;

   Utils::setComboBoxSelection(hFTList, static_cast<int>(
      SendMessage(hFTList, CB_FINDSTRING, (WPARAM)-1, (LPARAM)fsInfo->fileType.label.c_str())));
   CheckDlgButton(_hSelf, IDC_FOLD_DEF_AUTO_FOLD_APPLY, fsInfo->autoFold ? BST_CHECKED : BST_UNCHECKED);

   loadingEdits = FALSE;

   enableMoveStructButtons();
   fillFoldBlocks();
}

void FoldStructDialog::enableMoveStructButtons() {
   int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return;

   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_FILE_DOWN_BUTTON),
      (idxFS < static_cast<int>(vFoldStructs.size()) - 1));
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_FILE_UP_BUTTON), (idxFS > 0));
}

void FoldStructDialog::enableStructSelection() {
   bool enable{ cleanStructVals && cleanBlockVals && cleanEndRecVals };
   bool structRecsExist{ SendMessage(hFoldStructs, LB_GETCOUNT, 0, 0) > 0 };
   bool fileTypesExist{ vFileTypes.size() > 0 };

   ShowWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_FILE_NEW_BTN), enable ? SW_SHOW : SW_HIDE);
   ShowWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_FILE_RESET_BTN), enable ? SW_HIDE : SW_SHOW);

   EnableWindow(hFoldStructs, enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_AUTO_FOLD_APPLY), fileTypesExist);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_FILE_ACCEPT_BTN), fileTypesExist);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_FILE_NEW_BTN), fileTypesExist && enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_FILE_CLONE_BTN), fileTypesExist && enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_FILE_DEL_BTN), structRecsExist);

   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_EXTRACT_BTN), enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_APPEND_BTN), enable);

   if (enable) {
      enableMoveStructButtons();
   }
   else {
      EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_FILE_DOWN_BUTTON), FALSE);
      EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_FILE_UP_BUTTON), FALSE);
   }

   indicateCleanStatus();
}

int FoldStructDialog::moveStructType(move_dir dir) {
   const int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return LB_ERR;

   switch (dir) {
   case MOVE_DOWN:
      if (idxFS >= static_cast<int>(vFoldStructs.size()) - 1) return LB_ERR;
      break;

   case MOVE_UP:
      if (idxFS == 0) return LB_ERR;
      break;

   default:
      return LB_ERR;
   }

   FoldStructInfo currStruct{ vFoldStructs[idxFS] };
   FoldStructInfo& adjStruct{ vFoldStructs[idxFS + dir] };

   vFoldStructs[idxFS] = adjStruct;
   vFoldStructs[idxFS + dir] = currStruct;

   SendMessage(hFoldStructs, LB_DELETESTRING, (WPARAM)idxFS, NULL);
   SendMessage(hFoldStructs, LB_INSERTSTRING, (WPARAM)(idxFS + dir),
      (LPARAM)vFoldStructs[idxFS + dir].fileType.label.c_str());
   SendMessage(hFoldStructs, LB_SETCURSEL, idxFS + dir, NULL);

   cleanStructsFile = FALSE;
   indicateCleanStatus();
   enableMoveStructButtons();

   return idxFS + dir;
}

int FoldStructDialog::structEditAccept(bool accept) {
   if (cleanStructVals) return 0;

   int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return -1;

   FoldStructInfo& fsInfo{ vFoldStructs[idxFS] };

   if (accept) {
      int index{ static_cast<int>(SendMessage(hFTList, CB_GETCURSEL, 0, 0)) };
      if (index >= static_cast<int>(vFileTypes.size())) return -1;

      fsInfo.fileType.type = vFileTypes[index].type;
      fsInfo.fileType.label = vFileTypes[index].label;
      fsInfo.autoFold = (IsDlgButtonChecked(_hSelf, IDC_FOLD_DEF_AUTO_FOLD_APPLY) == BST_CHECKED);
   }
   else if (fsInfo.fileType.label.empty()) {
      structEditDelete();
      return 0;
   }
   else {
      Utils::setComboBoxSelection(hFTList, static_cast<int>(
         SendMessage(hFTList, CB_FINDSTRING, (WPARAM)-1, (LPARAM)fsInfo.fileType.label.c_str())));
      CheckDlgButton(_hSelf, IDC_FOLD_DEF_AUTO_FOLD_APPLY, fsInfo.autoFold ? BST_CHECKED : BST_UNCHECKED);
   }

   // Update FT Listbox Entry
   SendMessage(hFoldStructs, LB_DELETESTRING, (WPARAM)idxFS, NULL);
   SendMessage(hFoldStructs, LB_INSERTSTRING, (WPARAM)idxFS, (LPARAM)fsInfo.fileType.label.c_str());
   SendMessage(hFoldStructs, LB_SETCURSEL, idxFS, NULL);

   cleanStructsFile = FALSE;
   cleanStructVals = TRUE;
   enableStructSelection();

   return 1;
}

void FoldStructDialog::structEditNew() {
   FoldStructInfo newFS{};

   vFoldStructs.push_back(newFS);

   size_t moveTo = vFoldStructs.size() - 1;

   SendMessage(hFoldStructs, LB_ADDSTRING, NULL, (LPARAM)newFS.fileType.label.c_str());
   SendMessage(hFoldStructs, LB_SETCURSEL, (WPARAM)moveTo, NULL);
   onFoldStructSelect();

   cleanStructsFile = FALSE;
   cleanStructVals = FALSE;
   enableStructSelection();
}

void FoldStructDialog::structEditClone() {
   int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return;

   FoldStructInfo& FS{ vFoldStructs[idxFS] };
   FoldStructInfo NF{};

   NF.fileType = FS.fileType;
   NF.autoFold = FS.autoFold;

   // Block Info
   size_t blockCount = FS.vBlocks.size();
   NF.vBlocks.resize(blockCount);

   for (size_t i{}; i < blockCount; ++i) {
      BlockInfo& NFBI{ NF.vBlocks[i] };
      BlockInfo& FSBI{ FS.vBlocks[i] };
      NFBI.hdrRec = FSBI.hdrRec;
      NFBI.priority = FSBI.priority;
      NFBI.recursive = FSBI.recursive;

      // End Recs Info
      size_t endRecsCount = FSBI.vEndRecs.size();
      NFBI.vEndRecs.resize(endRecsCount);

      for (size_t j{}; j < endRecsCount; ++j) {
         NFBI.vEndRecs[j] = FSBI.vEndRecs[j];
      }
   }

   vFoldStructs.push_back(NF);

   SendMessage(hFoldStructs, LB_ADDSTRING, NULL, (LPARAM)NF.fileType.label.c_str());
   SendMessage(hFoldStructs, LB_SETCURSEL, (WPARAM)(vFoldStructs.size() - 1), NULL);
   onFoldStructSelect();

   cleanStructsFile = FALSE;
   cleanStructVals = FALSE;
   enableStructSelection();
}

int FoldStructDialog::structEditDelete() {
   int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return LB_ERR;

   vFoldStructs.erase(vFoldStructs.begin() + idxFS);

   int lastFile = static_cast<int>(vFoldStructs.size()) - 1;
   int moveTo = (idxFS <= lastFile - 1) ? idxFS : lastFile;

   SendMessage(hFoldStructs, LB_DELETESTRING, (WPARAM)idxFS, NULL);
   SendMessage(hFoldStructs, LB_SETCURSEL, (WPARAM)moveTo, NULL);

   cleanStructsFile = FALSE;
   cleanStructVals = TRUE;
   onFoldStructSelect();

   return moveTo;
}

void FoldStructDialog::fillFoldBlocks() {
   FoldStructInfo* fsInfo;

   if (!getCurrentFoldStructInfo(fsInfo)) {
      FoldStructInfo newFs{};
      fsInfo = &newFs;
   }

   vector <BlockInfo>& blockInfoList{ fsInfo->vBlocks };

   // Fill Fold Blocks Listbox
   SendMessage(hFoldBlocks, LB_RESETCONTENT, NULL, NULL);

   for (const auto BI : blockInfoList) {
      SendMessage(hFoldBlocks, LB_ADDSTRING, NULL, (LPARAM)BI.hdrRec.label.c_str());
   }

   if (blockInfoList.size())
      SendMessage(hFoldBlocks, LB_SETCURSEL, 0, NULL);

   // Fill Header Rec Types Listbox
   loadRecTypesList(fsInfo->fileType.type);
   SendMessage(hHdrRTList, CB_RESETCONTENT, NULL, NULL);
   SendMessage(hExplRTList, CB_RESETCONTENT, NULL, NULL);

   for (const auto RT : vRecTypes) {
      SendMessage(hHdrRTList, CB_ADDSTRING, NULL, (LPARAM)RT.label.c_str());
      SendMessage(hExplRTList, CB_ADDSTRING, NULL, (LPARAM)RT.label.c_str());
   }

   cleanBlockVals = TRUE;
   onFoldBlockSelect();
}

void FoldStructDialog::onFoldBlockSelect() {
   BlockInfo* blockInfo;

   if (!getCurrentBlockInfo(blockInfo)) {
      BlockInfo newBlock{};
      blockInfo = &newBlock;
   }

   loadingEdits = TRUE;

   Utils::setComboBoxSelection(hHdrRTList, static_cast<int>(
      SendMessage(hHdrRTList, CB_FINDSTRING, (WPARAM)-1, (LPARAM)blockInfo->hdrRec.label.c_str())));
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_PRIORITY_EDIT, to_wstring(blockInfo->priority).c_str());
   CheckDlgButton(_hSelf, IDC_FOLD_DEF_HDR_REC_RECURSIVE, blockInfo->recursive ? BST_CHECKED : BST_UNCHECKED);

   loadingEdits = FALSE;

   enableMoveBlockButtons();
   fillImplicitEndRecs();
   fillExplicitEndRecs(blockInfo);
}

void FoldStructDialog::enableMoveBlockButtons() {
   int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return;

   int idxBlock{ getCurrentBlockIndex() };
   if (idxBlock == LB_ERR) return;

   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_DOWN_BTN),
      (idxBlock < static_cast<int>(vFoldStructs[idxFS].vBlocks.size()) - 1));
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_UP_BTN), (idxBlock > 0));
}

void FoldStructDialog::enableBlockSelection() {
   bool enable{ cleanBlockVals && cleanEndRecVals };
   bool blockRecsExist{ SendMessage(hFoldBlocks, LB_GETCOUNT, 0, 0) > 0 };
   bool recTypesExist{ vRecTypes.size() > 0 };

   ShowWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_NEW_BTN), cleanBlockVals ? SW_SHOW : SW_HIDE);
   ShowWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_RESET_BTN), cleanBlockVals ? SW_HIDE : SW_SHOW);

   EnableWindow(hFoldBlocks, enable);
   EnableWindow(hHdrRTList, blockRecsExist && recTypesExist);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_PRIORITY_EDIT), blockRecsExist && recTypesExist);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_RECURSIVE), blockRecsExist && recTypesExist);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_ACCEPT_BTN), blockRecsExist && recTypesExist);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_NEW_BTN), recTypesExist && enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_CLONE_BTN), blockRecsExist && recTypesExist && enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_DEL_BTN), blockRecsExist);

   if (enable) {
      enableMoveBlockButtons();
   }
   else {
      EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_DOWN_BTN), FALSE);
      EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_UP_BTN), FALSE);
   }

   enableStructSelection();
}

int FoldStructDialog::moveBlockType(move_dir dir) {
   const int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return LB_ERR;

   const int idxBlock{ getCurrentBlockIndex() };
   if (idxBlock == LB_ERR) return LB_ERR;

   vector<BlockInfo>& blockList{ vFoldStructs[idxFS].vBlocks };

   switch (dir) {
   case MOVE_DOWN:
      if (idxBlock >= static_cast<int>(blockList.size()) - 1) return LB_ERR;
      break;

   case MOVE_UP:
      if (idxBlock == 0) return LB_ERR;
      break;

   default:
      return LB_ERR;
   }

   BlockInfo currType{ blockList[idxBlock] };
   BlockInfo& adjType{ blockList[idxBlock + dir] };

   blockList[idxBlock] = adjType;
   blockList[idxBlock + dir] = currType;

   SendMessage(hFoldBlocks, LB_DELETESTRING, (WPARAM)idxBlock, NULL);
   SendMessage(hFoldBlocks, LB_INSERTSTRING, (WPARAM)(idxBlock + dir),
      (LPARAM)blockList[idxBlock + dir].hdrRec.label.c_str());
   SendMessage(hFoldBlocks, LB_SETCURSEL, idxBlock + dir, NULL);

   cleanStructsFile = FALSE;
   indicateCleanStatus();
   enableMoveBlockButtons();

   return idxBlock + dir;
}

int FoldStructDialog::blockEditAccept(bool accept) {
   if (cleanBlockVals) return 0;

   int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return LB_ERR;

   int idxBlock{ getCurrentBlockIndex() };
   if (idxBlock == LB_ERR) return LB_ERR;

   BlockInfo& BI{ vFoldStructs[idxFS].vBlocks[idxBlock] };

   if (accept) {
      int index{ static_cast<int>(SendMessage(hHdrRTList, CB_GETCURSEL, 0, 0)) };
      if (index >= static_cast<int>(vRecTypes.size())) return -1;

      BI.hdrRec.type = vRecTypes[index].type;
      BI.hdrRec.label = vRecTypes[index].label;
      BI.priority = static_cast<int>(GetDlgItemInt(_hSelf, IDC_FOLD_DEF_HDR_PRIORITY_EDIT, NULL, FALSE));
      BI.recursive = (IsDlgButtonChecked(_hSelf, IDC_FOLD_DEF_HDR_REC_RECURSIVE) == BST_CHECKED);
   }
   else if (BI.hdrRec.label.empty()) {
      blockEditDelete();
      return 0;
   }
   else {
      Utils::setComboBoxSelection(hHdrRTList, static_cast<int>(
         SendMessage(hHdrRTList, CB_FINDSTRING, (WPARAM)-1, (LPARAM)BI.hdrRec.label.c_str())));
      SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_PRIORITY_EDIT, to_wstring(BI.priority).c_str());
      CheckDlgButton(_hSelf, IDC_FOLD_DEF_HDR_REC_RECURSIVE, BI.recursive ? BST_CHECKED : BST_UNCHECKED);
   }

   SendMessage(hFoldBlocks, LB_DELETESTRING, (WPARAM)idxBlock, NULL);
   SendMessage(hFoldBlocks, LB_INSERTSTRING, (WPARAM)idxBlock, (LPARAM)BI.hdrRec.label.c_str());
   SendMessage(hFoldBlocks, LB_SETCURSEL, idxBlock, NULL);

   cleanStructsFile = FALSE;
   cleanBlockVals = TRUE;
   enableBlockSelection();

   return 1;
}

void FoldStructDialog::blockEditNew(bool clone) {
   int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return;

   int idxBlock{ getCurrentBlockIndex() };
   if (clone && idxBlock == LB_ERR) return;

   vector<BlockInfo>& blocks{ vFoldStructs[idxFS].vBlocks };
   BlockInfo NB{};

   if (clone) {
      BlockInfo& BI{ blocks[idxBlock] };
      NB.hdrRec = BI.hdrRec;
      NB.priority = BI.priority;
      NB.recursive = BI.recursive;

      size_t endRecCount{ BI.vEndRecs.size() };
      NB.vEndRecs.resize(endRecCount);
      for (size_t i{}; i < endRecCount; ++i) {
         NB.vEndRecs[i] = BI.vEndRecs[i];
      }
   }

   blocks.push_back(NB);
   size_t moveTo = blocks.size() - 1;

   SendMessage(hFoldBlocks, LB_ADDSTRING, NULL, (LPARAM)NB.hdrRec.label.c_str());
   SendMessage(hFoldBlocks, LB_SETCURSEL, (WPARAM)moveTo, NULL);
   onFoldBlockSelect();

   cleanStructsFile = FALSE;
   cleanBlockVals = clone;
   enableBlockSelection();
}

int FoldStructDialog::blockEditDelete() {
   int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return LB_ERR;

   int idxBlock{ getCurrentBlockIndex() };
   if (idxBlock == LB_ERR) return LB_ERR;

   vector<BlockInfo>& blocks{ vFoldStructs[idxFS].vBlocks };
   blocks.erase(blocks.begin() + idxBlock);

   int lastRec = static_cast<int>(blocks.size()) - 1;
   int moveTo = (idxBlock <= lastRec - 1) ? idxBlock : lastRec;

   SendMessage(hFoldBlocks, LB_DELETESTRING, (WPARAM)idxBlock, NULL);
   SendMessage(hFoldBlocks, LB_SETCURSEL, (WPARAM)moveTo, NULL);

   cleanStructsFile = FALSE;
   cleanBlockVals = TRUE;
   onFoldBlockSelect();

   return moveTo;
}

void FoldStructDialog::fillImplicitEndRecs() {
   FoldStructInfo* fsInfo;

   if (!getCurrentFoldStructInfo(fsInfo)) {
      FoldStructInfo newFs{};
      fsInfo = &newFs;
   }

   int recTypeIndex{ static_cast<int>(SendMessage(hHdrRTList, CB_GETCURSEL, 0, 0)) };
   int priority{ static_cast<int>(GetDlgItemInt(_hSelf, IDC_FOLD_DEF_HDR_PRIORITY_EDIT, NULL, FALSE)) };
   bool recursive{ IsDlgButtonChecked(_hSelf, IDC_FOLD_DEF_HDR_REC_RECURSIVE) == BST_CHECKED };
   int threshold{ priority - (recursive ? 1 : 0) };

   vector <BlockInfo>& blockInfoList{ fsInfo->vBlocks };
   SendMessage(hImplRecs, LB_RESETCONTENT, NULL, NULL);

   for (const auto BI : blockInfoList) {
      if (BI.hdrRec.label.empty()) continue;
      if (recursive && (BI.hdrRec.type == vRecTypes[recTypeIndex].type)) continue;
      if (BI.priority > threshold) continue;

      SendMessage(hImplRecs, LB_ADDSTRING, NULL, (LPARAM)BI.hdrRec.label.c_str());
   }

   SendMessage(hImplRecs, LB_ADDSTRING, NULL, (LPARAM)FOLD_IMPLICIT_END_OF_FILE);
}

void FoldStructDialog::fillExplicitEndRecs(BlockInfo* blockInfo) {
   SendMessage(hExplRecs, LB_RESETCONTENT, NULL, NULL);

   for (const auto ER : blockInfo->vEndRecs) {
      SendMessage(hExplRecs, LB_ADDSTRING, NULL, (LPARAM)ER.label.c_str());
   }

   if (blockInfo->vEndRecs.size())
      SendMessage(hExplRecs, LB_SETCURSEL, 0, NULL);

   onEndRecSelect();

   cleanEndRecVals = TRUE;
   enableBlockSelection();
}

void FoldStructDialog::onEndRecSelect() {
   loadingEdits = TRUE;

   Utils::setComboBoxSelection(hExplRTList, static_cast<int>(
      SendMessage(hExplRTList, CB_FINDSTRING, (WPARAM)-1, (LPARAM)Utils::getListBoxItem(hExplRecs).c_str())));

   loadingEdits = FALSE;
   enableEndRecSelection();
}

void FoldStructDialog::enableEndRecSelection() {
   bool enable{ cleanEndRecVals };
   bool blockRecsExist{ SendMessage(hFoldBlocks, LB_GETCOUNT, 0, 0) > 0 };
   bool endRecsExist{ SendMessage(hExplRecs, LB_GETCOUNT, 0, 0) > 0 };
   bool recTypesExist{ vRecTypes.size() > 0 };

   ShowWindow(GetDlgItem(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_NEW), cleanEndRecVals ? SW_SHOW : SW_HIDE);
   ShowWindow(GetDlgItem(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_RESET), cleanEndRecVals ? SW_HIDE : SW_SHOW);

   EnableWindow(hExplRecs, enable);
   EnableWindow(hExplRTList, endRecsExist && recTypesExist);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_ACCEPT), endRecsExist && recTypesExist);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_NEW), blockRecsExist && recTypesExist && enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_DEL), endRecsExist);

   enableBlockSelection();
}

int FoldStructDialog::endRecEditAccept(bool accept) {
   if (cleanEndRecVals) return 0;

   int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return LB_ERR;

   int idxBlock{ getCurrentBlockIndex() };
   if (idxBlock == LB_ERR) return LB_ERR;

   int idxEndRec{ static_cast<int>(SendMessage(hExplRecs, LB_GETCURSEL, 0, 0)) };
   if (idxEndRec == LB_ERR) return LB_ERR;

   TypeInfo& TI{ vFoldStructs[idxFS].vBlocks[idxBlock].vEndRecs[idxEndRec] };

   if (accept) {
      int index{ static_cast<int>(SendMessage(hExplRTList, CB_GETCURSEL, 0, 0)) };
      if (index >= static_cast<int>(vRecTypes.size())) return -1;

      TI.type = vRecTypes[index].type;
      TI.label = vRecTypes[index].label;
   }
   else if (TI.label.empty()) {
      endRecEditDelete();
      return 0;
   }
   else {
      Utils::setComboBoxSelection(hExplRTList, static_cast<int>(
         SendMessage(hExplRTList, CB_FINDSTRING, (WPARAM)-1, (LPARAM)Utils::getListBoxItem(hExplRecs).c_str())));
   }

   SendMessage(hExplRecs, LB_DELETESTRING, (WPARAM)idxEndRec, NULL);
   SendMessage(hExplRecs, LB_INSERTSTRING, (WPARAM)idxEndRec, (LPARAM)TI.label.c_str());
   SendMessage(hExplRecs, LB_SETCURSEL, idxEndRec, NULL);

   cleanStructsFile = FALSE;
   cleanEndRecVals = TRUE;
   enableEndRecSelection();

   return 1;
}

void FoldStructDialog::endRecEditNew() {
   if (!vRecTypes.size()) return;

   int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return;

   int idxBlock{ getCurrentBlockIndex() };
   if (idxBlock == LB_ERR) return;

   vector<TypeInfo>& endRecs{ vFoldStructs[idxFS].vBlocks[idxBlock].vEndRecs };

   endRecs.push_back(TypeInfo{});
   size_t moveTo = endRecs.size() - 1;

   SendMessage(hExplRecs, LB_ADDSTRING, NULL, (LPARAM)L"");
   SendMessage(hExplRecs, LB_SETCURSEL, (WPARAM)moveTo, NULL);
   onEndRecSelect();

   cleanStructsFile = FALSE;
   cleanEndRecVals = FALSE;
   enableEndRecSelection();
}

int FoldStructDialog::endRecEditDelete() {
   int endRecCount{ static_cast<int>(SendMessage(hExplRecs, LB_GETCOUNT, 0, 0)) };
   if (endRecCount == LB_ERR) return LB_ERR;
   if (!endRecCount) return 0;

   int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return LB_ERR;

   int idxBlock{ getCurrentBlockIndex() };
   if (idxBlock == LB_ERR) return LB_ERR;

   int idxEndRec{ static_cast<int>(SendMessage(hExplRecs, LB_GETCURSEL, 0, 0)) };
   if (idxEndRec == LB_ERR) return LB_ERR;

   vector<TypeInfo>& endRecs{ vFoldStructs[idxFS].vBlocks[idxBlock].vEndRecs };
   endRecs.erase(endRecs.begin() + idxEndRec);

   int lastRec = static_cast<int>(endRecs.size()) - 1;
   int moveTo = (idxEndRec <= lastRec - 1) ? idxEndRec : lastRec;

   SendMessage(hExplRecs, LB_DELETESTRING, (WPARAM)idxEndRec, NULL);
   SendMessage(hExplRecs, LB_SETCURSEL, (WPARAM)moveTo, NULL);

   cleanStructsFile = FALSE;
   cleanEndRecVals = TRUE;
   onEndRecSelect();

   return moveTo;
}

bool FoldStructDialog::promptDiscardChangesNo() {
   if (!(cleanStructsFile && cleanStructVals && cleanBlockVals && cleanEndRecVals)) {
      if (MessageBox(_hSelf, FOLD_DEF_DISCARD_CHANGES, FOLD_STRUCT_DIALOG_TITLE,
         MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
         return TRUE;
   }

   return false;
}

void FoldStructDialog::saveFoldStructInfo() {
   MessageBox(_hSelf, L"", L"", MB_OK);
}

void FoldStructDialog::showEximDialog(bool bExtract) {
   _eximDlg.doDialog((HINSTANCE)_gModule);
   _eximDlg.initDialog(bExtract, TRUE);

   if (bExtract) {
      //int idxFT{ getCurrentFileTypeIndex() };
      //if (idxFT == LB_ERR) return;

      wstring ftCode{}, ftConfig{};
      //if (getFileTypeConfig(idxFT, TRUE, ftCode, ftConfig) < 0) {
      //   _eximDlg.display(false);
      //   return;
      //}

      _eximDlg.setFileTypeData(ftConfig);
   }
}

int FoldStructDialog::getCurrentFoldStructIndex() {
   int idxFS;

   idxFS = static_cast<int>(SendMessage(hFoldStructs, LB_GETCURSEL, NULL, NULL));
   if (idxFS == LB_ERR) return LB_ERR;

   return idxFS;
}

bool FoldStructDialog::getCurrentFoldStructInfo(FoldStructInfo*& structInfo) {
   int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return FALSE;

   structInfo = &vFoldStructs[idxFS];
   return TRUE;
}

int FoldStructDialog::getFoldStructInfo(size_t /*idxFT*/, bool /*cr_lf*/, wstring& /*ftCode*/, wstring& /*ftConfig*/) {
   return 0;
}

int FoldStructDialog::getCurrentBlockIndex() {
   int idxBlock;

   idxBlock = static_cast<int>(SendMessage(hFoldBlocks, LB_GETCURSEL, NULL, NULL));
   if (idxBlock == LB_ERR) return LB_ERR;

   return idxBlock;
}

bool FoldStructDialog::getCurrentBlockInfo(BlockInfo*& blockInfo) {
   int idxFS{ getCurrentFoldStructIndex() };
   if (idxFS == LB_ERR) return FALSE;

   int idxBlock{ getCurrentBlockIndex() };
   if (idxBlock == LB_ERR) return FALSE;

   blockInfo = &vFoldStructs[idxFS].vBlocks[idxBlock];
   return TRUE;
}

