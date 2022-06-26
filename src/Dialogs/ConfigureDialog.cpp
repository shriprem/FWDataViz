#include "ConfigureDialog.h"
#include "EximFileTypeDialog.h"
#include "FieldTypeDialog.h"

#pragma comment(lib, "comctl32.lib")

extern HINSTANCE _gModule;
extern ConfigureDialog _configDlg;

EximFileTypeDialog _eximDlg;
FieldTypeDialog _fieldTypeDlg;

LRESULT CALLBACK procNumberEditControl(HWND hwnd, UINT messageId, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
   switch (messageId) {
   case WM_CHAR:
   {
      wchar_t editChar{ static_cast<WCHAR>(wParam) };
      bool valid{ editChar < ' ' || (editChar >= '0' && editChar <= '9') };

      int editSel{ static_cast<int>(SendMessage(hwnd, EM_GETSEL, NULL, NULL)) };

      if (LOWORD(editSel) == 0) {
         wchar_t editText[MAX_PATH + 1];
         GetWindowText(hwnd, editText, MAX_PATH);

         if (HIWORD(editSel) == 0 && editText[0] == '-')
            valid = editChar < ' ';
         else
            valid |= editChar == '-';
      }

      if (!valid) {
         showEditBalloonTip(hwnd, FWVIZ_DIALOG_NUMERIC_TITLE, FWVIZ_DIALOG_NUMERIC_MSG);
         return FALSE;
      }
      break;
   }

   case WM_PASTE:
   {
      wstring clipText;
      Utils::getClipboardText(GetParent(hwnd), clipText);

      int editSel{ static_cast<int>(SendMessage(hwnd, EM_GETSEL, NULL, NULL)) };

      wchar_t editText[MAX_PATH + 1];
      GetWindowText(hwnd, editText, MAX_PATH);

      wstring clipValid{ (editText[0] == '-' && (LOWORD(editSel) > 0 || HIWORD(editSel) == 0)) ? L"" : L"-?" };
      clipValid += L"[0-9]*";

      if (!regex_match(clipText, wregex(clipValid))) {
         showEditBalloonTip(hwnd, FWVIZ_DIALOG_NUMERIC_TITLE, FWVIZ_DIALOG_NUMERIC_MSG);
         return FALSE;
      }
      break;
   }
   }

   return DefSubclassProc(hwnd, messageId, wParam, lParam);
}

