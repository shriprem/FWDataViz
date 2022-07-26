#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"
#include <regex>
#include <vector>

using std::vector;
using std::unordered_map;
using Utils::showEditBalloonTip;

extern NppData nppData;
extern ConfigIO _configIO;

class FoldStructDialog : public StaticDialog {
public:
   FoldStructDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   void initDialog(const unordered_map<wstring, string>& mFDtoT, const unordered_map<string, wstring>& mFTtoD);
   void refreshDarkMode();

   HWND hFilesLB{};

private:
   HWND hFoldFileStructs{}, hFoldFileType{}, hAutoFold{}, hHdrRecs{}, hHdrRecType{}, hHdrPriority{}, hHdrRecursive{},
      hImplicitRecs{}, hExplicitRecs{}, hExplicitRecType{};

   // File Type data
   unordered_map<wstring, string> cMapFileDescToType{};
   unordered_map<string, wstring> cMapFileTypeToDesc{};

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM);

   void localize();
   bool promptDiscardChangesNo();
   void saveConfigInfo();
   void showEximDialog(bool bExtract);

   void fillFileTypes();
   void onFileTypeSelect();
   int fileEditAccept();
   void fileEditNew();
   void fileEditClone();
   int fileEditDelete();
};

