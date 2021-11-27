#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"

extern NppData nppData;
extern ConfigIO _configIO;

class JumpToField : public StaticDialog {
public:
   JumpToField() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   void initDialog(const string fileType, int recordIndex, int fieldIndex, const vector<wstring>& fieldLabels);
   void setFileTypeData(const wstring& ftConfig);

protected:
   string initFileType{};
   int initRecordRegIndex;
   HWND hFieldList, hCaretFlash;

   INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
   void localize();
   void onJumpBtnClick();

   int getTbarPosition();
   void syncTbarToText(int optVal = FALSE);
};
