#pragma once
#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"

extern NppData nppData;
extern ConfigIO _configIO;

class FieldTypeDialog : public StaticDialog {
public:
   FieldTypeDialog() : StaticDialog() {};
   ~FieldTypeDialog();

   void doDialog(HINSTANCE hInst);

private:
   bool loadingEdits, newFieldDef, cleanStyleDefs, styleDefColor;
   string fieldDefConfigFile{}, fieldDefStyle;
   wstring fieldDefLabel, fieldDefRegex;

   HWND hFieldsLB;
   HBRUSH hbr;
   COLORREF styleBack, styleFore, customColors[16];

   INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
   void localize();

   int getCurrentFieldIndex();
   string getNewStyle();
   string getStyleConfig();

   void fillFields();
   void onFieldSelect();
   void enableFieldSelection();
   void fieldEditNew();
   void fieldEditClone();
   void fieldEditDelete();

   int getStyleDefColor(bool back);
   void setStyleDefColor(bool setEdit, int color, bool back);
   void setOutputFontStyle();
   void fillStyleDefs();
   void styleDefSave();

   void setPangram();
   INT_PTR colorStaticControl(WPARAM wParam, LPARAM lParam);
   void chooseStyleDefColor(bool back);
};

