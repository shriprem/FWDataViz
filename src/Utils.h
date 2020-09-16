#pragma once

#include "PluginDefinition.h"

#define PREFS_TIP_MAX_WIDTH         400

namespace Utils {
   HWND addTooltip(HWND hDlg, int controlID, LPWSTR pTitle, LPWSTR pMessage);
   bool checkBaseOS(winVer os);
   wstring getVersionInfo(LPCWSTR key);
   void loadBitmap(HWND hDlg, int controlID, int resource);
   void setFont(HWND hDlg, int controlID, wstring &name, int height,
      int weight=FW_REGULAR, bool italic=FALSE, bool underline=FALSE);
   bool setFontBold(HWND hDlg, int controlID);
   bool setFontItalic(HWND hDlg, int controlID);
   bool setFontUnderline(HWND hDlg, int controlID);
}
