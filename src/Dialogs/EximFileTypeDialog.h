#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"

extern NppData nppData;
extern ConfigIO _configIO;

class EximFileTypeDialog : public StaticDialog {
public:
   EximFileTypeDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   void initDialog(bool bExtract);
   void setFileTypeData(LPCWSTR ftConfig);

protected:
   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
   void localize(bool bExtract);
   void loadExtractFile();
   void saveExtractFile();
};
