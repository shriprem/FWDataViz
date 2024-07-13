#pragma once

#include "StyleDefComponent.h"
#include "../NPP/StaticDialog.h"

#include <regex>
#include <vector>

constexpr int THEME_ITEM_LIMIT{ 999 };
constexpr int STYLE_ITEM_LIMIT{ 99 };
constexpr int SWATCH_ITEM_COUNT{ 29 };

using std::wregex;
using std::regex_replace;
using std::vector;

class ThemeDialog : public StaticDialog, StyleDefComponent {
public:
   ThemeDialog() : StaticDialog() {};

   void doDialog(HINSTANCE hInst);
   void display(bool toShow = TRUE);
   void refreshDarkMode();
   void initPreviewSwatch(int idxStart = 0, int idxEnd = SWATCH_ITEM_COUNT);

private:
   enum move_dir {
      MOVE_DOWN = 1,
      MOVE_UP = -1
   };

   struct ThemeType {
      wstring label;
      StyleInfo eolStyle;
      vector<StyleInfo> vStyleInfo;
   };

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM);
   void localize();
   void indicateCleanStatus();
   int loadConfigInfo();
   int loadThemeInfo(int vIndex, const wstring& themeType, const wstring& sThemeFile);
   bool promptDiscardChangesNo();
   void saveConfigInfo();
   void showEximDialog(bool bExtract);
   int appendThemeConfigs(const wstring& sThemeFile);

   int getCurrentThemeIndex() const;
   bool getCurrentThemeInfo(ThemeType*& fileInfo);
   ThemeType getNewTheme();
   void getThemeConfig(size_t idxTh, bool cr_lf, wstring& themeLabel, wstring& ttConfig);

   int getCurrentStyleIndex() const;
   bool getCurrentStyleInfo(StyleInfo*& styleInfo);
   StyleInfo getNewStyle();

   void fillThemes();
   void onThemeSelect();
   void onThemeSelectFill(ThemeType* themeInfo);
   void enableMoveThemeButtons();
   void enableThemeSelection();
   int moveThemeType(move_dir dir);
   void themeEditAccept(bool accept = true);
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

   void setStyleDefColor(bool setEdit, int color, bool back);
   void fillStyleDefs();
   wstring getStyleConfig(int idx, StyleInfo& style);
   void styleDefsAccept();

   INT_PTR colorPreviewSwatch(WPARAM wParam, LPARAM lParam);
   void processSwatchClick(int ctrlID);
   void chooseStyleDefColor(bool back);

   wstring themeFile{};
   int swatchTopIndex{};
   bool loadingEdits{}, cleanConfigFile{ true }, cleanThemeVals{ true };

   HWND hThemesLB{}, hStylesLB{};

   vector<ThemeType> vThemeTypes;
};

LRESULT CALLBACK procStylesListBox(HWND hwnd, UINT messageId, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);