LRESULT CALLBACK procFieldEditMessages(HWND hwnd, UINT messageId, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
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
      create(IDD_FILE_TYPE_DEFINER_DIALOG);
   }

   hFilesLB = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_LIST_BOX);
   hFileEOL = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_EOL_EDIT);
   hFileThemes = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LIST);
   hRecsLB = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_LIST_BOX);
   hRecStart = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_START_EDIT);
   hRecRegex = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_REGEX_EDIT);
   hRecThemes = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_THEME_LIST);
   hFieldLabels = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FIELD_LABELS_EDIT);
   hFieldWidths = GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FIELD_WIDTHS_EDIT);

   for (int i{}, id{ IDC_FWVIZ_DEF_ADFT_LINE_EDIT_01 }; i < ADFT_MAX; ++i) {
      hADFTLine[i] = GetDlgItem(_hSelf, id++);
      hADFTRegex[i] = GetDlgItem(_hSelf, id++);
   }

   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_FILE_DESC_EDIT, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);
   SendDlgItemMessage(_hSelf, IDC_FWVIZ_DEF_REC_DESC_EDIT, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);

   SendMessage(hFileEOL, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);
   SendMessage(hADFTRegex[0], EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);
   SendMessage(hADFTRegex[1], EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);
   SendMessage(hADFTRegex[2], EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);
   SendMessage(hRecStart, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);
   SendMessage(hRecRegex, EM_LIMITTEXT, (WPARAM)MAX_PATH, NULL);
   SendMessage(hFieldLabels, EM_LIMITTEXT, (WPARAM)FW_LINE_MAX_LENGTH, NULL);
   SendMessage(hFieldWidths, EM_LIMITTEXT, (WPARAM)FW_LINE_MAX_LENGTH, NULL);

   SetWindowSubclass(hFieldLabels, procFieldEditMessages, NULL, NULL);
   SetWindowSubclass(hFieldWidths, procFieldEditMessages, NULL, NULL);

   SetWindowSubclass(hADFTLine[0], procNumberEditControl, NULL, NULL);
   SetWindowSubclass(hADFTLine[1], procNumberEditControl, NULL, NULL);
   SetWindowSubclass(hADFTLine[2], procNumberEditControl, NULL, NULL);

   Utils::loadBitmap(_hSelf, IDC_FWVIZ_DEF_FILE_DOWN_BUTTON, IDB_VIZ_MOVE_DOWN_BITMAP);
   hToolTips[0] = Utils::addTooltip(_hSelf, IDC_FWVIZ_DEF_FILE_DOWN_BUTTON, NULL, FWVIZ_DEF_FILE_MOVE_DOWN, FALSE);

   Utils::loadBitmap(_hSelf, IDC_FWVIZ_DEF_FILE_UP_BUTTON, IDB_VIZ_MOVE_UP_BITMAP);
   hToolTips[1] = Utils::addTooltip(_hSelf, IDC_FWVIZ_DEF_FILE_UP_BUTTON, NULL, FWVIZ_DEF_FILE_MOVE_UP, FALSE);

   Utils::loadBitmap(_hSelf, IDC_FWVIZ_DEF_MCBS_INFO_BUTTON, IDB_VIZ_INFO_BITMAP);
   hToolTips[2] = Utils::addTooltip(_hSelf, IDC_FWVIZ_DEF_MCBS_INFO_BUTTON,
      FWVIZ_DEF_MCBS_HINT_TITLE, FWVIZ_DEF_MCBS_HINT_TEXT, TRUE);
   SendMessage(hToolTips[2], TTM_SETDELAYTIME, TTDT_AUTOPOP, (LPARAM)(30000));

   Utils::loadBitmap(_hSelf, IDC_FWVIZ_DEF_ADFT_INFO_BUTTON, IDB_VIZ_INFO_BITMAP);
   hToolTips[3] = Utils::addTooltip(_hSelf, IDC_FWVIZ_DEF_ADFT_INFO_BUTTON,
      FWVIZ_DEF_ADFT_HINT_TITLE, FWVIZ_DEF_ADFT_HINT_TEXT, TRUE);
   SendMessage(hToolTips[3], TTM_SETDELAYTIME, TTDT_AUTOPOP, (LPARAM)(30000));

   Utils::loadBitmap(_hSelf, IDC_FWVIZ_DEF_REC_DOWN_BUTTON, IDB_VIZ_MOVE_DOWN_BITMAP);
   hToolTips[4] = Utils::addTooltip(_hSelf, IDC_FWVIZ_DEF_REC_DOWN_BUTTON, NULL, FWVIZ_DEF_REC_MOVE_DOWN, FALSE);

   Utils::loadBitmap(_hSelf, IDC_FWVIZ_DEF_REC_UP_BUTTON, IDB_VIZ_MOVE_UP_BITMAP);
   hToolTips[5] = Utils::addTooltip(_hSelf, IDC_FWVIZ_DEF_REC_UP_BUTTON, NULL, FWVIZ_DEF_REC_MOVE_UP, FALSE);

   Utils::loadBitmap(_hSelf, IDC_FWVIZ_DEF_REC_THEME_INFOBTN, IDB_VIZ_INFO_BITMAP);
   hToolTips[6] = Utils::addTooltip(_hSelf, IDC_FWVIZ_DEF_REC_THEME_INFOBTN,
      FWVIZ_DEF_RECTHEME_HINT_HDR, FWVIZ_DEF_RECTHEME_HINT_TXT, TRUE);
   SendMessage(hToolTips[6], TTM_SETDELAYTIME, TTDT_AUTOPOP, (LPARAM)(30000));

   Utils::loadBitmap(_hSelf, IDC_FWVIZ_DEF_INFO_BUTTON, IDB_VIZ_INFO_BITMAP);
   hToolTips[7] = Utils::addTooltip(_hSelf, IDC_FWVIZ_DEF_INFO_BUTTON, NULL, VIZ_PANEL_INFO_TIP, FALSE);

   Utils::setFontUnderline(_hSelf, IDC_FWVIZ_DEF_ADFT_GROUP_LABEL);

   if (_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);

   loadConfigInfo();
   fillFileTypes();
}

void ConfigureDialog::refreshDarkMode() {
   NPPDM_AutoThemeChildControls(_hSelf);
   redraw();

   if (_eximDlg.isCreated())
      _eximDlg.refreshDarkMode();

   if (_fieldTypeDlg.isCreated())
      _fieldTypeDlg.refreshDarkMode();
}

