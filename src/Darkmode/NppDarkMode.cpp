// This file is part of Notepad++ project
// Copyright (C)2021 adzm / Adam D. Walling

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


#include "NppDarkMode.h"
#include "DarkMode.h"

#include <Uxtheme.h>
#include <Vssym32.h>
#include <Shlwapi.h>
#include <cmath>
#include <cassert>

#ifdef __GNUC__
#include <cmath>
#define WINAPI_LAMBDA WINAPI
#else
#define WINAPI_LAMBDA
#endif

namespace NppDarkMode
{
   struct Brushes
   {
      HBRUSH background = nullptr;
      HBRUSH softerBackground = nullptr;
      HBRUSH hotBackground = nullptr;
      HBRUSH pureBackground = nullptr;
      HBRUSH errorBackground = nullptr;

      Brushes(const Colors& colors)
         : background(::CreateSolidBrush(colors.background))
         , softerBackground(::CreateSolidBrush(colors.softerBackground))
         , hotBackground(::CreateSolidBrush(colors.hotBackground))
         , pureBackground(::CreateSolidBrush(colors.pureBackground))
         , errorBackground(::CreateSolidBrush(colors.errorBackground))
      {}

      ~Brushes()
      {
         ::DeleteObject(background);         background = nullptr;
         ::DeleteObject(softerBackground);   softerBackground = nullptr;
         ::DeleteObject(hotBackground);      hotBackground = nullptr;
         ::DeleteObject(pureBackground);     pureBackground = nullptr;
         ::DeleteObject(errorBackground); errorBackground = nullptr;
      }

      void change(const Colors& colors)
      {
         ::DeleteObject(background);
         ::DeleteObject(softerBackground);
         ::DeleteObject(hotBackground);
         ::DeleteObject(pureBackground);
         ::DeleteObject(errorBackground);

         background = ::CreateSolidBrush(colors.background);
         softerBackground = ::CreateSolidBrush(colors.softerBackground);
         hotBackground = ::CreateSolidBrush(colors.hotBackground);
         pureBackground = ::CreateSolidBrush(colors.pureBackground);
         errorBackground = ::CreateSolidBrush(colors.errorBackground);
      }
   };

   struct Pens
   {
      HPEN darkerTextPen = nullptr;
      HPEN edgePen = nullptr;

      Pens(const Colors& colors)
         : darkerTextPen(::CreatePen(PS_SOLID, 1, colors.darkerText))
         , edgePen(::CreatePen(PS_SOLID, 1, colors.edge))
      {}

      ~Pens()
      {
         ::DeleteObject(darkerTextPen);   darkerTextPen = nullptr;
         ::DeleteObject(edgePen);      edgePen = nullptr;
      }

      void change(const Colors& colors)
      {
         ::DeleteObject(darkerTextPen);
         ::DeleteObject(edgePen);

         darkerTextPen = ::CreatePen(PS_SOLID, 1, colors.darkerText);
         edgePen = ::CreatePen(PS_SOLID, 1, colors.edge);
      }

   };

   // black (default)
   static const Colors darkColors{
      HEXRGB(0x202020), // background
      HEXRGB(0x404040), // softerBackground
      HEXRGB(0x404040), // hotBackground
      HEXRGB(0x202020), // pureBackground
      HEXRGB(0xB00000), // errorBackground
      HEXRGB(0xE0E0E0), // textColor
      HEXRGB(0xC0C0C0), // darkerTextColor
      HEXRGB(0x808080), // disabledTextColor
      HEXRGB(0xFFFF00), // linkTextColor
      HEXRGB(0x646464)  // edgeColor
   };

   struct Theme
   {
      Colors _colors;
      Brushes _brushes;
      Pens _pens;

      Theme(const Colors& colors)
         : _colors(colors)
         , _brushes(colors)
         , _pens(colors)
      {}

      void change(const Colors& colors)
      {
         _colors = colors;
         _brushes.change(colors);
         _pens.change(colors);
      }
   };

   Theme tCurrent(darkColors);

   static boolean _isDarkModeEnabled;

   void queryNPPDarkmode()
   {
      _isDarkModeEnabled = static_cast<boolean>(nppMessage(NPPM_ISDARKMODEENABLED, 0, 0));

      Colors customColors{};
      nppMessage(NPPM_GETDARKMODECOLORS, sizeof(customColors), reinterpret_cast<LPARAM>(&customColors));
      tCurrent.change(customColors);
   }

   void initDarkMode()
   {
      queryNPPDarkmode();

      ::InitDarkMode();
      ::SetDarkMode(_isDarkModeEnabled, true);
   }

   bool isEnabled()
   {
      return _isDarkModeEnabled;
   }

   COLORREF invertLightness(COLORREF c)
   {
      WORD h = 0;
      WORD s = 0;
      WORD l = 0;
      ColorRGBToHLS(c, &h, &l, &s);

      l = 240 - l;

      COLORREF invert_c = ColorHLSToRGB(h, l, s);

      return invert_c;
   }

   COLORREF invertLightnessSofter(COLORREF c)
   {
      WORD h = 0;
      WORD s = 0;
      WORD l = 0;
      ColorRGBToHLS(c, &h, &l, &s);

      l = min(240 - l, 211);

      COLORREF invert_c = ColorHLSToRGB(h, l, s);

      return invert_c;
   }

   TreeViewStyle treeViewStyle = TreeViewStyle::classic;

