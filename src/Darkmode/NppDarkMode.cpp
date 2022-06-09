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

extern HWND nppHandle;

namespace NppDarkMode
{
   struct Brushes
   {
      HBRUSH background = nullptr;
      HBRUSH softerBackground = nullptr;
      HBRUSH hotBackground = nullptr;
      HBRUSH pureBackground = nullptr;
      HBRUSH errorBackground = nullptr;

      HBRUSH edgeBrush = nullptr;
      HBRUSH hotEdgeBrush = nullptr;
      HBRUSH disabledEdgeBrush = nullptr;

      Brushes(const Colors& colors)
         : background(::CreateSolidBrush(colors.background))
         , softerBackground(::CreateSolidBrush(colors.softerBackground))
         , hotBackground(::CreateSolidBrush(colors.hotBackground))
         , pureBackground(::CreateSolidBrush(colors.pureBackground))
         , errorBackground(::CreateSolidBrush(colors.errorBackground))

         , edgeBrush(::CreateSolidBrush(colors.edge))
         , hotEdgeBrush(::CreateSolidBrush(colors.hotEdge))
         , disabledEdgeBrush(::CreateSolidBrush(colors.disabledEdge))
      {}

      ~Brushes()
      {
         ::DeleteObject(background);         background = nullptr;
         ::DeleteObject(softerBackground);   softerBackground = nullptr;
         ::DeleteObject(hotBackground);      hotBackground = nullptr;
         ::DeleteObject(pureBackground);     pureBackground = nullptr;
         ::DeleteObject(errorBackground);    errorBackground = nullptr;

         ::DeleteObject(edgeBrush);          edgeBrush = nullptr;
         ::DeleteObject(hotEdgeBrush);       hotEdgeBrush = nullptr;
         ::DeleteObject(disabledEdgeBrush);	disabledEdgeBrush = nullptr;
      }

      void change(const Colors& colors)
      {
         ::DeleteObject(background);
         ::DeleteObject(softerBackground);
         ::DeleteObject(hotBackground);
         ::DeleteObject(pureBackground);
         ::DeleteObject(errorBackground);

         ::DeleteObject(edgeBrush);
         ::DeleteObject(hotEdgeBrush);
         ::DeleteObject(disabledEdgeBrush);

         background = ::CreateSolidBrush(colors.background);
         softerBackground = ::CreateSolidBrush(colors.softerBackground);
         hotBackground = ::CreateSolidBrush(colors.hotBackground);
         pureBackground = ::CreateSolidBrush(colors.pureBackground);
         errorBackground = ::CreateSolidBrush(colors.errorBackground);

         edgeBrush = ::CreateSolidBrush(colors.edge);
         hotEdgeBrush = ::CreateSolidBrush(colors.hotEdge);
         disabledEdgeBrush = ::CreateSolidBrush(colors.disabledEdge);
      }
   };

   struct Pens
   {
      HPEN darkerTextPen = nullptr;
      HPEN edgePen = nullptr;
      HPEN hotEdgePen = nullptr;
      HPEN disabledEdgePen = nullptr;

      Pens(const Colors& colors)
         : darkerTextPen(::CreatePen(PS_SOLID, 1, colors.darkerText))
         , edgePen(::CreatePen(PS_SOLID, 1, colors.edge))
         , hotEdgePen(::CreatePen(PS_SOLID, 1, colors.hotEdge))
         , disabledEdgePen(::CreatePen(PS_SOLID, 1, colors.disabledEdge))
      {}

      ~Pens()
      {
         ::DeleteObject(darkerTextPen);   darkerTextPen = nullptr;
         ::DeleteObject(edgePen);         edgePen = nullptr;
         ::DeleteObject(hotEdgePen);      hotEdgePen = nullptr;
         ::DeleteObject(disabledEdgePen);	disabledEdgePen = nullptr;
      }

