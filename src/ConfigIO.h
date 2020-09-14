#pragma once

#ifndef _PREFERENCES_INI_H
#define _PREFERENCES_INI_H

#include "NPP/PluginInterface.h"
#include <codecvt>
#include <locale>
#include <unordered_map>
#include <vector>

extern NppData nppData;

using std::string;
using std::wstring;
using std::vector;

class ConfigIO {
public:
   void init();
   string getConfigStringA(LPCWSTR sectionName, LPCWSTR keyName, LPCWSTR defaultValue = L"", LPCWSTR fileName=L"");
   wstring getConfigString(LPCWSTR sectionName, LPCWSTR keyName, LPCWSTR defaultValue = L"", LPCWSTR fileName=L"");
   int Tokenize(const wstring &text, vector<wstring> &results, LPCWSTR delim=L",");
   int Tokenize(const wstring &text, vector<int> &results, LPCWSTR delim = L",");
   wstring NarrowToWide(const string &str);
   string WideToNarrow(const wstring &wStr);

   void setThemeFilePath(const wstring theme=L"VT_Basic");
   wstring getStyleValue(LPCWSTR styleName);
   void getStyleColor(LPCWSTR styleName, int &color, bool foreColor);
   void getStyleBool(LPCWSTR styleName, int &var);

protected:
   TCHAR pluginConfigDir[MAX_PATH];
   TCHAR themeConfigFile[MAX_PATH];

   enum CF_TYPES {
      CONFIG_MAIN,
      CONFIG_PREFS,
      CONFIG_BASIC_THEME,
      CONFIG_FILE_COUNT
   };

   const wstring CONFIG_FILES[CONFIG_FILE_COUNT] { L"Visualizer.ini", L"VisualizerPrefs.ini",L"VT_Basic.ini"};
   wstring CONFIG_FILE_PATHS[CONFIG_FILE_COUNT] { };
   int defaultBackColor{};
   int defaultForeColor{};
};

#endif //_PREFERENCES_INI_H