#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"
#include <regex>
#include <vector>

using std::vector;

extern NppData nppData;
extern ConfigIO _configIO;

class FoldStructDialog : public StaticDialog {
public:
   FoldStructDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);
   void refreshDarkMode();

private:
   enum move_dir {
      MOVE_DOWN = 1,
      MOVE_UP = -1
   };

   // Type Info
   struct TypeInfo {
      string type{};
      wstring label{};
   };

   vector<TypeInfo> vFileTypes{};
   vector<TypeInfo> vRecTypes{};

   // Block Info
   struct BlockInfo {
      TypeInfo hdrRec{};
      int priority{};
      bool recursive{};
      vector<TypeInfo> vEndRecs{};
   };

   // Fold Struct Info
   struct FoldStructInfo {
      TypeInfo fileType{};
      bool autoFold{};
      vector<BlockInfo> vBlocks{};
   };

   vector<FoldStructInfo> vFoldStructs{};

   HWND hFoldStructs{}, hFTList{}, hFoldBlocks{}, hHdrRTList{}, hImplRecs{}, hExplRecs{}, hExplRTList{};
   wstring structsFile{};
   bool loadingEdits{}, cleanStructsFile{TRUE}, cleanStructVals{TRUE}, cleanBlockVals{TRUE}, cleanEndRecVals{TRUE};

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM);

   void localize();
   void indicateCleanStatus();
   bool promptDiscardChangesNo();
   void saveFoldStructInfo();
   void showEximDialog(bool bExtract);

   int getCurrentFoldStructIndex();
   bool getCurrentFoldStructInfo(FoldStructInfo*& structInfo);
   int getFoldStructInfo(size_t idxFT, bool cr_lf, wstring& ftCode, wstring& ftConfig);

   int getCurrentBlockIndex();
   bool getCurrentBlockInfo(BlockInfo*& blockInfo);

   int loadFileTypesList();
   int loadRecTypesList(string fileType);

   int loadStructsInfo();
   int loadFoldStructInfo(int vIndex, const wstring& sStructsFile);
   void fillFoldStructs();
   void onFoldStructSelect();
   void onFoldStructSelectFill(FoldStructInfo* fsInfo);
   void enableMoveStructButtons();
   void enableStructSelection();
   int moveStructType(move_dir dir);
   int structEditAccept(bool accept = TRUE);
   void structEditNew();
   void structEditClone();
   int structEditDelete();

   void fillFoldBlocks();
   void onFoldBlockSelect();
   void onFoldBlockSelectFill(BlockInfo* blockInfo);
   void enableMoveBlockButtons();
   void enableBlockSelection();
   int moveBlockType(move_dir dir);
   int blockEditAccept(bool accept = TRUE);
   void blockEditNew(bool clone);
   int blockEditDelete();

   void fillImplicitEndRecs();
   void fillExplicitEndRecs(BlockInfo* blockInfo);
   void onEndRecSelect();
   void onEndRecSelectFill();
   void enableEndRecSelection();
   int endRecEditAccept(bool accept = TRUE);
   void endRecEditNew();
   int endRecEditDelete();
};

