// This file was developed for the FWDataViz and GotoLineCol plugins,
// culled from the Notepad++'s Darkmode implementation in Parameters.cpp.
// Copyright (C) 2022 Shridhar Kumar <shriprem@gmail.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "NppDarkMode.h"
#include "tinyxml2.h"

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

   void init(const bool darkModeQuerySupported);
   DarkModeConf getDarkModeConf();

   static COLORREF nppBackgroundRGB();

private:
   bool _isLocal{};
   wstring _nppPath{};

   DarkModeConf _darkmode;

   static NppParameters* getInstancePointer() {
      static NppParameters* instance = new NppParameters;
      return instance;
   };

   bool isLocal();
   wstring getSettingsFolder();
   bool loadConfigSettings(const bool darkModeQuerySupported);
};

