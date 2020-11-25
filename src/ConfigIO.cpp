#include "ConfigIO.h"

void ConfigIO::init() {
   TCHAR sPluginDirectory[MAX_PATH];

   nppMessage(NPPM_GETPLUGINHOMEPATH, MAX_PATH, (LPARAM)sPluginDirectory);
   PathAppend(sPluginDirectory, PLUGIN_FOLDER_NAME);

   nppMessage(NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)pluginConfigDir);

   // if no existing config path, create it
   if (!PathFileExists(pluginConfigDir))
   {
      CreateDirectory(pluginConfigDir, NULL);
   }

   // if no existing config folder for this plugin, create it
   PathAppend(pluginConfigDir, PLUGIN_FOLDER_NAME);

   if (!PathFileExists(pluginConfigDir)) {
      CreateDirectory(pluginConfigDir, NULL);
   }

   // if no existing config backup folder for this plugin, create it
   PathCombine(pluginConfigBackupDir, pluginConfigDir, L"Backup");

   if (!PathFileExists(pluginConfigBackupDir)) {
      CreateDirectory(pluginConfigBackupDir, NULL);
   }

   // if config files are missing, copy them from the plugins folder
   const wstring sDefaultsPrefix{ L"defaults_" };

   TCHAR sDefaultsFile[MAX_PATH];
   TCHAR sConfigFile[MAX_PATH];

   for (int i{}; i < CONFIG_FILE_COUNT; i++) {
      PathCombine(sConfigFile, pluginConfigDir, CONFIG_FILES[i].c_str());
      CONFIG_FILE_PATHS[i] = wstring{ sConfigFile };

      if (!PathFileExists(sConfigFile)) {
         PathCombine(sDefaultsFile, sPluginDirectory, (sDefaultsPrefix + CONFIG_FILES[i]).c_str());
         CopyFile(sDefaultsFile, sConfigFile, TRUE);
      }
   }

   // initialize instance variables
   resetCurrentConfigFile();
   PathCombine(defaultConfigFile, sPluginDirectory, (sDefaultsPrefix + CONFIG_FILES[CONFIG_MAIN]).c_str());

   defaultBackColor = static_cast<int>
      (nppMessage(NPPM_GETEDITORDEFAULTBACKGROUNDCOLOR, NULL, NULL));
   defaultForeColor = static_cast<int>
      (nppMessage(NPPM_GETEDITORDEFAULTFOREGROUNDCOLOR, NULL, NULL));
}

wstring ConfigIO::getPluginConfigDir() {
   return wstring{ pluginConfigDir };
}

int ConfigIO::setCurrentConfigFile(const wstring& docFileType) {
   int sectionCount{};
   wstring sectionList{};

   if (docFileType.length() < 1) {
      resetCurrentConfigFile();
      return 0;
   }

   sectionCount = getConfigSectionList(sectionList, CONFIG_FILES[CONFIG_MAIN]);
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
   currentConfigFile = CONFIG_FILE_PATHS[CONFIG_MAIN];
}

string ConfigIO::getConfigStringA(const wstring& sectionName, const wstring& keyName,
   const wstring& defaultValue, wstring fileName) {
   return WideToNarrow(getConfigString(sectionName, keyName, defaultValue, fileName));
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
   setConfigString(sectionName, keyName, NarrowToWide(keyValue), fileName);
}

