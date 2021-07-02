#pragma once

#include "../NPP/StaticDialog.h"
#include "VisualizerPanel.h"

#define DISPLAY_COUNT 10

extern NppData nppData;
extern ConfigIO _configIO;

typedef VisualizerPanel::RecordInfo RecordInfo;

class DataExtractDialog : public StaticDialog {
public:
   DataExtractDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   void initDialog(const wstring fileType, const vector<RecordInfo>& recInfoList);

protected:
   wstring initDocFileType{};
   const vector<RecordInfo>* pRecInfoList;

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
   void localize();
   void initDisplayFields();
   void onRecTypeChange(int ctrlID);
};
