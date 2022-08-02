#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"

extern NppData nppData;
extern ConfigIO _configIO;

class EximFileTypeDialog : public StaticDialog {
public:
   enum exim_clients {
      FWTYPES_DLG,
      THEMES_DLG,
      FOLDS_DLG,
      EXIM_DLGS
   };

   EximFileTypeDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   void initDialog(HWND hWnd, exim_clients mode, bool bExtract);
   void refreshDarkMode();
   void setFileTypeData(const wstring& ftConfig);

private:
   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
   void localize();

   void appendExtractFile();
   void loadExtractFile();
   void saveExtractFile();

   wstring getEditControlText();

   bool extractMode{};
   exim_clients exim_mode{};
   HWND hClientDlg{};

   LPCWSTR APPEND_TITLE[EXIM_DLGS]{ EXIM_APPEND_FT_TITLE, EXIM_APPEND_THEME_TITLE, EXIM_APPEND_FOLD_TITLE };
   LPCWSTR EXTRACT_TITLE[EXIM_DLGS]{ EXIM_EXTRACT_FT_TITLE, EXIM_EXTRACT_THEME_TITLE, EXIM_EXTRACT_FOLD_TITLE };
   LPCWSTR EDIT_LABEL[EXIM_DLGS]{ EXIM_EDIT_FT_LABEL, EXIM_EDIT_THEME_LABEL, EXIM_EDIT_FOLD_LABEL };
   LPCWSTR APPEND_BTN[EXIM_DLGS]{ EXIM_APPEND_FT_BTN, EXIM_APPEND_THEME_BTN, EXIM_APPEND_FOLD_BTN };
};