INT_PTR CALLBACK ConfigureDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
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

      case IDC_FWVIZ_DEF_MCBS_INFO_BUTTON:
         ShellExecute(NULL, L"open", FWVIZ_DEF_MCBS_INFO_README, NULL, NULL, SW_SHOW);
         break;

      case IDC_FWVIZ_DEF_ADFT_INFO_BUTTON:
         ShellExecute(NULL, L"open", FWVIZ_DEF_ADFT_INFO_README, NULL, NULL, SW_SHOW);
         break;

      case IDC_FWVIZ_DEF_REC_THEME_INFOBTN:
         ShellExecute(NULL, L"open", FWVIZ_RT_THEME_INFO_README, NULL, NULL, SW_SHOW);
         break;

      case IDC_FWVIZ_DEF_INFO_BUTTON:
         ShellExecute(NULL, L"open", FWVIZ_DEF_INFO_README, NULL, NULL, SW_SHOW);
         break;

      case IDC_FWVIZ_DEF_FILE_DESC_EDIT:
      case IDC_FWVIZ_DEF_FILE_EOL_EDIT:
      case IDC_FWVIZ_DEF_MCBS_CHECKBOX:
      case IDC_FWVIZ_DEF_FILE_THEME_LIST:
      case IDC_FWVIZ_DEF_ADFT_LINE_EDIT_01:
      case IDC_FWVIZ_DEF_ADFT_REGEX_EDT_01:
      case IDC_FWVIZ_DEF_ADFT_LINE_EDIT_02:
      case IDC_FWVIZ_DEF_ADFT_REGEX_EDT_02:
      case IDC_FWVIZ_DEF_ADFT_LINE_EDIT_03:
      case IDC_FWVIZ_DEF_ADFT_REGEX_EDT_03:
         switch HIWORD(wParam) {
         case BN_CLICKED:
         case EN_CHANGE:
         case CBN_SELCHANGE:
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

      case IDC_FWVIZ_DEF_FILE_CLONE_BTN:
         fileEditClone();
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
      case IDC_FWVIZ_DEF_REC_THEME_LIST:
         switch HIWORD(wParam) {
         case EN_CHANGE:
         case CBN_SELCHANGE:
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
         recEditNew(FALSE);
         break;

      case IDC_FWVIZ_DEF_REC_CLONE_BTN:
         recEditNew(TRUE);
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
            syncFieldEditScrolling(hFieldLabels, hFieldWidths);
            break;
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
            syncFieldEditScrolling(hFieldWidths, hFieldLabels);
            break;
         }

         break;

      case IDC_FWVIZ_DEF_FIELD_ACCEPT_BTN:
         fieldEditsAccept();
         break;

      case IDC_FWVIZ_DEF_FIELD_RESET_BTN:
         fillFieldTypes();
         break;

      case IDC_FWVIZ_FIELD_TYPE_BUTTON:
         _fieldTypeDlg.doDialog((HINSTANCE)_gModule);
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
            wstring backupConfigFile;

            if (_configIO.queryConfigFileName(_hSelf, TRUE, TRUE, backupConfigFile)) {
               if (_configIO.fixIfUTF16File(backupConfigFile)) {
                  configFile = backupConfigFile;
                  loadConfigInfo();
                  fillFileTypes();
                  cleanConfigFile = FALSE;
                  enableFileSelection();
               }
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

void ConfigureDialog::localize() {
   SetWindowText(_hSelf, FWVIZ_DIALOG_TITLE);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_GROUP_BOX, FWVIZ_DEF_FILE_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_DESC_LABEL, FWVIZ_DEF_FILE_DESC_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_MCBS_CHECKBOX, FWVIZ_DEF_MCBS_CHECKBOX);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_EOL_LABEL, FWVIZ_DEF_FILE_EOL_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_ADFT_GROUP_LABEL, FWVIZ_DEF_ADFT_GROUP_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_ADFT_LINE_LABEL, FWVIZ_DEF_ADFT_LINE_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_ADFT_REGEX_LABEL, FWVIZ_DEF_ADFT_REGEX_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_THEME_LABEL, FWVIZ_DEF_FILE_THEME_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_ACCEPT_BTN, FWVIZ_DEF_FILE_ACCEPT_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_NEW_BTN, FWVIZ_DEF_FILE_NEW_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_CLONE_BTN, FWVIZ_DEF_FILE_CLONE_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_DEL_BTN, FWVIZ_DEF_FILE_DEL_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_GROUP_BOX, FWVIZ_DEF_REC_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_DESC_LABEL, FWVIZ_DEF_REC_DESC_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_START_LABEL, FWVIZ_DEF_REC_START_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_REGEX_LABEL, FWVIZ_DEF_REC_REGEX_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_THEME_LABEL, FWVIZ_DEF_REC_THEME_LABEL);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_ACCEPT_BTN, FWVIZ_DEF_REC_ACCEPT_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_NEW_BTN, FWVIZ_DEF_REC_NEW_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_CLONE_BTN, FWVIZ_DEF_REC_CLONE_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_DEL_BTN, FWVIZ_DEF_REC_DEL_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_GROUP_BOX, FWVIZ_DEF_FIELD_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_WIDTHS_TEXT, FWVIZ_DEF_FIELD_WIDTHS_TEXT);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_LABELS_TEXT, FWVIZ_DEF_FIELD_LABELS_TEXT);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_ACCEPT_BTN, FWVIZ_DEF_FIELD_ACCEPT_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FIELD_RESET_BTN, FWVIZ_DEF_FIELD_RESET_BTN);
   SetDlgItemText(_hSelf, IDC_FWVIZ_FIELD_TYPE_BUTTON, FWVIZ_DEF_FIELD_TYPE_BTN);
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

int ConfigureDialog::loadConfigInfo() {
   vector<string> fileTypeList;
   int fileTypeCount{ _configIO.getConfigValueList(fileTypeList, "Base", "FileTypes", "", Utils::WideToNarrow(configFile)) };

   vFileTypes.clear();
   vFileTypes.resize(fileTypeCount);

   for (int i{}; i < fileTypeCount; ++i) {
      loadFileTypeInfo(i, fileTypeList[i], configFile);
   }

   return static_cast<int>(vFileTypes.size());
}

