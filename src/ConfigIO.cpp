#include "ConfigIO.h"

void ConfigIO::init() {
   TCHAR sPluginDirectory[MAX_PATH]{};

   nppMessage(NPPM_GETPLUGINHOMEPATH, MAX_PATH, (LPARAM)sPluginDirectory);
   PathAppend(sPluginDirectory, PLUGIN_FOLDER_NAME);

   nppMessage(NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)pluginConfigDir);

   // If no existing config path, create it
   if (!PathFileExists(pluginConfigDir)) CreateDirectory(pluginConfigDir, NULL);

   // If no existing config folder for this plugin, create it
   PathAppend(pluginConfigDir, PLUGIN_FOLDER_NAME);
   if (!PathFileExists(pluginConfigDir)) CreateDirectory(pluginConfigDir, NULL);

   // If no existing config backup folder for this plugin, create it
   PathCombine(pluginConfigBackupDir, pluginConfigDir, L"Backup\\");
   if (!PathFileExists(pluginConfigBackupDir)) CreateDirectory(pluginConfigBackupDir, NULL);

   // Reconcile config files
   const wstring sDefaultPrefix{ L"default_" };

   TCHAR sDefaultsFile[MAX_PATH];
   TCHAR sConfigFile[MAX_PATH];
   TCHAR sThemeDatFile[MAX_PATH];

   // Rename any existing Themes.dat file to Themes.ini
   PathCombine(sThemeDatFile, pluginConfigDir, L"Themes.dat");
   PathCombine(sConfigFile, pluginConfigDir, L"Themes.ini");
   if (PathFileExists(sThemeDatFile) && !PathFileExists(sConfigFile))
      MoveFile(sThemeDatFile, sConfigFile);

   // If config files are missing, copy them from the plugins folder
   for (int i{}; i < CONFIG_FILE_COUNT; i++) {
      PathCombine(sConfigFile, pluginConfigDir, CONFIG_FILES[i].c_str());
      CONFIG_FILE_PATHS[i] = wstring{ sConfigFile };

      if (!PathFileExists(sConfigFile)) {
         PathCombine(sDefaultsFile, sPluginDirectory, (sDefaultPrefix + CONFIG_FILES[i]).c_str());
         CopyFile(sDefaultsFile, sConfigFile, TRUE);
      }
   }

   // initialize instance variables
   resetCurrentConfigFile();
   PathCombine(defaultConfigFile, sPluginDirectory, (sDefaultPrefix + CONFIG_FILES[CONFIG_VIZ]).c_str());
}

int ConfigIO::setCurrentConfigFile(const wstring& docFileType) {
   int sectionCount{};
   wstring sectionList{};

   if (docFileType.length() < 1) {
      resetCurrentConfigFile();
      return 0;
   }

   sectionCount = getConfigSectionList(sectionList, CONFIG_FILE_PATHS[CONFIG_VIZ]);
   if (sectionCount > 0 && sectionList.find(docFileType) != std::string::npos) {
      resetCurrentConfigFile();
      return 1;
   }

   sectionCount = getConfigSectionList(sectionList, defaultConfigFile);
   if (sectionCount > 0 && sectionList.find(docFileType) != std::string::npos) {
      currentConfigFile = wstring{ defaultConfigFile };
      return 2;
   }

   resetCurrentConfigFile();
   return -1;
}

void ConfigIO::resetCurrentConfigFile() {
   currentConfigFile = CONFIG_FILE_PATHS[CONFIG_VIZ];
}

wstring ConfigIO::getExtractTemplatesFile() {
   return CONFIG_FILE_PATHS[CONFIG_EXTRACTS];
}

string ConfigIO::getConfigStringA(const wstring& sectionName, const wstring& keyName,
   const wstring& defaultValue, wstring fileName) {
   return Utils::WideToNarrow(getConfigString(sectionName, keyName, defaultValue, fileName));
}

