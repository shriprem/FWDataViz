#include "ConfigIO.h"

void ConfigIO::init() {
   const wstring sPluginFolder{ L"FWDataViz" };
   TCHAR sPluginDirectory[MAX_PATH];

   nppMessage(NPPM_GETNPPDIRECTORY, MAX_PATH, (LPARAM)sPluginDirectory);
   PathAppendW(sPluginDirectory, (L"plugins\\" + sPluginFolder).c_str());

   nppMessage(NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)pluginConfigDir);

   // if no existing config path, create it
   if (!PathFileExists(pluginConfigDir))
   {
      CreateDirectory(pluginConfigDir, NULL);
   }

   PathAppendW(pluginConfigDir, sPluginFolder.c_str());

   // if no existing config file, create it
   if (!PathFileExists(pluginConfigDir)) {
      CreateDirectory(pluginConfigDir, NULL);
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

string ConfigIO::getConfigStringA(LPCWSTR sectionName, LPCWSTR keyName, LPCWSTR defaultValue, LPCWSTR fileName)
{
   return WideToNarrow(getConfigString(sectionName, keyName, defaultValue, fileName));
}

wstring ConfigIO::getConfigString(LPCWSTR sectionName, LPCWSTR keyName, LPCWSTR defaultValue, LPCWSTR fileName) {
   const int bufSize{ 32000 };
   wchar_t ftBuf[bufSize];

   if (wstring{ fileName }.length() < 1)
      fileName = CONFIG_FILE_PATHS[CONFIG_MAIN].c_str();

   GetPrivateProfileStringW(sectionName, keyName, defaultValue, ftBuf, bufSize, fileName);

   return wstring{ ftBuf };
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