   COLORREF treeViewBg = Utils::intToRGB(static_cast<int>(nppMessage(NPPM_GETEDITORDEFAULTBACKGROUNDCOLOR, NULL, NULL)));
   double lighnessTreeView = 50.0;

   // adapted from https://stackoverflow.com/a/56678483
   double calculatePerceivedLighness(COLORREF c)
   {
      auto linearValue = [](double colorChannel) -> double
      {
         colorChannel /= 255.0;
         if (colorChannel <= 0.04045)
            return colorChannel / 12.92;
         return std::pow(((colorChannel + 0.055) / 1.055), 2.4);
      };

      double r = linearValue(static_cast<double>(GetRValue(c)));
      double g = linearValue(static_cast<double>(GetGValue(c)));
      double b = linearValue(static_cast<double>(GetBValue(c)));

      double luminance = 0.2126 * r + 0.7152 * g + 0.0722 * b;

      double lighness = (luminance <= 216.0 / 24389.0) ? (luminance * 24389.0 / 27.0) : (std::pow(luminance, (1.0 / 3.0)) * 116.0 - 16.0);
      return lighness;
   }

   COLORREF getBackgroundColor()         { return tCurrent._colors.background; }
   COLORREF getSofterBackgroundColor()   { return tCurrent._colors.softerBackground; }
   COLORREF getHotBackgroundColor()      { return tCurrent._colors.hotBackground; }
   COLORREF getDarkerBackgroundColor()   { return tCurrent._colors.pureBackground; }
   COLORREF getErrorBackgroundColor()    { return tCurrent._colors.errorBackground; }
   COLORREF getTextColor()               { return tCurrent._colors.text; }
   COLORREF getDarkerTextColor()         { return tCurrent._colors.darkerText; }
   COLORREF getDisabledTextColor()       { return tCurrent._colors.disabledText; }
   COLORREF getLinkTextColor()           { return tCurrent._colors.linkText; }
   COLORREF getEdgeColor()               { return tCurrent._colors.edge; }

   HBRUSH getBackgroundBrush()           { return tCurrent._brushes.background; }
   HBRUSH getSofterBackgroundBrush()     { return tCurrent._brushes.softerBackground; }
   HBRUSH getHotBackgroundBrush()        { return tCurrent._brushes.hotBackground; }
   HBRUSH getDarkerBackgroundBrush()     { return tCurrent._brushes.pureBackground; }
   HBRUSH getErrorBackgroundBrush()      { return tCurrent._brushes.errorBackground; }

   HPEN getDarkerTextPen()               { return tCurrent._pens.darkerTextPen; }
   HPEN getEdgePen()                     { return tCurrent._pens.edgePen; }

   // from DarkMode.h
   void allowDarkModeForApp(bool allow)
   {
      ::AllowDarkModeForApp(allow);
   }

   bool allowDarkModeForWindow(HWND hWnd, bool allow)
   {
      return ::AllowDarkModeForWindow(hWnd, allow);
   }

   void setTitleBarThemeColor(HWND hWnd)
   {
      ::RefreshTitleBarThemeColor(hWnd);
   }

   void enableDarkScrollBarForWindowAndChildren(HWND hwnd)
   {
      ::EnableDarkScrollBarForWindowAndChildren(hwnd);
   }

   struct ButtonData
   {
      HTHEME hTheme = nullptr;
      int iStateID = 0;

      ~ButtonData()
      {
         closeTheme();
      }

      bool ensureTheme(HWND hwnd)
      {
         if (!hTheme)
         {
            hTheme = OpenThemeData(hwnd, L"Button");
         }
         return hTheme != nullptr;
      }

      void closeTheme()
      {
         if (hTheme)
         {
            CloseThemeData(hTheme);
            hTheme = nullptr;
         }
      }
   };

