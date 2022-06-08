#pragma once

#include "PluginDefinition.h"
#include <codecvt>
#include <regex>
#include <ShlObj_core.h>

#define PREFS_TIP_MAX_WIDTH         400
#define mbox(message)               MessageBox(NULL, message, L"", MB_OK)
#define mboxA(message)              MessageBoxA(NULL, message, "", MB_OK)

namespace Utils {
   int StringtoInt(const string& str, int base = 10);
   int StringtoInt(const wstring& str, int base = 10);
   LPCWSTR ToUpper(LPWSTR str);
   wstring NarrowToWide(const string& str);
   string WideToNarrow(const wstring& wStr);
   bool isInvalidRegex(const string& expr);
   bool isInvalidRegex(const wstring& expr, HWND hWnd, const wstring& context);

   COLORREF intToRGB(int color);
   int scaleDPIX(int x);
   int scaleDPIY(int y);
   wstring getSpecialFolder(int folderID);
   wstring getKnownFolderPath(REFKNOWNFOLDERID folderID);

   HWND addTooltip(HWND hDlg, int controlID, LPWSTR pTitle, LPWSTR pMessage, bool bBalloon = TRUE);
   HWND addTooltip(HWND hDlg, int controlID, LPWSTR pTitle, LPWSTR pMessage, int duration, bool bBalloon);
   void addToolbarIcon(int menuIndex, int std, int fluent, int dark);
   void checkMenuItem(int menuIndex, bool check);
   void showEditBalloonTip(HWND hEdit, LPCWSTR title, LPCWSTR text);
   bool checkBaseOS(winVer os);
   float getNPPVersion();
   bool checkKeyHeldDown(int vKey);
   void setComboBoxSelection(HWND hList, int index);
   bool getClipboardText(HWND hwnd, wstring& clipText);
   wstring getVersionInfo(LPCWSTR key);
   void loadBitmap(HWND hDlg, int controlID, int resource);
   void setFont(HWND hDlg, int controlID, wstring& name, int height,
      int weight = FW_REGULAR, bool italic = FALSE, bool underline = FALSE);
   bool setFontRegular(HWND hDlg, int controlID);
   bool setFontBold(HWND hDlg, int controlID);
   bool setFontItalic(HWND hDlg, int controlID);
   bool setFontUnderline(HWND hDlg, int controlID);
}
