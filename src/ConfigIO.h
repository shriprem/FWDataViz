#pragma once

#include "Utils.h"
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

#define PREF_ADFT             "AutoDetectFileType"
#define PREF_CARET_FLASH      "CaretFlashSeconds"
#define PREF_MBCHARS_SHOW     "ShowMBCharsOnPanel"
#define PREF_MBCHARS_STATE    "PanelMBCharState"
#define PREF_JUMP_FIELD_SEQ   "ShowJumpFieldSeqNo"
#define PREF_DEF_BACKGROUND   "DefaultBackground"
#define PREF_SHOW_CALLTIP     "ShowCalltip"
#define PREF_COPY_TRIM        "CopyFieldTrim"
#define PREF_HOP_RT_LEFT_EDGE "HopRightLeftEdge"
#define PREF_PASTE_LPAD       "PasteFieldLPAD"
#define PREF_PASTE_RPAD       "PasteFieldRPAD"
#define PREF_CLEARVIZ_AUTO    "ClearVizWithAutoDetect"
#define PREF_CLEARVIZ_PANEL   "ClearVizOnPanelClose"
#define PREF_FOLDLINE_COLOR   "FoldLineColor"
#define PREF_FOLDLINE_ALPHA   "FoldLineAlpha"

struct StyleInfo {
   int backColor{};
   int foreColor{};
   int bold{};
   int italics{};
};

struct FoldingInfo {
   int recTypeIndex{};
   int priority{ 0 };
   bool recursive{ FALSE };
   wstring endRecords{};
};

using std::vector;

class ConfigIO {
public:
   enum CF_TYPES {
      CONFIG_VIZ,
      CONFIG_THEMES,
      CONFIG_PREFS,
      CONFIG_EXTRACTS,
      CONFIG_FIELD_TYPES,
      CONFIG_FOLDSTRUCTS,
   };

   void init();
   int setVizConfig(const string& docFileType);
   void userVizConfig();
   void defaultVizConfig();
   bool isCurrentVizConfigDefault() const { return (wCurrentConfigFile == defaultConfigFile); }
   wstring getConfigFile(CF_TYPES cfType);
   wstring getActiveConfigFile(CF_TYPES cfType);

   string getConfigStringA(const string& section, const string& key, const string& defaultVal = "", string file = "") const;
   string getConfigStringA(const wstring& section, const string& key, const string& defaultVal = "", wstring file = L"") const;
   wstring getConfigWideChar(const string& section, const string& key, const string& defaultVal = "", string file = "") const;
   wstring getConfigWideChar(const wstring& section, const string& key, const string& defaultVal = "", wstring file = L"") const;

   void setConfigStringA(const string& section, const string& key, const string& value, string file = "") const;
   void setConfigMultiByte(const string& section, const string& key, const wstring& value, string file = "") const;

   int getConfigInt(const string& section, const string& key, const int& defaultVal = 0, string file = "") const;
   int getConfigInt(const wstring& section, const string& key, const int& defaultVal = 0, wstring file = L"") const;

   wstring getStyleValue(const wstring& theme, const string& styleName, wstring file = L"") const;
   void getFullStyle(const wstring& theme, const string& styleName, StyleInfo& style, wstring file = L"") const;
   string getFieldStyleText(const wstring& fieldName) const;
   void parseFieldStyle(const string& styleText, StyleInfo& style);

   int getFoldStructCount(wstring file = L"") const;
   string getFoldStructValueA(string foldStructType, string key, wstring file = L"") const;
   string getFoldStructValue(wstring foldStructType, string key, wstring file = L"") const;
   void getFoldStructFoldingInfo(wstring foldStructType, vector<FoldingInfo>& vFoldInfo, wstring file = L"");

   wstring getPreference(const string key, const string defaultVal = "") const;
   void setPreference(const string key, const wstring value) const;

   bool getPreferenceBool(const string key, const bool defaultVal = TRUE) const;
   void setPreferenceBool(const string key, const bool value) const;

   int getPreferenceInt(const string key, const int defaultVal = 0) const;
   void setPreferenceInt(const string key, const int value) const;

   void setPanelMBCharState(UINT state) const;
   bool getMultiByteLexing(string fileType) const;

   int getConfigAllSections(string& sections, const string file);
   int getConfigAllSectionsList(vector<string>& sectionsList, const string file);
   int getConfigAllSectionsList(vector<wstring>& sectionsList, const wstring file);

   int getConfigAllKeys(const string& section, string& keys, const string file);
   int getConfigAllKeysList(const string& section, vector<string>& keysList, const string file);
   int getConfigAllKeysList(const string& section, vector<wstring>& keysList, const string file);

   int getConfigValueList(vector<string>& valList, const string& section, const string& key,
      const string& defaultVal = "", string file = "");
   int getThemesList(vector<wstring>& valList, wstring file = L"");

   int Tokenize(const string& text, vector<string>& results, const string& delim = ",");
   int Tokenize(const wstring& text, vector<wstring>& results, const wstring& delim = L",");
   int Tokenize(const string& text, vector<int>& results, const string& delim = ",");

   void ActivateNewLineTabs(wstring& str);
   void deleteKey(const string& section, const string& key, string file = "") const;
   void deleteKey(const wstring& section, const wstring& key, wstring file = L"") const;
   void deleteSection(const string& section, string file = "");

   string readConfigFile(wstring file = L"") const;
   bool queryConfigFileName(HWND hwnd, bool bOpen, bool backupFolder, wstring& backupConfigFile) const;
   void saveConfigFile(const wstring& fileData, wstring file);

   int getBackupTempFileName(wstring& tempFileName) const;
   void backupConfigFile(wstring file) const;
   void viewBackupFolder() const;
   void flushConfigFile();

   bool checkConfigFilesforUTF8();
   bool fixIfNotUTF8File(CF_TYPES cfType);
   bool fixIfNotUTF8File(wstring file);

protected:
   TCHAR pluginConfigDir[MAX_PATH]{};
   TCHAR pluginConfigBackupDir[MAX_PATH]{};
   TCHAR defaultConfigFile[MAX_PATH]{};
   TCHAR defaultThemeFile[MAX_PATH]{};
   TCHAR defaultFoldStructFile[MAX_PATH]{};

   static constexpr int CONFIG_FILE_COUNT{ CONFIG_FOLDSTRUCTS + 1 };
   const wstring CONFIG_FILES[CONFIG_FILE_COUNT]{
      L"Visualizer.ini", L"Themes.ini", L"Preferences.ini", L"Extracts.ini", L"FieldTypes.ini", L"FoldStructs.ini"};
   wstring WCONFIG_FILE_PATHS[CONFIG_FILE_COUNT];
   string CONFIG_FILE_PATHS[CONFIG_FILE_COUNT];

   wstring wCurrentConfigFile{}, wCurrentThemeFile{}, wCurrentFoldStructFile{};
   string currentConfigFile{};

   enum ENC_TYPE {
      UTF8,
      UTF8_BOM,
      UCS16_LE,
      UCS16_BE
   };

   ENC_TYPE getBOM(wstring file);
};

