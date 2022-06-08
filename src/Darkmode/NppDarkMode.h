// This file is part of Notepad++ project
// Copyright (c) 2021 adzm / Adam D. Walling

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

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "uxtheme.lib")

#include "..\Utils.h"

constexpr COLORREF HEXRGB(DWORD rrggbb) {
   // from 0xRRGGBB like natural #RRGGBB
   // to the little-endian 0xBBGGRR
   return
      ((rrggbb & 0xFF0000) >> 16) |
      ((rrggbb & 0x00FF00) ) |
      ((rrggbb & 0x0000FF) << 16);
}

namespace NppDarkMode
{
   struct Colors
   {
      COLORREF background = 0;
      COLORREF softerBackground = 0;
      COLORREF hotBackground = 0;
      COLORREF pureBackground = 0;
      COLORREF errorBackground = 0;
      COLORREF text = 0;
      COLORREF darkerText = 0;
      COLORREF disabledText = 0;
      COLORREF linkText = 0;
      COLORREF edge = 0;
      COLORREF hotEdge = 0;
      COLORREF disabledEdge = 0;
   };


   enum class ToolTipsType
   {
      tooltip,
      toolbar,
      listview,
      treeview,
      tabbar
   };

   enum class TreeViewStyle
   {
      classic = 0,
      light = 1,
      dark = 2
   };

   void initDarkMode();
   void queryNPPDarkmode();      // sync options from NPP instance
   bool isEnabled();
   bool isExperimentalSupported();
   bool isWindows11();

   COLORREF invertLightness(COLORREF c);
   COLORREF invertLightnessSofter(COLORREF c);
   double calculatePerceivedLighness(COLORREF c);

   COLORREF getBackgroundColor();
   COLORREF getSofterBackgroundColor();
   COLORREF getHotBackgroundColor();
   COLORREF getDarkerBackgroundColor();
   COLORREF getErrorBackgroundColor();

   COLORREF getTextColor();
   COLORREF getDarkerTextColor();
   COLORREF getDisabledTextColor();
   COLORREF getLinkTextColor();

   COLORREF getEdgeColor();
   COLORREF getHotEdgeColor();
   COLORREF getDisabledEdgeColor();

   HBRUSH getBackgroundBrush();
   HBRUSH getDarkerBackgroundBrush();
   HBRUSH getSofterBackgroundBrush();
   HBRUSH getHotBackgroundBrush();
   HBRUSH getErrorBackgroundBrush();

   HBRUSH getEdgeBrush();
   HBRUSH getHotEdgeBrush();
   HBRUSH getDisabledEdgeBrush();

   HPEN getDarkerTextPen();
   HPEN getEdgePen();
   HPEN getHotEdgePen();
   HPEN getDisabledEdgePen();

   // from DarkMode.h
   void allowDarkModeForApp(bool allow);
   bool allowDarkModeForWindow(HWND hWnd, bool allow);
   void setTitleBarThemeColor(HWND hWnd);

   // enhancements to DarkMode.h
   void enableDarkScrollBarForWindowAndChildren(HWND hwnd);

   inline void paintRoundFrameRect(HDC hdc, const RECT rect, const HPEN hpen, int width = 0, int height = 0);

   void subclassButtonControl(HWND hwnd);
   void subclassComboBoxControl(HWND hwnd);
   void subclassGroupboxControl(HWND hwnd);
   void subclassTabControl(HWND hwnd);
   bool subclassUpDownControl(HWND hwnd);

   void autoSubclassAndThemeChildControls(HWND hwndParent, bool subclass = true, bool theme = true);
   void autoThemeChildControls(HWND hwndParent);


   void setDarkTitleBar(HWND hwnd);
   void setDarkExplorerTheme(HWND hwnd);
   void setDarkScrollBar(HWND hwnd);
   void setDarkTooltips(HWND hwnd, ToolTipsType type);
   void setDarkLineAbovePanelToolbar(HWND hwnd);
   void setDarkListView(HWND hwnd);

   void disableVisualStyle(HWND hwnd, bool doDisable);
   void calculateTreeViewStyle();
   void setTreeViewStyle(HWND hwnd);

   void setBorder(HWND hwnd, bool border = true);
   void initSysLink(HWND hCtl);

   LRESULT onCtlColor(HDC hdc);
   LRESULT onCtlColorSofter(HDC hdc);
   LRESULT onCtlColorDarker(HDC hdc);
   LRESULT onCtlColorError(HDC hdc);
   LRESULT onCtlColorSysLink(HDC hdc);

   LRESULT onCtlColorIfEnabled(HDC hdc, bool bEnabled);
   LRESULT onCtlHiliteIfEnabled(HDC hdc, bool bEnabled);
   INT_PTR onCtlColorListbox(WPARAM wParam, LPARAM lParam);
}