int ConfigureDialog::loadFileTypeInfo(int vIndex, const string& fileType, const wstring& wConfigFile) {
   string sConfigFile{ Utils::WideToNarrow(wConfigFile) };
   FileType& FT = vFileTypes[vIndex];

   FT.label = _configIO.getConfigWideChar(fileType, "FileLabel", "", sConfigFile);
   FT.theme = _configIO.getConfigWideChar(fileType, "FileTheme", "", sConfigFile);
   FT.eol = _configIO.getConfigWideChar(fileType, "RecordTerminator", "", sConfigFile);
   FT.multiByte = (_configIO.getConfigStringA(fileType, "MultiByteChars", "N") == "Y");

   // Load ADFT data
   for (int i{}; i < ADFT_MAX; ++i) {
      char idx[5];
      snprintf(idx, 5, "%02d", i + 1);

      FT.lineNums[i] = _configIO.getConfigInt(fileType, "ADFT_Line_" + string{ idx }, 0, sConfigFile);
      FT.regExprs[i] = _configIO.getConfigWideChar(fileType, "ADFT_Regex_" + string{ idx }, "", sConfigFile);
   }

   // Load Record Type data
   vector<string> recTypesList;
   int recTypeCount{ _configIO.getConfigValueList(recTypesList, fileType, "RecordTypes", "", sConfigFile) };

   FT.vRecTypes.clear();
   FT.vRecTypes.resize(recTypeCount);

   for (int j{}; j < recTypeCount; ++j) {
      string& recType = recTypesList[j];
      RecordType& RT = FT.vRecTypes[j];

      RT.label = _configIO.getConfigWideChar(fileType, (recType + "_Label"), "", sConfigFile);
      RT.marker = _configIO.getConfigWideChar(fileType, (recType + "_Marker"), "", sConfigFile);
      RT.theme = _configIO.getConfigWideChar(fileType, (recType + "_Theme"), "", sConfigFile);
      RT.fieldWidths = _configIO.getConfigWideChar(fileType, (recType + "_FieldWidths"), "", sConfigFile);
      RT.fieldLabels = _configIO.getConfigWideChar(fileType, (recType + "_FieldLabels"), "", sConfigFile);
   }

   return recTypeCount;
}

