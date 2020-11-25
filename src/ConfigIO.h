#pragma once

#include "PluginDefinition.h"
#include <codecvt>
#include <locale>
#include <commdlg.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <ShlObj_core.h>
#include <time.h>
#include <unordered_map>
#include <vector>

using std::vector;

class ConfigIO {
public:
   void init();

   wstring getPluginConfigDir();
   int setCurrentConfigFile(const wstring& docFileType);
   void resetCurrentConfigFile();

   string getConfigStringA(const wstring& sectionName, const wstring& keyName,
      const wstring& defaultValue = L"", wstring fileName = L"");
   wstring getConfigString(const wstring& sectionName, const wstring& keyName,
      const wstring& defaultValue = L"", wstring fileName = L"");
   int getConfigSectionList(wstring& sections, wstring fileName);

   void setConfigStringA(const wstring& sectionName, const wstring& keyName,
      const string& keyValue, wstring fileName=L"");
   void setConfigString(const wstring& sectionName, const wstring& keyName,
      const wstring& keyValue, wstring fileName=L"");

   void flushConfigFile();
   void openConfigFile(LPWSTR configData, const size_t readLength, wstring fileName = L"");
   void saveConfigFile(const wstring &fileData, wstring fileName = L"");

   int Tokenize(const wstring &text, vector<wstring> &results, LPCWSTR delim=L",");
   int Tokenize(const wstring &text, vector<int> &results, LPCWSTR delim=L",");

   LPCWSTR ToUpper(LPWSTR str);
   wstring NarrowToWide(const string &str);
   string WideToNarrow(const wstring &wStr);

   void setThemeFilePath(const wstring theme=L"VT_Spectrum");
   wstring getStyleValue(const wstring& styleName);
   void getStyleColor(const wstring& styleName, int &color, bool foreColor);
   void getStyleBool(const wstring& styleName, int &var);

   void backupMoveConfigFile();
   BOOL queryConfigFileName(HWND hwnd, bool bOpen, bool bBackupFolder, wstring &backupConfigFile);
   void viewBackupFolder();
   int getBackupTempFileName(wstring &tempFileName);
   vector<wstring> getFileList(const wstring& path, const wstring& fileMask);

   bool getCaretFramed();
   void setCaretFramed(bool framed);

protected:
   TCHAR pluginConfigDir[MAX_PATH];
   TCHAR pluginConfigBackupDir[MAX_PATH];
   TCHAR themeConfigFile[MAX_PATH];
   TCHAR defaultConfigFile[MAX_PATH];

   enum CF_TYPES {
      CONFIG_MAIN,
      CONFIG_PREFS,
      CONFIG_THEME_SPECTRUM,
      CONFIG_THEME_RAINBOW,
      CONFIG_FILE_COUNT
   };

   const wstring CONFIG_FILES[CONFIG_FILE_COUNT] { L"Visualizer.ini", L"VisualizerPrefs.ini",
      L"VT_Spectrum.ini", L"VT_Rainbow.ini"};
   wstring CONFIG_FILE_PATHS[CONFIG_FILE_COUNT] { };
   wstring currentConfigFile{};

   int defaultBackColor{};
   int defaultForeColor{};
};
