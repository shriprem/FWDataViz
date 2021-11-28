#pragma once
#include "../Utils.h"
#include "../ConfigIO.h"

extern NppData nppData;
extern ConfigIO _configIO;

class StyleDefComponent {
public:
   ~StyleDefComponent();

protected:
   bool cleanStyleDefs, styleDefColor;

   HWND _hDialog;
   HBRUSH hbr;
   COLORREF styleBack, styleFore, customColors[16];

   void initComponent(HWND hDlg);
   void localize();
   int getStyleDefColor(bool back);
   void setStyleDefColor(bool setEdit, int color, bool back);
   void setOutputFontStyle();
   void fillStyleDefs(StyleInfo& style);

   void setPangram();
   INT_PTR colorStaticControl(WPARAM wParam, LPARAM lParam);
   void chooseStyleDefColor(bool back);
};

LRESULT CALLBACK procHexColorEditControl(HWND hwnd, UINT messageId, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);