void ConfigureDialog::fillFileTypes() {
   // Fill File Types Listbox
   SendMessage(hFilesLB, LB_RESETCONTENT, NULL, NULL);

   for (const FileType FT : vFileTypes) {
      SendMessage(hFilesLB, LB_ADDSTRING, NULL, (LPARAM)FT.label.c_str());
   }

   if (vFileTypes.size() > 0)
      SendMessage(hFilesLB, LB_SETCURSEL, 0, NULL);

   // Fill Files & Records Themes Droplists
   SendMessage(hFileThemes, CB_RESETCONTENT, NULL, NULL);
   SendMessage(hRecThemes, CB_RESETCONTENT, NULL, NULL);
   SendMessage(hRecThemes, CB_ADDSTRING, NULL, (LPARAM)FWVIZ_DEF_REC_THEME_FROM_FT);

   vector<wstring> themesList;
   _configIO.getThemesList(themesList);

   for (const wstring theme : themesList) {
      SendMessage(hFileThemes, CB_ADDSTRING, NULL, (LPARAM)theme.c_str());
      SendMessage(hRecThemes, CB_ADDSTRING, NULL, (LPARAM)theme.c_str());
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

bool ConfigureDialog::getCurrentFileTypeInfo(FileType*& fileInfo) {
   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return FALSE;

   fileInfo = &vFileTypes[idxFT];
   return TRUE;
}

ConfigureDialog::FileType ConfigureDialog::getNewFileType() {
   FileType newFile;

   newFile.theme = L"Spectrum";
   newFile.multiByte = FALSE;
   newFile.vRecTypes = vector<RecordType>{ getNewRec() };

   return newFile;
}

int ConfigureDialog::getFileTypeConfig(size_t idxFT, bool cr_lf, wstring& ftCode, wstring& ftConfig) {
   size_t recTypeCount;
   wchar_t fileTypeCode[60], recTypeCode[10];
   wstring new_line, rawCode, adft{}, recTypes{}, rtConfig{}, recTypePrefix;

   FileType& FT = vFileTypes[idxFT];
   new_line = cr_lf ? L"\r\n" : L"\n";

   string utf8Code{ Utils::WideToNarrow(FT.label) };
   utf8Code = regex_replace(utf8Code, std::regex("[^\x20-\x7F]"), "");
   rawCode = Utils::NarrowToWide(regex_replace(utf8Code, std::regex("(,|=|\\[|\\])"), " "));
   rawCode = regex_replace(rawCode, wregex(L"(^( )+)|(( )+$)"), L"");
   rawCode = regex_replace(rawCode, wregex(L"( ){2,}"), L" ");
   rawCode = regex_replace(rawCode, wregex(L" "), L"_").substr(0, 50);

   swprintf(fileTypeCode, 60, L"FT%03d_%s", static_cast<int>(idxFT + 1), rawCode.c_str());
   Utils::ToUpper(fileTypeCode);

   // ADFT Info
   for (int i{}; i < ADFT_MAX; ++i) {
      if (Utils::isInvalidRegex(FT.regExprs[i], _hSelf,
         wstring(FWVIZ_DEF_FILE_DESC_LABEL) + L" " + FT.label + new_line +
         FWVIZ_DEF_ADFT_GROUP_LABEL + L" - " + FWVIZ_DEF_ADFT_LINE_LABEL + L" " + to_wstring(i + 1)))
         return -2;

      wchar_t idx[5];
      swprintf(idx, 5, L"%02d", i + 1);

      wstring lineNum{ (FT.lineNums[i] == 0) ? L"" : to_wstring(FT.lineNums[i]) };

      adft +=
         L"ADFT_Line_" + wstring{ idx } + L"=" + lineNum + new_line +
         L"ADFT_Regex_" + wstring{ idx } + L"=" + FT.regExprs[i] + new_line;
   }

   // Rec Info
   recTypeCount = (FT.vRecTypes.size() > 999) ? 999 : FT.vRecTypes.size();

   for (size_t j{}; j < recTypeCount; ++j) {
      RecordType& RT = FT.vRecTypes[j];

      if (Utils::isInvalidRegex(RT.marker, _hSelf,
         wstring(FWVIZ_DEF_FILE_DESC_LABEL) + L" " + FT.label + new_line +
         FWVIZ_DEF_REC_DESC_LABEL + L" " + RT.label + new_line +
         FWVIZ_DEF_REC_REGEX_LABEL))
         return -2;

      swprintf(recTypeCode, 10, L"REC%03d", static_cast<int>(j + 1));
      recTypePrefix = wstring{ recTypeCode };
      recTypes += (j == 0 ? L"RecordTypes=" : L",") + recTypePrefix;

      rtConfig +=
         recTypePrefix + L"_Label=" + RT.label + new_line +
         recTypePrefix + L"_Marker=" + RT.marker + new_line +
         recTypePrefix + L"_FieldLabels=" + RT.fieldLabels + new_line +
         recTypePrefix + L"_FieldWidths=" + RT.fieldWidths + new_line;

      if ((RT.theme != L"") && (RT.theme != FWVIZ_DEF_REC_THEME_FROM_FT) && (RT.theme != FT.theme))
         rtConfig += recTypePrefix + L"_Theme=" + RT.theme + new_line;
   }

   // Output
   ftCode = wstring{ fileTypeCode };
   ftConfig = L"[" + ftCode + L"]" + new_line +
      L"FileLabel=" + FT.label + new_line +
      L"FileTheme=" + FT.theme + new_line +
      L"RecordTerminator=" + FT.eol + new_line +
      L"MultiByteChars=" + (FT.multiByte ? L"Y" : L"N") + new_line +
      adft + recTypes + new_line + rtConfig;

   return 1;
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
   newRec.marker = L"";
   newRec.fieldLabels = L"";
   newRec.fieldWidths = L"";
   newRec.theme = L"";
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
   SetWindowText(hFileEOL, fileInfo->eol.c_str());

   CheckDlgButton(_hSelf, IDC_FWVIZ_DEF_MCBS_CHECKBOX, fileInfo->multiByte ? BST_CHECKED : BST_UNCHECKED);

   for (int i{}; i < ADFT_MAX; ++i) {
      wstring lineNum{ (fileInfo->lineNums[i] == 0) ? L"" : to_wstring(fileInfo->lineNums[i]) };

      SetWindowText(hADFTLine[i], lineNum.c_str());
      SetWindowText(hADFTRegex[i], fileInfo->regExprs[i].c_str());
   }

   loadingEdits = FALSE;

   Utils::setComboBoxSelection(hFileThemes, static_cast<int>(
      SendMessage(hFileThemes, CB_FINDSTRING, (WPARAM)-1, (LPARAM)fileInfo->theme.c_str())));

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
   EnableWindow(hFilesLB, enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_NEW_BTN), enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_FILE_CLONE_BTN), enable);
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
   const int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return LB_ERR;

   switch (dir) {
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
   FileType& adjType = vFileTypes[idxFT + dir];

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

   vector <RecordType>& recInfoList = fileInfo->vRecTypes;

   // Fill Rec Types Listbox
   SendMessage(hRecsLB, LB_RESETCONTENT, NULL, NULL);

   for (const RecordType RT : recInfoList) {
      SendMessage(hRecsLB, LB_ADDSTRING, NULL, (LPARAM)RT.label.c_str());
   }

   if (recInfoList.size() > 0)
      SendMessage(hRecsLB, LB_SETCURSEL, 0, NULL);

   cleanRecVals = TRUE;
   onRecTypeSelect();
}

void ConfigureDialog::onRecTypeSelect() {
   RecordType* recInfo;

   if (!getCurrentRecInfo(recInfo)) {
      RecordType newRec{ getNewRec() };
      recInfo = &newRec;
   }

   loadingEdits = TRUE;

   SetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_DESC_EDIT, recInfo->label.c_str());

   wstring regExpr = recInfo->marker;

   SetWindowText(hRecRegex, regExpr.c_str());
   SetWindowText(hRecStart, getOnlyStartsWith(regExpr).c_str());

   loadingEdits = FALSE;

   if (recInfo->theme == L"")
      recInfo->theme = FWVIZ_DEF_REC_THEME_FROM_FT;

   Utils::setComboBoxSelection(hRecThemes, static_cast<int>(
      SendMessage(hRecThemes, CB_FINDSTRING, (WPARAM)-1, (LPARAM)recInfo->theme.c_str())));

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
   EnableWindow(hRecsLB, enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_NEW_BTN), enable);
   EnableWindow(GetDlgItem(_hSelf, IDC_FWVIZ_DEF_REC_CLONE_BTN), enable);

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
   const int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return LB_ERR;

   const int idxRec{ getCurrentRecIndex() };
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

   cleanConfigFile = FALSE;
   indicateCleanStatus();
   enableMoveRecButtons();

   return idxRec + dir;
}

