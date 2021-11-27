#include "FieldTypeDialog.h"

FieldTypeDialog::~FieldTypeDialog() {
   if (hbr != NULL) DeleteObject(hbr);
}

void FieldTypeDialog::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_FIELD_TYPE_DEFINER_DIALOG);
   }

   fieldDefConfigFile = Utils::WideToNarrow(_configIO.getConfigFile(_configIO.CONFIG_FIELD_TYPES));
   hFieldsLB = GetDlgItem(_hSelf, IDC_FIELD_TYPE_LIST_BOX);

   SendDlgItemMessage(_hSelf, IDC_FIELD_TYPE_DESC_EDIT, EM_LIMITTEXT, MAX_PATH, NULL);
   SendDlgItemMessage(_hSelf, IDC_FIELD_STYLE_DEF_BACK_EDIT, EM_LIMITTEXT, 6, NULL);
   SendDlgItemMessage(_hSelf, IDC_FIELD_STYLE_DEF_FORE_EDIT, EM_LIMITTEXT, 6, NULL);

   SetWindowSubclass(GetDlgItem(_hSelf, IDC_FIELD_STYLE_DEF_BACK_EDIT), procHexColorEditControl, NULL, NULL);
   SetWindowSubclass(GetDlgItem(_hSelf, IDC_FIELD_STYLE_DEF_FORE_EDIT), procHexColorEditControl, NULL, NULL);

   Utils::loadBitmap(_hSelf, IDC_FIELD_TYPE_INFO_BUTTON, IDB_VIZ_INFO_BITMAP);
   Utils::addTooltip(_hSelf, IDC_FIELD_TYPE_INFO_BUTTON, NULL, VIZ_PANEL_INFO_TIP, FALSE);

   bool recentOS = Utils::checkBaseOS(WV_VISTA);
   wstring fontName = recentOS ? L"Consolas" : L"Courier New";
   int fontHeight = recentOS ? 8 : 7;

   Utils::setFont(_hSelf, IDC_FIELD_STYLE_DEF_OUTPUT, fontName, fontHeight);

   if (_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);

   fillFields();
   setPangram();
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

      case IDC_FIELD_STYLE_DEF_BACK_EDIT:
      case IDC_FIELD_STYLE_DEF_FORE_EDIT:
         switch (HIWORD(wParam)) {
         case EN_CHANGE:
            bool back = (LOWORD(wParam) == IDC_FIELD_STYLE_DEF_BACK_EDIT);
            setStyleDefColor(FALSE, getStyleDefColor(back), back);
            if (!loadingEdits) {
               cleanStyleDefs = FALSE;
               enableFieldSelection();
            }
            break;
         }
         break;

      case IDC_FIELD_STYLE_DEF_BACKCOLOR:
         chooseStyleDefColor(TRUE);
         break;

      case IDC_FIELD_STYLE_DEF_FORECOLOR:
         chooseStyleDefColor(FALSE);
         break;

      case IDC_FIELD_STYLE_DEF_BOLD:
      case IDC_FIELD_STYLE_DEF_ITALICS:
         setOutputFontStyle();
         cleanStyleDefs = FALSE;
         enableFieldSelection();
         break;

      case IDC_FIELD_STYLE_DEF_OUTPUT:
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

      if (NppDarkMode::isEnabled()) {
         return NppDarkMode::onCtlColorDarker(reinterpret_cast<HDC>(wParam));
      }
      break;

   case WM_CTLCOLORDLG:
   case WM_CTLCOLORLISTBOX:
      if (NppDarkMode::isEnabled()) {
         return NppDarkMode::onCtlColorDarker(reinterpret_cast<HDC>(wParam));
      }
      break;

   case WM_CTLCOLOREDIT:
      if (NppDarkMode::isEnabled()) {
         return NppDarkMode::onCtlColorSofter(reinterpret_cast<HDC>(wParam));
      }
      break;

   case WM_INITDIALOG:
      if (NppDarkMode::isEnabled()) {
         NppDarkMode::autoSubclassAndThemeChildControls(_hSelf);
      }
      break;

   case WM_PRINTCLIENT:
      if (NppDarkMode::isEnabled()) {
         return TRUE;
      }
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
   SetDlgItemText(_hSelf, IDC_FIELD_STYLE_DEF_GROUP_BOX, FIELD_STYLE_DEF_GROUP_BOX);
   SetDlgItemText(_hSelf, IDC_FIELD_STYLE_DEF_BACK_LABEL, FIELD_STYLE_DEF_BACK_LABEL);
   SetDlgItemText(_hSelf, IDC_FIELD_STYLE_DEF_FORE_LABEL, FIELD_STYLE_DEF_FORE_LABEL);
   SetDlgItemText(_hSelf, IDC_FIELD_STYLE_DEF_BOLD, FIELD_STYLE_DEF_BOLD);
   SetDlgItemText(_hSelf, IDC_FIELD_STYLE_DEF_ITALICS, FIELD_STYLE_DEF_ITALICS);
   SetDlgItemText(_hSelf, IDC_FIELD_STYLE_DEF_OUT_LABEL, FIELD_STYLE_DEF_OUT_LABEL);
   SetDlgItemText(_hSelf, IDC_FIELD_STYLE_DEF_SAVE_BTN, FIELD_STYLE_DEF_SAVE_BTN);
   SetDlgItemText(_hSelf, IDC_FIELD_STYLE_DEF_RESET_BTN, FIELD_STYLE_DEF_RESET_BTN);
}

