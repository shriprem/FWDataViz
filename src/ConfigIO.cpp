#include "ConfigIO.h"

void ConfigIO::init() {
   const wstring sPluginFolder{ L"FWDataViz" };
   TCHAR sPluginDirectory[MAX_PATH];

   nppMessage(NPPM_GETNPPDIRECTORY, MAX_PATH, (LPARAM)sPluginDirectory);
   PathAppend(sPluginDirectory, (L"plugins\\" + sPluginFolder).c_str());

   nppMessage(NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)pluginConfigDir);

   // if no existing config path, create it
   if (!PathFileExists(pluginConfigDir))
   {
      CreateDirectory(pluginConfigDir, NULL);
   }

   // if no existing config folder for this plugin, create it
   PathAppend(pluginConfigDir, sPluginFolder.c_str());

   if (!PathFileExists(pluginConfigDir)) {
      CreateDirectory(pluginConfigDir, NULL);
   }

   // if no existing config backup folder for this plugin, create it
   PathCombine(pluginConfigBackupDir, pluginConfigDir, L"Backup");

   if (!PathFileExists(pluginConfigBackupDir)) {
      CreateDirectory(pluginConfigBackupDir, NULL);
   }

   // if config files are missing copy them from the plugins folder
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

   defaultBackColor = static_cast<int>
      (nppMessage(NPPM_GETEDITORDEFAULTBACKGROUNDCOLOR, NULL, NULL));
   defaultForeColor = static_cast<int>
      (nppMessage(NPPM_GETEDITORDEFAULTFOREGROUNDCOLOR, NULL, NULL));
}

string ConfigIO::getConfigStringA(LPCWSTR sectionName, LPCWSTR keyName, LPCWSTR defaultValue, LPCWSTR fileName) {
   return WideToNarrow(getConfigString(sectionName, keyName, defaultValue, fileName));
}

wstring ConfigIO::getConfigString(LPCWSTR sectionName, LPCWSTR keyName, LPCWSTR defaultValue, LPCWSTR fileName) {
   const int bufSize{ 32000 };
   wchar_t ftBuf[bufSize];

   if (wstring{ fileName }.length() < 1)
      fileName = CONFIG_FILE_PATHS[CONFIG_MAIN].c_str();

   GetPrivateProfileString(sectionName, keyName, defaultValue, ftBuf, bufSize, fileName);

   return wstring{ ftBuf };
}

void ConfigIO::setConfigStringA(LPCWSTR sectionName, LPCWSTR keyName, LPCSTR keyValue, LPCWSTR fileName) {
   setConfigString(sectionName, keyName, NarrowToWide(keyValue).c_str(), fileName);
}

void ConfigIO::setConfigString(LPCWSTR sectionName, LPCWSTR keyName, LPCWSTR keyValue, LPCWSTR fileName) {
   if (wstring{ fileName }.length() < 1)
      fileName = CONFIG_FILE_PATHS[CONFIG_MAIN].c_str();

   WritePrivateProfileString(sectionName, keyName, keyValue, fileName);
}

void ConfigIO::flushConfigFile() {
   WritePrivateProfileString(NULL, NULL, NULL, NULL);
}

void ConfigIO::saveConfigFile(const wstring &fileData, LPCWSTR fileName) {
   if (wstring{ fileName }.length() < 1)
      fileName = CONFIG_FILE_PATHS[CONFIG_MAIN].c_str();

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

LPCWSTR ConfigIO::ToUpper(LPWSTR str, size_t strlen) {
   return std::use_facet<std::ctype<wchar_t>>(std::locale()).toupper(str, str + strlen);
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

wstring ConfigIO::getStyleValue(LPCWSTR styleName) {
   if (wstring{ themeConfigFile }.length() < 0)
      setThemeFilePath();

   return getConfigString(L"Styles", styleName, L"", themeConfigFile);
}

void ConfigIO::getStyleColor(LPCWSTR styleName, int &color, bool foreColor) {
   vector<int> rgb;

   Tokenize(getStyleValue(styleName), rgb);

   if (rgb.size() >= 3)
      color = rgb[0] | (rgb[1] << 8) | (rgb[2] << 16);
   else
      color = foreColor ? defaultForeColor : defaultBackColor;
}

void ConfigIO::getStyleBool(LPCWSTR styleName, int &var) {
   var = (getStyleValue(styleName).compare(L"Y") == 0) ? 1 : 0;
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

BOOL ConfigIO::getBackupConfigFileName(HWND hwnd, wstring *backupConfigFile) {
   OPENFILENAME ofn;

   TCHAR filePath[MAX_PATH];

   ZeroMemory(&ofn, sizeof(ofn));
   ofn.lStructSize = sizeof(ofn);
   ofn.hwndOwner = hwnd;
   ofn.lpstrFile = filePath;
   ofn.lpstrFile[0] = '\0';
   ofn.nMaxFile = sizeof(filePath);
   ofn.lpstrFilter = L"All\0*.*\0Ini Files\0*.INI\0";
   ofn.nFilterIndex = 2;
   ofn.lpstrFileTitle = NULL;
   ofn.nMaxFileTitle = 0;
   ofn.lpstrInitialDir = pluginConfigBackupDir;
   ofn.lpstrTitle = FWVIZ_OPEN_BKUP_CONFIG_DLG;
   ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

   BOOL ret{ GetOpenFileName(&ofn) };
   if (ret) *backupConfigFile = ofn.lpstrFile;

   return ret;
}

void ConfigIO::viewBackupFolder() {
   ShellExecute(NULL, L"open", pluginConfigBackupDir, NULL, NULL, SW_SHOWNORMAL);
}
