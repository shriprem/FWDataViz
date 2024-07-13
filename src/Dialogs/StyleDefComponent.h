#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"

extern NppData nppData;
extern ConfigIO _configIO;

class StyleDefComponent {
public:
   ~StyleDefComponent();

protected:
   void initComponent(HWND hDlg);
   void localize() const;
   int getStyleDefColor(bool back) const;
   void setStyleDefColor(bool setEdit, int color, bool back);
   void setOutputFontStyle() const;
   void fillStyleDefs(StyleInfo& style);

   void setPangram() const;
   INT_PTR colorStaticControl(WPARAM wParam, LPARAM lParam);
   void chooseStyleDefColor(bool back);

   bool cleanStyleDefs{ true }, styleDefColor{};

   HWND _hDialog{};
   HBRUSH hbr{};
   COLORREF styleBack{}, styleFore{}, customColors[16]{};
};

LRESULT CALLBACK procHexColorEditControl(HWND hwnd, UINT messageId, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);
