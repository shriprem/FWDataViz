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
      WCONFIG_FILE_PATHS[i] = wstring{ sConfigFile };
      CONFIG_FILE_PATHS[i] = Utils::WideToNarrow(sConfigFile);

      if (!PathFileExists(sConfigFile)) {
         PathCombine(sDefaultsFile, sPluginDirectory, (sDefaultPrefix + CONFIG_FILES[i]).c_str());
         CopyFile(sDefaultsFile, sConfigFile, TRUE);
      }
   }

   // initialize instance variables
   resetVizConfig();
   PathCombine(defaultConfigFile, sPluginDirectory, (sDefaultPrefix + CONFIG_FILES[CONFIG_VIZ]).c_str());
}

int ConfigIO::setVizConfig(const string& docFileType) {
   int sectionCount{};
   string sectionList{};

   if (docFileType.length() < 1) {
      resetVizConfig();
      return 0;
   }

   sectionCount = getConfigAllSections(sectionList, CONFIG_FILE_PATHS[CONFIG_VIZ]);
   if (sectionCount > 0 && sectionList.find(docFileType) != std::string::npos) {
      resetVizConfig();
      return 1;
   }

   sectionCount = getConfigAllSections(sectionList, Utils::WideToNarrow(defaultConfigFile));
   if (sectionCount > 0 && sectionList.find(docFileType) != std::string::npos) {
      wCurrentConfigFile = wstring{ defaultConfigFile };
      currentConfigFile = Utils::WideToNarrow(defaultConfigFile);
      return 2;
   }

   resetVizConfig();
   return -1;
}

void ConfigIO::resetVizConfig() {
   wCurrentConfigFile = WCONFIG_FILE_PATHS[CONFIG_VIZ];
   currentConfigFile = CONFIG_FILE_PATHS[CONFIG_VIZ];
}

wstring ConfigIO::getConfigFile(int cfType) {
   if (cfType < 0 && cfType >= CONFIG_FILE_COUNT) return L"";

   return WCONFIG_FILE_PATHS[cfType];
}

string ConfigIO::getConfigStringA(const string& section, const string& key, const string& default, string file) {
   const int bufSize{ FW_LINE_MAX_LENGTH };
   char ftBuf[bufSize];

   if (file.length() < 1) file = currentConfigFile;

   GetPrivateProfileStringA(section.c_str(), key.c_str(), default.c_str(), ftBuf, bufSize, file.c_str());

   return string{ ftBuf };
}

string ConfigIO::getConfigStringA(const wstring& section, const string& key, const string& default, wstring file) {
   return getConfigStringA(Utils::WideToNarrow(section), key, default, Utils::WideToNarrow(file));
}

wstring ConfigIO::getConfigWideChar(const string& section, const string& key, const string& default, string file) {
   return Utils::NarrowToWide(getConfigStringA(section, key, default, file));
}

wstring ConfigIO::getConfigWideChar(const wstring& section, const string& key, const string& default, wstring file) {
   return Utils::NarrowToWide(getConfigStringA(section, key, default, file));
}

void ConfigIO::setConfigStringA(const string& section, const string& key, const string& value, string file) {
   if (file.length() < 1) file = currentConfigFile;

   WritePrivateProfileStringA(section.c_str(), key.c_str(), value.c_str(), file.c_str());
}

void ConfigIO::setConfigMultiByte(const string& section, const string& key, const wstring& value, string file) {
   setConfigStringA(section, key, Utils::WideToNarrow(value), file);
}

int ConfigIO::getConfigInt(const string& section, const string& key, const int& default, string file) {
   string defVal{ to_string(default) };
   return Utils::StringtoInt(getConfigStringA(section, key, defVal, file));
}

int ConfigIO::getConfigInt(const wstring& section, const string& key, const int& default, wstring file) {
   string defVal{ to_string(default) };
   return Utils::StringtoInt(getConfigStringA(section, key, defVal, file));
}