int FieldTypeDialog::getCurrentFieldIndex() {
   int idxFT;

   idxFT = static_cast<int>(SendMessage(hFieldsLB, LB_GETCURSEL, NULL, NULL));
   if (idxFT == LB_ERR) return LB_ERR;

   return idxFT;
}

string FieldTypeDialog::getNewStyle() {
   int backColor{ static_cast<int>(nppMessage(NPPM_GETEDITORDEFAULTBACKGROUNDCOLOR, NULL, NULL)) };
   int foreColor{ static_cast<int>(nppMessage(NPPM_GETEDITORDEFAULTFOREGROUNDCOLOR, NULL, NULL)) };

   char styleDef[17];
   snprintf(styleDef, 17, "%06X %06X 00", backColor, foreColor);
   return string{ styleDef };
}

string FieldTypeDialog::getStyleConfig() {
   int backColor{ getStyleDefColor(TRUE) };
   int foreColor{ getStyleDefColor(FALSE) };
   bool bold{ (IsDlgButtonChecked(_hSelf, IDC_FIELD_STYLE_DEF_BOLD) == BST_CHECKED) };
   bool italics{ (IsDlgButtonChecked(_hSelf, IDC_FIELD_STYLE_DEF_ITALICS) == BST_CHECKED) };

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

   for (int i{}; i < keyCount; i++) {
      SendMessage(hFieldsLB, LB_ADDSTRING, NULL, (LPARAM)keyList[i].c_str());
   }

   SendMessage(hFieldsLB, LB_SETCURSEL, 0, NULL);
   onFieldSelect();
}

void FieldTypeDialog::onFieldSelect() {
   int idxFT{ getCurrentFieldIndex() };
   if (idxFT == LB_ERR) return;

   wchar_t label[MAX_PATH];
   SendMessage(hFieldsLB, LB_GETTEXT, (WPARAM)idxFT, (LPARAM)label);

   fieldDefLabel = label;
   fieldDefRegex = _configIO.getConfigWideChar("Validations", Utils::WideToNarrow(label), "", fieldDefConfigFile);
   fieldDefStyle = _configIO.getConfigStringA("Styles", Utils::WideToNarrow(label), "", fieldDefConfigFile);

   fillStyleDefs();
}

