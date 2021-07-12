#pragma once

#include "../NPP/StaticDialog.h"
#include "VisualizerPanel.h"

#define LINES_PER_PAGE 10
#define LINES_BUFFER_LIMIT 30
#define MAX_TEMPLATE_NAME 50

extern NppData nppData;
extern ConfigIO _configIO;

typedef VisualizerPanel::RecordInfo RecordInfo;

class DataExtractDialog : public StaticDialog {
public:
   DataExtractDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   void initDialog(const wstring fileType, const vector<RecordInfo>& recInfoList);
   bool processKey(HWND hCtrl, WPARAM wParam);
   bool processSysKey(HWND hCtrl, WPARAM wParam);

protected:
   int currentLineItem{};
   wstring initFileType{}, initFileTypeLabel{}, extractsConfigFile{};
   const vector<RecordInfo>* pRecInfoList;

   HWND hIndicator, hTemplatesList;

   struct LineItemInfo {
      string prefix;
      int recType;
      int fieldType;
      string suffix;
   };
   vector<LineItemInfo> LIBuffer{};

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

   size_t getValidLineItems(vector<LineItemInfo>& validLIs, bool validFieldType, bool activateNLT);
   void extractData();

   int loadTemplatesList();
   void loadTemplate();
   wstring getSelectedTemplate();
   wstring getTemplateName();
   void enableSaveTemplate();
   void enableDeleteTemplate();
   void saveTemplate();
   void newTemplate();
   void deleteTemplate();

};
