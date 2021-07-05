#pragma once

#include "../NPP/StaticDialog.h"
#include "VisualizerPanel.h"
#include <ShlObj_core.h>

#define LINE_ITEM_COUNT 10
#define MAX_TEMPLATE_NAME 50

extern NppData nppData;
extern ConfigIO _configIO;

typedef VisualizerPanel::RecordInfo RecordInfo;

class DataExtractDialog : public StaticDialog {
public:
   DataExtractDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   void initDialog(const wstring fileType, const vector<RecordInfo>& recInfoList);

protected:
   int currentLineItem{};
   wstring initDocFileType{}, extractsConfigFile{};
   const vector<RecordInfo>* pRecInfoList;

   HWND hIndicator;

   struct LineItemInfo {
      string prefix;
      size_t recType;
      size_t fieldType;
      string suffix;
   };

   vector <LineItemInfo> validLineItems;

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
   void localize();
   void initLineItems();
   void initLineItemFieldList(int line);
   void moveIndicators(int line, bool focusPrefix);
   void resetDropDown(HWND hList);

   void addLineItem(int line);
   void delLineItem(int line);
   void clearLineItem(int line);
   void copyLineItem(int fromLine, int toLine);
   bool getLineItem(int line, LineItemInfo& lineItem);
   void setLineItem(int line, LineItemInfo& lineItem);
   void swapLineItems(int lineFrom, int lineTo);

   size_t getReconciledLineItems(bool activateNLT);
   void extractData();

   int loadTemplatesList();
   void loadTemplate();
   void enableSaveTemplate();
   void enableDeleteTemplate();
   void saveTemplate();
   void newTemplate();
   void deleteTemplate();
};
