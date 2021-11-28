#pragma once

#include "StyleDefComponent.h"
#include "../NPP/StaticDialog.h"

class FieldTypeDialog : public StaticDialog, StyleDefComponent {
public:
   FieldTypeDialog() : StaticDialog() {};

   void doDialog(HINSTANCE hInst);

private:
   bool loadingEdits, newFieldDef;
   string fieldDefConfigFile{}, fieldDefStyle;
   wstring fieldDefLabel, fieldDefRegex;

   HWND hFieldsLB;

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

   void setStyleDefColor(bool setEdit, int color, bool back);
   void fillStyleDefs();
   void styleDefSave();

   void chooseStyleDefColor(bool back);
};

