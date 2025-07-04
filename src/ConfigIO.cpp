#include "ConfigIO.h"

void ConfigIO::init() {
   TCHAR sPluginDirectory[MAX_PATH]{};

   NppMessage(NPPM_GETPLUGINHOMEPATH, MAX_PATH, (LPARAM)sPluginDirectory);
   PathAppend(sPluginDirectory, PLUGIN_FOLDER_NAME);

   NppMessage(NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)pluginConfigDir);

   // If no existing config path, create it
   if (!Utils::checkDirectoryExists(pluginConfigDir)) CreateDirectory(pluginConfigDir, NULL);

   // If no existing config folder for this plugin, create it
   PathAppend(pluginConfigDir, PLUGIN_FOLDER_NAME);
   if (!Utils::checkDirectoryExists(pluginConfigDir)) CreateDirectory(pluginConfigDir, NULL);

   // If no existing config backup folder for this plugin, create it
   PathCombine(pluginConfigBackupDir, pluginConfigDir, L"Backup\\");
   if (!Utils::checkDirectoryExists(pluginConfigBackupDir)) CreateDirectory(pluginConfigBackupDir, NULL);

   // Reconcile config files
   const wstring sDefaultPrefix{ L"default_" };

   TCHAR sDefaultsFile[MAX_PATH];
   TCHAR sConfigFile[MAX_PATH];
   TCHAR sThemeDatFile[MAX_PATH];

   // Rename any existing Themes.dat file to Themes.ini
   PathCombine(sThemeDatFile, pluginConfigDir, L"Themes.dat");
   PathCombine(sConfigFile, pluginConfigDir, L"Themes.ini");
   if (Utils::checkFileExists(sThemeDatFile) && !Utils::checkFileExists(sConfigFile))
      MoveFile(sThemeDatFile, sConfigFile);

   // If config files are missing, copy them from the plugins folder
   for (int i{}; i < CONFIG_FILE_COUNT; ++i) {
      PathCombine(sConfigFile, pluginConfigDir, CONFIG_FILES[i].c_str());
      WCONFIG_FILE_PATHS[i] = wstring{ sConfigFile };
      CONFIG_FILE_PATHS[i] = Utils::WideToNarrow(sConfigFile);

      if (!Utils::checkFileExists(sConfigFile)) {
         PathCombine(sDefaultsFile, sPluginDirectory, (sDefaultPrefix + CONFIG_FILES[i]).c_str());
         CopyFile(sDefaultsFile, sConfigFile, TRUE);
      }
   }

   // initialize instance variables
   userVizConfig();
   PathCombine(defaultConfigFile, sPluginDirectory, (sDefaultPrefix + CONFIG_FILES[CONFIG_VIZ]).c_str());
   PathCombine(defaultThemeFile, sPluginDirectory, (sDefaultPrefix + CONFIG_FILES[CONFIG_THEMES]).c_str());
   PathCombine(defaultFoldStructFile, sPluginDirectory, (sDefaultPrefix + CONFIG_FILES[CONFIG_FOLDSTRUCTS]).c_str());
}

int ConfigIO::setVizConfig(const string& docFileType) {
   int sectionCount{};
   string sectionList{};

   if (docFileType.empty()) {
      userVizConfig();
      return 0;
   }

   sectionCount = getConfigAllSections(sectionList, CONFIG_FILE_PATHS[CONFIG_VIZ]);
   if (sectionCount > 0 && sectionList.find(docFileType) != string::npos) {
      userVizConfig();
      return 1;
   }

   sectionCount = getConfigAllSections(sectionList, Utils::WideToNarrow(defaultConfigFile));
   if (sectionCount > 0 && sectionList.find(docFileType) != string::npos) {
      defaultVizConfig();
      return 2;
   }

   userVizConfig();
   return -1;
}