void ConfigIO::setConfigString(const wstring& sectionName, const wstring& keyName,
   const wstring& keyValue, wstring fileName) {
   if (fileName.length() < 1) fileName = currentConfigFile;

   WritePrivateProfileString(sectionName.c_str(), keyName.c_str(), keyValue.c_str(), fileName.c_str());
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

void ConfigIO::saveConfigFile(const wstring &fileData, wstring fileName) {
   if (fileName.length() < 1) fileName = currentConfigFile;

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

int ConfigIO::Tokenize(const wstring &text, vector<wstring> &results, LPCWSTR delim) {
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

int ConfigIO::Tokenize(const wstring &text, vector<int> &results, LPCWSTR delim) {
   vector<wstring> interims;

   results.clear();
   if (text.length() < 1) return 0;

   Tokenize(text, interims, delim);

   for (auto istr : interims)
      results.emplace_back(std::stoi(istr));

   return static_cast<int>(results.size());
}

LPCWSTR ConfigIO::ToUpper(LPWSTR str) {
   return std::use_facet<std::ctype<wchar_t>>(std::locale()).toupper(str, str + wcslen(str));
}

wstring ConfigIO::NarrowToWide(const string &str) {
   return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
}

string ConfigIO::WideToNarrow(const wstring &wStr) {
   return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wStr);
}

void ConfigIO::setThemeFilePath(const wstring theme) {
   PathCombine(themeConfigFile, pluginConfigDir, (theme + wstring{ L".ini" }).c_str());
}

wstring ConfigIO::getStyleValue(const wstring& styleName) {
   if (wstring{ themeConfigFile }.length() < 0)
      setThemeFilePath();

   return getConfigString(L"Styles", styleName, L"", themeConfigFile);
}

void ConfigIO::getStyleColor(const wstring& styleName, int &color, bool foreColor) {
   vector<int> rgb;

   Tokenize(getStyleValue(styleName), rgb);

   if (rgb.size() >= 3)
      color = rgb[0] | (rgb[1] << 8) | (rgb[2] << 16);
   else
      color = foreColor ? defaultForeColor : defaultBackColor;
}

void ConfigIO::getStyleBool(const wstring& styleName, int &var) {
   var = (getStyleValue(styleName) == L"Y") ? 1 : 0;
}

void ConfigIO::backupMoveConfigFile() {
   char aFileName[50];
   time_t rawTime;
   struct tm* timeInfo;

   time(&rawTime);
   timeInfo = localtime(&rawTime);
   strftime(aFileName, 50, "Visualizer_%Y%m%d_%H%M%S.ini", timeInfo);

   TCHAR backupFilePath[MAX_PATH];
   wstring wFileName;

   wFileName = NarrowToWide(aFileName);
   PathCombine(backupFilePath, pluginConfigBackupDir, wFileName.c_str());
   MoveFile(CONFIG_FILE_PATHS[CONFIG_MAIN].c_str(), backupFilePath);
}

BOOL ConfigIO::queryConfigFileName(HWND hwnd, bool bOpen, bool bBackupFolder, wstring &backupConfigFile) {
   OPENFILENAME ofn;

   TCHAR filePath[MAX_PATH];

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

   if (bBackupFolder)
      ofn.lpstrInitialDir = pluginConfigBackupDir;
   else {
      TCHAR desktopPath[MAX_PATH];
      SHGetSpecialFolderPath(NULL, desktopPath, CSIDL_DESKTOP, FALSE);
      ofn.lpstrInitialDir = desktopPath;
   }

   BOOL bOK = bOpen ? GetOpenFileName(&ofn) : GetSaveFileName(&ofn);

   if (bOK) backupConfigFile = ofn.lpstrFile;

   return bOK;
}

void ConfigIO::viewBackupFolder() {
   ShellExecute(NULL, L"open", pluginConfigBackupDir, NULL, NULL, SW_SHOWNORMAL);
}

int ConfigIO::getBackupTempFileName(wstring &tempFileName) {
   TCHAR tmpFilePath[MAX_PATH];

   if (GetTempFileName(pluginConfigBackupDir, L"FT_", 0, tmpFilePath) == 0) {
      tempFileName = L"";
      return -1;
   }

   tempFileName = tmpFilePath;
   return 0;
}

vector<wstring> ConfigIO::getFileList(const wstring& path, const wstring& fileMask) {
   namespace fs = std::filesystem;

   vector<wstring> file_list{};
   for (const auto item : fs::directory_iterator(path)) {
      if (fileMask.length() < 1 || std::regex_match(item.path().filename().wstring(), std::wregex(fileMask)))
         file_list.push_back(item.path().filename());
   }

   return file_list;
}

bool ConfigIO::getCaretFramed() {
   return getConfigString(L"Preferences", L"FramedCaret", L"Y", CONFIG_FILE_PATHS[CONFIG_PREFS]) == L"Y";
}

void ConfigIO::setCaretFramed(bool framed) {
   setConfigString(L"Preferences", L"FramedCaret", framed ? L"Y" : L"N", CONFIG_FILE_PATHS[CONFIG_PREFS]);
}