   void renderButton(HWND hwnd, HDC hdc, HTHEME hTheme, int iPartID, int iStateID)
   {
      RECT rcClient = { 0 };
      WCHAR szText[256] = { 0 };
      DWORD nState = static_cast<DWORD>(SendMessage(hwnd, BM_GETSTATE, 0, 0));
      DWORD uiState = static_cast<DWORD>(SendMessage(hwnd, WM_QUERYUISTATE, 0, 0));
      DWORD nStyle = GetWindowLong(hwnd, GWL_STYLE);

      HFONT hFont = nullptr;
      HFONT hOldFont = nullptr;
      HFONT hCreatedFont = nullptr;
      LOGFONT lf = { 0 };
      if (SUCCEEDED(GetThemeFont(hTheme, hdc, iPartID, iStateID, TMT_FONT, &lf)))
      {
         hCreatedFont = CreateFontIndirect(&lf);
         hFont = hCreatedFont;
      }

      if (!hFont) {
         hFont = reinterpret_cast<HFONT>(SendMessage(hwnd, WM_GETFONT, 0, 0));
      }

      hOldFont = static_cast<HFONT>(SelectObject(hdc, hFont));

      DWORD dtFlags = DT_LEFT; // DT_LEFT is 0
      dtFlags |= (nStyle & BS_MULTILINE) ? DT_WORDBREAK : DT_SINGLELINE;
      dtFlags |= ((nStyle & BS_CENTER) == BS_CENTER) ? DT_CENTER : (nStyle & BS_RIGHT) ? DT_RIGHT : 0;
      dtFlags |= ((nStyle & BS_VCENTER) == BS_VCENTER) ? DT_VCENTER : (nStyle & BS_BOTTOM) ? DT_BOTTOM : 0;
      dtFlags |= (uiState & UISF_HIDEACCEL) ? DT_HIDEPREFIX : 0;

      if (!(nStyle & BS_MULTILINE) && !(nStyle & BS_BOTTOM) && !(nStyle & BS_TOP))
      {
         dtFlags |= DT_VCENTER;
      }

      GetClientRect(hwnd, &rcClient);
      GetWindowText(hwnd, szText, _countof(szText));

      SIZE szBox = { 13, 13 };
      GetThemePartSize(hTheme, hdc, iPartID, iStateID, NULL, TS_DRAW, &szBox);

      RECT rcText = rcClient;
      GetThemeBackgroundContentRect(hTheme, hdc, iPartID, iStateID, &rcClient, &rcText);

      RECT rcBackground = rcClient;
      if (dtFlags & DT_SINGLELINE)
      {
         rcBackground.top += (rcText.bottom - rcText.top - szBox.cy) / 2;
      }
      rcBackground.bottom = rcBackground.top + szBox.cy;
      rcBackground.right = rcBackground.left + szBox.cx;
      rcText.left = rcBackground.right + 3;

      DrawThemeParentBackground(hwnd, hdc, &rcClient);
      DrawThemeBackground(hTheme, hdc, iPartID, iStateID, &rcBackground, nullptr);

      DTTOPTS dtto = { sizeof(DTTOPTS), DTT_TEXTCOLOR };
      dtto.crText = getTextColor();

      if (nStyle & WS_DISABLED)
      {
         dtto.crText = getDisabledTextColor();
      }

      DrawThemeTextEx(hTheme, hdc, iPartID, iStateID, szText, -1, dtFlags, &rcText, &dtto);

      if ((nState & BST_FOCUS) && !(uiState & UISF_HIDEFOCUS))
      {
         RECT rcTextOut = rcText;
         dtto.dwFlags |= DTT_CALCRECT;
         DrawThemeTextEx(hTheme, hdc, iPartID, iStateID, szText, -1, dtFlags | DT_CALCRECT, &rcTextOut, &dtto);
         RECT rcFocus = rcTextOut;
         rcFocus.bottom++;
         rcFocus.left--;
         rcFocus.right++;
         DrawFocusRect(hdc, &rcFocus);
      }

      if (hCreatedFont) DeleteObject(hCreatedFont);
      SelectObject(hdc, hOldFont);
   }

   void paintButton(HWND hwnd, HDC hdc, ButtonData& buttonData)
   {
      DWORD nState = static_cast<DWORD>(SendMessage(hwnd, BM_GETSTATE, 0, 0));
      DWORD nStyle = GetWindowLong(hwnd, GWL_STYLE);
      DWORD nButtonStyle = nStyle & 0xF;

      int iPartID = BP_CHECKBOX;
      if (nButtonStyle == BS_CHECKBOX || nButtonStyle == BS_AUTOCHECKBOX || nButtonStyle == BS_AUTO3STATE)
      {
         iPartID = BP_CHECKBOX;
      }
      else if (nButtonStyle == BS_RADIOBUTTON || nButtonStyle == BS_AUTORADIOBUTTON)
      {
         iPartID = BP_RADIOBUTTON;
      }
      else
      {
         assert(false);
      }

      // states of BP_CHECKBOX and BP_RADIOBUTTON are the same
      int iStateID = RBS_UNCHECKEDNORMAL;

      if (nStyle & WS_DISABLED)                 iStateID = RBS_UNCHECKEDDISABLED;
      else if (nState & BST_PUSHED)             iStateID = RBS_UNCHECKEDPRESSED;
      else if (nState & BST_HOT)                iStateID = RBS_UNCHECKEDHOT;

      if (nState & BST_CHECKED)                 iStateID += 4;
      else if (nState & BST_INDETERMINATE)      iStateID += 8;

      if (BufferedPaintRenderAnimation(hwnd, hdc))
      {
         return;
      }

      BP_ANIMATIONPARAMS animParams = { sizeof(animParams) };
      animParams.style = BPAS_LINEAR;
      if (iStateID != buttonData.iStateID)
      {
         GetThemeTransitionDuration(buttonData.hTheme, iPartID, buttonData.iStateID, iStateID, TMT_TRANSITIONDURATIONS, &animParams.dwDuration);
      }

      RECT rcClient = { 0 };
      GetClientRect(hwnd, &rcClient);

      HDC hdcFrom = nullptr;
      HDC hdcTo = nullptr;
      HANIMATIONBUFFER hbpAnimation = BeginBufferedAnimation(hwnd, hdc, &rcClient, BPBF_COMPATIBLEBITMAP, nullptr, &animParams, &hdcFrom, &hdcTo);
      if (hbpAnimation)
      {
         if (hdcFrom)
         {
            renderButton(hwnd, hdcFrom, buttonData.hTheme, iPartID, buttonData.iStateID);
         }
         if (hdcTo)
         {
            renderButton(hwnd, hdcTo, buttonData.hTheme, iPartID, iStateID);
         }

         buttonData.iStateID = iStateID;

         EndBufferedAnimation(hbpAnimation, TRUE);
      }
      else
      {
         renderButton(hwnd, hdc, buttonData.hTheme, iPartID, iStateID);

         buttonData.iStateID = iStateID;
      }
   }

   constexpr UINT_PTR g_buttonSubclassID = 42;