void ConfigIO::userVizConfig() {
   wCurrentConfigFile = WCONFIG_FILE_PATHS[CONFIG_VIZ];
   currentConfigFile = CONFIG_FILE_PATHS[CONFIG_VIZ];

   wCurrentThemeFile = WCONFIG_FILE_PATHS[CONFIG_THEMES];
   wCurrentFoldStructFile = WCONFIG_FILE_PATHS[CONFIG_FOLDSTRUCTS];
}

void ConfigIO::defaultVizConfig() {
   wCurrentConfigFile = wstring{ defaultConfigFile };
   currentConfigFile = Utils::WideToNarrow(defaultConfigFile);

   wCurrentThemeFile = wstring{ defaultThemeFile };
   wCurrentFoldStructFile = wstring{ defaultFoldStructFile };
}

wstring ConfigIO::getConfigFile(CF_TYPES cfType) {
   if (cfType < 0 || cfType >= CONFIG_FILE_COUNT) return L"";

   return WCONFIG_FILE_PATHS[cfType];
}

wstring ConfigIO::getActiveConfigFile(CF_TYPES cfType) {
   switch (cfType) {
   case CONFIG_VIZ:
      return wCurrentConfigFile;
   case CONFIG_THEMES:
      return wCurrentThemeFile;
   case CONFIG_FOLDSTRUCTS:
      return wCurrentFoldStructFile;
   default:
      return WCONFIG_FILE_PATHS[cfType];
   }
}

string ConfigIO::getConfigStringA(const string& section, const string& key, const string& defaultVal, string file) const {
   const int bufSize{ FW_LINE_MAX_LENGTH };
   string ftBuf(bufSize, '\0');

   if (file.empty()) file = currentConfigFile;

   GetPrivateProfileStringA(section.c_str(), key.c_str(), defaultVal.c_str(), ftBuf.data(), bufSize, file.c_str());

   return string{ ftBuf.c_str()};
}

string ConfigIO::getConfigStringA(const wstring& section, const string& key, const string& defaultVal, wstring file) const {
   return getConfigStringA(Utils::WideToNarrow(section), key, defaultVal, Utils::WideToNarrow(file));
}

wstring ConfigIO::getConfigWideChar(const string& section, const string& key, const string& defaultVal, string file) const {
   return Utils::NarrowToWide(getConfigStringA(section, key, defaultVal, file));
}

wstring ConfigIO::getConfigWideChar(const wstring& section, const string& key, const string& defaultVal, wstring file) const {
   return Utils::NarrowToWide(getConfigStringA(section, key, defaultVal, file));
}

void ConfigIO::setConfigStringA(const string& section, const string& key, const string& value, string file) const {
   if (file.empty()) file = currentConfigFile;

   WritePrivateProfileStringA(section.c_str(), key.c_str(), value.c_str(), file.c_str());
}

void ConfigIO::setConfigMultiByte(const string& section, const string& key, const wstring& value, string file) const {
   setConfigStringA(section, key, Utils::WideToNarrow(value), file);
}

int ConfigIO::getConfigInt(const string& section, const string& key, const int& defaultVal, string file) const {
   string defVal{ to_string(defaultVal) };
   return Utils::StringtoInt(getConfigStringA(section, key, defVal, file));
}

int ConfigIO::getConfigInt(const wstring& section, const string& key, const int& defaultVal, wstring file) const {
   string defVal{ to_string(defaultVal) };
   return Utils::StringtoInt(getConfigStringA(section, key, defVal, file));
}

wstring ConfigIO::getStyleValue(const wstring& theme, const string& styleName, wstring file) const {
   return getConfigWideChar(theme, styleName, "", (file.empty()) ? wCurrentThemeFile : file);
}

void ConfigIO::getFullStyle(const wstring& theme, const string& styleName, StyleInfo& style, wstring file) const {
   wstring val = getStyleValue(theme, styleName, file);

   if (val.length() < 16) {
      style.backColor = static_cast<int>(NppMessage(NPPM_GETEDITORDEFAULTBACKGROUNDCOLOR));
      style.foreColor = static_cast<int>(NppMessage(NPPM_GETEDITORDEFAULTFOREGROUNDCOLOR));
      style.bold = 1;
      style.italics = 1;
      return;
   }

   style.backColor = Utils::StringtoInt(val.substr(0, 6), 16);
   style.foreColor = Utils::StringtoInt(val.substr(7, 6), 16);
   style.bold = Utils::StringtoInt(val.substr(14, 1));
   style.italics = Utils::StringtoInt(val.substr(15, 1));
}