void FieldTypeDialog::enableFieldSelection() {
   EnableWindow(GetDlgItem(_hSelf, IDC_FIELD_TYPE_LIST_BOX), cleanStyleDefs);
   EnableWindow(GetDlgItem(_hSelf, IDC_FIELD_TYPE_NEW_BTN), cleanStyleDefs);
   EnableWindow(GetDlgItem(_hSelf, IDC_FIELD_TYPE_CLONE_BTN), cleanStyleDefs);

   wchar_t fieldLabel[MAX_PATH + 1];
   GetDlgItemText(_hSelf, IDC_FIELD_TYPE_DESC_EDIT, fieldLabel, MAX_PATH + 1);
   EnableWindow(GetDlgItem(_hSelf, IDC_FIELD_STYLE_DEF_SAVE_BTN), (wstring{ fieldLabel }.length() > 0));

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
   SendMessage(hFieldsLB, LB_SETCURSEL, (WPARAM)lbLast, NULL);

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
   SendMessage(hFieldsLB, LB_SETCURSEL, (WPARAM)lbLast, NULL);

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

   SendMessage(hFieldsLB, LB_DELETESTRING, (WPARAM)idxFT, NULL);

   int lbLast{ static_cast<int>(SendMessage(hFieldsLB, LB_GETCOUNT, NULL, NULL)) - 1 };
   if (lbLast < 0) {
      fieldEditNew();
      return;
   }

   SendMessage(hFieldsLB, LB_SETCURSEL, (WPARAM)((idxFT <= lbLast) ? idxFT : lbLast), NULL);

   newFieldDef = FALSE;
   onFieldSelect();
}

int FieldTypeDialog::getStyleDefColor(bool back) {
   TCHAR buf[10];

   GetDlgItemText(_hSelf, back ? IDC_FIELD_STYLE_DEF_BACK_EDIT : IDC_FIELD_STYLE_DEF_FORE_EDIT, buf, 7);

   return Utils::StringtoInt(buf, 16);
}

void FieldTypeDialog::setStyleDefColor(bool setEdit, int color, bool back) {
   if (setEdit) {
      TCHAR buf[10];
      swprintf(buf, 7, L"%06X", color);

      loadingEdits = TRUE;
      SetDlgItemText(_hSelf, back ? IDC_FIELD_STYLE_DEF_BACK_EDIT : IDC_FIELD_STYLE_DEF_FORE_EDIT, buf);
      loadingEdits = FALSE;
   }

   // Set styleBack | styleFore here. Will be used in WM_CTLCOLORSTATIC, triggered by the setFontRegular() calls
   styleDefColor = TRUE;
   (back ? styleBack : styleFore) = Utils::intToRGB(color);
   Utils::setFontRegular(_hSelf, back ? IDC_FIELD_STYLE_DEF_BACKCOLOR : IDC_FIELD_STYLE_DEF_FORECOLOR);
   setOutputFontStyle();
}

void FieldTypeDialog::setOutputFontStyle() {
   Utils::setFontRegular(_hSelf, IDC_FIELD_STYLE_DEF_OUTPUT);

   if (IsDlgButtonChecked(_hSelf, IDC_FIELD_STYLE_DEF_BOLD) == BST_CHECKED)
      Utils::setFontBold(_hSelf, IDC_FIELD_STYLE_DEF_OUTPUT);

   if (IsDlgButtonChecked(_hSelf, IDC_FIELD_STYLE_DEF_ITALICS) == BST_CHECKED)
      Utils::setFontItalic(_hSelf, IDC_FIELD_STYLE_DEF_OUTPUT);
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

   CheckDlgButton(_hSelf, IDC_FIELD_STYLE_DEF_BOLD, style.bold ? BST_CHECKED : BST_UNCHECKED);
   CheckDlgButton(_hSelf, IDC_FIELD_STYLE_DEF_ITALICS, style.italics ? BST_CHECKED : BST_UNCHECKED);

   setStyleDefColor(TRUE, style.backColor, TRUE);
   setStyleDefColor(TRUE, style.foreColor, FALSE);

   cleanStyleDefs = TRUE;
   enableFieldSelection();
}

