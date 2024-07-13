#include "StyleDefComponent.h"

StyleDefComponent::~StyleDefComponent() {
   if (hbr != NULL) DeleteObject(hbr);
}

void StyleDefComponent::initComponent(HWND hDlg) {
   _hDialog = hDlg;

   SendDlgItemMessage(_hDialog, IDC_STYLE_DEF_BACK_EDIT, EM_LIMITTEXT, 6, NULL);
   SendDlgItemMessage(_hDialog, IDC_STYLE_DEF_FORE_EDIT, EM_LIMITTEXT, 6, NULL);

   SetWindowSubclass(GetDlgItem(_hDialog, IDC_STYLE_DEF_BACK_EDIT), procHexColorEditControl, NULL, NULL);
   SetWindowSubclass(GetDlgItem(_hDialog, IDC_STYLE_DEF_FORE_EDIT), procHexColorEditControl, NULL, NULL);

   bool recentOS = Utils::checkBaseOS(WV_VISTA);
   wstring fontName = recentOS ? L"Consolas" : L"Courier New";
   int fontHeight = recentOS ? 8 : 7;

   Utils::setFont(_hDialog, IDC_STYLE_DEF_PREVIEW_BOX, fontName, fontHeight);

   setPangram();
}

void StyleDefComponent::localize() const {
   SetDlgItemText(_hDialog, IDC_STYLE_DEF_GROUP_BOX, STYLE_DEF_GROUP_BOX);
   SetDlgItemText(_hDialog, IDC_STYLE_DEF_BACK_LABEL, STYLE_DEF_BACK_LABEL);
   SetDlgItemText(_hDialog, IDC_STYLE_DEF_FORE_LABEL, STYLE_DEF_FORE_LABEL);
   SetDlgItemText(_hDialog, IDC_STYLE_DEF_BOLD, STYLE_DEF_BOLD);
   SetDlgItemText(_hDialog, IDC_STYLE_DEF_ITALICS, STYLE_DEF_ITALICS);
   SetDlgItemText(_hDialog, IDC_STYLE_DEF_PREVIEW_LABEL, STYLE_DEF_PREVIEW_LABEL);
}

int StyleDefComponent::getStyleDefColor(bool back) const {
   TCHAR buf[10];

   GetDlgItemText(_hDialog, back ? IDC_STYLE_DEF_BACK_EDIT : IDC_STYLE_DEF_FORE_EDIT, buf, 7);

   return Utils::StringtoInt(buf, 16);
}

void StyleDefComponent::setStyleDefColor(bool setEdit, int color, bool back) {
   if (setEdit) {
      TCHAR buf[10];
      swprintf(buf, 7, L"%06X", color);

      SetDlgItemText(_hDialog, back ? IDC_STYLE_DEF_BACK_EDIT : IDC_STYLE_DEF_FORE_EDIT, buf);
   }

   // Set styleBack | styleFore here. Will be used in WM_CTLCOLORSTATIC, triggered by the setFontRegular() calls
   styleDefColor = TRUE;
   (back ? styleBack : styleFore) = Utils::intToRGB(color);
   Utils::setFontRegular(_hDialog, back ? IDC_STYLE_DEF_BACKCOLOR : IDC_STYLE_DEF_FORECOLOR);
   setOutputFontStyle();
}

void StyleDefComponent::setOutputFontStyle() const {
   Utils::setFontRegular(_hDialog, IDC_STYLE_DEF_PREVIEW_BOX);

   if (IsDlgButtonChecked(_hDialog, IDC_STYLE_DEF_BOLD) == BST_CHECKED)
      Utils::setFontBold(_hDialog, IDC_STYLE_DEF_PREVIEW_BOX);

   if (IsDlgButtonChecked(_hDialog, IDC_STYLE_DEF_ITALICS) == BST_CHECKED)
      Utils::setFontItalic(_hDialog, IDC_STYLE_DEF_PREVIEW_BOX);
}

void StyleDefComponent::fillStyleDefs(StyleInfo& style) {
   CheckDlgButton(_hDialog, IDC_STYLE_DEF_BOLD, style.bold ? BST_CHECKED : BST_UNCHECKED);
   CheckDlgButton(_hDialog, IDC_STYLE_DEF_ITALICS, style.italics ? BST_CHECKED : BST_UNCHECKED);

   setStyleDefColor(TRUE, style.backColor, TRUE);
   setStyleDefColor(TRUE, style.foreColor, FALSE);

   cleanStyleDefs = TRUE;
}

void StyleDefComponent::setPangram() const {
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

   SetDlgItemText(_hDialog, IDC_STYLE_DEF_PREVIEW_BOX, (pangrams[rand() % count]).c_str());
}

INT_PTR StyleDefComponent::colorStaticControl(WPARAM wParam, LPARAM lParam) {
   HDC hdc = (HDC)wParam;
   DWORD ctrlID = GetDlgCtrlID((HWND)lParam);

   if (hbr != NULL) DeleteObject(hbr);

   switch (ctrlID) {
   case IDC_STYLE_DEF_BACKCOLOR:
      SetBkColor(hdc, styleBack);
      hbr = CreateSolidBrush(styleBack);
      return (INT_PTR)hbr;

   case IDC_STYLE_DEF_FORECOLOR:
      SetBkColor(hdc, styleFore);
      hbr = CreateSolidBrush(styleFore);
      return (INT_PTR)hbr;

   case IDC_STYLE_DEF_PREVIEW_BOX:
      SetBkColor(hdc, styleBack);
      SetTextColor(hdc, styleFore);
      hbr = CreateSolidBrush(styleBack);
      return (INT_PTR)hbr;

   default:
      return NULL;
   }
}

void StyleDefComponent::chooseStyleDefColor(bool back) {
   int color = getStyleDefColor(back);

   CHOOSECOLOR cc;
   ZeroMemory(&cc, sizeof(cc));

   cc.lStructSize = sizeof(cc);
   cc.hwndOwner = _hDialog;
   cc.rgbResult = Utils::intToRGB(color);
   cc.lpCustColors = (LPDWORD)customColors;
   cc.Flags = CC_FULLOPEN | CC_RGBINIT;

   if (!ChooseColor(&cc)) return;

   color = static_cast<int>(cc.rgbResult);
   setStyleDefColor(TRUE, color, back);

   cleanStyleDefs = FALSE;
}

LRESULT CALLBACK procHexColorEditControl(HWND hwnd, UINT messageId, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
   switch (messageId) {
   case WM_CHAR:
   {
      char ch{ static_cast<char>(wParam) };
      if (!((ch == VK_BACK) || (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'))) {
      Utils::showEditBalloonTip(hwnd, STYLE_DEF_HEX_TITLE, STYLE_DEF_HEX_CHARS_ONLY);
      return FALSE;
      }
      break;
   }

   case WM_PASTE:
   {
      wstring clipText;
      Utils::getClipboardText(GetParent(hwnd), clipText);
      if (!regex_match(clipText, std::wregex(L"^[0-9A-Fa-f]{0,6}$"))) {
         Utils::showEditBalloonTip(hwnd, STYLE_DEF_HEX_TITLE, STYLE_DEF_HEX_CHARS_ONLY);
         return FALSE;
      }
      break;
   }
   }

   return DefSubclassProc(hwnd, messageId, wParam, lParam);
}
