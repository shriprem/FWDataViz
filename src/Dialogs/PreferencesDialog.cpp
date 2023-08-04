#include "PreferencesDialog.h"
#include "VisualizerPanel.h"

constexpr int PREF_TIP_LONG{ 30 };

extern VisualizerPanel _vizPanel;

void PreferencesDialog::doDialog(HINSTANCE hInst) {
   if (!isCreated()) {
      Window::init(hInst, nppData._nppHandle);
      create(IDD_PREFERENCES_DIALOG);
   }

   initCheckbox(IDC_PREF_CLEARVIZ_AUTO, PREF_CLEARVIZ_AUTO, FALSE);
   initCheckbox(IDC_PREF_CLEARVIZ_PANEL, PREF_CLEARVIZ_PANEL, FALSE);
   initCheckbox(IDC_PREF_MBCHARS_STATE, PREF_MBCHARS_SHOW, FALSE);
   initCheckbox(IDC_PREF_HOP_FIELD_LEFT_EDGE, PREF_HOP_RT_LEFT_EDGE, FALSE);

   Utils::addTooltip(_hSelf, IDC_PREF_CLEARVIZ_AUTO, NULL, PREF_CLEARVIZ_AUTO_TIP, PREF_TIP_LONG, TRUE);
   Utils::addTooltip(_hSelf, IDC_PREF_CLEARVIZ_PANEL, NULL, PREF_CLEARVIZ_PANEL_TIP, PREF_TIP_LONG, TRUE);
   Utils::addTooltip(_hSelf, IDC_PREF_MBCHARS_STATE, NULL, PREF_MBCHARS_STATE_TIP, PREF_TIP_LONG, TRUE);
   Utils::addTooltip(_hSelf, IDC_PREF_HOP_FIELD_LEFT_EDGE, NULL, PREF_HOP_RT_LEFT_EDGE_TIP, PREF_TIP_LONG, TRUE);

   displayFoldLineColor();
   int foldLineAlpha{ _configIO.getPreferenceInt(PREF_FOLDLINE_ALPHA, MAXBYTE) };
   SendMessage(GetDlgItem(_hSelf, IDC_PREF_FOLD_LINE_ALPHA_SLIDER), TBM_SETPOS, TRUE, foldLineAlpha);

   if constexpr(_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
}

void PreferencesDialog::refreshDarkMode() {
   NPPDM_AutoThemeChildControls(_hSelf);
   redraw();
   SendMessage(GetDlgItem(_hSelf, IDC_PREF_FOLD_LINE_ALPHA_SLIDER), TBM_SETRANGEMIN, FALSE, 0);
}

INT_PTR PreferencesDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) {
   switch (message) {
   case WM_COMMAND:
      switch LOWORD(wParam) {
      case IDC_PREF_CLEARVIZ_AUTO:
         setCheckbox(IDC_PREF_CLEARVIZ_AUTO, PREF_CLEARVIZ_AUTO);
         break;

      case IDC_PREF_CLEARVIZ_PANEL:
         setCheckbox(IDC_PREF_CLEARVIZ_PANEL, PREF_CLEARVIZ_PANEL);
         break;

      case IDC_PREF_MBCHARS_STATE:
         setCheckbox(IDC_PREF_MBCHARS_STATE, PREF_MBCHARS_SHOW);
         _vizPanel.initMBCharsCheckbox();
         break;

      case IDC_PREF_HOP_FIELD_LEFT_EDGE:
         setCheckbox(IDC_PREF_HOP_FIELD_LEFT_EDGE, PREF_HOP_RT_LEFT_EDGE);
         _vizPanel.updateHopRightTip();
         break;

      case IDC_PREF_FOLD_LINE_COLOR:
         chooseColor();
         break;

      case IDCANCEL:
      case IDCLOSE:
         display(FALSE);
         return TRUE;
      }
      break;

   case WM_HSCROLL:
      if (lParam == (LPARAM)GetDlgItem(_hSelf, IDC_PREF_FOLD_LINE_ALPHA_SLIDER)) setFoldLineAlpha();
      break;

   case WM_INITDIALOG:
      NPPDM_AutoSubclassAndThemeChildControls(_hSelf);
      break;

   case WM_CTLCOLORDLG:
      if (NPPDM_IsEnabled()) {
         return NPPDM_OnCtlColorDarker(reinterpret_cast<HDC>(wParam));
      }
      break;

   case WM_CTLCOLORSTATIC:
   {
      INT_PTR ptr = colorStaticControl(wParam, lParam);
      if (ptr != NULL) return ptr;

      if (NPPDM_IsEnabled()) {
         return NPPDM_OnCtlColorDarker(reinterpret_cast<HDC>(wParam));
      }
      break;
   }

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

void PreferencesDialog::localize() {
   SetWindowText(_hSelf, PREFERENCES_DIALOG_TITLE);
   SetDlgItemText(_hSelf, IDC_PREF_CLEARVIZ_AUTO, PREFERENCES_CLEARVIZ_AUTO);
   SetDlgItemText(_hSelf, IDC_PREF_CLEARVIZ_PANEL, PREFERENCES_CLEARVIZ_PANEL);
   SetDlgItemText(_hSelf, IDC_PREF_MBCHARS_STATE, PREFERENCES_MBCHARS_STATE);
   SetDlgItemText(_hSelf, IDC_PREF_HOP_FIELD_LEFT_EDGE, PREFERENCES_HOP_LEFT_EDGE);
   SetDlgItemText(_hSelf, IDCLOSE, PREFERENCES_CLOSE_BTN);
}

void PreferencesDialog::initCheckbox(int nIDButton, const string& preference, bool default) {
   CheckDlgButton(_hSelf, nIDButton, _configIO.getPreferenceBool(preference, default) ? BST_CHECKED : BST_UNCHECKED);
}

void PreferencesDialog::setCheckbox(int nIDButton, const string& preference) {
   bool checked{ IsDlgButtonChecked(_hSelf, nIDButton) == BST_CHECKED };

   _configIO.setPreferenceBool(preference, checked);
}

void PreferencesDialog::chooseColor() {
   COLORREF customColors[16];

   CHOOSECOLOR cc;
   ZeroMemory(&cc, sizeof(cc));

   cc.lStructSize = sizeof(cc);
   cc.hwndOwner = _hSelf;
   cc.rgbResult = getPreferenceFoldLineColor();
   cc.lpCustColors = (LPDWORD)customColors;
   cc.Flags = CC_FULLOPEN | CC_RGBINIT;

   if (!ChooseColor(&cc)) return;

   setPreferenceFoldLineColor(cc.rgbResult);
   displayFoldLineColor();
}

COLORREF PreferencesDialog::getPreferenceFoldLineColor() {
   return Utils::intToRGB(Utils::StringtoInt(_configIO.getPreference(PREF_FOLDLINE_COLOR, "0"), 16));
}

void PreferencesDialog::setPreferenceFoldLineColor(COLORREF rgbColor) {
   wchar_t hexColor[10];
   swprintf(hexColor, 7, L"%06X", static_cast<int>(rgbColor));
   _configIO.setPreference(PREF_FOLDLINE_COLOR, hexColor);
}

void PreferencesDialog::displayFoldLineColor() {
   Utils::setFontRegular(_hSelf, IDC_PREF_FOLD_LINE_COLOR);
   applyFoldLineColorAlpha();
}

INT_PTR PreferencesDialog::colorStaticControl(WPARAM wParam, LPARAM lParam) {
   HDC hdc = (HDC)wParam;
   DWORD ctrlID = GetDlgCtrlID((HWND)lParam);
   COLORREF rgbColor{ getPreferenceFoldLineColor() };

   if (hbr != NULL) DeleteObject(hbr);


   if (ctrlID == IDC_PREF_FOLD_LINE_COLOR) {
      SetBkColor(hdc, rgbColor);
      hbr = CreateSolidBrush(rgbColor);
      return (INT_PTR)hbr;
   }

   return NULL;
}

void PreferencesDialog::setFoldLineAlpha() {
   _configIO.setPreferenceInt(PREF_FOLDLINE_ALPHA,
      static_cast<int>(SendMessage(GetDlgItem(_hSelf, IDC_PREF_FOLD_LINE_ALPHA_SLIDER), TBM_GETPOS, 0, 0)));
   applyFoldLineColorAlpha();
}

void PreferencesDialog::applyFoldLineColorAlpha() {
   HWND hScintilla{ getCurrentScintilla() };
   if (!hScintilla) return;

   int foldColorAlpha{ static_cast<int>(getPreferenceFoldLineColor()) };
   foldColorAlpha |= _configIO.getPreferenceInt(PREF_FOLDLINE_ALPHA, MAXBYTE) << 24;
   SendMessage(hScintilla, SCI_SETELEMENTCOLOUR, SC_ELEMENT_FOLD_LINE, foldColorAlpha);
}
