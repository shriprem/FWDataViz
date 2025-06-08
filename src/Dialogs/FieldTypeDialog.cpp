#include "FieldTypeDialog.h"

void FieldTypeDialog::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_FIELD_TYPE_DEFINER_DIALOG);
   }

   initComponent(_hSelf);

   fieldDefConfigFile = Utils::WideToNarrow(_configIO.getConfigFile(_configIO.CONFIG_FIELD_TYPES));
   hFieldsLB = GetDlgItem(_hSelf, IDC_FIELD_TYPE_LIST_BOX);

   SendDlgItemMessage(_hSelf, IDC_FIELD_TYPE_DESC_EDIT, EM_LIMITTEXT, MAX_PATH, NULL);

   Utils::loadBitmap(_hSelf, IDC_FIELD_TYPE_INFO_BUTTON, IDB_VIZ_INFO_BITMAP);
   Utils::addTooltip(_hSelf, IDC_FIELD_TYPE_INFO_BUTTON, L"", VIZ_PANEL_INFO_TIP, FALSE);

   if constexpr(_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
   fillFields();
}

void FieldTypeDialog::refreshDarkMode() {
   NPPDM_AutoThemeChildControls(_hSelf);
   redraw();
}

INT_PTR FieldTypeDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
   switch (message) {
   case WM_COMMAND:
      switch LOWORD(wParam) {
      case IDC_FIELD_TYPE_LIST_BOX:
         switch HIWORD(wParam) {
         case LBN_SELCHANGE:
            onFieldSelect();
            break;
         }
         break;

      case IDC_FIELD_TYPE_INFO_BUTTON:
         ShellExecute(NULL, L"open", FIELD_TYPE_INFO_README, NULL, NULL, SW_SHOW);
         break;

      case IDC_FIELD_TYPE_DESC_EDIT:
         switch HIWORD(wParam) {
         case EN_CHANGE:
            if (!loadingEdits) {
               cleanStyleDefs = FALSE;
               enableFieldSelection();
            }
            break;
         }
         break;

      case IDC_FIELD_TYPE_NEW_BTN:
         fieldEditNew();
         break;

      case IDC_FIELD_TYPE_CLONE_BTN:
         fieldEditClone();
         break;

      case IDC_FIELD_TYPE_DEL_BTN:
         fieldEditDelete();
         break;

      case IDC_STYLE_DEF_BACK_EDIT:
      case IDC_STYLE_DEF_FORE_EDIT:
         switch (HIWORD(wParam)) {
         case EN_CHANGE:
            bool back = (LOWORD(wParam) == IDC_STYLE_DEF_BACK_EDIT);
            setStyleDefColor(FALSE, getStyleDefColor(back), back);
            if (!loadingEdits) {
               cleanStyleDefs = FALSE;
               enableFieldSelection();
            }
            break;
         }
         break;

      case IDC_STYLE_DEF_BACKCOLOR:
         chooseStyleDefColor(TRUE);
         break;

      case IDC_STYLE_DEF_FORECOLOR:
         chooseStyleDefColor(FALSE);
         break;

      case IDC_STYLE_DEF_BOLD:
      case IDC_STYLE_DEF_ITALICS:
         setOutputFontStyle();
         cleanStyleDefs = FALSE;
         enableFieldSelection();
         break;

      case IDC_STYLE_DEF_PREVIEW_BOX:
         setPangram();
         break;

      case IDC_FIELD_STYLE_DEF_SAVE_BTN:
         styleDefSave();
         break;

      case IDC_FIELD_STYLE_DEF_RESET_BTN:
         (newFieldDef) ? fieldEditDelete() : onFieldSelect();
         break;

      case IDCANCEL:
      case IDCLOSE:
         display(FALSE);
         return TRUE;

      case IDC_JUMP_FIELD_LIST:
         switch HIWORD(wParam) {
         case CBN_SELCHANGE:
            break;
         }
         break;

      }
      break;

   case WM_CTLCOLORSTATIC:
      if (styleDefColor) {
         INT_PTR ptr = colorStaticControl(wParam, lParam);
         if (ptr != NULL) return ptr;
      }

      switch (GetDlgCtrlID((HWND)lParam)) {
      case IDC_FIELD_TYPE_REGEX_LABEL:
         return NPPDM_OnCtlColorIfEnabled(reinterpret_cast<HDC>(wParam), FALSE);

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

   case WM_INITDIALOG:
      NPPDM_AutoSubclassAndThemeChildControls(_hSelf);
      break;
   }

   return FALSE;
}