void ConfigureDialog::fillFieldTypes() {
   RecordType* recInfo;

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
      (WPARAM)SendMessage(hThis, EM_LINEINDEX, (WPARAM)-1, NULL), NULL));

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

   RecordType* recInfo;
   if (!getCurrentRecInfo(recInfo)) return;

   wstring fieldValues(FW_LINE_MAX_LENGTH + 1, '\0');
   wstring vals{};

   // Field Labels
   GetWindowText(hFieldLabels, fieldValues.data(), (FW_LINE_MAX_LENGTH + 1));

   // Replace any trailing spaces + newlines with commas
   vals = regex_replace(wstring(fieldValues.c_str()), wregex(L" *\r\n"), L",");

   // Replace any commas + leading spaces with commas
   vals = regex_replace(vals, wregex(L", +([^,]*)"), L",$1");

   // Trim any leading & trailing spaces for the entire string
   vals = regex_replace(vals, wregex(L"^ +(.*[^ ]) +$"), L"$1");

   recInfo->fieldLabels = vals;
   SetWindowText(hFieldLabels, regex_replace(vals, wregex(L","), L"\r\n").c_str());

   // Field Widths
   GetWindowText(hFieldWidths, fieldValues.data(), (FW_LINE_MAX_LENGTH + 1));

   // Replace any newlines with commas.
   // No processing needed for leading & trailing spaces since this is a numeric edit control
   vals = regex_replace(wstring(fieldValues.c_str()), wregex(L"\r\n"), L",");

   recInfo->fieldWidths = vals.c_str();

   cleanConfigFile = FALSE;
   cleanFieldVals = TRUE;
   enableRecSelection();
}

void ConfigureDialog::onRecStartEditChange() {
   wstring startText(MAX_PATH + 1, '\0');
   GetWindowText(hRecStart, startText.data(), MAX_PATH);
   startText = startText.c_str();

   wstring startReg{ L"." };
   if (!startText.empty())
      startReg = L"^" + regex_replace(startText, std::wregex(L"[" + REGEX_META_CHARS + L"]"), L"\\$&");

   SetWindowText(hRecRegex, startReg.c_str());
}

void ConfigureDialog::onRecRegexEditChange() {
   wstring regexText(MAX_PATH + 1, '\0');
   GetWindowText(hRecRegex, regexText.data(), MAX_PATH);
   SetWindowText(hRecStart, getOnlyStartsWith(regexText.c_str()).c_str());
}

int ConfigureDialog::recEditAccept() {
   if (cleanRecVals) return 0;

   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return -1;

   int idxRec{ getCurrentRecIndex() };
   if (idxRec == LB_ERR) return -1;

   RecordType& recInfo = vFileTypes[idxFT].vRecTypes[idxRec];

   wchar_t recDesc[MAX_PATH + 1];

   GetDlgItemText(_hSelf, IDC_FWVIZ_DEF_REC_DESC_EDIT, recDesc, MAX_PATH);
   recInfo.label = recDesc;

   wchar_t regexVal[MAX_PATH + 1];

   GetWindowText(hRecRegex, regexVal, MAX_PATH);

   if (Utils::isInvalidRegex(regexVal, _hSelf, FWVIZ_DEF_REC_REGEX_LABEL))
      return -2;
   else
      recInfo.marker = regexVal;

   wchar_t themeVal[MAX_PATH + 1];

   GetWindowText(hRecThemes, themeVal, MAX_PATH);
   recInfo.theme = (wstring{ themeVal } == FWVIZ_DEF_REC_THEME_FROM_FT) ? L"" : themeVal;

   SendMessage(hRecsLB, LB_DELETESTRING, (WPARAM)idxRec, NULL);
   SendMessage(hRecsLB, LB_INSERTSTRING, (WPARAM)idxRec, (LPARAM)recDesc);
   SendMessage(hRecsLB, LB_SETCURSEL, idxRec, NULL);

   cleanConfigFile = FALSE;
   cleanRecVals = TRUE;
   enableRecSelection();

   return 1;
}

