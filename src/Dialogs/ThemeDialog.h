#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"
#include <regex>
#include <vector>

#define THEME_ITEM_LIMIT 999
#define STYLE_ITEM_LIMIT 99
#define SWATCH_ITEM_COUNT 29

using std::wregex;
using std::regex_replace;
using std::vector;
using Utils::showEditBalloonTip;

extern NppData nppData;
extern ConfigIO _configIO;

class ThemeDialog : public StaticDialog {
public:
   ThemeDialog() : StaticDialog() {};
   ~ThemeDialog();

   void doDialog(HINSTANCE hInst);
   int appendThemeConfigs(const wstring& sThemeFile);
   void initPreviewSwatch(int idxStart=0, int idxEnd=SWATCH_ITEM_COUNT);


protected:
   enum move_dir {
      MOVE_DOWN = 1,
      MOVE_UP = -1
   };

   struct ThemeType {
      wstring label;
      StyleInfo eolStyle;
      vector<StyleInfo> vStyleInfo;
   };

   vector<ThemeType> vThemeTypes;
   wstring themeFile{ L"" };
   int swatchTopIndex{};
   bool loadingEdits, cleanConfigFile, cleanThemeVals, cleanStyleDefs, styleDefColor;

   HWND hThemesLB, hStylesLB;
   HBRUSH hbr;
   COLORREF styleBack, styleFore, customColors[16];

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM);
   void localize();
   void indicateCleanStatus();
   int loadConfigInfo();
   int loadThemeInfo(int vIndex, const wstring& themeType, const wstring& sThemeFile);
   bool promptDiscardChangesNo();
   void saveConfigInfo();
   void showEximDialog(bool bExtract);

   int getCurrentThemeIndex();
   bool getCurrentThemeInfo(ThemeType*& fileInfo);
   ThemeType getNewTheme();
   void getThemeConfig(size_t idxTh, bool cr_lf, wstring& themeLabel, wstring& ttConfig);

   int getCurrentStyleIndex();
   bool getCurrentStyleInfo(StyleInfo*& styleInfo);
   StyleInfo getNewStyle();

   void fillThemes();
   void onThemeSelect();
   void enableMoveThemeButtons();
   void enableThemeSelection();
   int moveThemeType(move_dir dir);
   void themeEditAccept();
   void themeEditNew();
   void themeEditClone();
   int themeEditDelete();
   bool checkThemeLimit(bool clone);

   void fillStyles();
   void onStyleSelect();
   void enableMoveStyleButtons();
   void enableStyleSelection();
   int moveStyleType(move_dir dir);
   void styleEditNew(bool clone);
   int styleEditDelete();

   int getStyleDefColor(bool back);
   void setStyleDefColor(bool setEdit, int color, bool back);
   void setOutputFontStyle();
   void fillStyleDefs();
   wstring getStyleConfig(int idx, StyleInfo& style);
   void styleDefsAccept();
   void setPangram();

   INT_PTR colorStaticControl(WPARAM wParam, LPARAM lParam);
   INT_PTR colorPreviewSwatch(WPARAM wParam, LPARAM lParam);
   void processSwatchClick(int ctrlID);
   void chooseStyleDefColor(bool back);
};