wstring ConfigIO::getConfigString(const wstring& sectionName, const wstring& keyName,
   const wstring& defaultValue, wstring fileName) {
   const int bufSize{ 32000 };
   wchar_t ftBuf[bufSize];

   if (fileName.length() < 1) fileName = currentConfigFile;

   GetPrivateProfileString(sectionName.c_str(), keyName.c_str(), defaultValue.c_str(),
      ftBuf, bufSize, fileName.c_str());

   return wstring{ ftBuf };
}

int ConfigIO::getConfigInt(const wstring& sectionName, const wstring& keyName,
   const int& defaultValue, wstring fileName) {
   return Utils::StringtoInt(getConfigString(sectionName, keyName, to_wstring(defaultValue), fileName));
}

int ConfigIO::getConfigSectionList(wstring& sections, wstring fileName) {
   const int bufSize{ 32000 };
   wchar_t ftBuf[bufSize];

   DWORD charCount{};
   charCount = GetPrivateProfileString(NULL, L"", L"", ftBuf, bufSize, fileName.c_str());
   if (charCount < 1) return 0;

   int sectionCount{};
   for (unsigned int i{}; i < charCount; i++) {
      if (ftBuf[i] == 0) {
         ftBuf[i] = ',';
         sectionCount++;
      }
   }

   sections = ftBuf;
   return sectionCount;
}

void ConfigIO::setConfigStringA(const wstring& sectionName, const wstring& keyName,
   const string& keyValue, wstring fileName) {
   setConfigString(sectionName, keyName, Utils::NarrowToWide(keyValue), fileName);
}

void ConfigIO::setConfigString(const wstring& sectionName, const wstring& keyName,
   const wstring& keyValue, wstring fileName) {
   if (fileName.length() < 1) fileName = currentConfigFile;

   WritePrivateProfileString(sectionName.c_str(), keyName.c_str(), keyValue.c_str(), fileName.c_str());
}

void ConfigIO::deleteKey(const wstring& sectionName, const wstring& keyName, wstring fileName) {
   if (fileName.length() < 1) fileName = currentConfigFile;

   WritePrivateProfileString(sectionName.c_str(), keyName.c_str(), NULL, fileName.c_str());
}

void ConfigIO::deleteSection(const wstring& sectionName, wstring fileName) {
   if (fileName.length() < 1) fileName = currentConfigFile;

   WritePrivateProfileString(sectionName.c_str(), NULL, NULL, fileName.c_str());
}

void ConfigIO::flushConfigFile() {
   WritePrivateProfileString(NULL, NULL, NULL, NULL);
}

void ConfigIO::openConfigFile(LPWSTR configData, const size_t readLength, wstring fileName) {
   if (fileName.length() < 1) fileName = currentConfigFile;

   using std::ios;
   std::wifstream fs;

   ZeroMemory(configData, readLength);
   fs.open(fileName, ios::binary);

   unsigned short bom[2]{};

   bom[0] = fs.get();
   bom[1] = fs.get();

   if (bom[0] == 0xFF && bom[1] == 0xFE) {
      std::locale ucs16(std::locale(), new std::codecvt_utf16<wchar_t, 1114111UL, (std::codecvt_mode)5>);
      fs.imbue(ucs16);
   }

   fs.seekg(0);
   fs.read(configData, readLength);
   fs.close();
}

void ConfigIO::saveConfigFile(const wstring& fileData, bool bViz, wstring fileName) {
   if (fileName.length() < 1)
      fileName = CONFIG_FILE_PATHS[bViz ? CONFIG_VIZ : CONFIG_THEMES];

   using std::ios;
   std::wofstream fs;

   fs.open(fileName, ios::out | ios::binary | ios::trunc);
   fs.write(fileData.c_str(), fileData.length());

   bool writeFailed{ fs.bad() };
   fs.clear();
   fs.close();

   if (writeFailed) {
      std::locale ucs16(std::locale(), new std::codecvt_utf16<wchar_t, 1114111UL, (std::codecvt_mode)3>);
      fs.open(fileName, ios::out | ios::binary | ios::trunc);
      fs.imbue(ucs16);
      fs.write(fileData.c_str(), fileData.length());
      fs.close();
   }

   flushConfigFile();
}

