#include "Utils.h"

extern HINSTANCE _gModule;
extern FuncItem pluginMenuItems[MI_COUNT];

// ***************** PRIVATE *****************

enum fontStyle {
   FS_REGULAR,
   FS_BOLD,
   FS_ITALIC,
   FS_UNDERLINE
};

bool changeFontStyle(HWND hDlg, int controlID, fontStyle style) {
   HWND hwndCtrl = GetDlgItem(hDlg, controlID);
   LOGFONT lf{ 0 };

   HFONT hFont = (HFONT)SendMessage(hwndCtrl, WM_GETFONT, NULL, NULL);

   if (!GetObject(hFont, sizeof(lf), &lf))
      return FALSE;

   switch (style) {
      case FS_REGULAR:
         lf.lfWeight = FW_REGULAR;
         lf.lfItalic = FALSE;
         lf.lfUnderline = FALSE;
         break;

      case FS_BOLD:
         lf.lfWeight = FW_BOLD;
         break;

      case FS_ITALIC:
         lf.lfItalic = TRUE;
         break;

      case FS_UNDERLINE:
         lf.lfUnderline = TRUE;
         break;
   }

   hFont = CreateFontIndirect(&lf);
   SendMessage(hwndCtrl, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(true, 0));

   return TRUE;
}


// ***************** PUBLIC *****************

int Utils::StringtoInt(const string& str, int base) {
   if (str.empty()) return 0;

   try {
      return stoi(str, nullptr, base);
   }
   catch (...) {
      return 0;
   }
}

int Utils::StringtoInt(const wstring& str, int base) {
   if (str.empty()) return 0;

   try {
      return stoi(str, nullptr, base);
   }
   catch (...) {
      return 0;
   }
}

LPCWSTR Utils::ToUpper(LPWSTR str) {
   return std::use_facet<std::ctype<wchar_t>>(std::locale()).toupper(str, str + wcslen(str));
}

wstring Utils::NarrowToWide(const string& str) {
   return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
}

string Utils::WideToNarrow(const wstring& wStr) {
   return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wStr);
}

bool Utils::isInvalidRegex(const string& expr) {
   try {
      std::regex re(expr);
   }
   catch (const std::regex_error&) {
      return true;
   }

   return false;
}

bool Utils::isInvalidRegex(const wstring& expr, HWND hWnd, const wstring& context) {
   try {
      std::wregex re(expr);
   }
   catch (const std::regex_error& e) {
      MessageBox(hWnd,
         (context + ((!context.empty()) ? L"\r\n" : L"") + NarrowToWide(e.what())).c_str(),
         UTILS_REGEX_ERROR,
         MB_OK | MB_ICONERROR);
      return true;
   }

   return false;
}

COLORREF Utils::intToRGB(int color) {
   return RGB(GetRValue(color), GetGValue(color), GetBValue(color));
}

wstring Utils::getSpecialFolder(int folderID) {
   TCHAR sFolderPath[MAX_PATH]{};
   const HRESULT ret = SHGetFolderPath(NULL, folderID, NULL, SHGFP_TYPE_CURRENT, sFolderPath);

   return ((ret == S_OK) ? wstring{ sFolderPath } : L"");
}

wstring Utils::getKnownFolderPath(REFKNOWNFOLDERID folderID) {
   PWSTR path;

   const HRESULT ret = SHGetKnownFolderPath(folderID, KF_FLAG_DEFAULT, NULL, &path);
   if (ret != S_OK) return L"";

   wstring sFolderPath{ path };
   CoTaskMemFree(path);
   return sFolderPath;
}

HWND Utils::addTooltip(HWND hDlg, int controlID, const wstring& pTitle, const wstring& pMessage, bool bBalloon) {
   if (!controlID || !hDlg || pMessage.empty())
      return FALSE;

   // Get the window of the tool.
   HWND hwndCtrl = GetDlgItem(hDlg, controlID);

   UINT ttsBalloon = bBalloon ? TTS_BALLOON : NULL;

   // Create the tooltip.
   HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
      WS_POPUP | TTS_ALWAYSTIP | ttsBalloon,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      hDlg, NULL,
      _gModule, NULL);

   if (!hwndCtrl || !hwndTip)
      return (HWND)NULL;

   // Associate the tooltip with the tool.
   TOOLINFO toolInfo{};
   toolInfo.cbSize = sizeof(toolInfo);
   toolInfo.hwnd = hDlg;
   toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
   toolInfo.uId = (UINT_PTR)hwndCtrl;
   toolInfo.lpszText = (LPWSTR)pMessage.c_str();
   SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
   SendMessage(hwndTip, TTM_SETTITLE, TTI_INFO, (LPARAM)pTitle.c_str());
   SendMessage(hwndTip, TTM_SETMAXTIPWIDTH, 0, (LPARAM)PREFS_TIP_MAX_WIDTH);

   return hwndTip;
}