   LRESULT CALLBACK ButtonSubclass(
      HWND hWnd,
      UINT uMsg,
      WPARAM wParam,
      LPARAM lParam,
      UINT_PTR uIdSubclass,
      DWORD_PTR dwRefData
   )
   {
      UNREFERENCED_PARAMETER(uIdSubclass);

      auto pButtonData = reinterpret_cast<ButtonData*>(dwRefData);

      switch (uMsg)
      {
         case WM_UPDATEUISTATE:
            if (HIWORD(wParam) & (UISF_HIDEACCEL | UISF_HIDEFOCUS))
            {
               InvalidateRect(hWnd, nullptr, FALSE);
            }
            break;
         case WM_NCDESTROY:
            RemoveWindowSubclass(hWnd, ButtonSubclass, g_buttonSubclassID);
            delete pButtonData;
            break;
         case WM_ERASEBKGND:
            if (isEnabled() && pButtonData->ensureTheme(hWnd))
            {
               return TRUE;
            }
            else
            {
               break;
            }
         case WM_THEMECHANGED:
            pButtonData->closeTheme();
            break;
         case WM_PRINTCLIENT:
         case WM_PAINT:
            if (isEnabled() && pButtonData->ensureTheme(hWnd))
            {
               PAINTSTRUCT ps = { 0 };
               HDC hdc = reinterpret_cast<HDC>(wParam);
               if (!hdc)
               {
                  hdc = BeginPaint(hWnd, &ps);
               }

               paintButton(hWnd, hdc, *pButtonData);

               if (ps.hdc)
               {
                  EndPaint(hWnd, &ps);
               }

               return 0;
            }
            else
            {
               break;
            }
         case WM_SIZE:
         case WM_DESTROY:
            BufferedPaintStopAllAnimations(hWnd);
            break;
         case WM_ENABLE:
            if (isEnabled())
            {
               // skip the button's normal wndproc so it won't redraw out of wm_paint
               LRESULT lr = DefWindowProc(hWnd, uMsg, wParam, lParam);
               InvalidateRect(hWnd, nullptr, FALSE);
               return lr;
            }
            break;
      }
      return DefSubclassProc(hWnd, uMsg, wParam, lParam);
   }

   void subclassButtonControl(HWND hwnd)
   {
      DWORD_PTR pButtonData = reinterpret_cast<DWORD_PTR>(new ButtonData());
      SetWindowSubclass(hwnd, ButtonSubclass, g_buttonSubclassID, pButtonData);
   }

   void paintGroupbox(HWND hwnd, HDC hdc, ButtonData& buttonData)
   {
      DWORD nStyle = GetWindowLong(hwnd, GWL_STYLE);
      int iPartID = BP_GROUPBOX;
      int iStateID = GBS_NORMAL;

      if (nStyle & WS_DISABLED)
      {
         iStateID = GBS_DISABLED;
      }

      RECT rcClient = { 0 };
      GetClientRect(hwnd, &rcClient);

      RECT rcText = rcClient;
      RECT rcBackground = rcClient;

      HFONT hFont = nullptr;
      HFONT hOldFont = nullptr;
      HFONT hCreatedFont = nullptr;
      LOGFONT lf = { 0 };
      if (SUCCEEDED(GetThemeFont(buttonData.hTheme, hdc, iPartID, iStateID, TMT_FONT, &lf)))
      {
         hCreatedFont = CreateFontIndirect(&lf);
         hFont = hCreatedFont;
      }

      if (!hFont)
      {
         hFont = reinterpret_cast<HFONT>(SendMessage(hwnd, WM_GETFONT, 0, 0));
      }

      hOldFont = static_cast<HFONT>(::SelectObject(hdc, hFont));

      WCHAR szText[256] = { 0 };
      GetWindowText(hwnd, szText, _countof(szText));

      auto style = static_cast<long>(::GetWindowLongPtr(hwnd, GWL_STYLE));
      bool isCenter = (style & BS_CENTER) == BS_CENTER;

      if (szText[0])
      {
         SIZE textSize = { 0 };
         GetTextExtentPoint32(hdc, szText, static_cast<int>(wcslen(szText)), &textSize);

         int centerPosX = isCenter ? ((rcClient.right - rcClient.left - textSize.cx) / 2) : 7;

         rcBackground.top += textSize.cy / 2;
         rcText.left += centerPosX;
         rcText.bottom = rcText.top + textSize.cy;
         rcText.right = rcText.left + textSize.cx + 4;

         ExcludeClipRect(hdc, rcText.left, rcText.top, rcText.right, rcText.bottom);
      }
      else
      {
         SIZE textSize = { 0 };
         GetTextExtentPoint32(hdc, L"M", 1, &textSize);
         rcBackground.top += textSize.cy / 2;
      }

      RECT rcContent = rcBackground;
      GetThemeBackgroundContentRect(buttonData.hTheme, hdc, BP_GROUPBOX, iStateID, &rcBackground, &rcContent);
      ExcludeClipRect(hdc, rcContent.left, rcContent.top, rcContent.right, rcContent.bottom);

      //DrawThemeParentBackground(hwnd, hdc, &rcClient);
      DrawThemeBackground(buttonData.hTheme, hdc, BP_GROUPBOX, iStateID, &rcBackground, nullptr);

      SelectClipRgn(hdc, nullptr);

      if (szText[0])
      {
         rcText.right -= 2;
         rcText.left += 2;

         DTTOPTS dtto = { sizeof(DTTOPTS), DTT_TEXTCOLOR };
         dtto.crText = getTextColor();

         DWORD textFlags = isCenter ? DT_CENTER : DT_LEFT;

         DrawThemeTextEx(buttonData.hTheme, hdc, BP_GROUPBOX, iStateID, szText, -1, textFlags | DT_SINGLELINE, &rcText, &dtto);
      }

      if (hCreatedFont) DeleteObject(hCreatedFont);
      SelectObject(hdc, hOldFont);
   }