int ConfigIO::Tokenize(const wstring& text, vector<wstring>& results, LPCWSTR delim) {
   std::size_t nStart{}, nEnd;

   results.clear();
   if (text.length() < 1) return 0;

   while ((nEnd = text.find(delim, nStart)) != wstring::npos) {
      if (nEnd > nStart)
         results.emplace_back(text.substr(nStart, nEnd - nStart));

      nStart = nEnd + 1;
   }

   if (nStart < text.length())
      results.emplace_back(text.substr(nStart));

   return static_cast<int>(results.size());
}

int ConfigIO::Tokenize(const wstring& text, vector<int>& results, LPCWSTR delim) {
   vector<wstring> interims;

   results.clear();
   if (text.length() < 1) return 0;

   Tokenize(text, interims, delim);

   for (wstring istr : interims)
      results.emplace_back(Utils::StringtoInt(istr));

   return static_cast<int>(results.size());
}

void ConfigIO::ActivateNewLineTabs(string& str) {
   str = std::regex_replace(str, std::regex("\\\\n"), "\n");
   str = std::regex_replace(str, std::regex("\\\\t"), "\t");
}

vector<wstring> ConfigIO::getAvailableThemesList() {
   vector<wstring> themesVector;
   wstring themesList;

   themesList = getConfigString(L"Base", L"Themes", L"", CONFIG_FILE_PATHS[CONFIG_THEMES]);
   Tokenize(themesList, themesVector);

   return themesVector;
}

wstring ConfigIO::getStyleValue(const wstring& theme, const wstring& styleName, wstring fileName) {
   if (fileName.length() < 1) fileName = CONFIG_FILE_PATHS[CONFIG_THEMES];
   return getConfigString(theme, styleName, L"", fileName);
}

void ConfigIO::getFullStyle(const wstring& theme, const wstring& styleName, StyleInfo& style, wstring fileName) {
   if (fileName.length() < 1) fileName = CONFIG_FILE_PATHS[CONFIG_THEMES];

   wstring val = getStyleValue(theme, styleName, fileName);

   if (val.length() < 16) {
      style.backColor = static_cast<int>(nppMessage(NPPM_GETEDITORDEFAULTBACKGROUNDCOLOR, NULL, NULL));
      style.foreColor = static_cast<int>(nppMessage(NPPM_GETEDITORDEFAULTFOREGROUNDCOLOR, NULL, NULL));
      style.bold = 1;
      style.italics = 1;
      return;
   }

   style.backColor = Utils::StringtoInt(val.substr(0, 6), 16);
   style.foreColor = Utils::StringtoInt(val.substr(7, 6), 16);
   style.bold = Utils::StringtoInt(val.substr(14, 1));
   style.italics = Utils::StringtoInt(val.substr(15, 1));
}

void ConfigIO::backupConfigFile(bool bViz) {
   string backupTemplate{};
   wstring srcFile{};
   char backupFile[50];
   wchar_t backupFilePath[MAX_PATH];

   if (bViz) {
      if (currentConfigFile == CONFIG_FILE_PATHS[CONFIG_VIZ]) {
         backupTemplate = "Visualizer_%Y%m%d_%H%M%S.ini";
         srcFile = CONFIG_FILE_PATHS[CONFIG_VIZ];
      }
      else {
         backupTemplate = "default_Visualizer_%Y%m%d_%H%M%S.ini";
         srcFile = defaultConfigFile;
      }
   }
   else {
      backupTemplate = "Themes_%Y%m%d_%H%M%S.ini";
      srcFile = CONFIG_FILE_PATHS[CONFIG_THEMES];
   }

   time_t rawTime;
   struct tm* timeInfo;

   time(&rawTime);
   timeInfo = localtime(&rawTime);
   strftime(backupFile, 50, backupTemplate.c_str(), timeInfo);

   PathCombine(backupFilePath, pluginConfigBackupDir, Utils::NarrowToWide(backupFile).c_str());

   if (srcFile == defaultConfigFile)
      CopyFile(srcFile.c_str(), backupFilePath, FALSE);
   else
      CopyFile(srcFile.c_str(), backupFilePath, FALSE);
}