      void change(const Colors& colors)
      {
         ::DeleteObject(darkerTextPen);
         ::DeleteObject(edgePen);
         ::DeleteObject(hotEdgePen);
         ::DeleteObject(disabledEdgePen);

         darkerTextPen = ::CreatePen(PS_SOLID, 1, colors.darkerText);
         edgePen = ::CreatePen(PS_SOLID, 1, colors.edge);
         hotEdgePen = ::CreatePen(PS_SOLID, 1, colors.hotEdge);
         disabledEdgePen = ::CreatePen(PS_SOLID, 1, colors.disabledEdge);
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
      HEXRGB(0x646464),  // edgeColor
      HEXRGB(0x9B9B9B),	// hotEdgeColor
      HEXRGB(0x484848)	// disabledEdgeColor
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

   COLORREF intToRGB(int color) {
      return RGB(GetRValue(color), GetGValue(color), GetBValue(color));
   }

   int scaleDPIX(int x) {
      HDC hdc = GetDC(NULL);
      if (!hdc) return 0;

      int scaleX{ MulDiv(x, GetDeviceCaps(hdc, LOGPIXELSX), 96) };
      ReleaseDC(NULL, hdc);
      return scaleX;
   }

   int scaleDPIY(int y) {
      HDC hdc = GetDC(NULL);
      if (!hdc) return 0;

      int scaleY{ MulDiv(y, GetDeviceCaps(hdc, LOGPIXELSY), 96) };
      ReleaseDC(NULL, hdc);
      return scaleY;
   }

   int nppMessage(UINT messageID, WPARAM wparam, LPARAM lparam) {
      return static_cast<int>(SendMessage(nppHandle, messageID, wparam, lparam));
   }

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

   bool isExperimentalSupported()
   {
      return g_darkModeSupported;
   }

   bool isWindows11() {
      return IsWindows11();
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

   COLORREF treeViewBg = intToRGB(static_cast<int>(nppMessage(NPPM_GETEDITORDEFAULTBACKGROUNDCOLOR, NULL, NULL)));
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
   COLORREF getHotEdgeColor()            { return tCurrent._colors.hotEdge; }
   COLORREF getDisabledEdgeColor()       { return tCurrent._colors.disabledEdge; }

   HBRUSH getBackgroundBrush()           { return tCurrent._brushes.background; }
   HBRUSH getSofterBackgroundBrush()     { return tCurrent._brushes.softerBackground; }
   HBRUSH getHotBackgroundBrush()        { return tCurrent._brushes.hotBackground; }
   HBRUSH getDarkerBackgroundBrush()     { return tCurrent._brushes.pureBackground; }
   HBRUSH getErrorBackgroundBrush()      { return tCurrent._brushes.errorBackground; }

   HBRUSH getEdgeBrush()                 { return tCurrent._brushes.edgeBrush; }
   HBRUSH getHotEdgeBrush()              { return tCurrent._brushes.hotEdgeBrush; }
   HBRUSH getDisabledEdgeBrush()         { return tCurrent._brushes.disabledEdgeBrush; }

   HPEN getDarkerTextPen()               { return tCurrent._pens.darkerTextPen; }
   HPEN getEdgePen()                     { return tCurrent._pens.edgePen; }
   HPEN getHotEdgePen()                  { return tCurrent._pens.hotEdgePen; }
   HPEN getDisabledEdgePen()             { return tCurrent._pens.disabledEdgePen; }

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

   inline void paintRoundFrameRect(HDC hdc, const RECT rect, const HPEN hpen, int width, int height)
   {
      auto holdBrush = ::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
      auto holdPen = ::SelectObject(hdc, hpen);
      ::RoundRect(hdc, rect.left, rect.top, rect.right, rect.bottom, width, height);
      ::SelectObject(hdc, holdBrush);
      ::SelectObject(hdc, holdPen);
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
      const auto nStyle = GetWindowLongPtr(hwnd, GWL_STYLE);
      const auto nButtonStyle = nStyle & BS_TYPEMASK;

      int iPartID = BP_CHECKBOX;

      // Plugin might use BS_3STATE and BS_AUTO3STATE button style
      if (nButtonStyle == BS_CHECKBOX || nButtonStyle == BS_AUTOCHECKBOX || nButtonStyle == BS_3STATE || nButtonStyle == BS_AUTO3STATE)
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
      bool isDisabled = (nStyle & WS_DISABLED) == WS_DISABLED;
      int iPartID = BP_GROUPBOX;
      int iStateID = isDisabled ? GBS_DISABLED : GBS_NORMAL;

      RECT rcClient{};
      GetClientRect(hwnd, &rcClient);

      RECT rcText = rcClient;
      RECT rcBackground = rcClient;

      HFONT hFont = nullptr;
      HFONT hOldFont = nullptr;
      HFONT hCreatedFont = nullptr;
      LOGFONT lf = {};
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

      WCHAR szText[256] = { '\0' };
      GetWindowText(hwnd, szText, _countof(szText));

      auto style = static_cast<long>(::GetWindowLongPtr(hwnd, GWL_STYLE));
      bool isCenter = (style & BS_CENTER) == BS_CENTER;

      if (szText[0])
      {
         SIZE textSize = {};
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
         SIZE textSize{};
         GetTextExtentPoint32(hdc, L"M", 1, &textSize);
         rcBackground.top += textSize.cy / 2;
      }

      RECT rcContent = rcBackground;
      GetThemeBackgroundContentRect(buttonData.hTheme, hdc, BP_GROUPBOX, iStateID, &rcBackground, &rcContent);
      ExcludeClipRect(hdc, rcContent.left, rcContent.top, rcContent.right, rcContent.bottom);

      //DrawThemeParentBackground(hwnd, hdc, &rcClient);
      //DrawThemeBackground(buttonData.hTheme, hdc, BP_GROUPBOX, iStateID, &rcBackground, nullptr);
      paintRoundFrameRect(hdc, rcBackground, getEdgePen());

      SelectClipRgn(hdc, nullptr);

      if (szText[0])
      {
         rcText.right -= 2;
         rcText.left += 2;

         DTTOPTS dtto = { sizeof(DTTOPTS), DTT_TEXTCOLOR };
         dtto.crText = isDisabled ? getDisabledTextColor() : getTextColor();

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
         if ((dwStyle & TCS_BUTTONS) || (dwStyle & TCS_VERTICAL))
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

         POINT ptCursor{};
         ::GetCursorPos(&ptCursor);
         ScreenToClient(hWnd, &ptCursor);

         int nTabs = TabCtrl_GetItemCount(hWnd);

         int nSelTab = TabCtrl_GetCurSel(hWnd);
         for (int i = 0; i < nTabs; ++i)
         {
            RECT rcItem{};
            TabCtrl_GetItemRect(hWnd, i, &rcItem);
            RECT rcFrame = rcItem;

            RECT rcIntersect{};
            if (IntersectRect(&rcIntersect, &ps.rcPaint, &rcItem))
            {
               bool bHot = PtInRect(&rcItem, ptCursor);
               bool isSelectedTab = (i == nSelTab);

               HRGN hClip = CreateRectRgnIndirect(&rcItem);

               SelectClipRgn(hdc, hClip);

               SetTextColor(hdc, (bHot || isSelectedTab) ? getTextColor() : getDarkerTextColor());

               ::InflateRect(&rcItem, -1, -1);
               rcItem.right += 1;

               // for consistency getBackgroundBrush()
               // would be better, than getSofterBackgroundBrush(),
               // however default getBackgroundBrush() has same color
               // as getDarkerBackgroundBrush()
               ::FillRect(hdc, &rcItem, isSelectedTab ? getDarkerBackgroundBrush() : bHot ? getHotBackgroundBrush() : getSofterBackgroundBrush());

               SetBkMode(hdc, TRANSPARENT);

               TCHAR label[MAX_PATH]{};
               TCITEM tci{};
               tci.mask = TCIF_TEXT;
               tci.pszText = label;
               tci.cchTextMax = MAX_PATH - 1;

               ::SendMessage(hWnd, TCM_GETITEM, i, reinterpret_cast<LPARAM>(&tci));

               RECT rcText = rcItem;
               rcText.left += scaleDPIX(5);
               rcText.right -= scaleDPIX(3);

               if (isSelectedTab)
               {
                  rcText.bottom -= scaleDPIY(4);
                  ::InflateRect(&rcFrame, 0, 1);
               }
               if (i != nTabs - 1)
               {
                  rcFrame.right += 1;
               }

               ::FrameRect(hdc, &rcFrame, getEdgeBrush());

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
      {
         RemoveWindowSubclass(hWnd, TabSubclass, g_tabSubclassID);
         break;
      }

      case WM_PARENTNOTIFY:
      {
         switch (LOWORD(wParam))
         {
         case WM_CREATE:
         {
            auto hwndUpdown = reinterpret_cast<HWND>(lParam);
            if (subclassUpDownControl(hwndUpdown))
            {
               return 0;
            }
            break;
         }
         }
         return 0;
      }

      }
      return DefSubclassProc(hWnd, uMsg, wParam, lParam);
   }

   void subclassTabControl(HWND hwnd)
   {
      SetWindowSubclass(hwnd, TabSubclass, g_tabSubclassID, 0);
   }

   constexpr UINT_PTR g_customBorderSubclassID = 42;

   LRESULT CALLBACK CustomBorderSubclass(
      HWND hWnd,
      UINT uMsg,
      WPARAM wParam,
      LPARAM lParam,
      UINT_PTR uIdSubclass,
      DWORD_PTR dwRefData
   )
   {
      UNREFERENCED_PARAMETER(dwRefData);

      static bool isHotStatic = false;

      switch (uMsg)
      {
      case WM_NCPAINT:
      {
         if (!isEnabled())
         {
            break;
         }

         DefSubclassProc(hWnd, uMsg, wParam, lParam);

         HDC hdc = ::GetWindowDC(hWnd);
         RECT rcClient{};
         ::GetClientRect(hWnd, &rcClient);
         rcClient.right += (2 * ::GetSystemMetrics(SM_CXEDGE));

         auto style = ::GetWindowLongPtr(hWnd, GWL_STYLE);
         bool hasVerScrollbar = (style & WS_VSCROLL) == WS_VSCROLL;
         if (hasVerScrollbar)
         {
            rcClient.right += ::GetSystemMetrics(SM_CXVSCROLL);
         }

         rcClient.bottom += (2 * ::GetSystemMetrics(SM_CYEDGE));

         bool hasHorScrollbar = (style & WS_HSCROLL) == WS_HSCROLL;
         if (hasHorScrollbar)
         {
            rcClient.bottom += ::GetSystemMetrics(SM_CXHSCROLL);
         }

         HPEN hPen = ::CreatePen(PS_SOLID, 1, getBackgroundColor());
         RECT rcInner = rcClient;
         ::InflateRect(&rcInner, -1, -1);
         paintRoundFrameRect(hdc, rcInner, hPen);
         ::DeleteObject(hPen);

         bool hasFocus = ::GetFocus() == hWnd;

         POINT ptCursor{};
         ::GetCursorPos(&ptCursor);
         ::ScreenToClient(hWnd, &ptCursor);

         bool isHot = ::PtInRect(&rcClient, ptCursor);

         bool isWindowEnabled = ::IsWindowEnabled(hWnd) == TRUE;
         HPEN hEnabledPen = ((isHotStatic && isHot) || hasFocus ? getHotEdgePen() : getEdgePen());

         paintRoundFrameRect(hdc, rcClient, isWindowEnabled ? hEnabledPen : getDisabledEdgePen());

         ::ReleaseDC(hWnd, hdc);

         return 0;
      }
      break;

      case WM_NCCALCSIZE:
      {
         if (!isEnabled())
         {
            break;
         }

         auto lpRect = reinterpret_cast<LPRECT>(lParam);
         ::InflateRect(lpRect, -(::GetSystemMetrics(SM_CXEDGE)), -(::GetSystemMetrics(SM_CYEDGE)));

         auto style = ::GetWindowLongPtr(hWnd, GWL_STYLE);
         bool hasVerScrollbar = (style & WS_VSCROLL) == WS_VSCROLL;
         if (hasVerScrollbar)
         {
            lpRect->right -= ::GetSystemMetrics(SM_CXVSCROLL);
         }

         bool hasHorScrollbar = (style & WS_HSCROLL) == WS_HSCROLL;
         if (hasHorScrollbar)
         {
            lpRect->bottom -= ::GetSystemMetrics(SM_CXHSCROLL);
         }

         return 0;
      }
      break;

      case WM_MOUSEMOVE:
      {
         if (!isEnabled())
         {
            break;
         }

         if (::GetFocus() == hWnd)
         {
            break;
         }

         TRACKMOUSEEVENT tme{};
         tme.cbSize = sizeof(TRACKMOUSEEVENT);
         tme.dwFlags = TME_LEAVE;
         tme.hwndTrack = hWnd;
         tme.dwHoverTime = HOVER_DEFAULT;
         TrackMouseEvent(&tme);

         if (!isHotStatic)
         {
            isHotStatic = true;
            ::SetWindowPos(hWnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
         }
      }
      break;

      case WM_MOUSELEAVE:
      {
         if (!isEnabled())
         {
            break;
         }

         if (isHotStatic)
         {
            isHotStatic = false;
            ::SetWindowPos(hWnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
         }

         TRACKMOUSEEVENT tme{};
         tme.cbSize = sizeof(TRACKMOUSEEVENT);
         tme.dwFlags = TME_LEAVE | TME_CANCEL;
         tme.hwndTrack = hWnd;
         tme.dwHoverTime = HOVER_DEFAULT;
         TrackMouseEvent(&tme);
      }
      break;

      case WM_NCDESTROY:
      {
         RemoveWindowSubclass(hWnd, CustomBorderSubclass, uIdSubclass);
      }
      break;
      }
      return DefSubclassProc(hWnd, uMsg, wParam, lParam);
   }

   void subclassCustomBorderForListBoxAndEditControls(HWND hwnd)
   {
      SetWindowSubclass(hwnd, CustomBorderSubclass, g_customBorderSubclassID, 0);
   }

   constexpr UINT_PTR g_comboBoxSubclassID = 42;

   LRESULT CALLBACK ComboBoxSubclass(
      HWND hWnd,
      UINT uMsg,
      WPARAM wParam,
      LPARAM lParam,
      UINT_PTR uIdSubclass,
      DWORD_PTR dwRefData
   )
   {
      auto hwndEdit = reinterpret_cast<HWND>(dwRefData);

      switch (uMsg)
      {
      case WM_PAINT:
      {
         if (!isEnabled())
         {
            break;
         }

         RECT rc{};
         ::GetClientRect(hWnd, &rc);

         PAINTSTRUCT ps;
         auto hdc = ::BeginPaint(hWnd, &ps);

         ::SelectObject(hdc, reinterpret_cast<HFONT>(::SendMessage(hWnd, WM_GETFONT, 0, 0)));
         ::SetBkColor(hdc, getBackgroundColor());

         auto holdBrush = ::SelectObject(hdc, getDarkerBackgroundBrush());

         RECT rcArrow = {rc.right - scaleDPIX(17), rc.top + 1, rc.right - 1, rc.bottom - 1};
         bool hasFocus{};

         // CBS_DROPDOWN text is handled by parent by WM_CTLCOLOREDIT
         auto style = ::GetWindowLongPtr(hWnd, GWL_STYLE);
         if ((style & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
         {
            hasFocus = ::GetFocus() == hWnd;

            RECT rcTextBg = rc;
            rcTextBg.left += 1;
            rcTextBg.top += 1;
            rcTextBg.right = rcArrow.left - 1;
            rcTextBg.bottom -= 1;
            ::FillRect(hdc, &rcTextBg, getBackgroundBrush()); // erase background on item change

            auto index = static_cast<int>(::SendMessage(hWnd, CB_GETCURSEL, 0, 0));
            if (index != CB_ERR)
            {
               ::SetTextColor(hdc, getTextColor());
               ::SetBkColor(hdc, getBackgroundColor());
               auto bufferLen = static_cast<size_t>(::SendMessage(hWnd, CB_GETLBTEXTLEN, index, 0));
               TCHAR* buffer = new TCHAR[(bufferLen + 1)];
               ::SendMessage(hWnd, CB_GETLBTEXT, index, reinterpret_cast<LPARAM>(buffer));

               RECT rcText = rc;
               rcText.left += 4;
               rcText.right = rcArrow.left - 5;

               ::DrawText(hdc, buffer, -1, &rcText, DT_NOPREFIX | DT_LEFT | DT_VCENTER | DT_SINGLELINE);
               delete[]buffer;
            }
         }
         else if ((style & CBS_DROPDOWN) == CBS_DROPDOWN && hwndEdit != NULL)
         {
            hasFocus = ::GetFocus() == hwndEdit;
         }

         POINT ptCursor{};
         ::GetCursorPos(&ptCursor);
         ScreenToClient(hWnd, &ptCursor);

         bool isHot = PtInRect(&rc, ptCursor);
         bool isWindowEnabled = ::IsWindowEnabled(hWnd) == TRUE;

         auto colorEnabledText = isHot ? getTextColor() : getDarkerTextColor();
         ::SetTextColor(hdc, isWindowEnabled ? colorEnabledText : getDisabledTextColor());
         ::SetBkColor(hdc, isHot ? getHotBackgroundColor() : getBackgroundColor());
         ::ExtTextOut(hdc,
            rcArrow.left + (rcArrow.right - rcArrow.left) / 2 - scaleDPIX(4),
            rcArrow.top + 3,
            ETO_OPAQUE | ETO_CLIPPED,
            &rcArrow, L"˅",
            1,
            nullptr);
         ::SetBkColor(hdc, getBackgroundColor());

         auto hEnabledPen = (isHot || hasFocus) ? getHotEdgePen() : getEdgePen();
         auto hSelectedPen = isWindowEnabled ? hEnabledPen : getDisabledEdgePen();
         auto holdPen = static_cast<HPEN>(::SelectObject(hdc, hSelectedPen));

         POINT edge[] = {
            {rcArrow.left - 1, rcArrow.top},
            {rcArrow.left - 1, rcArrow.bottom}
         };
         ::Polyline(hdc, edge, _countof(edge));

         int roundCornerValue = isWindows11() ? scaleDPIX(4) : 0;
         paintRoundFrameRect(hdc, rc, hSelectedPen, roundCornerValue, roundCornerValue);

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
      DWORD_PTR hwndEditData = 0;
      auto style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
      if ((style & CBS_DROPDOWN) == CBS_DROPDOWN)
      {
         POINT pt = { 5, 5 };
         hwndEditData = reinterpret_cast<DWORD_PTR>(::ChildWindowFromPoint(hwnd, pt));
      }
      SetWindowSubclass(hwnd, ComboBoxSubclass, g_comboBoxSubclassID, hwndEditData);
   }

   constexpr UINT_PTR g_listViewSubclassID = 42;

   LRESULT CALLBACK ListViewSubclass(
      HWND hWnd,
      UINT uMsg,
      WPARAM wParam,
      LPARAM lParam,
      UINT_PTR uIdSubclass,
      DWORD_PTR dwRefData
   )
   {
      UNREFERENCED_PARAMETER(dwRefData);

      switch (uMsg)
      {
      case WM_NCDESTROY:
      {
         ::RemoveWindowSubclass(hWnd, ListViewSubclass, uIdSubclass);
         break;
      }

      case WM_NOTIFY:
      {
         switch (reinterpret_cast<LPNMHDR>(lParam)->code)
         {
         case NM_CUSTOMDRAW:
         {
            auto lpnmcd = reinterpret_cast<LPNMCUSTOMDRAW>(lParam);
            switch (lpnmcd->dwDrawStage)
            {
            case CDDS_PREPAINT:
            {
               if (isExperimentalSupported() && isEnabled())
               {
                  return CDRF_NOTIFYITEMDRAW;
               }
               return CDRF_DODEFAULT;
            }

            case CDDS_ITEMPREPAINT:
            {
               SetTextColor(lpnmcd->hdc, getDarkerTextColor());

               return CDRF_NEWFONT;
            }

            default:
               return CDRF_DODEFAULT;
            }
         }
         break;
         }
         break;
      }
      break;
      }
      return DefSubclassProc(hWnd, uMsg, wParam, lParam);
   }

   void subclassListViewControl(HWND hwnd)
   {
      SetWindowSubclass(hwnd, ListViewSubclass, g_listViewSubclassID, 0);
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
         TCHAR className[classNameLen] {};
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

            auto style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
            bool isComboBox = (style & LBS_COMBOBOX) == LBS_COMBOBOX;
            auto exStyle = ::GetWindowLongPtr(hwnd, GWL_EXSTYLE);
            bool hasClientEdge = (exStyle & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE;

            if (p.subclass && !isComboBox && hasClientEdge)
            {
               subclassCustomBorderForListBoxAndEditControls(hwnd);
            }

#ifndef __MINGW64__ // mingw build for 64 bit has issue with GetWindowSubclass, it is undefined

            bool changed = false;
            if (::GetWindowSubclass(hwnd, CustomBorderSubclass, g_customBorderSubclassID, nullptr) == TRUE)
            {
               if (isEnabled())
               {
                  if (hasClientEdge)
                  {
                     ::SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle & ~WS_EX_CLIENTEDGE);
                     changed = true;
                  }
               }
               else if (!hasClientEdge)
               {
                  ::SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle | WS_EX_CLIENTEDGE);
                  changed = true;
               }
            }

            if (changed)
            {
               ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
            }

#endif // !__MINGW64__

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

            auto exStyle = ::GetWindowLongPtr(hwnd, GWL_EXSTYLE);
            bool hasClientEdge = (exStyle & WS_EX_CLIENTEDGE) == WS_EX_CLIENTEDGE;
            if (p.subclass && hasClientEdge)
            {
               subclassCustomBorderForListBoxAndEditControls(hwnd);
            }

#ifndef __MINGW64__ // mingw build for 64 bit has issue with GetWindowSubclass, it is undefined

            bool changed = false;
            if (::GetWindowSubclass(hwnd, CustomBorderSubclass, g_customBorderSubclassID, nullptr) == TRUE)
            {
               if (isEnabled())
               {
                  if (hasClientEdge)
                  {
                     ::SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle & ~WS_EX_CLIENTEDGE);
                     changed = true;
                  }
               }
               else if (!hasClientEdge)
               {
                  ::SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle | WS_EX_CLIENTEDGE);
                  changed = true;
               }
            }

            if (changed)
            {
               ::SetWindowPos(hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
            }

#endif // !__MINGW64__

            return TRUE;
         }

         if (wcscmp(className, WC_BUTTON) == 0)
         {
            auto nButtonStyle = ::GetWindowLongPtr(hwnd, GWL_STYLE);
            switch (nButtonStyle & BS_TYPEMASK)
            {
               case BS_CHECKBOX:
               case BS_AUTOCHECKBOX:
               case BS_3STATE:
               case BS_AUTO3STATE:
               case BS_RADIOBUTTON:
               case BS_AUTORADIOBUTTON:
               {
                  auto nButtonAllStyles = ::GetWindowLongPtr(hwnd, GWL_STYLE);
                  if ((nButtonAllStyles & BS_PUSHLIKE) == BS_PUSHLIKE)
                  {
                     if (p.theme)
                     {
                        SetWindowTheme(hwnd, p.themeClassName, nullptr);
                     }
                     break;
                  }
                  if (p.subclass)
                  {
                     subclassButtonControl(hwnd);
                  }
                  break;
               }
               case BS_GROUPBOX:
               {
                  if (p.subclass)
                  {
                     subclassGroupboxControl(hwnd);
                  }
                  break;
               }
               case BS_DEFPUSHBUTTON:
               case BS_PUSHBUTTON:
               {
                  if (p.theme)
                  {
                     SetWindowTheme(hwnd, p.themeClassName, nullptr);
                  }
                  break;
               }
            }
            return TRUE;
         }

         if (wcscmp(className, TOOLBARCLASSNAME) == 0)
         {
            setDarkLineAbovePanelToolbar(hwnd);
            setDarkTooltips(hwnd, ToolTipsType::toolbar);

            return TRUE;
         }

         if (wcscmp(className, WC_LISTVIEW) == 0)
         {
            setDarkListView(hwnd);
            setDarkTooltips(hwnd, ToolTipsType::listview);

            int fgColor{ static_cast<int>(nppMessage(NPPM_GETEDITORDEFAULTFOREGROUNDCOLOR, 0, 0)) };
            int bgColor{ static_cast<int>(nppMessage(NPPM_GETEDITORDEFAULTBACKGROUNDCOLOR, 0, 0)) };

            ListView_SetTextColor(hwnd, fgColor);
            ListView_SetTextBkColor(hwnd, bgColor);
            ListView_SetBkColor(hwnd, bgColor);

            if (p.subclass)
            {
               auto exStyle = ListView_GetExtendedListViewStyle(hwnd);
               ListView_SetExtendedListViewStyle(hwnd, exStyle | LVS_EX_DOUBLEBUFFER);
               subclassListViewControl(hwnd);
            }

            return TRUE;
         }

         if (wcscmp(className, WC_TREEVIEW) == 0)
         {
            int fgColor{ static_cast<int>(nppMessage(NPPM_GETEDITORDEFAULTFOREGROUNDCOLOR, 0, 0)) };
            int bgColor{ static_cast<int>(nppMessage(NPPM_GETEDITORDEFAULTBACKGROUNDCOLOR, 0, 0)) };

            TreeView_SetTextColor(hwnd, fgColor);
            TreeView_SetBkColor(hwnd, bgColor);

            calculateTreeViewStyle();
            setTreeViewStyle(hwnd);

            setDarkTooltips(hwnd, ToolTipsType::treeview);

            return TRUE;
         }

         // Plugin might use rich edit control version 2.0 and later
         if (wcscmp(className, L"RichEdit20W") == 0 || wcscmp(className, L"RICHEDIT50W") == 0)
         {
            if (p.theme)
            {
               //dark scrollbar for rich edit control
               SetWindowTheme(hwnd, p.themeClassName, nullptr);
            }

            return TRUE;
         }

         if (wcscmp(className, UPDOWN_CLASS) == 0)
         {
            subclassUpDownControl(hwnd);
            return TRUE;
         }

         return TRUE;
      }, reinterpret_cast<LPARAM>(&p));
   }

   LRESULT darkToolBarNotifyCustomDraw(LPARAM lParam)
   {
      auto nmtbcd = reinterpret_cast<LPNMTBCUSTOMDRAW>(lParam);
      static int roundCornerValue = 0;

      switch (nmtbcd->nmcd.dwDrawStage)
      {
      case CDDS_PREPAINT:
      {
         if (isEnabled())
         {
            roundCornerValue = isWindows11() ? scaleDPIX(5) : 0;

            ::FillRect(nmtbcd->nmcd.hdc, &nmtbcd->nmcd.rc, getDarkerBackgroundBrush());
            return CDRF_NOTIFYITEMDRAW;
         }
         return CDRF_DODEFAULT;
      }

      case CDDS_ITEMPREPAINT:
      {
         nmtbcd->hbrLines = getEdgeBrush();
         nmtbcd->clrText = getTextColor();
         nmtbcd->clrTextHighlight = getTextColor();
         nmtbcd->clrBtnFace = getBackgroundColor();
         nmtbcd->clrBtnHighlight = getSofterBackgroundColor();
         nmtbcd->clrHighlightHotTrack = getHotBackgroundColor();
         nmtbcd->nStringBkMode = TRANSPARENT;
         nmtbcd->nHLStringBkMode = TRANSPARENT;

         if ((nmtbcd->nmcd.uItemState & CDIS_CHECKED) == CDIS_CHECKED)
         {
            auto holdBrush = ::SelectObject(nmtbcd->nmcd.hdc, getSofterBackgroundBrush());
            auto holdPen = ::SelectObject(nmtbcd->nmcd.hdc, getEdgePen());
            ::RoundRect(nmtbcd->nmcd.hdc, nmtbcd->nmcd.rc.left, nmtbcd->nmcd.rc.top, nmtbcd->nmcd.rc.right, nmtbcd->nmcd.rc.bottom, roundCornerValue, roundCornerValue);
            ::SelectObject(nmtbcd->nmcd.hdc, holdBrush);
            ::SelectObject(nmtbcd->nmcd.hdc, holdPen);

            nmtbcd->nmcd.uItemState &= ~CDIS_CHECKED;
         }

         return TBCDRF_HILITEHOTTRACK | TBCDRF_USECDCOLORS | CDRF_NOTIFYPOSTPAINT;
      }

      case CDDS_ITEMPOSTPAINT:
      {
         bool isDropDown = false;

         auto exStyle = ::SendMessage(nmtbcd->nmcd.hdr.hwndFrom, TB_GETEXTENDEDSTYLE, 0, 0);
         if ((exStyle & TBSTYLE_EX_DRAWDDARROWS) == TBSTYLE_EX_DRAWDDARROWS)
         {
            TBBUTTONINFO tbButtonInfo{};
            tbButtonInfo.cbSize = sizeof(TBBUTTONINFO);
            tbButtonInfo.dwMask = TBIF_STYLE;
            ::SendMessage(nmtbcd->nmcd.hdr.hwndFrom, TB_GETBUTTONINFO, nmtbcd->nmcd.dwItemSpec, reinterpret_cast<LPARAM>(&tbButtonInfo));

            isDropDown = (tbButtonInfo.fsStyle & BTNS_DROPDOWN) == BTNS_DROPDOWN;
         }

         if (!isDropDown && (nmtbcd->nmcd.uItemState & CDIS_HOT) == CDIS_HOT)
         {
            paintRoundFrameRect(nmtbcd->nmcd.hdc, nmtbcd->nmcd.rc, getHotEdgePen(), roundCornerValue, roundCornerValue);
         }

         return CDRF_DODEFAULT;
      }

      default:
         return CDRF_DODEFAULT;
      }
   }

   LRESULT darkListViewNotifyCustomDraw(LPARAM lParam)
   {
      auto lplvcd = reinterpret_cast<LPNMLVCUSTOMDRAW>(lParam);

      switch (lplvcd->nmcd.dwDrawStage)
      {
      case CDDS_PREPAINT:
      {
         return CDRF_NOTIFYITEMDRAW;
      }

      case CDDS_ITEMPREPAINT:
      {
         auto isSelected = ListView_GetItemState(lplvcd->nmcd.hdr.hwndFrom, lplvcd->nmcd.dwItemSpec, LVIS_SELECTED) == LVIS_SELECTED;

         if (isEnabled())
         {
            if (isSelected)
            {
               lplvcd->clrText = getTextColor();
               lplvcd->clrTextBk = getSofterBackgroundColor();

               ::FillRect(lplvcd->nmcd.hdc, &lplvcd->nmcd.rc, getSofterBackgroundBrush());
            }
            else if ((lplvcd->nmcd.uItemState & CDIS_HOT) == CDIS_HOT)
            {
               lplvcd->clrText = getTextColor();
               lplvcd->clrTextBk = getHotBackgroundColor();

               ::FillRect(lplvcd->nmcd.hdc, &lplvcd->nmcd.rc, getHotBackgroundBrush());
            }
         }

         if (isSelected)
         {
            ::DrawFocusRect(lplvcd->nmcd.hdc, &lplvcd->nmcd.rc);
         }

         return CDRF_NEWFONT;
      }

      default:
         return CDRF_DODEFAULT;
      }
   }

   LRESULT darkTreeViewNotifyCustomDraw(LPARAM lParam)
   {
      auto lptvcd = reinterpret_cast<LPNMTVCUSTOMDRAW>(lParam);

      switch (lptvcd->nmcd.dwDrawStage)
      {
      case CDDS_PREPAINT:
      {
         if (isEnabled())
         {
            return CDRF_NOTIFYITEMDRAW;
         }
         return CDRF_DODEFAULT;
      }

      case CDDS_ITEMPREPAINT:
      {
         if ((lptvcd->nmcd.uItemState & CDIS_SELECTED) == CDIS_SELECTED)
         {
            lptvcd->clrText = getTextColor();
            lptvcd->clrTextBk = getSofterBackgroundColor();
            ::FillRect(lptvcd->nmcd.hdc, &lptvcd->nmcd.rc, getSofterBackgroundBrush());

            return CDRF_NEWFONT | CDRF_NOTIFYPOSTPAINT;
         }

         if ((lptvcd->nmcd.uItemState & CDIS_HOT) == CDIS_HOT)
         {
            lptvcd->clrText = getTextColor();
            lptvcd->clrTextBk = getHotBackgroundColor();
            ::FillRect(lptvcd->nmcd.hdc, &lptvcd->nmcd.rc, getHotBackgroundBrush());

            return CDRF_NEWFONT | CDRF_NOTIFYPOSTPAINT;
         }

         return CDRF_DODEFAULT;
      }

      case CDDS_ITEMPOSTPAINT:
      {
         RECT rcFrame = lptvcd->nmcd.rc;
         rcFrame.left -= 1;
         rcFrame.right += 1;

         if ((lptvcd->nmcd.uItemState & CDIS_HOT) == CDIS_HOT)
         {
            paintRoundFrameRect(lptvcd->nmcd.hdc, rcFrame, getHotEdgePen(), 0, 0);
         }
         else if ((lptvcd->nmcd.uItemState & CDIS_SELECTED) == CDIS_SELECTED)
         {
            paintRoundFrameRect(lptvcd->nmcd.hdc, rcFrame, getEdgePen(), 0, 0);
         }

         return CDRF_DODEFAULT;

      }

      default:
         return CDRF_DODEFAULT;
      }
   }

   void autoThemeChildControls(HWND hwndParent)
   {
      autoSubclassAndThemeChildControls(hwndParent, false, true);
   }

   constexpr UINT_PTR g_tabUpDownSubclassID = 42;

   LRESULT CALLBACK UpDownSubclass(
      HWND hWnd,
      UINT uMsg,
      WPARAM wParam,
      LPARAM lParam,
      UINT_PTR uIdSubclass,
      DWORD_PTR dwRefData
   )
   {
      auto pButtonData = reinterpret_cast<ButtonData*>(dwRefData);
      auto style = ::GetWindowLongPtr(hWnd, GWL_STYLE);

      switch (uMsg)
      {
      case WM_PRINTCLIENT:
      case WM_PAINT:
      {
         if (!isEnabled())
         {
            break;
         }

         bool hasTheme = pButtonData->ensureTheme(hWnd);

         RECT rcClient{};
         ::GetClientRect(hWnd, &rcClient);

         PAINTSTRUCT ps{};
         auto hdc = ::BeginPaint(hWnd, &ps);

         auto holdPen = static_cast<HPEN>(::SelectObject(hdc, getEdgePen()));
         ::FillRect(hdc, &rcClient, getDarkerBackgroundBrush());

         RECT rcArrowUp{};
         RECT rcArrowDown{};

         if ((style & UDS_HORZ) == UDS_HORZ) {
            rcArrowUp = {
               rcClient.left, rcClient.top,
               rcClient.right - ((rcClient.right - rcClient.left) / 2) , rcClient.bottom
            };

            rcArrowDown = {
               rcArrowUp.right, rcClient.top,
               rcClient.right, rcClient.bottom
            };
         }
         else {
            rcArrowUp = {
               rcClient.left, rcClient.top,
               rcClient.right, rcClient.bottom - ((rcClient.bottom - rcClient.top) / 2)
            };

            rcArrowDown = {
               rcClient.left, rcArrowUp.bottom,
               rcClient.right, rcArrowUp.bottom + ((rcClient.bottom - rcClient.top) / 2)
            };
         }

         POINT ptCursor = {};
         ::GetCursorPos(&ptCursor);
         ::ScreenToClient(hWnd, &ptCursor);

         bool isHotUp = ::PtInRect(&rcArrowUp, ptCursor);
         bool isHotDown = ::PtInRect(&rcArrowDown, ptCursor);
         if (isHotDown) isHotUp = false;

         ::SetBkMode(hdc, TRANSPARENT);

         if (hasTheme)
         {
            if ((style & UDS_HORZ) == UDS_HORZ) {
               ::DrawThemeBackground(pButtonData->hTheme, hdc, BP_PUSHBUTTON, isHotUp ? PBS_HOT : PBS_NORMAL, &rcArrowUp, nullptr);
               ::DrawThemeBackground(pButtonData->hTheme, hdc, BP_PUSHBUTTON, isHotDown ? PBS_HOT : PBS_NORMAL, &rcArrowDown, nullptr);
            }
            else {
               int roundCornerValue = isWindows11() ? scaleDPIX(4) : 0;
               holdPen = static_cast<HPEN>(::SelectObject(hdc, getEdgePen()));

               paintRoundFrameRect(hdc, rcArrowUp, getEdgePen(), roundCornerValue, roundCornerValue);
               paintRoundFrameRect(hdc, rcArrowDown, getEdgePen(), roundCornerValue, roundCornerValue);
            }
         }
         else
         {
            ::FillRect(hdc, &rcArrowUp, isHotUp ? getHotBackgroundBrush() : getBackgroundBrush());
            ::FillRect(hdc, &rcArrowDown, isHotDown ? getHotBackgroundBrush() : getBackgroundBrush());
         }

         LOGFONT lf{};
         auto font = reinterpret_cast<HFONT>(SendMessage(hWnd, WM_GETFONT, 0, 0));
         ::GetObject(font, sizeof(lf), &lf);
         lf.lfHeight = (scaleDPIY(16) - 5) * -1;
         lf.lfWeight = 900;
         auto holdFont = static_cast<HFONT>(::SelectObject(hdc, CreateFontIndirect(&lf)));

         if ((style & UDS_HORZ) == UDS_HORZ) {
            auto mPosX = ((rcArrowUp.right - rcArrowUp.left - scaleDPIX(7) + 1) / 2);
            auto mPosY = ((rcArrowUp.bottom - rcArrowUp.top + lf.lfHeight - scaleDPIY(1) - 3) / 2);

            ::SetTextColor(hdc, isHotUp ? getTextColor() : getDarkerTextColor());
            ::ExtTextOut(hdc,
               rcArrowUp.left + mPosX,
               rcArrowUp.top + mPosY,
               ETO_CLIPPED,
               &rcArrowUp, L"<",
               1,
               nullptr);

            ::SetTextColor(hdc, isHotDown ? getTextColor() : getDarkerTextColor());
            ::ExtTextOut(hdc,
               rcArrowDown.left + mPosX - scaleDPIX(2) + 3,
               rcArrowDown.top + mPosY,
               ETO_CLIPPED,
               &rcArrowDown, L">",
               1,
               nullptr);
         }
         else {
            auto mPosX = scaleDPIX(4);
            auto mPosY = scaleDPIX(2);

            ::SetTextColor(hdc, isHotUp ? getTextColor() : getDarkerTextColor());
            ::ExtTextOut(hdc,
               rcArrowUp.left + mPosX,
               rcArrowUp.top - mPosY,
               ETO_CLIPPED,
               &rcArrowUp, L"˄",
               1,
               nullptr);

            ::SetTextColor(hdc, isHotDown ? getTextColor() : getDarkerTextColor());
            ::ExtTextOut(hdc,
               rcArrowDown.left + mPosX,
               rcArrowDown.top - mPosY,
               ETO_CLIPPED,
               &rcArrowDown, L"˅",
               1,
               nullptr);
         }

         if (!hasTheme)
         {
            auto holdBrush = ::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
            ::Rectangle(hdc, rcArrowUp.left, rcArrowUp.top, rcArrowUp.right, rcArrowUp.bottom);
            ::Rectangle(hdc, rcArrowDown.left, rcArrowDown.top, rcArrowDown.right, rcArrowDown.bottom);
            ::SelectObject(hdc, holdBrush);
         }

         ::SelectObject(hdc, holdPen);
         ::SelectObject(hdc, holdFont);
         ::EndPaint(hWnd, &ps);
         return FALSE;
      }

      case WM_THEMECHANGED:
      {
         pButtonData->closeTheme();
         break;
      }

      case WM_NCDESTROY:
      {
         ::RemoveWindowSubclass(hWnd, UpDownSubclass, uIdSubclass);
         delete pButtonData;
         break;
      }

      case WM_ERASEBKGND:
      {
         if (isEnabled())
         {
            RECT rcClient{};
            ::GetClientRect(hWnd, &rcClient);
            ::FillRect(reinterpret_cast<HDC>(wParam), &rcClient, getDarkerBackgroundBrush());
            return TRUE;
         }
         break;
      }
      }
      return DefSubclassProc(hWnd, uMsg, wParam, lParam);
   }

   bool subclassUpDownControl(HWND hwnd)
   {
      constexpr size_t classNameLen = 16;
      TCHAR className[classNameLen]{};
      GetClassName(hwnd, className, classNameLen);
      if (wcscmp(className, UPDOWN_CLASS) == 0)
      {
         auto pButtonData = reinterpret_cast<DWORD_PTR>(new ButtonData());
         SetWindowSubclass(hwnd, UpDownSubclass, g_tabUpDownSubclassID, pButtonData);
         setDarkExplorerTheme(hwnd);
         return true;
      }

      return false;
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
      COLORSCHEME scheme{};
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
      if (isExperimentalSupported())
      {
         bool useDark = isEnabled();

         HWND hHeader = ListView_GetHeader(hwnd);
         allowDarkModeForWindow(hHeader, useDark);
         SetWindowTheme(hHeader, useDark ? L"ItemsView" : nullptr, nullptr);

         allowDarkModeForWindow(hwnd, useDark);
         SetWindowTheme(hwnd, L"Explorer", nullptr);
      }
   }

   void disableVisualStyle(HWND hwnd, bool doDisable)
   {
      if (doDisable)
      {
         SetWindowTheme(hwnd, L"", L"");
      }
      else
      {
         SetWindowTheme(hwnd, nullptr, nullptr);
      }
   }

   // range to determine when it should be better to use classic style
   constexpr double middleGrayRange = 2.0;

   void calculateTreeViewStyle()
   {
      COLORREF bgColor{};// = NppParameters::getInstance().getCurrentDefaultBgColor();

      if (treeViewBg != bgColor || lighnessTreeView == 50.0)
      {
         lighnessTreeView = calculatePerceivedLighness(bgColor);
         treeViewBg = bgColor;
      }

      if (lighnessTreeView < (50.0 - middleGrayRange))
      {
         treeViewStyle = TreeViewStyle::dark;
      }
      else if (lighnessTreeView > (50.0 + middleGrayRange))
      {
         treeViewStyle = TreeViewStyle::light;
      }
      else
      {
         treeViewStyle = TreeViewStyle::classic;
      }
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

   INT_PTR onCtlColorListbox(WPARAM wParam, LPARAM lParam)
   {
      auto hdc = reinterpret_cast<HDC>(wParam);
      auto hwnd = reinterpret_cast<HWND>(lParam);

      auto style = ::GetWindowLongPtr(hwnd, GWL_STYLE);
      bool isComboBox = (style & LBS_COMBOBOX) == LBS_COMBOBOX;
      if (!isComboBox && ::IsWindowEnabled(hwnd))
      {
         return static_cast<INT_PTR>(onCtlColorSofter(hdc));
      }
      return static_cast<INT_PTR>(onCtlColor(hdc));
   }
}