void FieldTypeDialog::localize() {
   SetWindowText(_hSelf, FIELD_TYPE_DEF_DLG_TITLE);
   SetDlgItemText(_hSelf, IDC_FIELD_TYPE_GROUP_BOX, FIELD_TYPE_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_FIELD_TYPE_NEW_BTN, FIELD_TYPE_NEW_BTN);
   SetDlgItemText(_hSelf, IDC_FIELD_TYPE_CLONE_BTN, FIELD_TYPE_CLONE_BTN);
   SetDlgItemText(_hSelf, IDC_FIELD_TYPE_DEL_BTN, FIELD_TYPE_DEL_BTN);
   SetDlgItemText(_hSelf, IDC_FIELD_TYPE_DESC_LABEL, FIELD_TYPE_DESC_LABEL);
   SetDlgItemText(_hSelf, IDC_FIELD_TYPE_REGEX_LABEL, FIELD_TYPE_REGEX_LABEL);
   SetDlgItemText(_hSelf, IDC_FIELD_STYLE_DEF_SAVE_BTN, FIELD_STYLE_DEF_SAVE_BTN);
   SetDlgItemText(_hSelf, IDC_FIELD_STYLE_DEF_RESET_BTN, FIELD_STYLE_DEF_RESET_BTN);

   StyleDefComponent::localize();
}

int FieldTypeDialog::getCurrentFieldIndex() const {
   int idxFT;

   idxFT = static_cast<int>(SendMessage(hFieldsLB, LB_GETCURSEL, NULL, NULL));
   if (idxFT == LB_ERR) return LB_ERR;

   return idxFT;
}

string FieldTypeDialog::getNewStyle() {
   int backColor{ static_cast<int>(NppMessage(NPPM_GETEDITORDEFAULTBACKGROUNDCOLOR)) };
   int foreColor{ static_cast<int>(NppMessage(NPPM_GETEDITORDEFAULTFOREGROUNDCOLOR)) };

   char styleDef[17];
   snprintf(styleDef, 17, "%06X %06X 00", backColor, foreColor);
   return string{ styleDef };
}

string FieldTypeDialog::getStyleConfig() {
   int backColor{ getStyleDefColor(TRUE) };
   int foreColor{ getStyleDefColor(FALSE) };
   bool bold{ (IsDlgButtonChecked(_hSelf, IDC_STYLE_DEF_BOLD) == BST_CHECKED) };
   bool italics{ (IsDlgButtonChecked(_hSelf, IDC_STYLE_DEF_ITALICS) == BST_CHECKED) };

   char styleDef[15];
   snprintf(styleDef, 15, "%06X %06X ", backColor, foreColor);

   return string{ styleDef } + (bold ? "1" : "0") + (italics ? "1" : "0");
}

void FieldTypeDialog::fillFields() {
   SendMessage(hFieldsLB, LB_RESETCONTENT, NULL, NULL);

   int keyCount{};
   vector<wstring> keyList{};

   keyCount = _configIO.getConfigAllKeysList("Styles", keyList, fieldDefConfigFile);
   if (keyCount < 1) {
      fieldEditNew();
      return;
   }

   for (int i{}; i < keyCount; ++i) {
      SendMessage(hFieldsLB, LB_ADDSTRING, NULL, (LPARAM)keyList[i].c_str());
   }

   SendMessage(hFieldsLB, LB_SETCURSEL, 0, NULL);
   onFieldSelect();
}

void FieldTypeDialog::onFieldSelect() {
   int idxFT{ getCurrentFieldIndex() };
   if (idxFT == LB_ERR) return;

   wstring label(MAX_PATH, '\0');
   SendMessage(hFieldsLB, LB_GETTEXT, idxFT, (LPARAM)label.c_str());
   label = label.c_str();

   fieldDefLabel = label;
   fieldDefRegex = _configIO.getConfigWideChar("Validations", Utils::WideToNarrow(label), "", fieldDefConfigFile);
   fieldDefStyle = _configIO.getConfigStringA("Styles", Utils::WideToNarrow(label), "", fieldDefConfigFile);

   fillStyleDefs();
}

void FieldTypeDialog::enableFieldSelection() {
   EnableWindow(GetDlgItem(_hSelf, IDC_FIELD_TYPE_LIST_BOX), cleanStyleDefs);
   EnableWindow(GetDlgItem(_hSelf, IDC_FIELD_TYPE_NEW_BTN), cleanStyleDefs);
   EnableWindow(GetDlgItem(_hSelf, IDC_FIELD_TYPE_CLONE_BTN), cleanStyleDefs);

   wstring fieldLabel(MAX_PATH + 1, '\0');
   GetDlgItemText(_hSelf, IDC_FIELD_TYPE_DESC_EDIT, fieldLabel.data(), MAX_PATH + 1);
   EnableWindow(GetDlgItem(_hSelf, IDC_FIELD_STYLE_DEF_SAVE_BTN), !wstring(fieldLabel.c_str()).empty());

   if (cleanStyleDefs) {
      SetDlgItemText(_hSelf, IDC_FIELD_STYLE_DEF_SAVE_BTN, FIELD_STYLE_DEF_SAVE_BTN);
      Utils::setFontRegular(_hSelf, IDC_FIELD_STYLE_DEF_SAVE_BTN);
   }
   else {
      SetDlgItemText(_hSelf, IDC_FIELD_STYLE_DEF_SAVE_BTN, (wstring(FIELD_STYLE_DEF_SAVE_BTN) + L"*").c_str());
      Utils::setFontBold(_hSelf, IDC_FIELD_STYLE_DEF_SAVE_BTN);
   }
}