BOOL ConfigIO::queryConfigFileName(HWND hwnd, bool bOpen, bool backupFolder, bool bViz, wstring& backupConfigFile) {
   OPENFILENAME ofn;

   TCHAR filePath[MAX_PATH]{};

   ZeroMemory(&ofn, sizeof(ofn));
   ofn.lStructSize = sizeof(ofn);
   ofn.hwndOwner = hwnd;
   ofn.lpstrFile = filePath;
   ofn.lpstrFile[0] = '\0';
   ofn.nMaxFile = sizeof(filePath);
   ofn.lpstrDefExt = L"ini";
   ofn.nFilterIndex = 2;
   ofn.lpstrFileTitle = NULL;
   ofn.nMaxFileTitle = 0;
   ofn.lpstrTitle = bOpen ? FWVIZ_OPEN_BKUP_CONFIG_DLG : FWVIZ_SAVE_BKUP_CONFIG_DLG;
   ofn.Flags = bOpen ? (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST) : OFN_OVERWRITEPROMPT;

   if (backupFolder)
      ofn.lpstrInitialDir = pluginConfigBackupDir;
   else {
      wstring desktopPath = Utils::getKnownFolderPath(FOLDERID_Desktop);
      ofn.lpstrInitialDir = desktopPath.c_str();
   }

   if (bViz)
      ofn.lpstrFilter = L"All\0*.*\0Ini Files\0*.ini\0";
   else
      ofn.lpstrFilter = L"All\0*.*\0Ini Files\0*.ini\0DAT Files\0*.dat\0";

   BOOL bOK = bOpen ? GetOpenFileName(&ofn) : GetSaveFileName(&ofn);

   if (bOK) backupConfigFile = ofn.lpstrFile;

   return bOK;
}

void ConfigIO::viewBackupFolder() {
   ShellExecute(NULL, L"open", pluginConfigBackupDir, NULL, NULL, SW_SHOWNORMAL);
}

int ConfigIO::getBackupTempFileName(wstring& tempFileName) {
   TCHAR tmpFilePath[MAX_PATH];

   if (GetTempFileName(pluginConfigBackupDir, L"FWViz_", 0, tmpFilePath) == 0) {
      tempFileName = L"";
      return -1;
   }

   tempFileName = tmpFilePath;
   return 0;
}

wstring ConfigIO::getPreference(const wstring key, const wstring default) {
   return getConfigString(L"Preferences", key, default, CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

void ConfigIO::setPreference(const wstring key, const wstring value) {
   setConfigString(L"Preferences", key, value, CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

bool ConfigIO::getPreferenceBool(const wstring key, const bool default) {
   return getConfigString(L"Preferences", key, default ? L"Y" : L"N", CONFIG_FILE_PATHS[CONFIG_PREFS]) == L"Y";
}

void ConfigIO::setPreferenceBool(const wstring key, const bool value) {
   setConfigString(L"Preferences", key, value ? L"Y" : L"N", CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

int ConfigIO::getPreferenceInt(const wstring key, const int default) {
   return getConfigInt(L"Preferences", key, default, CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

void ConfigIO::setPreferenceInt(const wstring key, const int value) {
   setConfigString(L"Preferences", key, to_wstring(value), CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

void ConfigIO::setPanelMBCharState(UINT state) {
   setConfigString(L"Preferences", L"PanelMBCharState",
      (state == BST_INDETERMINATE) ? L"FT" : ((state == BST_CHECKED) ? L"Y" : L"N"),
      CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

bool ConfigIO::getMultiByteLexing(wstring fileType) {
   wstring state{ getConfigString(L"Preferences", L"PanelMBCharState", L"FT", CONFIG_FILE_PATHS[CONFIG_PREFS]) };

   if (state == L"FT")
      return (getConfigString(fileType, L"MultiByteChars", L"N") == L"Y");
   else
      return (state == L"Y");
}

