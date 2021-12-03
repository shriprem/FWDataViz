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

   Utils::addTooltip(_hSelf, IDC_PREF_CLEARVIZ_AUTO, NULL, PREF_CLEARVIZ_AUTO_TIP, PREF_TIP_LONG, TRUE);
   Utils::addTooltip(_hSelf, IDC_PREF_CLEARVIZ_PANEL, NULL, PREF_CLEARVIZ_PANEL_TIP, PREF_TIP_LONG, TRUE);
   Utils::addTooltip(_hSelf, IDC_PREF_MBCHARS_STATE, NULL, PREF_MBCHARS_STATE_TIP, PREF_TIP_LONG, TRUE);

   if (_gLanguage != LANG_ENGLISH) localize();
   goToCenter();

   SendMessage(_hParent, NPPM_DMMSHOW, 0, (LPARAM)_hSelf);
}

INT_PTR PreferencesDialog::run_dlgProc(UINT message, WPARAM wParam, LPARAM /*lParam*/) {
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

      case IDCANCEL:
      case IDCLOSE:
         display(FALSE);
         return TRUE;
      }
      break;

   case WM_INITDIALOG:
      if (NPPDM_IsEnabled()) {
         NPPDM_AutoSubclassAndThemeChildControls(_hSelf);
      }
      break;

   case WM_CTLCOLORDLG:
   case WM_CTLCOLORLISTBOX:
   case WM_CTLCOLORSTATIC:
      if (NPPDM_IsEnabled()) {
         return NPPDM_OnCtlColorDarker(reinterpret_cast<HDC>(wParam));
      }
      break;

   case WM_PRINTCLIENT:
      if (NPPDM_IsEnabled()) {
         return TRUE;
      }
      break;
   }

   return FALSE;
}

void PreferencesDialog::localize() {
   SetWindowText(_hSelf, PREFERENCES_DIALOG_TITLE);
   SetDlgItemText(_hSelf, IDC_PREF_CLEARVIZ_AUTO, PREFERENCES_CLEARVIZ_AUTO);
   SetDlgItemText(_hSelf, IDC_PREF_CLEARVIZ_PANEL, PREFERENCES_CLEARVIZ_PANEL);
   SetDlgItemText(_hSelf, IDC_PREF_MBCHARS_STATE, PREFERENCES_MBCHARS_STATE);
   SetDlgItemText(_hSelf, IDCLOSE, PREFERENCES_CLOSE_BTN);
}

void PreferencesDialog::initCheckbox(int nIDButton, const string& preference, bool default) {
   CheckDlgButton(_hSelf, nIDButton, _configIO.getPreferenceBool(preference, default) ? BST_CHECKED : BST_UNCHECKED);
}

void PreferencesDialog::setCheckbox(int nIDButton, const string& preference) {
   bool checked{ IsDlgButtonChecked(_hSelf, nIDButton) == BST_CHECKED };

   _configIO.setPreferenceBool(preference, checked);
}