void FieldTypeDialog::styleDefSave() {
   if (cleanStyleDefs) return;

   int idxFT{ getCurrentFieldIndex() };
   if (idxFT == LB_ERR) return;

   wchar_t fieldLabel[MAX_PATH + 1];
   GetDlgItemText(_hSelf, IDC_FIELD_TYPE_DESC_EDIT, fieldLabel, MAX_PATH + 1);
   if (wstring{ fieldLabel }.length() < 1) return;

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

   SendMessage(hFieldsLB, LB_DELETESTRING, (WPARAM)idxFT, NULL);
   SendMessage(hFieldsLB, LB_INSERTSTRING, (WPARAM)idxFT, (LPARAM)fieldLabel);
   SendMessage(hFieldsLB, LB_SETCURSEL, idxFT, NULL);

   newFieldDef = FALSE;
   cleanStyleDefs = TRUE;
   enableFieldSelection();
}

void FieldTypeDialog::setPangram() {
   constexpr int count{ 10 };

   wstring pangrams[count] = {
      L"SPHINX OF BLACK QUARTZ,\r\nJUDGE MY VOW.",
      L"A QUART JAR OF OIL\r\nMIXED WITH ZINC OXIDE\r\nMAKES A VERY BRIGHT PAINT.",
      L"A WIZARD'S JOB IS TO\r\nVEX CHUMPS QUICKLY IN FOG.",
      L"AMAZINGLY FEW DISCOTHEQUES\r\nPROVIDE JUKEBOXES.",
      L"PACK MY BOX WITH\r\nFIVE DOZEN LIQUOR JUGS.",
      L"THE LAZY MAJOR WAS FIXING\r\nCUPID'S BROKEN QUIVER.",
      L"MY FAXED JOKE WON A PAGER\r\nIN THE CABLE TV QUIZ SHOW.",
      L"THE FIVE BOXING WIZARDS\r\nJUMP QUICKLY.",
      L"FEW BLACK TAXIS\r\nDRIVE UP MAJOR ROADS\r\nON QUIET HAZY NIGHTS.",
      L"WE PROMPTLY JUDGED\r\nANTIQUE IVORY BUCKLES\r\nFOR THE NEXT PRIZE."
   };

   SetDlgItemText(_hSelf, IDC_FIELD_STYLE_DEF_OUTPUT, (pangrams[rand() % count]).c_str());
}

INT_PTR FieldTypeDialog::colorStaticControl(WPARAM wParam, LPARAM lParam) {
   HDC hdc = (HDC)wParam;
   DWORD ctrlID = GetDlgCtrlID((HWND)lParam);

   if (hbr != NULL) DeleteObject(hbr);

   switch (ctrlID) {
   case IDC_FIELD_STYLE_DEF_BACKCOLOR:
      SetBkColor(hdc, styleBack);
      hbr = CreateSolidBrush(styleBack);
      return (INT_PTR)hbr;

   case IDC_FIELD_STYLE_DEF_FORECOLOR:
      SetBkColor(hdc, styleFore);
      hbr = CreateSolidBrush(styleFore);
      return (INT_PTR)hbr;

   case IDC_FIELD_STYLE_DEF_OUTPUT:
      SetBkColor(hdc, styleBack);
      SetTextColor(hdc, styleFore);
      hbr = CreateSolidBrush(styleBack);
      return (INT_PTR)hbr;

   default:
      return NULL;
   }
}

void FieldTypeDialog::chooseStyleDefColor(bool back) {
   int color = getStyleDefColor(back);

   CHOOSECOLOR cc;
   ZeroMemory(&cc, sizeof(cc));

   cc.lStructSize = sizeof(cc);
   cc.hwndOwner = _hSelf;
   cc.rgbResult = Utils::intToRGB(color);
   cc.lpCustColors = (LPDWORD)customColors;
   cc.Flags = CC_FULLOPEN | CC_RGBINIT;

   if (!ChooseColor(&cc)) return;

   color = static_cast<int>(cc.rgbResult);
   setStyleDefColor(TRUE, color, back);

   cleanStyleDefs = FALSE;
   enableFieldSelection();
}