string ConfigIO::getFieldStyleText(const wstring& fieldName) const {
   return getConfigStringA("Styles", Utils::WideToNarrow(fieldName), "", CONFIG_FILE_PATHS[CONFIG_FIELD_TYPES]);
}

void ConfigIO::parseFieldStyle(const string& styleText, StyleInfo& style) {
   if (styleText.length() != 16) return;

   style.backColor = Utils::StringtoInt(styleText.substr(0, 6), 16);
   style.foreColor = Utils::StringtoInt(styleText.substr(7, 6), 16);
   style.bold = Utils::StringtoInt(styleText.substr(14, 1));
   style.italics = Utils::StringtoInt(styleText.substr(15, 1));
}

int ConfigIO::getFoldStructCount(wstring file) const {
   if (file.empty()) file = wCurrentFoldStructFile;

   return Utils::StringtoInt(getConfigStringA(L"Base", "FoldStructCount", "0", file));
}

string ConfigIO::getFoldStructValueA(string foldStructType, string key, wstring file) const {
   if (file.empty()) file = wCurrentFoldStructFile;

   return getConfigStringA(foldStructType, key, "", Utils::WideToNarrow(file));
}

string ConfigIO::getFoldStructValue(wstring foldStructType, string key, wstring file) const {
   if (file.empty()) file = wCurrentFoldStructFile;

   return getConfigStringA(foldStructType, key, "", file);
}

void ConfigIO::getFoldStructFoldingInfo(wstring foldStructType, vector<FoldingInfo>& vFoldInfo, wstring file) {
   if (file.empty()) file = wCurrentFoldStructFile;

   string headerRecs{ getFoldStructValue(foldStructType, "HeaderRecords", file) };
   vector<string> headerRecList{};
   int headerCount{ Tokenize(headerRecs, headerRecList) };

   vFoldInfo.clear();
   vFoldInfo.resize(headerCount);

   for (int i{}; i < headerCount; ++i) {
      int recTypeIndex{ Utils::StringtoInt(headerRecList[i].substr(3))};

      vFoldInfo[i].recTypeIndex = recTypeIndex;
      vFoldInfo[i].priority = getConfigInt(foldStructType, headerRecList[i] + "_Priority", 0, file);
      vFoldInfo[i].recursive = (getConfigStringA(foldStructType, headerRecList[i] + "_Recursive", "N", file) == "Y");
      vFoldInfo[i].endRecords = getConfigWideChar(foldStructType, headerRecList[i] + "_EndRecords", "", file);
   }
}

