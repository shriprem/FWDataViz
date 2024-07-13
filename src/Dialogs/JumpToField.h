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
   void refreshDarkMode();
   void initDialog(const string fileType, int recordIndex, int fieldIndex, const vector<wstring>& fieldLabels);
   void setFileTypeData(const wstring& ftConfig);

private:
   INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
   void localize();
   void loadJumpList(int fieldIndex = -1);
   void onJumpSeqNumPref();
   void onJumpBtnClick();

   int getTbarPosition() const;
   void setTbarPosition(int val, bool savePref);

   string initFileType{};
   int initRecordRegIndex{};

   HWND hFieldList{}, hCaretFlash{};

   const vector<wstring>* pFieldLabels{};
};
