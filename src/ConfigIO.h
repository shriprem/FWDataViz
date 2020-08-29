#pragma once

#ifndef _PREFERENCES_INI_H
#define _PREFERENCES_INI_H

#include "NPP/PluginInterface.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <Windows.h>


#define TO_STR(str) std::to_string(str)
#define TO_WSTR(str) std::to_wstring(str)
#define TO_LPCSTR(str) std::to_string(str).c_str()
#define TO_LPCWSTR(str) std::to_wstring(str).c_str()

extern NppData nppData;
const int CONFIG_STR_MAX_LEN = 100;

class ConfigIO {
public:
   void init();
   std::wstring getConfigStringW(LPCWSTR fileName, LPCWSTR sectionName, LPCWSTR keyName, LPCWSTR defaultValue = L"");
   int getPref(LPCWSTR fileName, LPCWSTR sectionName, LPCWSTR key, int default);
   void setPref(LPCWSTR fileName, LPCWSTR sectionName, LPCWSTR key, int val);
   void TokenizeW(LPCWSTR str, std::vector<std::wstring> &results, LPCWSTR delim = L",");
   void TokenizeW(LPCWSTR str, std::vector<int> &results, LPCWSTR delim = L",");

   LPCWSTR getMainIniFile();
   LPCWSTR getPreferencesIniFile();
   LPCWSTR getBasicThemeIniFile();

protected:
   TCHAR pluginConfigDir[MAX_PATH];

   enum CF_TYPES {
      CONFIG_MAIN,
      CONFIG_PREFS,
      CONFIG_BASIC_THEME,
      CONFIG_FILE_COUNT
   };

   const std::wstring CONFIG_FILES[CONFIG_FILE_COUNT] { L"Visualizer.ini", L"VisualizerPrefs.ini", L"VT_Basic.ini" };
   std::wstring CONFIG_FILE_PATHS[CONFIG_FILE_COUNT] { };
};

#endif //_PREFERENCES_INI_H