HWND Utils::addTooltip(HWND hDlg, int controlID, const wstring& pTitle, const wstring& pMessage, int duration, bool bBalloon) {
   HWND hwndTip{ addTooltip(hDlg, controlID, pTitle, pMessage, bBalloon) };
   SendMessage(hwndTip, TTM_SETDELAYTIME, TTDT_AUTOPOP, (LPARAM)(duration * 1000));
   return hwndTip;
}

void Utils::updateTooltip(HWND hDlg, int controlID, HWND hTip, const wstring& pMessage) {
   TOOLINFO toolInfo{};
   toolInfo.cbSize = sizeof(toolInfo);
   toolInfo.hwnd = hDlg;
   toolInfo.uId = (UINT_PTR)GetDlgItem(hDlg, controlID);
   toolInfo.lpszText = (LPWSTR)pMessage.c_str();
   SendMessage(hTip, TTM_UPDATETIPTEXT, 0, (LPARAM)&toolInfo);
}

void Utils::addToolbarIcon(int menuIndex, int std, int fluent, int dark) {
   toolbarIconsWithDarkMode tbIcon{};

   tbIcon.hToolbarBmp = LoadBitmap(_gModule, MAKEINTRESOURCE(std));
   tbIcon.hToolbarIcon = LoadIcon(_gModule, MAKEINTRESOURCE(fluent));
   tbIcon.hToolbarIconDarkMode = LoadIcon(_gModule, MAKEINTRESOURCE(dark));

   NppMessage(NPPM_ADDTOOLBARICON_FORDARKMODE, pluginMenuItems[menuIndex]._cmdID, (LPARAM)&tbIcon);
}

void Utils::checkMenuItem(int menuIndex, bool check) {
   NppMessage(NPPM_SETMENUITEMCHECK, pluginMenuItems[menuIndex]._cmdID, check);
}

void Utils::showEditBalloonTip(HWND hEdit, LPCWSTR title, LPCWSTR text) {
   EDITBALLOONTIP tip{};

   tip.cbStruct = sizeof(tip);
   tip.pszTitle = title;
   tip.pszText = text;
   tip.ttiIcon = TTI_ERROR;

   SendMessage(hEdit, EM_SHOWBALLOONTIP, NULL, (LPARAM)&tip);
   MessageBeep(MB_OK);
}

bool Utils::checkBaseOS(winVer os) {
   return (NppMessage(NPPM_GETWINDOWSVERSION) >= os);
}

float Utils::getNPPVersion() {
   long versionNum{ static_cast<long>(NppMessage(NPPM_GETNPPVERSION, TRUE)) };

   return std::stof(to_wstring(HIWORD(versionNum)) + L"." + to_wstring(LOWORD(versionNum)));
}

bool Utils::checkKeyHeldDown(int vKey) {
   return (GetKeyState(vKey) & 0x8000) > 0;
}

wstring Utils::getListBoxItem(HWND hList, bool currentSelection, const int itemIndex) {
   int index{};

   if (currentSelection) {
      index = static_cast<int>(SendMessage(hList, LB_GETCURSEL, 0, 0));
      if (index == LB_ERR) return L"";
   }
   else
      index = itemIndex;

   int itemLength{ static_cast<int>(SendMessage(hList, LB_GETTEXTLEN, index, 0)) };

   wstring itemText(itemLength + 1, '\0');
   SendMessage(hList, LB_GETTEXT, index, (LPARAM)itemText.c_str());
   return itemText;
}

void Utils::setComboBoxSelection(HWND hList, int index) {
   SendMessage(hList, CB_SETCURSEL, index, 0);
   InvalidateRect(hList, nullptr, FALSE);
}

bool Utils::getClipboardText(HWND hwnd, wstring& clipText) {
   bool bRet{ FALSE };

   if (!IsClipboardFormatAvailable(CF_UNICODETEXT))
      return bRet;

   if (!OpenClipboard(hwnd))
      return bRet;

   HGLOBAL hglb = GetClipboardData(CF_UNICODETEXT);
   if (hglb != NULL) {

      LPTSTR lpClipData = (LPTSTR)GlobalLock(hglb);
      if (lpClipData != NULL) {
         bRet = TRUE;
         clipText = wstring{ lpClipData };
         GlobalUnlock(hglb);
      }

      CloseClipboard();
   }

   return bRet;
}

