#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"
#include <regex>
#include <vector>

using std::wregex;
using std::regex_replace;
using std::vector;
using Utils::showEditBalloonTip;

extern NppData nppData;
extern ConfigIO _configIO;

class ConfigureDialog : public StaticDialog {
public:
   ConfigureDialog() : StaticDialog() {};
   void doDialog(HINSTANCE hInst);

   void setFieldEditCaretOnFocus(HWND hEdit);
   void hiliteFieldEditPairedItem(HWND hThis, HWND hThat);
   void syncFieldEditScrolling(HWND hThis, HWND hThat);

   HWND hFilesLB, hFileEOL, hRecsLB, hRecStart, hRecRegex, hFieldLabels, hFieldWidths;

protected:
   enum move_dir {
      MOVE_DOWN = 1,
      MOVE_UP = -1
   };

   struct RecordType {
      wstring label;
      string marker;
      wstring fieldLabels;
      wstring fieldWidths;
   };

   struct FileType {
      wstring label;
      string eol;
      wstring theme;
      vector<RecordType> vRecTypes;
   };

   wstring configFile{ L"" };
   vector<FileType> vFileTypes;
   bool loadingEdits, cleanConfigFile, cleanFileVals, cleanRecVals, cleanFieldVals;

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM);
   void localize();
   void indicateCleanStatus();
   int loadConfigInfo();
   void reloadConfigInfo();
   bool promptDiscardChangesNo();
   void saveConfigInfo();
   void showEximDialog(bool bExtract);
   void initEximDialog();

   int getCurrentFileTypeIndex();
   bool getCurrentFileTypeInfo(FileType* &fileInfo);
   FileType getNewFileType();
   void getFileTypeConfig(size_t idxFT, bool cr_lf, wstring &ftCode, wstring &ftConfig);

   int getCurrentRecIndex();
   bool getCurrentRecInfo(RecordType* &recInfo);
   RecordType getNewRec();

   void fillFileTypes();
   void onFileTypeSelect();
   void enableMoveFileButtons();
   void enableFileSelection();
   int moveFileType(move_dir dir);
   void fileEditAccept();
   void fileEditNew();
   int fileEditDelete();

   void fillRecTypes();
   void onRecTypeSelect();
   void enableMoveRecButtons();
   void enableRecSelection();
   int moveRecType(move_dir dir);
   void onRecStartEditChange();
   void onRecRegexEditChange();
   void recEditAccept();
   void recEditNew();
   int recEditDelete();

   void fillFieldTypes();
   void fieldEditsAccept();

   int editLabelsCaret{}, editWidthsCaret{};
};