wstring ConfigIO::getPreference(const string key, const string defaultVal) const {
   return getConfigWideChar("Preferences", key, defaultVal, CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

void ConfigIO::setPreference(const string key, const wstring value) const {
   setConfigStringA("Preferences", key.c_str(), Utils::WideToNarrow(value.c_str()), CONFIG_FILE_PATHS[CONFIG_PREFS].c_str());
}

bool ConfigIO::getPreferenceBool(const string key, const bool defaultVal) const {
   return getConfigStringA("Preferences", key, defaultVal ? "Y" : "N", CONFIG_FILE_PATHS[CONFIG_PREFS]) == "Y";
}

void ConfigIO::setPreferenceBool(const string key, const bool value) const {
   setConfigStringA("Preferences", key, value ? "Y" : "N", CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

int ConfigIO::getPreferenceInt(const string key, const int defaultVal) const {
   return getConfigInt("Preferences", key, defaultVal, CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

void ConfigIO::setPreferenceInt(const string key, const int value) const {
   setConfigStringA("Preferences", key, to_string(value), CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

void ConfigIO::setPanelMBCharState(UINT state) const {
   setConfigStringA("Preferences", "PanelMBCharState",
      (state == BST_INDETERMINATE) ? "FT" : ((state == BST_CHECKED) ? "Y" : "N"),
      CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

bool ConfigIO::getMultiByteLexing(string fileType) const {
   string state{ getConfigStringA("Preferences", "PanelMBCharState", "FT", CONFIG_FILE_PATHS[CONFIG_PREFS]) };

   if (state == "FT")
      return (getConfigStringA(fileType, "MultiByteChars", "N", currentConfigFile) == "Y");
   else
      return (state == "Y");
}

int ConfigIO::getConfigAllSections(string& sections, string file) {
   const int bufSize{ FW_LINE_MAX_LENGTH };
   string ftBuf(bufSize, '\0');

   DWORD charCount{};
   charCount = GetPrivateProfileStringA(NULL, "", "", ftBuf.data(), bufSize, file.c_str());
   if (charCount < 1) return 0;

   int sectionCount{};
   for (unsigned int i{}; i < charCount; ++i) {
      if (ftBuf[i] == 0) {
         ftBuf[i] = ',';
         ++sectionCount;
      }
   }

   sections = ftBuf.c_str();
   return sectionCount;
}

int ConfigIO::getConfigAllSectionsList(vector<string>& sectionsList, string file) {
   string sections{};

   int sectionCount{ getConfigAllSections(sections, file) };
   if (sectionCount < 1) return 0;

   return Tokenize(sections, sectionsList);
}

int ConfigIO::getConfigAllSectionsList(vector<wstring>& sectionsList, wstring file) {
   string sections{};

   int sectionCount{ getConfigAllSections(sections, Utils::WideToNarrow(file)) };
   if (sectionCount < 1) return 0;

   return Tokenize(Utils::NarrowToWide(sections), sectionsList);
}

int ConfigIO::getConfigAllKeys(const string& section, string& keys, const string file) {
   const int bufSize{ FW_LINE_MAX_LENGTH };
   string keyBuf(bufSize, '\0');

   DWORD charCount{};
   charCount = GetPrivateProfileStringA(section.c_str(), NULL, "", keyBuf.data(), bufSize, file.c_str());
   if (charCount < 1) return 0;

   int keyCount{};
   for (unsigned int i{}; i < charCount; ++i) {
      if (keyBuf[i] == 0) {
         keyBuf[i] = ',';
         ++keyCount;
      }
   }

   keys = keyBuf.c_str();
   return keyCount;
}

int ConfigIO::getConfigAllKeysList(const string& section, vector<string>& keysList, const string file) {
   string keys{};

   int keyCount{ getConfigAllKeys(section, keys, file) };
   if (keyCount < 1) return 0;

   return Tokenize(keys, keysList);
}

int ConfigIO::getConfigAllKeysList(const string& section, vector<wstring>& keysList, const string file) {
   string keys{};

   int keyCount{ getConfigAllKeys(section, keys, file) };
   if (keyCount < 1) return 0;

   return Tokenize(Utils::NarrowToWide(keys), keysList);
}

int ConfigIO::getConfigValueList(vector<string>& valList, const string& section, const string& key,
   const string& defaultVal, string file) {
   return Tokenize(getConfigStringA(section, key, defaultVal, file), valList);
}

int ConfigIO::getThemesList(vector<wstring>& valList, wstring file) {
   if (file.empty()) file = wCurrentThemeFile;

   return Tokenize(getConfigWideChar(L"Base", "Themes", "", file), valList);
}

int ConfigIO::Tokenize(const string& text, vector<string>& results, const string& delim) {
   std::size_t nStart{}, nEnd;

   results.clear();
   if (text.empty()) return 0;

   while ((nEnd = text.find(delim, nStart)) != string::npos) {
      if (nEnd > nStart)
         results.emplace_back(text.substr(nStart, nEnd - nStart));

      nStart = nEnd + 1;
   }

   if (nStart < text.length())
      results.emplace_back(text.substr(nStart));

   return static_cast<int>(results.size());
}

int ConfigIO::Tokenize(const wstring& text, vector<wstring>& results, const wstring& delim) {
   std::size_t nStart{}, nEnd;

   results.clear();
   if (text.empty()) return 0;

   while ((nEnd = text.find(delim, nStart)) != wstring::npos) {
      if (nEnd > nStart)
         results.emplace_back(text.substr(nStart, nEnd - nStart));

      nStart = nEnd + 1;
   }

   if (nStart < text.length())
      results.emplace_back(text.substr(nStart));

   return static_cast<int>(results.size());
}

int ConfigIO::Tokenize(const string& text, vector<int>& results, const string& delim) {
   vector<string> interims;

   results.clear();
   if (text.empty()) return 0;

   Tokenize(text, interims, delim);

   for (string istr : interims)
      results.emplace_back(Utils::StringtoInt(istr));

   return static_cast<int>(results.size());
}

void ConfigIO::ActivateNewLineTabs(wstring& str) {
   str = std::regex_replace(str, std::wregex(L"\\\\n"), L"\n");
   str = std::regex_replace(str, std::wregex(L"\\\\t"), L"\t");
}

void ConfigIO::deleteKey(const string& section, const string& key, string file) const {
   if (file.empty()) file = currentConfigFile;

   WritePrivateProfileStringA(section.c_str(), key.c_str(), NULL, file.c_str());
}

void ConfigIO::deleteKey(const wstring& section, const wstring& key, wstring file) const {
   if (file.empty()) file = wCurrentConfigFile;

   WritePrivateProfileString(section.c_str(), key.c_str(), NULL, file.c_str());
}

void ConfigIO::deleteSection(const string& section, string file) {
   WritePrivateProfileStringA(section.c_str(), NULL, NULL, file.c_str());
}

string ConfigIO::readConfigFile(wstring file) const {
   if (file.empty()) file = wCurrentConfigFile;

   using std::ios;
   if (std::ifstream ifs{ file, ios::binary | ios::ate }) {
      int size = static_cast<int>(ifs.tellg());
      string str(size, '\0');
      ifs.seekg(0);
      ifs.read(str.data(), size);
      return str;
   }

   return "";
}

bool ConfigIO::queryConfigFileName(HWND hwnd, bool bOpen, bool backupFolder, wstring& backupConfigFile) const {
   OPENFILENAME ofn;

   TCHAR filePath[MAX_PATH]{};

   ZeroMemory(&ofn, sizeof(ofn));
   ofn.lStructSize = sizeof(ofn);
   ofn.hwndOwner = hwnd;
   ofn.lpstrFile = filePath;
   ofn.lpstrFile[0] = '\0';
   ofn.nMaxFile = sizeof(filePath);
   ofn.lpstrFilter = L"All\0*.*\0Ini Files\0*.ini\0";
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


   BOOL bOK = bOpen ? GetOpenFileName(&ofn) : GetSaveFileName(&ofn);

   if (bOK) backupConfigFile = ofn.lpstrFile;

   return bOK;
}

void ConfigIO::saveConfigFile(const wstring& fileData, wstring file) {
   using std::ios;

   if (std::ofstream fs{ file, ios::out | ios::binary | ios::trunc }) {
      string mbFileData{ Utils::WideToNarrow(fileData) };
      fs.write(mbFileData.data(), mbFileData.length());
   }

   flushConfigFile();
}

int ConfigIO::getBackupTempFileName(wstring& tempFileName) const {
   TCHAR tmpFilePath[MAX_PATH];

   if (GetTempFileName(pluginConfigBackupDir, L"FWViz_", 0, tmpFilePath) == 0) {
      tempFileName = L"";
      return -1;
   }

   tempFileName = tmpFilePath;
   return 0;
}

void ConfigIO::backupConfigFile(wstring file) const {
   using fsp = std::filesystem::path;

   char backupFile[MAX_PATH + 1];
   wchar_t backupFilePath[MAX_PATH + 1];

   wstring backupTemplate{ wstring{ fsp(file).stem().c_str() }.substr(0, 30) };
   backupTemplate +=  L"_%Y%m%d_%H%M%S" + wstring{fsp(file).extension().c_str()};

   time_t rawTime;
   struct tm* timeInfo;

   time(&rawTime);
   timeInfo = localtime(&rawTime);
   strftime(backupFile, MAX_PATH, Utils::WideToNarrow(backupTemplate).c_str(), timeInfo);

   PathCombine(backupFilePath, pluginConfigBackupDir, Utils::NarrowToWide(backupFile).c_str());
   CopyFile(file.c_str(), backupFilePath, FALSE);
}

void ConfigIO::viewBackupFolder() const {
   ShellExecute(NULL, L"open", pluginConfigBackupDir, NULL, NULL, SW_SHOWNORMAL);
}

bool ConfigIO::checkConfigFilesforUTF8() {
   bool status{ true };

   if (!fixIfNotUTF8File(WCONFIG_FILE_PATHS[CONFIG_VIZ]))
      status = false;

   if (!fixIfNotUTF8File(WCONFIG_FILE_PATHS[CONFIG_THEMES]))
      status = false;

   if (!fixIfNotUTF8File(WCONFIG_FILE_PATHS[CONFIG_FOLDSTRUCTS]))
      status = false;

   return status;
}

bool ConfigIO::fixIfNotUTF8File(CF_TYPES cfType) {
   if (cfType < 0 || cfType >= CONFIG_FILE_COUNT) return false;

   return fixIfNotUTF8File(WCONFIG_FILE_PATHS[cfType]);
}

ConfigIO::ENC_TYPE ConfigIO::getBOM(wstring file) {
   using std::ios;

   if (std::wifstream fs{ file, ios::binary }) {
      unsigned short bom[3]{};
      bom[0] = fs.get();
      bom[1] = fs.get();
      bom[2] = fs.get();
      fs.close();

      if (bom[0] == 0xFF && bom[1] == 0xFE)
         return UCS16_LE;
      else if (bom[0] == 0xFE && bom[1] == 0xFF)
         return UCS16_BE;
      else if (bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF)
         return UTF8_BOM;
   }

   return UTF8;
}

bool ConfigIO::fixIfNotUTF8File(wstring file) {
   using std::ios;

   ENC_TYPE encoding{ getBOM(file) };
   if (encoding == UTF8) return true;

   backupConfigFile(file);

   switch (encoding)
   {
   case UTF8_BOM:
      if (std::ifstream ifs{ file, ios::binary | ios::ate }) {
         int size = static_cast<int>(ifs.tellg()) - 3;
         string str(size, '\0');
         ifs.seekg(3);
         ifs.read(str.data(), size);
         ifs.close();

         string mbData{ str.c_str() };

         if (std::ofstream ofs{ file, ios::out | ios::binary | ios::trunc}) {
            ofs.write(mbData.c_str(), mbData.length());
         }
      }
      break;

   case UCS16_BE:
   case UCS16_LE:
      if (std::wifstream wifs{ file, ios::binary | ios::ate }) {
         wifs.imbue(std::locale(wifs.getloc(), new std::codecvt_utf16<wchar_t, 0x10FFFF, std::consume_header>));

         int size = static_cast<int>(wifs.tellg());
         wstring wstr(size, '\0');
         wifs.seekg(0);
         wifs.read(wstr.data(), size);
         wifs.close();

         string mbData{ Utils::WideToNarrow(wstr).c_str() };

         if (std::ofstream ofs{ file, ios::out | ios::binary | ios::trunc }) {
            ofs.write(mbData.c_str(), mbData.length());
         }
      }
      break;
   }

   return (getBOM(file) == UTF8);
}

void ConfigIO::flushConfigFile() {
   WritePrivateProfileString(NULL, NULL, NULL, NULL);
}