wstring ConfigIO::getStyleValue(const wstring& theme, const string& styleName, wstring file) {
   return getConfigWideChar(theme, styleName, "", (file.length() < 1) ? WCONFIG_FILE_PATHS[CONFIG_THEMES] : file);
}

void ConfigIO::getFullStyle(const wstring& theme, const string& styleName, StyleInfo& style, wstring file) {
   wstring val = getStyleValue(theme, styleName, file);

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

string ConfigIO::getFieldStyleText(const wstring& fieldName) {
   return getConfigStringA("Styles", Utils::WideToNarrow(fieldName), "", CONFIG_FILE_PATHS[CONFIG_FIELD_TYPES]);
}

void ConfigIO::parseFieldStyle(const string& styleText, StyleInfo& style) {
   style.backColor = Utils::StringtoInt(styleText.substr(0, 6), 16);
   style.foreColor = Utils::StringtoInt(styleText.substr(7, 6), 16);
   style.bold = Utils::StringtoInt(styleText.substr(14, 1));
   style.italics = Utils::StringtoInt(styleText.substr(15, 1));
}

wstring ConfigIO::getPreference(const string key, const string default) {
   return getConfigWideChar("Preferences", key, default, CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

void ConfigIO::setPreference(const string key, const wstring value) {
   setConfigStringA("Preferences", key.c_str(), Utils::WideToNarrow(value.c_str()), CONFIG_FILE_PATHS[CONFIG_PREFS].c_str());
}

bool ConfigIO::getPreferenceBool(const string key, const bool default) {
   return getConfigStringA("Preferences", key, default ? "Y" : "N", CONFIG_FILE_PATHS[CONFIG_PREFS]) == "Y";
}

void ConfigIO::setPreferenceBool(const string key, const bool value) {
   setConfigStringA("Preferences", key, value ? "Y" : "N", CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

int ConfigIO::getPreferenceInt(const string key, const int default) {
   return getConfigInt("Preferences", key, default, CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

void ConfigIO::setPreferenceInt(const string key, const int value) {
   setConfigStringA("Preferences", key, to_string(value), CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

void ConfigIO::setPanelMBCharState(UINT state) {
   setConfigStringA("Preferences", "PanelMBCharState",
      (state == BST_INDETERMINATE) ? "FT" : ((state == BST_CHECKED) ? "Y" : "N"),
      CONFIG_FILE_PATHS[CONFIG_PREFS]);
}

bool ConfigIO::getMultiByteLexing(string fileType) {
   string state{ getConfigStringA("Preferences", "PanelMBCharState", "FT", CONFIG_FILE_PATHS[CONFIG_PREFS]) };

   if (state == "FT")
      return (getConfigStringA(fileType, "MultiByteChars", "N", currentConfigFile) == "Y");
   else
      return (state == "Y");
}

int ConfigIO::getConfigAllSections(string& sections, string file) {
   const int bufSize{ FW_LINE_MAX_LENGTH };
   char ftBuf[bufSize];

   DWORD charCount{};
   charCount = GetPrivateProfileStringA(NULL, "", "", ftBuf, bufSize, file.c_str());
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

int ConfigIO::getConfigValueList(vector<string>& valList, const string& section, const string& key,
   const string& default, string file) {
   return Tokenize(getConfigStringA(section, key, default, file), valList);
}

int ConfigIO::getThemesList(vector<wstring>& valList, wstring file) {
   if (file.length() < 1) file = WCONFIG_FILE_PATHS[CONFIG_THEMES];

   return Tokenize(getConfigWideChar(L"Base", "Themes", "", file), valList);
}

int ConfigIO::Tokenize(const string& text, vector<string>& results, const string& delim) {
   std::size_t nStart{}, nEnd;

   results.clear();
   if (text.length() < 1) return 0;

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

int ConfigIO::Tokenize(const string& text, vector<int>& results, const string& delim) {
   vector<string> interims;

   results.clear();
   if (text.length() < 1) return 0;

   Tokenize(text, interims, delim);

   for (string istr : interims)
      results.emplace_back(Utils::StringtoInt(istr));

   return static_cast<int>(results.size());
}

void ConfigIO::ActivateNewLineTabs(wstring& str) {
   str = std::regex_replace(str, std::wregex(L"\\\\n"), L"\n");
   str = std::regex_replace(str, std::wregex(L"\\\\t"), L"\t");
}

void ConfigIO::deleteKey(const wstring& section, const wstring& key, wstring file) {
   if (file.length() < 1) file = wCurrentConfigFile;

   WritePrivateProfileString(section.c_str(), key.c_str(), NULL, file.c_str());
}

void ConfigIO::deleteSection(const string& section, string file) {
   WritePrivateProfileStringA(section.c_str(), NULL, NULL, file.c_str());
}

string ConfigIO::readConfigFile(wstring file) {
   if (file.length() < 1) file = wCurrentConfigFile;

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

bool ConfigIO::queryConfigFileName(HWND hwnd, bool bOpen, bool backupFolder, wstring& backupConfigFile) {
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

int ConfigIO::getBackupTempFileName(wstring& tempFileName) {
   TCHAR tmpFilePath[MAX_PATH];

   if (GetTempFileName(pluginConfigBackupDir, L"FWViz_", 0, tmpFilePath) == 0) {
      tempFileName = L"";
      return -1;
   }

   tempFileName = tmpFilePath;
   return 0;
}

void ConfigIO::backupConfigFile(wstring file) {
   using fsp = std::filesystem::path;

   char backupFile[MAX_PATH];
   wchar_t backupFilePath[MAX_PATH];

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

void ConfigIO::viewBackupFolder() {
   ShellExecute(NULL, L"open", pluginConfigBackupDir, NULL, NULL, SW_SHOWNORMAL);
}

bool ConfigIO::checkConfigFilesforUCS16() {
   bool status{ true };

   if (!fixIfUTF16File(WCONFIG_FILE_PATHS[CONFIG_VIZ]))
      status = false;

   if (!fixIfUTF16File(WCONFIG_FILE_PATHS[CONFIG_THEMES]))
      status = false;

   return status;
}

bool ConfigIO::fixIfUTF16File(int cfType) {
   if (cfType < 0 && cfType >= CONFIG_FILE_COUNT) return false;

   return fixIfUTF16File(WCONFIG_FILE_PATHS[cfType]);
}

bool ConfigIO::fixIfUTF16File(wstring file) {
   if (!hasBOM(file)) return true;

   convertFromUTF16ToUTF8(file);

   return hasBOM(file);
}

bool ConfigIO::hasBOM(wstring file) {
   using std::ios;

   if (std::wifstream fs{ file, ios::binary }) {
      unsigned short bom[2]{};
      bom[0] = fs.get();
      bom[1] = fs.get();

      return (bom[0] == 0xFF && bom[1] == 0xFE) || (bom[0] == 0xFE && bom[1] == 0xFF);
   }

   return true;
}

void ConfigIO::convertFromUTF16ToUTF8(wstring file) {
   using std::ios;

   backupConfigFile(file);

   if (std::wifstream wifs{ file, ios::binary | ios::ate }) {
      wifs.imbue(std::locale(wifs.getloc(),
         new std::codecvt_utf16<wchar_t, 0x10FFFF, std::consume_header>));

      int size = static_cast<int>(wifs.tellg());
      wstring wstr(size, '\0');
      wifs.seekg(0);
      wifs.read(wstr.data(), size);

      string mbData{ Utils::WideToNarrow(wstr).c_str() };

      if (std::ofstream ofs{ file, ios::out | ios::binary | ios::trunc }) {
         ofs.write(mbData.c_str(), mbData.length());
      }
   }
}

void ConfigIO::flushConfigFile() {
   WritePrivateProfileString(NULL, NULL, NULL, NULL);
}

