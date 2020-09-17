#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"
#include <vector>

using std::vector;

extern NppData nppData;
extern ConfigIO _configIO;

class ConfigureDialog : public StaticDialog {
public:
   ConfigureDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);

protected:
   struct RecordInfo {
      wstring fileID;
      wstring recID;
      wstring label;
      string marker;
      vector<int> fieldWidths;
      vector<wstring> fieldLabels;
   };

   struct FileInfo {
      wstring fileID;
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

};


