#pragma once

#include "../Utils.h"
#include "../NPP/StaticDialog.h"

extern NppData nppData;

class EximFileTypeDialog : public StaticDialog {
public:
   EximFileTypeDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   void initDialog(bool bExtract);
   void setFileTypeData(LPWSTR fileTypeData);

protected:
   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
   void localize(bool bExtract);
};
