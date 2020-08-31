#include "PluginDefinition.h"

void ConfigIO::init() {
   const std::wstring sPluginFolder{ L"FWDataViz" };
   TCHAR sPluginDirectory[MAX_PATH];

   ::SendMessage(nppData._nppHandle, NPPM_GETNPPDIRECTORY, MAX_PATH, (LPARAM)sPluginDirectory);
   ::PathAppendW(sPluginDirectory, (L"plugins\\" + sPluginFolder).c_str());

   ::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)pluginConfigDir);

   // if no existing config path, create it
   if (!PathFileExists(pluginConfigDir))
   {
      ::CreateDirectory(pluginConfigDir, NULL);
   }

   ::PathAppendW(pluginConfigDir, sPluginFolder.c_str());

   // if no existing config file, create it
   if (!PathFileExists(pluginConfigDir)) {
      ::CreateDirectory(pluginConfigDir, NULL);
   }

   // if config files are missing copy them from the plugins folder
   const std::wstring sDefaultsPrefix{ L"defaults_" };

   TCHAR sDefaultsFile[MAX_PATH];
   TCHAR sConfigFile[MAX_PATH];


   for (int i{}; i < CONFIG_FILE_COUNT; i++) {
      ::PathCombine(sConfigFile, pluginConfigDir, CONFIG_FILES[i].c_str());
      CONFIG_FILE_PATHS[i] = std::wstring{ sConfigFile };

      if (!PathFileExists(sConfigFile)) {
         ::PathCombine(sDefaultsFile, sPluginDirectory, (sDefaultsPrefix + CONFIG_FILES[i]).c_str());
         ::CopyFile(sDefaultsFile, sConfigFile, TRUE);
      }
   }
}

std::wstring ConfigIO::getConfigString(LPCWSTR sectionName, LPCWSTR keyName, LPCWSTR defaultValue, LPCWSTR fileName) {
   const int bufSize{ 32000 };
   wchar_t ftBuf[bufSize];
   DWORD retLen;

   if (std::wstring{ fileName }.length() < 1) {
      fileName = CONFIG_FILE_PATHS[CONFIG_MAIN].c_str();
   }

   retLen = GetPrivateProfileStringW(sectionName, keyName, defaultValue, ftBuf, bufSize, fileName);

   return std::wstring{ ftBuf };
}

void ConfigIO::Tokenize(const std::wstring &text, std::vector<std::wstring> &results, LPCWSTR delim) {
   std::size_t nStart{}, nEnd;

   while ((nEnd = text.find(delim, nStart)) != std::wstring::npos) {
      if (nEnd > nStart) {
         results.emplace_back(text.substr(nStart, nEnd - nStart));
      }
      nStart = nEnd + 1;
   }
   if (nStart < text.length()) {
      results.emplace_back(text.substr(nStart));
   }
}

void ConfigIO::Tokenize(const std::wstring &text, std::vector<int> &results, LPCWSTR delim) {
   std::vector<std::wstring> interims;
   Tokenize(text, interims, delim);

   for (auto istr : interims) {
      results.emplace_back(std::stoi(istr));
   }
}

std::wstring ConfigIO::NarrowToWide(const std::string &str) {
   return std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str);
}

std::string ConfigIO::WideToNarrow(const std::wstring &wStr) {
   return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wStr);
}

void ConfigIO::setThemeFilePath(const std::wstring theme)
{
   ::PathCombine(themeConfigFile, pluginConfigDir, (theme + std::wstring{ L".ini" }).c_str());
}

std::wstring ConfigIO::getStyleValue(LPCWSTR styleName)
{
   if (std::wstring{ themeConfigFile }.length() < 0) {
      setThemeFilePath();
   }

   return getConfigString(L"Styles", styleName, L"", themeConfigFile);
}

void ConfigIO::getStyleColor(LPCWSTR styleName, int *color)
{
   std::vector<int> rgb;

   Tokenize(getStyleValue(styleName), rgb);

   if (rgb.size() >= 3) {
      color[0] = rgb[0];
      color[1] = rgb[1];
      color[2] = rgb[2];
   }
   else {
      color[0] = 0;
      color[1] = 0;
      color[2] = 0;
   }
}

void ConfigIO::getStyleBool(LPCWSTR styleName, bool &var)
{
   var = (getStyleValue(styleName).compare(L"Y") == 0);
}
