#ifndef _ABOUTDIALOG_H
#define _ABOUTDIALOG_H

#define PLUGIN_VERSION              L"Version 1.1.0.0"
#define PLUGIN_ATTRIBUTION          L"(c) 2020, Shridhar Kumar"
#define PLUGIN_GITHUB_PAGE          L"https://github.com/shriprem/Goto-Line-Col-NPP-Plugin"

#include "../PluginDefinition.h"
#include "../NPP/StaticDialog.h"

extern NppData nppData;

class AboutDialog : public StaticDialog
{
public:
   AboutDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);

protected:
   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam);
   void localize();
};

#endif // _ABOUTDIALOG_H