wstring Utils::getVersionInfo(LPCWSTR key) {
   wstring sVersionInfo;
   wchar_t sModuleFilePath[MAX_PATH];
   DWORD  verHandle{};
   DWORD  verSize{};
   UINT   querySize{};
   LPTSTR lpBuffer{};

   struct LANGANDCODEPAGE {
      WORD wLanguage;
      WORD wCodePage;
   } *lpTranslate{};

   GetModuleFileName(_gModule, sModuleFilePath, MAX_PATH);
   verSize = GetFileVersionInfoSize(sModuleFilePath, &verHandle);
   if (verSize < 1) return L"";

   string versionData(verSize, '\0');
   if (!GetFileVersionInfo(sModuleFilePath, NULL, verSize, versionData.data())) return L"";
   VerQueryValue(versionData.data(), L"\\VarFileInfo\\Translation", (VOID FAR* FAR*)& lpTranslate, &querySize);

   wstring verSubBlock(100, '\0');
   swprintf(verSubBlock.data(), 100, L"\\StringFileInfo\\%04X%04X\\%s",
      lpTranslate[0].wLanguage, lpTranslate[0].wCodePage, key);

   if (VerQueryValue(versionData.data(), verSubBlock.c_str(), (VOID FAR* FAR*)& lpBuffer, &querySize)
      && querySize)
      sVersionInfo = wstring{ lpBuffer };

   return sVersionInfo;
}

void Utils::loadBitmap(HWND hDlg, int controlID, int resource) {
   HWND hwndCtrl = GetDlgItem(hDlg, controlID);
   HBITMAP hBitmap = LoadBitmap(_gModule, MAKEINTRESOURCE(resource));

   if (hBitmap) {
      SendMessage(hwndCtrl, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
      DeleteObject(hBitmap);
   }
}

void Utils::setFont(HWND hDlg, int controlID, wstring& name, int height, int weight, bool italic, bool underline) {
   HWND hwndCtrl = GetDlgItem(hDlg, controlID);
   LOGFONT lf{ 0 };

   wcscpy(lf.lfFaceName, name.c_str());

   HDC hdc = GetDC(hDlg);
   lf.lfHeight = -MulDiv(height, GetDeviceCaps(hdc, LOGPIXELSY), 72);
   ReleaseDC(hDlg, hdc);

   lf.lfWeight = weight;
   lf.lfItalic = italic;
   lf.lfUnderline = underline;

   HFONT hFont = CreateFontIndirect(&lf);

   SendMessage(hwndCtrl, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(true, 0));
}

bool Utils::setFontRegular(HWND hDlg, int controlID) {
   return changeFontStyle(hDlg, controlID, FS_REGULAR);
}

bool Utils::setFontBold(HWND hDlg, int controlID) {
   return changeFontStyle(hDlg, controlID, FS_BOLD);
}

bool Utils::setFontItalic(HWND hDlg, int controlID) {
   return changeFontStyle(hDlg, controlID, FS_ITALIC);
}

bool Utils::setFontUnderline(HWND hDlg, int controlID) {
   return changeFontStyle(hDlg, controlID, FS_UNDERLINE);
}

int Utils::scaleDPIX(int x) {
   HDC hdc = GetDC(NULL);
   if (!hdc) return 0;

   int scaleX{ MulDiv(x, GetDeviceCaps(hdc, LOGPIXELSX), 96) };
   ReleaseDC(NULL, hdc);
   return scaleX;
}

int Utils::scaleDPIY(int y) {
   HDC hdc = GetDC(NULL);
   if (!hdc) return 0;

   int scaleY{ MulDiv(y, GetDeviceCaps(hdc, LOGPIXELSY), 96) };
   ReleaseDC(NULL, hdc);
   return scaleY;
}

int Utils::getTextPixelWidth(HWND hDlg, const wstring& text) {
   SIZE textSize{};
   HDC hDC{ GetDC(hDlg) };

   SelectObject(hDC, (HFONT)SendMessage(hDlg, WM_GETFONT, 0, 0));
   GetTextExtentPoint32(hDC, text.c_str(), static_cast<int>(text.length()), &textSize);

   ReleaseDC(hDlg, hDC);

   return textSize.cx;
}

bool Utils::checkDirectoryExists(LPCWSTR lpDirPath) {
   DWORD dwAttrib = GetFileAttributes(lpDirPath);
   return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool Utils::checkFileExists(LPCWSTR lpFilePath) {
   DWORD dwAttrib = GetFileAttributes(lpFilePath);
   return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
