#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"
#include <regex>
#include <vector>

#define STYLE_ITEM_LIMIT 28

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

   HWND hThemesLB, hStylesLB;

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

   wstring themeFile{ L"" };
   vector<ThemeType> vThemeTypes;
   bool loadingEdits, cleanConfigFile, cleanThemeVals, cleanStyleVals, cleanStyleDefs, styleDefColor;

   HBRUSH hbr;
   COLORREF styleBack, styleFore, customColors[16];

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM);
   void localize();
   void indicateCleanStatus();
   int loadConfigInfo();
   int loadThemeInfo(int vIndex, const wstring& themeType, const wstring& sThemeFile);
   bool promptDiscardChangesNo();
   void saveConfigInfo();
   void cloneThemeInfo();
   void showEximDialog(bool bExtract);

   int getCurrentThemeIndex();
   bool getCurrentThemeInfo(ThemeType* &fileInfo);
   ThemeType getNewTheme();
   void getThemeConfig(size_t idxTh, bool cr_lf, wstring& themeLabel, wstring& ttConfig);

   int getCurrentStyleIndex();
   bool getCurrentStyleInfo(StyleInfo* &styleInfo);
   StyleInfo getNewStyle();

   void fillThemes();
   void onThemeSelect();
   void enableMoveThemeButtons();
   void enableThemeSelection();
   int moveThemeType(move_dir dir);
   void themeEditAccept();
   void themeEditNew();
   int themeEditDelete();

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
   void initPreviewSwatch(int idxStart=0, int idxEnd=STYLE_ITEM_LIMIT);
   void processSwatchClick(int ctrlID);
   void chooseStyleDefColor(bool back);
};