   constexpr UINT_PTR g_groupboxSubclassID = 42;

   LRESULT CALLBACK GroupboxSubclass(
      HWND hWnd,
      UINT uMsg,
      WPARAM wParam,
      LPARAM lParam,
      UINT_PTR uIdSubclass,
      DWORD_PTR dwRefData
   )
   {
      UNREFERENCED_PARAMETER(uIdSubclass);

      auto pButtonData = reinterpret_cast<ButtonData*>(dwRefData);

      switch (uMsg)
      {
      case WM_NCDESTROY:
         RemoveWindowSubclass(hWnd, GroupboxSubclass, g_groupboxSubclassID);
         delete pButtonData;
         break;
      case WM_ERASEBKGND:
         if (isEnabled() && pButtonData->ensureTheme(hWnd))
         {
            return TRUE;
         }
         else
         {
            break;
         }
      case WM_THEMECHANGED:
         pButtonData->closeTheme();
         break;
      case WM_PRINTCLIENT:
      case WM_PAINT:
         if (isEnabled() && pButtonData->ensureTheme(hWnd))
         {
            PAINTSTRUCT ps = { 0 };
            HDC hdc = reinterpret_cast<HDC>(wParam);
            if (!hdc)
            {
               hdc = BeginPaint(hWnd, &ps);
            }

            paintGroupbox(hWnd, hdc, *pButtonData);

            if (ps.hdc)
            {
               EndPaint(hWnd, &ps);
            }

            return 0;
         }
         else
         {
            break;
         }
         break;
      }
      return DefSubclassProc(hWnd, uMsg, wParam, lParam);
   }

   void subclassGroupboxControl(HWND hwnd)
   {
      DWORD_PTR pButtonData = reinterpret_cast<DWORD_PTR>(new ButtonData());
      SetWindowSubclass(hwnd, GroupboxSubclass, g_groupboxSubclassID, pButtonData);
   }

   constexpr UINT_PTR g_tabSubclassID = 42;

