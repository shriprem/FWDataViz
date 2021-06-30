#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"

extern NppData nppData;
extern ConfigIO _configIO;

class DataExtractDialog : public StaticDialog {
public:
   DataExtractDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   void initDialog(const wstring fileType);

protected:
   const int listedItemCount{ 10 };
   wstring initDocFileType{};

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
   void localize();
};