void ConfigureDialog::recEditNew(bool clone) {
   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return;

   int idxRT{ getCurrentRecIndex() };
   if (clone && idxRT == LB_ERR) return;

   vector<RecordType>& records = vFileTypes[idxFT].vRecTypes;

   if (static_cast<int>(records.size()) >= REC_TYPE_LIMIT) {
      TCHAR buf[100];
      swprintf(buf, 100, FWVIZ_RT_LIMIT_ERROR, REC_TYPE_LIMIT);
      MessageBox(_hSelf, buf, clone ? FWVIZ_RT_CLONE_ACTION : FWVIZ_RT_NEW_ACTION, MB_OK | MB_ICONSTOP);
      return;
   }

   RecordType newRec{ getNewRec() };

   if (clone) {
      newRec.label = records[idxRT].label;
      newRec.marker = records[idxRT].marker;
      newRec.fieldLabels = records[idxRT].fieldLabels;
      newRec.fieldWidths = records[idxRT].fieldWidths;
   }

   records.push_back(newRec);

   size_t moveTo = records.size() - 1;

   SendMessage(hRecsLB, LB_ADDSTRING, NULL, (LPARAM)newRec.label.c_str());
   SendMessage(hRecsLB, LB_SETCURSEL, (WPARAM)moveTo, NULL);
   onRecTypeSelect();

   cleanConfigFile = FALSE;
   cleanRecVals = clone;
   enableRecSelection();
}

