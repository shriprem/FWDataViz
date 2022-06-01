#pragma once

#include "../NPP/StaticDialog.h"
#include "VisualizerPanel.h"

constexpr int LINES_PER_PAGE{ 10 };
constexpr int MAX_PAGES{ 3 };
constexpr int MAX_BUFFER_LINES{ LINES_PER_PAGE * MAX_PAGES };
constexpr int MAX_TEMPLATE_NAME{ 50 };

extern NppData nppData;
extern ConfigIO _configIO;

typedef VisualizerPanel::RecordInfo RecordInfo;

class DataExtractDialog : public StaticDialog {
public:
   DataExtractDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   void initDialog(const string fileType, const vector<RecordInfo>& recInfoList);
   void refreshDarkMode();
   bool processKey(HWND hCtrl, WPARAM wParam);
   bool processSysKey(HWND hCtrl, WPARAM wParam);

private:
   int currentLineItem{}, currentPage{};
   string extractsConfigFile{}, initFileType{};
   wstring initFileTypeLabel{};
   const vector<RecordInfo>* pRecInfoList{};

   HWND hIndicator{}, hTemplatesList{}, hTemplateName{};

   struct LineItemInfo {
      wstring prefix;
      int recType{};
      int fieldType{};
      wstring suffix;
   };
   vector<LineItemInfo> liBuffer{};

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
   void localize();
   void initRecTypeLists();
   void initLineItemFieldList(int line);
   void moveIndicators(int line, bool focusPrefix);
   void resetDropDown(HWND hList);

   bool isBlankLineItem(const LineItemInfo& lineItem);
   void addLineItem(int line);
   void delLineItem(int line);
   void clearLineItem(int line);
   void getLineItem(int line, LineItemInfo& lineItem);
   void setLineItem(int line, LineItemInfo& lineItem);
   void swapLineItems(int lineFrom, int lineTo);
   void gotoLine(int ctrlID, int lineTo);

   size_t getValidLineItems(vector<LineItemInfo>& validLIs, bool validFieldType, bool activateNLT);
   void extractData();

   int loadTemplatesList();
   void loadTemplate();
   string getSelectedTemplate();
   string getTemplateName();
   void enableSaveTemplate();
   void enableDeleteTemplate();
   void saveTemplate();
   void newTemplate();
   void deleteTemplate();

   int getPageCount(int items = 0);
   void loadPage(int pageNum);
   void readPage();
   void previousPage();
   void nextPage();
   void addPage();
   void deletePage();
   void enablePageButtons();
};
