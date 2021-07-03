#pragma once

#include "../NPP/StaticDialog.h"
#include "VisualizerPanel.h"
#include <ShlObj_core.h>

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
   int currentLine{};
   wstring initDocFileType{};
   const vector<RecordInfo>* pRecInfoList;

   HWND hIndicator;

   struct LineItem {
      wstring prefix;
      int recType;
      int fieldType;
      wstring suffix;
   };

   vector <LineItem> lineItemList;
   vector <LineItem> scanItemList;

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
   void localize();
   void initDisplayFields();
   void setLineItemFields(int line);
   void resetDropDown(HWND hList);
   void addLineItem(int line);
   void delLineItem(int line);
   void clearLineItem(int line);
   void copyLineItem(int fromLine, int toLine);
   void swapLineItems(int lineFrom, int lineTo);
   void moveIndicators(int line, bool focusPrefix);
};
