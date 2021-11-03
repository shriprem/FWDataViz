#pragma once

#include "Utils.h"
#include <locale>
#include <commdlg.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <ShlObj_core.h>
#include <time.h>
#include <unordered_map>
#include <vector>

#define PREF_ADFT             L"AutoDetectFileType"
#define PREF_CARET_FRAMED     L"FramedCaret"
#define PREF_CARET_FLASH      L"CaretFlashSeconds"
#define PREF_MBCHARS_SHOW     L"ShowMBCharsOnPanel"
#define PREF_MBCHARS_STATE    "PanelMBCharState"
#define PREF_COPY_TRIM        L"CopyFieldTrim"
#define PREF_PASTE_LPAD       "PasteFieldLPAD"
#define PREF_PASTE_RPAD       "PasteFieldRPAD"

struct StyleInfo {
   int backColor;
   int foreColor;
   int bold;
   int italics;
};

using std::vector;

class ConfigIO {
public:
   void init();

   int setCurrentConfigFile(const wstring& docFileType);
   void resetCurrentConfigFile();
   wstring getExtractTemplatesFile();

   string getConfigStringA(const string& section, const string& key, const string& default = "", string file = "");
   string getConfigStringA(const wstring& section, const wstring& key, const wstring& default = L"", wstring file = L"");
   wstring getConfigString(const wstring& section, const wstring& key, const wstring& default = L"", wstring file = L"");
   void setConfigString(const wstring& section, const wstring& key, const wstring& value, wstring file = L"");

   wstring getConfigWideChar(const string& section, const string& key, const string& default = "", string file = "");
   wstring getConfigWideChar(const wstring& section, const wstring& key, const wstring& default = L"", wstring file = L"");
   void setConfigMultiByte(const string& section, const string& key, const wstring& value, string file = "");
   void setConfigMultiByte(const wstring& section, const wstring& key, const wstring& value, wstring file = L"");

   int getConfigInt(const wstring& section, const wstring& key, const int& default = 0, wstring file = L"");
   int getConfigAllSections(wstring& sections, wstring file);
   int getConfigAllSectionsList(vector<wstring>& sectionsList, wstring file);

   void deleteKey(const wstring& section, const wstring& key, wstring file = L"");
   void deleteSection(const wstring& section, wstring file = L"");
   void flushConfigFile();

   void openConfigFile(LPSTR configData, const size_t readLength, wstring file = L"");
   void saveConfigFile(const wstring& fileData, bool bViz, wstring file = L"");

   int Tokenize(const wstring& text, vector<wstring>& results, LPCWSTR delim = L",");
   int Tokenize(const wstring& text, vector<int>& results, LPCWSTR delim = L",");

   void ActivateNewLineTabs(wstring& str);

   vector<wstring> getAvailableThemesList();
   wstring getStyleValue(const wstring& theme, const wstring& styleName, wstring file = L"");
   void getFullStyle(const wstring& theme, const wstring& styleName, StyleInfo& style, wstring file = L"");

   void backupConfigFile(bool bViz);
   BOOL queryConfigFileName(HWND hwnd, bool bOpen, bool backupFolder, bool bViz, wstring& backupConfigFile);
   void viewBackupFolder();
   int getBackupTempFileName(wstring& tempFileName);

   wstring getPreference(const string key, const string default = "");
   void setPreference(const string key, const wstring value);

   bool getPreferenceBool(const wstring key, const bool default = TRUE);
   void setPreferenceBool(const wstring key, const bool value);

   int getPreferenceInt(const wstring key, const int default = 0);
   void setPreferenceInt(const wstring key, const int value);

   void setPanelMBCharState(UINT state);
   bool getMultiByteLexing(wstring fileType);

protected:
   TCHAR pluginConfigDir[MAX_PATH];
   TCHAR pluginConfigBackupDir[MAX_PATH];
   TCHAR defaultConfigFile[MAX_PATH];

   enum CF_TYPES {
      CONFIG_VIZ,
      CONFIG_THEMES,
      CONFIG_PREFS,
      CONFIG_EXTRACTS,
      CONFIG_FILE_COUNT
   };

   const wstring CONFIG_FILES[CONFIG_FILE_COUNT]{ L"Visualizer.ini", L"Themes.ini", L"Preferences.ini", L"Extracts.ini"};
   wstring WCONFIG_FILE_PATHS[CONFIG_FILE_COUNT]{};
   string CONFIG_FILE_PATHS[CONFIG_FILE_COUNT]{};

   wstring wCurrentConfigFile{};
   string currentConfigFile{};
};