int ConfigureDialog::recEditDelete() {
   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return LB_ERR;

   int idxRec{ getCurrentRecIndex() };
   if (idxRec == LB_ERR) return LB_ERR;

   vector<RecordType>& records = vFileTypes[idxFT].vRecTypes;
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

int ConfigureDialog::fileEditAccept() {
   if (cleanFileVals) return 0;

   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return -1;

   FileType& fileInfo = vFileTypes[idxFT];

   wchar_t fileVal[MAX_PATH + 1];

   GetDlgItemText(_hSelf, IDC_FWVIZ_DEF_FILE_DESC_EDIT, fileVal, MAX_PATH);
   fileInfo.label = fileVal;

   GetWindowText(hFileThemes, fileVal, MAX_PATH);
   fileInfo.theme = fileVal;

   wchar_t eolVal[MAX_PATH + 1];

   GetWindowText(hFileEOL, eolVal, MAX_PATH);
   fileInfo.eol = eolVal;

   fileInfo.multiByte = (IsDlgButtonChecked(_hSelf, IDC_FWVIZ_DEF_MCBS_CHECKBOX) == BST_CHECKED);

   // ADFT Info
   wchar_t lineNum[MAX_PATH + 1];
   wchar_t regExpr[MAX_PATH + 1];

   for (int i{}; i < ADFT_MAX; ++i) {
      GetWindowText(hADFTLine[i], lineNum, MAX_PATH);
      fileInfo.lineNums[i] = Utils::StringtoInt(lineNum);

      GetWindowText(hADFTRegex[i], regExpr, MAX_PATH);
      if (Utils::isInvalidRegex(regExpr, _hSelf,
         wstring(FWVIZ_DEF_ADFT_GROUP_LABEL) + L" - " + FWVIZ_DEF_ADFT_LINE_LABEL + L" " + to_wstring(i + 1)))
         return -2;
      else
         fileInfo.regExprs[i] = regExpr;
   }

   // Update FT Listbox Entry
   SendMessage(hFilesLB, LB_DELETESTRING, (WPARAM)idxFT, NULL);
   SendMessage(hFilesLB, LB_INSERTSTRING, (WPARAM)idxFT, (LPARAM)fileInfo.label.c_str());
   SendMessage(hFilesLB, LB_SETCURSEL, idxFT, NULL);

   cleanConfigFile = FALSE;
   cleanFileVals = TRUE;
   enableFileSelection();

   return 1;
}

int ConfigureDialog::appendFileTypeConfigs(const wstring& sConfigFile) {
   int sectionCount{}, validCount{};
   vector<string> sectionList{};
   wstring sectionLabel{};

   sectionCount = _configIO.getConfigAllSectionsList(sectionList, Utils::WideToNarrow(sConfigFile));

   for (int i{}; i < sectionCount; ++i) {
      sectionLabel = _configIO.getConfigWideChar(sectionList[i], "FileLabel", "", Utils::WideToNarrow(sConfigFile));
      if (!sectionLabel.empty()) {
         if (!checkFTLimit(FALSE)) break;

         FileType newFile{ getNewFileType() };

         vFileTypes.push_back(newFile);
         loadFileTypeInfo(static_cast<int>(vFileTypes.size() - 1), sectionList[i], sConfigFile);
         SendMessage(hFilesLB, LB_ADDSTRING, NULL, (LPARAM)sectionLabel.c_str());
         ++validCount;
      }
   }

   SendMessage(hFilesLB, LB_SETCURSEL, (WPARAM)(vFileTypes.size() - 1), NULL);
   onFileTypeSelect();

   cleanConfigFile = FALSE;
   enableFileSelection();

   return validCount;
}

void ConfigureDialog::fileEditNew() {
   if (!checkFTLimit(FALSE)) return;

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

void ConfigureDialog::fileEditClone() {
   if (!checkFTLimit(TRUE)) return;

   int idxFT{ getCurrentFileTypeIndex() };
   if (idxFT == LB_ERR) return;

   FileType& FT = vFileTypes[idxFT];
   FileType NF{};

   NF.label = FT.label + L"_clone";
   NF.theme = FT.theme;
   NF.eol = FT.eol;
   NF.multiByte = FT.multiByte;

   // ADFT Info
   for (int i{}; i < ADFT_MAX; ++i) {
      NF.lineNums[i] = FT.lineNums[i];
      NF.regExprs[i] = FT.regExprs[i];
   }

   // Rec Info
   size_t recCount = FT.vRecTypes.size();
   NF.vRecTypes.resize(recCount);

   for (size_t i{}; i < recCount; ++i) {
      NF.vRecTypes[i].label = FT.vRecTypes[i].label;
      NF.vRecTypes[i].marker = FT.vRecTypes[i].marker;
      NF.vRecTypes[i].fieldLabels = FT.vRecTypes[i].fieldLabels;
      NF.vRecTypes[i].fieldWidths = FT.vRecTypes[i].fieldWidths;
   }

   vFileTypes.push_back(NF);

   SendMessage(hFilesLB, LB_ADDSTRING, NULL, (LPARAM)NF.label.c_str());
   SendMessage(hFilesLB, LB_SETCURSEL, (WPARAM)(vFileTypes.size() - 1), NULL);

   onFileTypeSelect();
   cleanConfigFile = FALSE;
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

bool ConfigureDialog::checkFTLimit(bool clone) {
   if (vFileTypes.size() < FILE_TYPE_LIMIT) return TRUE;

   TCHAR buf[100];
   swprintf(buf, 100, FWVIZ_FT_LIMIT_ERROR, FILE_TYPE_LIMIT);
   MessageBox(_hSelf, buf, clone ? FWVIZ_FT_CLONE_ACTION : FWVIZ_FT_NEW_ACTION, MB_OK | MB_ICONSTOP);
   return FALSE;
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
   if (_configIO.isCurrentVizConfigDefault() &&
      MessageBox(_hSelf, FWVIZ_DEFAULT_OVERWRITE, FWVIZ_DIALOG_TITLE,
         MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDNO)
      return;

   if (!cleanFieldVals) fieldEditsAccept();

   if (!cleanRecVals)
      if (recEditAccept() < 0) return;

   if (!cleanFileVals)
      if (fileEditAccept() < 0) return;

   size_t fileTypeCount;
   wstring fileData{}, fileTypes{}, ftCode{}, ftConfig{};

   fileTypeCount = (vFileTypes.size() > 999) ? 999 : vFileTypes.size();

   for (size_t i{}; i < fileTypeCount; ++i) {
      if (getFileTypeConfig(i, TRUE, ftCode, ftConfig) < 0) return;
      fileTypes += (i == 0 ? L"" : L",") + ftCode;
      fileData += ftConfig + L"\r\n";
   }

   fileData = L"[Base]\r\nFileTypes=" + fileTypes + L"\r\n\r\n" + fileData;

   _configIO.backupConfigFile(_configIO.getConfigFile(_configIO.CONFIG_VIZ));
   _configIO.saveConfigFile(fileData, _configIO.getConfigFile(_configIO.CONFIG_VIZ));

   cleanConfigFile = TRUE;
   indicateCleanStatus();
   RefreshVisualizerPanel();
}

void ConfigureDialog::showEximDialog(bool bExtract) {
   _eximDlg.doDialog((HINSTANCE)_gModule);
   _eximDlg.initDialog(bExtract, TRUE);

   if (bExtract) {
      int idxFT{ getCurrentFileTypeIndex() };
      if (idxFT == LB_ERR) return;

      wstring ftCode{}, ftConfig{};
      if (getFileTypeConfig(idxFT, TRUE, ftCode, ftConfig) < 0) {
         _eximDlg.display(false);
         return;
      }

      _eximDlg.setFileTypeData(ftConfig);
   }
}

wstring ConfigureDialog::getOnlyStartsWith(wstring expr) {
   return wstring{ (!expr.empty() && regex_match(expr, std::wregex(L"^\\^[^" + REGEX_META_CHARS + L"]+"))) ? expr.substr(1) : L"" };
}
