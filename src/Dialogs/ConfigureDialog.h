#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../NPP/StaticDialog.h"
#include <regex>
#include <vector>

constexpr int FILE_TYPE_LIMIT{ 999 };
constexpr int REC_TYPE_LIMIT{ 999 };
const wstring REGEX_META_CHARS{ L"\\^\\$\\\\(\\)\\{\\}\\[\\]\\<\\>\\.\\?\\*\\+\\,\\-\\|\\!\\:\\=" };

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
   void display(bool toShow = TRUE);
   void refreshDarkMode();

   void setFieldEditCaretOnFocus(HWND hEdit);
   void hiliteFieldEditPairedItem(HWND hThis, HWND hThat);
   void syncFieldEditScrolling(HWND hThis, HWND hThat);

   HWND hFieldLabels{}, hFieldWidths{};

private:
   enum move_dir {
      MOVE_DOWN = 1,
      MOVE_UP = -1
   };

   struct RecordType {
      wstring label;
      wstring marker;
      wstring fieldLabels;
      wstring fieldWidths;
      wstring theme{};
   };

   struct FileType {
      wstring label{};
      wstring theme{};
      wstring eol{};
      bool multiByte{};
      vector<RecordType> vRecTypes;
      int lineNums[ADFT_MAX]{};
      wstring regExprs[ADFT_MAX]{};
   };

   INT_PTR CALLBACK run_dlgProc(UINT Message, WPARAM wParam, LPARAM);
   void localize();
   void indicateCleanStatus();
   int loadConfigInfo();
   int loadFileTypeInfo(int vIndex, const string& fileType, const wstring& sConfigFile);
   bool promptDiscardChangesNo();
   void saveConfigInfo();
   void showEximDialog(bool bExtract);
   int appendFileTypeConfigs(const wstring& sConfigFile);
   wstring getOnlyStartsWith(wstring expr);

   int getCurrentFileTypeIndex() const;
   bool getCurrentFileTypeInfo(FileType*& fileInfo);
   FileType getNewFileType();
   int getFileTypeConfig(size_t idxFT, bool cr_lf, wstring& ftCode, wstring& ftConfig);

   int getCurrentRecIndex() const;
   bool getCurrentRecInfo(RecordType*& recInfo);
   RecordType getNewRec();

   void fillFileTypes();
   void onFileTypeSelect();
   void onFileTypeSelectFill(FileType* fileInfo);
   void enableMoveFileButtons();
   void enableFileSelection();
   int moveFileType(move_dir dir);
   int fileEditAccept(bool accept = true);
   void fileEditNew();
   void fileEditClone();
   int fileEditDelete();
   bool checkFTLimit(bool clone);

   void fillRecTypes();
   void onRecTypeSelect();
   void onRecTypeSelectFill(RecordType* recInfo);
   void enableMoveRecButtons();
   void enableRecSelection();
   int moveRecType(move_dir dir);
   void onRecStartEditChange() const;
   void onRecRegexEditChange();
   int recEditAccept(bool accept = true);
   void recEditNew(bool clone);
   int recEditDelete();

   void fillFieldTypes();
   void fieldEditsAccept();

   wstring configFile{};
   bool loadingEdits{}, cleanConfigFile{ true }, cleanFileVals{ true }, cleanRecVals{ true }, cleanFieldVals{ true };
   int editLabelsCaret{}, editWidthsCaret{};

   HWND hFilesLB{}, hFileEOL{}, hFileThemes{}, hADFTLine[ADFT_MAX]{}, hADFTRegex[ADFT_MAX]{},
      hRecsLB{}, hRecStart{}, hRecRegex{}, hRecThemes{};

   vector<FileType> vFileTypes;
};

LRESULT CALLBACK procNumberEditControl(HWND hwnd, UINT messageId, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);
LRESULT CALLBACK procFieldEditMessages(HWND hwnd, UINT messageId, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);
