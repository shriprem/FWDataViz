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

   hFoldFileStructs = GetDlgItem(_hSelf, IDC_FOLD_DEF_FILE_LIST_BOX);
   hFoldFileType = GetDlgItem(_hSelf, IDC_FOLD_DEF_FILE_TYPE_LIST);
   hAutoFold = GetDlgItem(_hSelf, IDC_FOLD_DEF_AUTO_FOLD_APPLY);
   hHdrRecs = GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_LIST_BOX);
   hHdrRecType = GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_TYPE_LIST);
   hHdrPriority = GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_PRIORITY_EDIT);
   hHdrRecursive = GetDlgItem(_hSelf, IDC_FOLD_DEF_HDR_REC_RECURSIVE);
   hImplicitRecs = GetDlgItem(_hSelf, IDC_FOLD_IMPLICIT_TRMNTRS_LIST);
   hExplicitRecs = GetDlgItem(_hSelf, IDC_FOLD_EXPLICIT_TRMNTRS_LIST);
   hExplicitRecType = GetDlgItem(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_LIST);

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
   SetFocus(_hSelf);
}

void FoldStructDialog::initDialog(const unordered_map<wstring, string>& mFDtoT, const unordered_map<string, wstring>& mFTtoD) {
   cMapFileDescToType = mFDtoT;
   cMapFileTypeToDesc = mFTtoD;

   fillFileTypes();
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
            onFileTypeSelect();
            break;
         }
         break;

      case IDC_FOLD_DEF_FILE_ACCEPT_BTN:
         fileEditAccept();
         break;

      case IDC_FOLD_DEF_FILE_NEW_BTN:
         fileEditNew();
         break;

      case IDC_FOLD_DEF_FILE_CLONE_BTN:
         fileEditClone();
         break;

      case IDC_FOLD_DEF_FILE_DEL_BTN:
         fileEditDelete();
         break;

      case IDCANCEL:
      case IDCLOSE:
         if (promptDiscardChangesNo()) return TRUE;

         display(FALSE);
         return TRUE;

      case IDC_FOLD_DEF_SAVE_BTN:
         SetCursor(LoadCursor(NULL, IDC_WAIT));
         saveConfigInfo();
         SetCursor(LoadCursor(NULL, IDC_ARROW));
         return TRUE;

      case IDC_FOLD_DEF_RESET_BTN:
         if (!promptDiscardChangesNo()) {
            //configFile = L"";
            //loadConfigInfo();
            fillFileTypes();
         }
         break;

      case IDC_FOLD_DEF_BACKUP_LOAD_BTN:
         if (!promptDiscardChangesNo()) {
            wstring backupConfigFile;

            if (_configIO.queryConfigFileName(_hSelf, TRUE, TRUE, backupConfigFile)) {
               if (_configIO.fixIfNotUTF8File(backupConfigFile)) {
                  //configFile = backupConfigFile;
                  //loadConfigInfo();
                  fillFileTypes();
                  //cleanConfigFile = FALSE;
                  //enableFileSelection();
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
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_FILE_CLONE_BTN, FOLD_DEF_FILE_CLONE_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_FILE_DEL_BTN, FOLD_DEF_FILE_DEL_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_GROUP_BOX, FOLD_DEF_HDR_REC_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_TYPE_LABEL, FOLD_DEF_HDR_REC_TYPE_LABEL);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_PRIORITY_LABEL, FOLD_DEF_HDR_PRIORITY_LABEL);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_RECURSIVE, FOLD_DEF_HDR_REC_RECURSIVE);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_ACCEPT_BTN, FOLD_DEF_HDR_REC_ACCEPT_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_NEW_BTN, FOLD_DEF_HDR_REC_NEW_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_CLONE_BTN, FOLD_DEF_HDR_REC_CLONE_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_HDR_REC_DEL_BTN, FOLD_DEF_HDR_REC_DEL_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_IMPLICIT_TRMNTRS_GROUP, FOLD_IMPLICIT_TRMNTRS_GROUP);
   SetDlgItemText(_hSelf, IDC_FOLD_EXPLICIT_TRMNTRS_GROUP, FOLD_EXPLICIT_TRMNTRS_GROUP);
   SetDlgItemText(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_LABEL, FOLD_EXPLICIT_ENDREC_LABEL);
   SetDlgItemText(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_ACCEPT, FOLD_EXPLICIT_ENDREC_ACCEPT);
   SetDlgItemText(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_NEW, FOLD_EXPLICIT_ENDREC_NEW);
   SetDlgItemText(_hSelf, IDC_FOLD_EXPLICIT_ENDREC_DEL, FOLD_EXPLICIT_ENDREC_DEL);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_SAVE_BTN, FOLD_DEF_SAVE_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_RESET_BTN, FOLD_DEF_RESET_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_BACKUP_LOAD_BTN, FOLD_DEF_BACKUP_LOAD_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_BACKUP_VIEW_BTN, FOLD_DEF_BACKUP_VIEW_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_EXTRACT_BTN, FOLD_DEF_EXTRACT_BTN);
   SetDlgItemText(_hSelf, IDC_FOLD_DEF_APPEND_BTN, FOLD_DEF_APPEND_BTN);
}

void FoldStructDialog::fillFileTypes() {
}

void FoldStructDialog::onFileTypeSelect() {
}

int FoldStructDialog::fileEditAccept() {
   return 0;
}

void FoldStructDialog::fileEditNew() {
}

void FoldStructDialog::fileEditClone() {
}

int FoldStructDialog::fileEditDelete() {
   return 0;
}

bool FoldStructDialog::promptDiscardChangesNo() {
   //if (!(cleanConfigFile && cleanFileVals && cleanRecVals && cleanFieldVals)) {
      //if (MessageBox(_hSelf, FWVIZ_DISCARD_CHANGES, FWVIZ_DIALOG_TITLE,
      //   MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
      //   return TRUE;
   //}

   return false;
}

void FoldStructDialog::saveConfigInfo() {
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
