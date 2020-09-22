#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"
#include <regex>
#include <vector>

using std::wregex;
using std::regex_replace;
using std::vector;

extern NppData nppData;
extern ConfigIO _configIO;

class ConfigureDialog : public StaticDialog {
public:
   ConfigureDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);

protected:
   struct RecordInfo {
      wstring label;
      string marker;
      wstring fieldWidths;
      wstring fieldLabels;
   };

   struct FileInfo {
      wstring label;
      string eol;
      wstring theme;
      vector<RecordInfo> records;
   };

   vector<FileInfo> fileInfoList;

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM);
   void localize();
   int loadConfigInfo();
   void fillFileTypes();
   bool getCurrentFileInfo(FileInfo &fileInfo);
   void onFileTypeSelect();
   void fillRecTypes();
   bool getCurrentRecInfo(RecordInfo &recInfo);
   void onRecTypeSelect();
   void fillFieldTypes();
   void onFocusLabelAndWidth(HWND hEdit);
   void syncFieldLabelAndWidth(HWND hThis, HWND hThat);

   HWND hEditLabels, hEditWidths;
   BOOL bFocusOnLabels{ TRUE };
   int editLabelsCaret{}, editWidthsCaret{};
};


