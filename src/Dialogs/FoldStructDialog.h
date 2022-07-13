#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"
#include <regex>
#include <vector>

using std::vector;
using Utils::showEditBalloonTip;

extern NppData nppData;
extern ConfigIO _configIO;

class FoldStructDialog : public StaticDialog {
public:
   FoldStructDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   void refreshDarkMode();

   HWND hFilesLB{};

private:
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