void FieldTypeDialog::fieldEditNew() {
   SendMessage(hFieldsLB, LB_ADDSTRING, NULL, (LPARAM)L"");

   int lbLast{ static_cast<int>(SendMessage(hFieldsLB, LB_GETCOUNT, NULL, NULL)) - 1 };
   SendMessage(hFieldsLB, LB_SETCURSEL, lbLast, NULL);

   fieldDefLabel = L"";
   fieldDefRegex = L"";
   fieldDefStyle = getNewStyle();

   fillStyleDefs();

   newFieldDef = TRUE;
   cleanStyleDefs = FALSE;
   enableFieldSelection();
}

void FieldTypeDialog::fieldEditClone() {
   fieldDefLabel += L"_clone";
   SendMessage(hFieldsLB, LB_ADDSTRING, NULL, (LPARAM)fieldDefLabel.c_str());

   int lbLast{ static_cast<int>(SendMessage(hFieldsLB, LB_GETCOUNT, NULL, NULL)) - 1 };
   SendMessage(hFieldsLB, LB_SETCURSEL, lbLast, NULL);

   fillStyleDefs();

   newFieldDef = TRUE;
   cleanStyleDefs = FALSE;
   enableFieldSelection();
}

void FieldTypeDialog::fieldEditDelete() {
   int idxFT{ getCurrentFieldIndex() };
   if (idxFT == LB_ERR) return;

   if (!newFieldDef) {
      _configIO.deleteKey("Styles", Utils::WideToNarrow(fieldDefLabel), fieldDefConfigFile);
      //>>_configIO.deleteKey("Validations", Utils::WideToNarrow(fieldDefLabel), fieldDefConfigFile);
   }

   SendMessage(hFieldsLB, LB_DELETESTRING, idxFT, NULL);

   int lbLast{ static_cast<int>(SendMessage(hFieldsLB, LB_GETCOUNT, NULL, NULL)) - 1 };
   if (lbLast < 0) {
      fieldEditNew();
      return;
   }

   SendMessage(hFieldsLB, LB_SETCURSEL, ((idxFT <= lbLast) ? idxFT : lbLast), NULL);

   newFieldDef = FALSE;
   onFieldSelect();
}

void FieldTypeDialog::setStyleDefColor(bool setEdit, int color, bool back) {
   loadingEdits = TRUE;
   StyleDefComponent::setStyleDefColor(setEdit, color, back);
   loadingEdits = FALSE;
}

void FieldTypeDialog::fillStyleDefs() {
   loadingEdits = TRUE;
   SetDlgItemText(_hSelf, IDC_FIELD_TYPE_DESC_EDIT, fieldDefLabel.c_str());
   //>>SetDlgItemText(_hSelf, IDC_FIELD_TYPE_REGEX_EDIT, fieldDefRegex.c_str());
   loadingEdits = FALSE;

   if (fieldDefStyle.length() != 16)
      fieldDefStyle = getNewStyle();

   StyleInfo style{};
   _configIO.parseFieldStyle(fieldDefStyle, style);

   StyleDefComponent::fillStyleDefs(style);
   enableFieldSelection();
}

void FieldTypeDialog::styleDefSave() {
   if (cleanStyleDefs) return;

   int idxFT{ getCurrentFieldIndex() };
   if (idxFT == LB_ERR) return;

   wchar_t fieldLabel[MAX_PATH + 1];
   GetDlgItemText(_hSelf, IDC_FIELD_TYPE_DESC_EDIT, fieldLabel, MAX_PATH + 1);
   if (wstring{ fieldLabel }.empty()) return;

   wchar_t fieldRegex[MAX_PATH + 1];
   GetDlgItemText(_hSelf, IDC_FIELD_TYPE_REGEX_EDIT, fieldRegex, MAX_PATH + 1);

   if (!newFieldDef) {
      _configIO.deleteKey("Styles", Utils::WideToNarrow(fieldDefLabel), fieldDefConfigFile);
      //>>_configIO.deleteKey("Validations", Utils::WideToNarrow(fieldDefLabel), fieldDefConfigFile);
   }

   fieldDefLabel = fieldLabel;
   fieldDefRegex = fieldRegex;
   fieldDefStyle = getStyleConfig();

   _configIO.setConfigStringA("Styles", Utils::WideToNarrow(fieldDefLabel), fieldDefStyle, fieldDefConfigFile);
   //>>_configIO.setConfigStringA("Validations", Utils::WideToNarrow(fieldDefLabel), fieldDefStyle, fieldDefConfigFile);

   SendMessage(hFieldsLB, LB_DELETESTRING, idxFT, NULL);
   SendMessage(hFieldsLB, LB_INSERTSTRING, idxFT, (LPARAM)fieldLabel);
   SendMessage(hFieldsLB, LB_SETCURSEL, idxFT, NULL);

   newFieldDef = FALSE;
   cleanStyleDefs = TRUE;
   enableFieldSelection();
}

void FieldTypeDialog::chooseStyleDefColor(bool back) {
   StyleDefComponent::chooseStyleDefColor(back);
   enableFieldSelection();
}
