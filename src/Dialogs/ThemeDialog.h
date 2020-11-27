#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"
#include <regex>
#include <vector>

using std::wregex;
using std::regex_replace;
using std::vector;
using Utils::showEditBalloonTip;

extern NppData nppData;
extern ConfigIO _configIO;

class ThemeDialog : public StaticDialog {
public:
   ThemeDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   int appendFileTypeConfigs(const wstring& sConfigFile);

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

   wstring configFile{ L"" };
   vector<ThemeType> vThemeTypes;
   bool loadingEdits, cleanConfigFile, cleanThemeVals, cleanStyleVals, cleanStyleDefs;

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM);
   void localize();
   void indicateCleanStatus();
   int addConfigInfo(int vIndex, const wstring& themeType);
   int loadConfigInfo();
   bool promptDiscardChangesNo();
   void saveConfigInfo();
   void cloneConfigInfo();
   void showEximDialog(bool bExtract);

   int getCurrentThemeIndex();
   bool getCurrentThemeInfo(ThemeType* &fileInfo);
   ThemeType getNewTheme();
   void getThemeConfig(size_t idxTh, bool cr_lf, wstring &ftCode, wstring &ftConfig);

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
   void styleEditNew();
   int styleEditDelete();

   void fillStyleDefs();
   void styleDefsAccept();
};


