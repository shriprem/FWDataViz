#pragma once

#include "NppDarkMode.h"
#include "tinyxml2.h"
#include "../Utils.h"

typedef tinyxml2::XMLDocument TXMLDocument;
typedef tinyxml2::XMLElement TXMLElement;
typedef tinyxml2::XMLError TXMLError;

using NppDarkMode::DarkModeConf;
using NppDarkMode::toolBarStatusType;

class NppParameters final
{
public:
   static NppParameters& getInstance() {
      return *getInstancePointer();
   };

   void init();
   DarkModeConf getDarkModeConf();
   toolBarStatusType gettoolBarStatusType();

   static COLORREF nppBackgroundRGB();

private:
   bool _isLocal{};
   wstring _nppPath{};

   DarkModeConf _darkmode;
   toolBarStatusType _tbType;

   static NppParameters* getInstancePointer() {
      static NppParameters* instance = new NppParameters;
      return instance;
   };

   bool isLocal();
   wstring getSettingsFolder();
   bool loadConfigSettings();
};

