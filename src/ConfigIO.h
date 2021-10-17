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

   string getConfigStringA(const wstring& sectionName, const wstring& keyName,
      const wstring& defaultValue = L"", wstring fileName = L"");
   wstring getConfigString(const wstring& sectionName, const wstring& keyName,
      const wstring& defaultValue = L"", wstring fileName = L"");
   int getConfigInt(const wstring& sectionName, const wstring& keyName,
      const int& defaultValue = 0, wstring fileName = L"");
   int getConfigSectionList(wstring& sections, wstring fileName);

   void setConfigStringA(const wstring& sectionName, const wstring& keyName,
      const string& keyValue, wstring fileName = L"");
   void setConfigString(const wstring& sectionName, const wstring& keyName,
      const wstring& keyValue, wstring fileName = L"");

   void deleteKey(const wstring& sectionName, const wstring& keyName, wstring fileName = L"");
   void deleteSection(const wstring& sectionName, wstring fileName = L"");
   void flushConfigFile();

   void openConfigFile(LPWSTR configData, const size_t readLength, wstring fileName = L"");
   void saveConfigFile(const wstring& fileData, bool bViz, wstring fileName = L"");

   int Tokenize(const wstring& text, vector<wstring>& results, LPCWSTR delim = L",");
   int Tokenize(const wstring& text, vector<int>& results, LPCWSTR delim = L",");

   void ActivateNewLineTabs(string& str);

   vector<wstring> getAvailableThemesList();
   wstring getStyleValue(const wstring& theme, const wstring& styleName, wstring fileName = L"");
   void getFullStyle(const wstring& theme, const wstring& styleName, StyleInfo& style, wstring fileName = L"");

   void backupConfigFile(bool bViz);
   BOOL queryConfigFileName(HWND hwnd, bool bOpen, bool backupFolder, bool bViz, wstring& backupConfigFile);
   void viewBackupFolder();
   int getBackupTempFileName(wstring& tempFileName);

   bool getAutoDetectFileType();
   void setAutoDetectFileType(bool detect);

   bool getCaretFramed();
   void setCaretFramed(bool framed);

   int getCaretFlashSeconds();
   void setCaretFlashSeconds(int seconds);

   bool getShowMBCharsOnPanel();
   wstring getPanelMBCharState();
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
   wstring CONFIG_FILE_PATHS[CONFIG_FILE_COUNT]{};
   wstring currentConfigFile{};
};
