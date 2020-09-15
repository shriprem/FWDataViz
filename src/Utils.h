#pragma once

#include "PluginDefinition.h"

using std::string;
using std::string_view;
using std::wstring;
using std::to_string;
using std::to_wstring;

namespace Utils {
   HWND addToolTip(HWND hDlg, int controlID, LPWSTR pTitle, LPWSTR pMessage);
   bool checkBaseOS(winVer os);
   void loadBitmap(HWND hDlg, int controlID, int resource);
   void setFont(HWND hDlg, int controlID, wstring &name, int height,
      int weight=FW_REGULAR, bool italic=FALSE, bool underline=FALSE);
   bool setFontBold(HWND hDlg, int controlID);
   bool setFontItalic(HWND hDlg, int controlID);
   bool setFontUnderline(HWND hDlg, int controlID);
}