   LRESULT CALLBACK TabSubclass(
      HWND hWnd,
      UINT uMsg,
      WPARAM wParam,
      LPARAM lParam,
      UINT_PTR uIdSubclass,
      DWORD_PTR dwRefData
   )
   {
      UNREFERENCED_PARAMETER(uIdSubclass);
      UNREFERENCED_PARAMETER(dwRefData);

      switch (uMsg)
      {
      case WM_PAINT:
      {
         if (!isEnabled())
         {
            break;
         }

         LONG_PTR dwStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
         if ((dwStyle & TCS_BOTTOM) || (dwStyle & TCS_BUTTONS) || (dwStyle & TCS_VERTICAL))
         {
            break;
         }

         PAINTSTRUCT ps;
         HDC hdc = ::BeginPaint(hWnd, &ps);
         ::FillRect(hdc, &ps.rcPaint, getDarkerBackgroundBrush());

         auto holdPen = static_cast<HPEN>(::SelectObject(hdc, getEdgePen()));

         HRGN holdClip = CreateRectRgn(0, 0, 0, 0);
         if (1 != GetClipRgn(hdc, holdClip))
         {
            DeleteObject(holdClip);
            holdClip = nullptr;
         }

         HFONT hFont = reinterpret_cast<HFONT>(SendMessage(hWnd, WM_GETFONT, 0, 0));
         auto hOldFont = SelectObject(hdc, hFont);

         POINT ptCursor = { 0 };
         ::GetCursorPos(&ptCursor);
         ScreenToClient(hWnd, &ptCursor);

         int nTabs = TabCtrl_GetItemCount(hWnd);

         int nSelTab = TabCtrl_GetCurSel(hWnd);
         for (int i = 0; i < nTabs; ++i)
         {
            RECT rcItem = { 0 };
            TabCtrl_GetItemRect(hWnd, i, &rcItem);

            RECT rcIntersect = { 0 };
            if (IntersectRect(&rcIntersect, &ps.rcPaint, &rcItem))
            {
               bool bHot = PtInRect(&rcItem, ptCursor);

               POINT edges[] = {
                  {rcItem.right - 1, rcItem.top},
                  {rcItem.right - 1, rcItem.bottom}
               };
               Polyline(hdc, edges, _countof(edges));
               rcItem.right -= 1;

               HRGN hClip = CreateRectRgnIndirect(&rcItem);

               SelectClipRgn(hdc, hClip);

               SetTextColor(hdc, (bHot || (i == nSelTab)) ? getTextColor() : getDarkerTextColor());

               // for consistency getBackgroundBrush()
               // would be better, than getSofterBackgroundBrush(),
               // however default getBackgroundBrush() has same color
               // as getDarkerBackgroundBrush()
               ::FillRect(hdc, &rcItem, (i == nSelTab) ? getDarkerBackgroundBrush() : getSofterBackgroundBrush());

               SetBkMode(hdc, TRANSPARENT);

               TCHAR label[MAX_PATH];
               TCITEM tci = { 0 };
               tci.mask = TCIF_TEXT;
               tci.pszText = label;
               tci.cchTextMax = MAX_PATH - 1;

               ::SendMessage(hWnd, TCM_GETITEM, i, reinterpret_cast<LPARAM>(&tci));

               RECT rcText = rcItem;
               rcText.left += Utils::scaleDPIX(6);
               rcText.right -= Utils::scaleDPIX(3);

               if (i == nSelTab)
               {
                  rcText.bottom -= Utils::scaleDPIY(4);
               }

               DrawText(hdc, label, -1, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

               DeleteObject(hClip);

               SelectClipRgn(hdc, holdClip);
            }
         }

         SelectObject(hdc, hOldFont);

         SelectClipRgn(hdc, holdClip);
         if (holdClip)
         {
            DeleteObject(holdClip);
            holdClip = nullptr;
         }

         SelectObject(hdc, holdPen);

         EndPaint(hWnd, &ps);
         return 0;
      }
      case WM_NCDESTROY:
         RemoveWindowSubclass(hWnd, TabSubclass, g_tabSubclassID);
         break;
      }
      return DefSubclassProc(hWnd, uMsg, wParam, lParam);
   }

   void subclassTabControl(HWND hwnd)
   {
      SetWindowSubclass(hwnd, TabSubclass, g_tabSubclassID, 0);
   }

   constexpr UINT_PTR g_comboBoxSubclassID = 42;

   LRESULT CALLBACK ComboBoxSubclass(
      HWND hWnd,
      UINT uMsg,
      WPARAM wParam,
      LPARAM lParam,
      UINT_PTR uIdSubclass,
      DWORD_PTR /*dwRefData*/
   )
   {
      switch (uMsg)
      {
         case WM_PAINT:
         {
            if (!isEnabled())
            {
               break;
            }

            RECT rc = { 0 };
            ::GetClientRect(hWnd, &rc);

            PAINTSTRUCT ps;
            auto hdc = ::BeginPaint(hWnd, &ps);

            auto holdPen = static_cast<HPEN>(::SelectObject(hdc, getEdgePen()));
            ::SelectObject(hdc, reinterpret_cast<HFONT>(::SendMessage(hWnd, WM_GETFONT, 0, 0)));
            ::SetBkColor(hdc, getBackgroundColor());

            ::SelectObject(hdc, ::GetStockObject(NULL_BRUSH)); // to avoid text flicker, use only border
            ::Rectangle(hdc, 0, 0, rc.right, rc.bottom);

            auto holdBrush = ::SelectObject(hdc, getDarkerBackgroundBrush());

            RECT arrowRc = {rc.right - Utils::scaleDPIX(17), rc.top + 1, rc.right - 1, rc.bottom - 1};

            // CBS_DROPDOWN text is handled by parent by WM_CTLCOLOREDIT
            auto style = ::GetWindowLongPtr(hWnd, GWL_STYLE);
            if ((style & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
            {
               RECT bkRc = rc;
               bkRc.left += 1;
               bkRc.top += 1;
               bkRc.right = arrowRc.left - 1;
               bkRc.bottom -= 1;
               ::FillRect(hdc, &bkRc, getSofterBackgroundBrush()); // erase background on item change

               auto index = static_cast<int>(::SendMessage(hWnd, CB_GETCURSEL, 0, 0));
               if (index != CB_ERR)
               {
                  ::SetTextColor(hdc, getTextColor());
                  ::SetBkColor(hdc, getSofterBackgroundColor());
                  auto bufferLen = static_cast<size_t>(::SendMessage(hWnd, CB_GETLBTEXTLEN, index, 0));
                  TCHAR* buffer = new TCHAR[(bufferLen + 1)];
                  ::SendMessage(hWnd, CB_GETLBTEXT, index, reinterpret_cast<LPARAM>(buffer));

                  RECT textRc = rc;
                  textRc.left += 4;
                  textRc.right = arrowRc.left - 5;

                  ::DrawText(hdc, buffer, -1, &textRc, DT_NOPREFIX | DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                  delete[]buffer;
               }
            }

            POINT ptCursor = { 0 };
            ::GetCursorPos(&ptCursor);
            ScreenToClient(hWnd, &ptCursor);

            bool isHot = PtInRect(&rc, ptCursor);

            ::SetTextColor(hdc, !isHot ? getTextColor() : getDarkerTextColor());
            ::SetBkColor(hdc, !isHot ? getHotBackgroundColor() : getBackgroundColor());
            ::ExtTextOut(hdc,
               arrowRc.left + (arrowRc.right - arrowRc.left) / 2 - Utils::scaleDPIX(4),
               arrowRc.top + 3,
               ETO_OPAQUE | ETO_CLIPPED,
               &arrowRc, L"˅", // Choose from: [V v ˅ ▼ ▾ ▽ ▿]
               1,
               nullptr);
            ::SetBkColor(hdc, getBackgroundColor());

            POINT edge[] = {
               {arrowRc.left - 1, arrowRc.top},
               {arrowRc.left - 1, arrowRc.bottom}
            };
            ::Polyline(hdc, edge, _countof(edge));

            ::SelectObject(hdc, holdPen);
            ::SelectObject(hdc, holdBrush);

            ::EndPaint(hWnd, &ps);
            return 0;
         }

         case WM_NCDESTROY:
         {
            ::RemoveWindowSubclass(hWnd, ComboBoxSubclass, uIdSubclass);
            break;
         }
      }
      return DefSubclassProc(hWnd, uMsg, wParam, lParam);
   }

   void subclassComboBoxControl(HWND hwnd)
   {
      SetWindowSubclass(hwnd, ComboBoxSubclass, g_comboBoxSubclassID, 0);
   }

   void autoSubclassAndThemeChildControls(HWND hwndParent, bool subclass, bool theme)
   {
      struct Params
      {
         const wchar_t* themeClassName = nullptr;
         bool subclass = false;
         bool theme = false;
      };

      Params p{
         isEnabled() ? L"DarkMode_Explorer" : nullptr
         , subclass
         , theme
      };

      ::EnableThemeDialogTexture(hwndParent, theme && !isEnabled() ? ETDT_ENABLETAB : ETDT_DISABLE);

      EnumChildWindows(hwndParent, [](HWND hwnd, LPARAM lParam) WINAPI_LAMBDA{
         auto& p = *reinterpret_cast<Params*>(lParam);
         const size_t classNameLen = 16;
         TCHAR className[classNameLen] = { 0 };
         GetClassName(hwnd, className, classNameLen);

         if (wcscmp(className, WC_COMBOBOX) == 0)
         {
            auto style = ::GetWindowLongPtr(hwnd, GWL_STYLE);

            if ((style & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST || (style & CBS_DROPDOWN) == CBS_DROPDOWN)
            {
               COMBOBOXINFO cbi = {};
               cbi.cbSize = sizeof(COMBOBOXINFO);
               BOOL result = GetComboBoxInfo(hwnd, &cbi);
               if (result == TRUE)
               {
                  if (p.theme && cbi.hwndList)
                  {
                     //dark scrollbar for listbox of combobox
                     SetWindowTheme(cbi.hwndList, p.themeClassName, nullptr);
                  }
               }

               subclassComboBoxControl(hwnd);
            }
            return TRUE;
         }

         if (wcscmp(className, WC_LISTBOX) == 0)
         {
            if (p.theme)
            {
               //dark scrollbar for listbox
               SetWindowTheme(hwnd, p.themeClassName, nullptr);
            }

            return TRUE;
         }

         if (wcscmp(className, WC_EDIT) == 0)
         {
            auto style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
            bool hasScrollBar = ((style & WS_HSCROLL) == WS_HSCROLL) || ((style & WS_VSCROLL) == WS_VSCROLL);
            if (p.theme && hasScrollBar)
            {
               //dark scrollbar for edit control
               SetWindowTheme(hwnd, p.themeClassName, nullptr);
            }

            return TRUE;
         }

         if (wcscmp(className, WC_BUTTON) == 0)
         {
            auto nButtonStyle = ::GetWindowLongPtr(hwnd, GWL_STYLE) & 0xF;
            switch (nButtonStyle)
            {
               case BS_CHECKBOX:
               case BS_AUTOCHECKBOX:
               case BS_AUTO3STATE:
               case BS_RADIOBUTTON:
               case BS_AUTORADIOBUTTON:
                  if (p.subclass)
                  {
                     subclassButtonControl(hwnd);
                  }
                  break;
               case BS_GROUPBOX:
                  if (p.subclass)
                  {
                     subclassGroupboxControl(hwnd);
                  }
                  break;
               case BS_DEFPUSHBUTTON:
               case BS_PUSHBUTTON:
                  if (p.theme)
                  {
                     SetWindowTheme(hwnd, p.themeClassName, nullptr);
                  }
                  break;
            }
            return TRUE;
         }
         return TRUE;
      }, reinterpret_cast<LPARAM>(&p));
   }

   void autoThemeChildControls(HWND hwndParent)
   {
      autoSubclassAndThemeChildControls(hwndParent, false, true);
   }

   void setDarkTitleBar(HWND hwnd)
   {
      allowDarkModeForWindow(hwnd, isEnabled());
      setTitleBarThemeColor(hwnd);
   }

   void setDarkExplorerTheme(HWND hwnd)
   {
      SetWindowTheme(hwnd, isEnabled() ? L"DarkMode_Explorer" : nullptr, nullptr);
   }

   void setDarkScrollBar(HWND hwnd)
   {
      setDarkExplorerTheme(hwnd);
   }

   void setDarkTooltips(HWND hwnd, ToolTipsType type)
   {
      UINT msg = 0;
      switch (type)
      {
         case ToolTipsType::toolbar:
            msg = TB_GETTOOLTIPS;
            break;
         case ToolTipsType::listview:
            msg = LVM_GETTOOLTIPS;
            break;
         case ToolTipsType::treeview:
            msg = TVM_GETTOOLTIPS;
            break;
         case ToolTipsType::tabbar:
            msg = TCM_GETTOOLTIPS;
            break;
         default:
            msg = 0;
            break;
      }

      if (msg == 0)
      {
         setDarkExplorerTheme(hwnd);
      }
      else
      {
         auto hTips = reinterpret_cast<HWND>(::SendMessage(hwnd, msg, 0, 0));
         if (hTips != nullptr)
         {
            setDarkExplorerTheme(hTips);
         }
      }
   }

   void setDarkLineAbovePanelToolbar(HWND hwnd)
   {
      COLORSCHEME scheme;
      scheme.dwSize = sizeof(COLORSCHEME);

      if (isEnabled())
      {
         scheme.clrBtnHighlight = getDarkerBackgroundColor();
         scheme.clrBtnShadow = getDarkerBackgroundColor();
      }
      else
      {
         scheme.clrBtnHighlight = CLR_DEFAULT;
         scheme.clrBtnShadow = CLR_DEFAULT;
      }

      ::SendMessage(hwnd, TB_SETCOLORSCHEME, 0, reinterpret_cast<LPARAM>(&scheme));
   }

   void setDarkListView(HWND hwnd)
   {
      bool useDark = isEnabled();

      HWND hHeader = ListView_GetHeader(hwnd);
      allowDarkModeForWindow(hHeader, useDark);
      SetWindowTheme(hHeader, useDark ? L"ItemsView" : nullptr, nullptr);

      allowDarkModeForWindow(hwnd, useDark);
      SetWindowTheme(hwnd, L"Explorer", nullptr);
   }

   void setTreeViewStyle(HWND hwnd)
   {
      auto style = static_cast<long>(::GetWindowLongPtr(hwnd, GWL_STYLE));
      bool hasHotStyle = (style & TVS_TRACKSELECT) == TVS_TRACKSELECT;
      bool change = false;
      switch (treeViewStyle)
      {
         case TreeViewStyle::light:
         {
            if (!hasHotStyle)
            {
               style |= TVS_TRACKSELECT;
               change = true;
            }
            SetWindowTheme(hwnd, L"Explorer", nullptr);
            break;
         }
         case TreeViewStyle::dark:
         {
            if (!hasHotStyle)
            {
               style |= TVS_TRACKSELECT;
               change = true;
            }
            SetWindowTheme(hwnd, L"DarkMode_Explorer", nullptr);
            break;
         }
         default:
         {
            if (hasHotStyle)
            {
               style &= ~TVS_TRACKSELECT;
               change = true;
            }
            SetWindowTheme(hwnd, nullptr, nullptr);
            break;
         }
      }

      if (change)
      {
         ::SetWindowLongPtr(hwnd, GWL_STYLE, style);
      }
   }

   void setBorder(HWND hwnd, bool border)
   {
      auto style = static_cast<long>(::GetWindowLongPtr(hwnd, GWL_STYLE));
      bool hasBorder = (style & WS_BORDER) == WS_BORDER;
      bool change = false;

      if (!hasBorder && border)
      {
         style |= WS_BORDER;
         change = true;
      }
      else if (hasBorder && !border)
      {
         style &= ~WS_BORDER;
         change = true;
      }

      if (change)
      {
         ::SetWindowLongPtr(hwnd, GWL_STYLE, style);
         ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
      }
   }

   void initSysLink(HWND hCtl) {
      LITEM item = { 0 };
      item.iLink = 0;
      item.mask = LIF_ITEMINDEX | LIF_STATE;
      item.state = LIS_DEFAULTCOLORS;
      item.stateMask = LIS_DEFAULTCOLORS;
      SendMessage(hCtl, LM_SETITEM, 0, (LPARAM)&item);
   }

   LRESULT onCtlColor(HDC hdc)
   {
      ::SetTextColor(hdc, getTextColor());
      ::SetBkColor(hdc, getBackgroundColor());
      return reinterpret_cast<LRESULT>(getBackgroundBrush());
   }

   LRESULT onCtlColorSofter(HDC hdc)
   {
      ::SetTextColor(hdc, getTextColor());
      ::SetBkColor(hdc, getSofterBackgroundColor());
      return reinterpret_cast<LRESULT>(getSofterBackgroundBrush());
   }

   LRESULT onCtlColorDarker(HDC hdc)
   {
      ::SetTextColor(hdc, getTextColor());
      ::SetBkColor(hdc, getDarkerBackgroundColor());
      return reinterpret_cast<LRESULT>(getDarkerBackgroundBrush());
   }

   LRESULT onCtlColorError(HDC hdc)
   {
      ::SetTextColor(hdc, getTextColor());
      ::SetBkColor(hdc, getErrorBackgroundColor());
      return reinterpret_cast<LRESULT>(getErrorBackgroundBrush());
   }

   LRESULT onCtlColorSysLink(HDC hdc)
   {
      if (isEnabled()) {
         SetTextColor(hdc, getLinkTextColor());
         SetBkColor(hdc, getBackgroundColor());
         return reinterpret_cast<LRESULT>(getBackgroundBrush());
      }
      else {
         SetTextColor(hdc, GetSysColor(COLOR_HIGHLIGHT));
         return FALSE;
      }
   }

   LRESULT onCtlColorIfEnabled(HDC hdc, bool bEnabled) {
      LRESULT result{ FALSE };

      if (isEnabled()) {
         result = onCtlColorDarker(hdc);
         SetTextColor(hdc, bEnabled ? getTextColor() : getDisabledTextColor());
      }
      else {
         SetTextColor(hdc, GetSysColor(bEnabled ? COLOR_WINDOWTEXT : COLOR_GRAYTEXT));
      }

      return result;
   }

   LRESULT onCtlHiliteIfEnabled(HDC hdc, bool bEnabled) {
      if (isEnabled()) {
         SetTextColor(hdc, bEnabled ? getLinkTextColor() : getDisabledTextColor());
         SetBkColor(hdc, getBackgroundColor());
         return reinterpret_cast<LRESULT>(getBackgroundBrush());
      }
      else {
         SetTextColor(hdc, GetSysColor(bEnabled ? COLOR_HIGHLIGHT : COLOR_GRAYTEXT));
         return FALSE;
      }
   }
}
