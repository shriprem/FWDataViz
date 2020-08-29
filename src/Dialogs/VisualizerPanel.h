//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#ifndef VISUALIZER_DLG_H
#define VISUALIZER_DLG_H

#include "../PluginDefinition.h"
#include "../NPP/DockingDlgInterface.h"

extern NppData nppData;
extern ConfigIO _configIO;

static bool idemPotentKey {FALSE};

class VisualizerPanel : public DockingDlgInterface {
public :
   int instance_id{ 0 };
   VisualizerPanel() :DockingDlgInterface(IDD_VISUALIZER_DOCKPANEL) {};

   void localize();
   virtual void display(bool toShow=true);
   void setParent(HWND parent2set);
   void loadPreferences();
   void loadFileTypes();

protected :
   virtual INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);

   static HWND getCurrentScintilla();
   static int setFocusOnEditor();
   char callTip[500];
};

#endif //VISUALIZER_DLG_H
