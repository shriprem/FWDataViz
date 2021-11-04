#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../SubmenuManager.h"
#include "../NPP/DockingDlgInterface.h"
#include "../NPP/menuCmdID.h"
#include <regex>

#define FW_DEBUG_LOAD_STYLES FALSE
#define FW_DEBUG_SET_STYLES FALSE
#define FW_DEBUG_LOAD_REGEX FALSE
#define FW_DEBUG_APPLY_LEXER FALSE
#define FW_DEBUG_LEXER_COUNT FALSE

#define FW_DOC_FILE_TYPE "FWVisualizerType"
#define FW_DOC_FILE_THEME "FWVisualizerTheme"

constexpr int FW_STYLE_CACHE_START_INDEX{ STYLE_LASTPREDEFINED + 1 };
constexpr int FW_STYLE_CACHE_ITEMS_LIMIT{ 200 };

static bool idemPotentKey{ FALSE };

extern NppData nppData;
extern ConfigIO _configIO;

using std::regex;
using std::vector;

class VisualizerPanel : public DockingDlgInterface {
public :
   struct RecordInfo {
      wstring label;
      string marker;
      regex regExpr;
      wstring theme;
      vector<int> fieldStarts;
      vector<int> fieldWidths;
      vector<wstring> fieldLabels;
   };

   VisualizerPanel() :DockingDlgInterface(IDD_VISUALIZER_DOCKPANEL) {};

   void initPanel();
   virtual void display(bool toShow=true);
   void setParent(HWND parent2set);
   void setFocusOnEditor();
   void showCaretFramedState(bool framed);

   void loadListFileTypes();
   bool getDocFileType(HWND hScintilla, string& fileType);
   bool getDocFileType(PSCIFUNC_T sci_func, void* sci_ptr, string& fileType);
   void loadListThemes();
   void onBufferActivate();
   void renderCurrentPage();
   void visualizeFile(string fileType, bool ab_cachedFT, bool autoFT, bool syncFT);

   void showJumpDialog();
   void jumpToField(const string fileType, const int recordIndex, const int fieldIdx);
   void fieldLeft();
   void fieldRight();
   void fieldCopy();
   void fieldPaste();
   void showExtractDialog();

#if FW_DEBUG_LEXER_COUNT
   int lexCount{};
#endif

protected :
   HWND hFTList, hThemesLB, hFieldInfo;

   // Field Info tracking
   int caretRecordStartPos, caretRecordEndPos, caretRecordRegIndex, caretEolMarkerPos, caretFieldIndex;
   bool leftAlign{};

   // File Type data
   std::unordered_map<wstring, string> mapFileDescToType;
   std::unordered_map<string, wstring> mapFileTypeToDesc;

   // Styleset data
   struct ThemeInfo {
      wstring name{};
      StyleInfo styleEOL;
      int rangeStartIndex;
      vector<StyleInfo> styleSet;
   };

   vector<ThemeInfo> themeSet;
   int loadedStyleCount;

   // Regex data
   string fwVizRegexed{};

   vector<RecordInfo> recInfoList;

   virtual INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
   void localize();
   bool detectFileType(HWND hScintilla, string& fileType);
   bool getDocTheme(HWND hScintilla, wstring& theme);
   bool getDocTheme(PSCIFUNC_T sci_func, void* sci_ptr, wstring& theme);
   void setDocFileType(HWND hScintilla, string fileType);
   void setDocTheme(HWND hScintilla, string fileType, string theme);
   void setADFTCheckbox();
   void setPanelMBCharState();
   void setPanelMBCharIndicator(string fileType);

   void enableThemeList(bool enable);
   void syncListFileTypes();
   void syncListThemes();
   void enableFieldControls(bool enable);

   void clearVisualize(bool sync=TRUE);
   int loadTheme(const wstring theme);
   int loadUsedThemes();
   int applyStyles();
   int loadLexer();
   void applyLexer(const size_t startLine, const size_t endLine);
   void clearLexer();
   void visualizeTheme();

   void displayCaretFieldInfo(const size_t startLine, const size_t endLine);
   void clearCaretFieldInfo();
   void resizeCaretFieldInfo(int width);
   int getFieldEdges(const string fileType, const int fieldIdx, const int rightPullback, int& leftPos, int& rightPos);
   void moveToFieldEdge(const string fileType, const int fieldIdx, bool rightEdge, bool hilite);
   void setFieldAlign(bool left);
   void popupSamplesMenu();

   static DWORD WINAPI threadPositionHighlighter(void*);
};
