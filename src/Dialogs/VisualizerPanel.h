#pragma once

#include "../Utils.h"
#include "../ConfigIO.h"
#include "../SubmenuManager.h"
#include "../NPP/DockingDlgInterface.h"
#include "../NPP/menuCmdID.h"
#include <regex>

#define FW_DEBUG_SET_STYLES FALSE
#define FW_DEBUG_LOAD_REGEX FALSE
#define FW_DEBUG_APPLY_LEXER FALSE
#define FW_DEBUG_APPLIED_STYLES FALSE
#define FW_DEBUG_LEXER_COUNT FALSE
#define FW_DEBUG_COPY_TRIM FALSE
#define FW_DEBUG_DOC_INFO FALSE
#define FW_DEBUG_FOLD_INFO FALSE

constexpr int FW_STYLE_THEMES_START_INDEX{ STYLE_LASTPREDEFINED + 1 };
constexpr int FW_STYLE_THEMES_MAX_ITEMS{ 200 };
constexpr int FW_STYLE_FIELDS_MIN_INDEX{ FW_STYLE_THEMES_START_INDEX + 36 };
constexpr int FW_TIP_LONG{ 30 };
constexpr int FW_TIP_MEDIUM{ 20 };
constexpr int FW_TIP_SHORT{ 10 };
constexpr int FOLDING_MARGIN{ 3 };

static bool idemPotentKey{ FALSE };

extern NppData nppData;
extern ConfigIO _configIO;

using std::regex;
using std::vector;

class VisualizerPanel : public DockingDlgInterface {
public:
   struct RecordInfo {
      wstring label;
      string marker;
      regex regExpr;
      wstring theme;
      vector<int> fieldStarts;
      vector<int> fieldWidths;
      vector<wstring> fieldLabels;
      vector<int> fieldStyles;
   };

   VisualizerPanel() :DockingDlgInterface(IDD_VISUALIZER_DOCKPANEL) {};
   ~VisualizerPanel() { if (hbr != NULL) DeleteObject(hbr); };

   void initPanel();
   virtual void display(bool toShow = TRUE);
   void refreshDarkMode();
   void initMBCharsCheckbox();
   void updateHopRightTip();
   void setParent(HWND parent2set);
   void setFocusOnEditor();

   void loadListFileTypes();
   bool getDocFileType(string& fileType);
   void loadListThemes() const;
   void onBufferActivate();
   void renderScrolledPage(void* view);
   void renderCurrentPage();
   void visualizeFile(string fileType, bool bCachedFT, bool bAutoFT, bool bSyncFT);
   void delDocInfo(intptr_t bufferID);

   void showConfigDialog();
   void showThemeDialog();
   void showJumpDialog();
   void showAboutDialog();

   void jumpToField(const string fileType, const int recordIndex, const int fieldIdx);
   void fieldLeft();
   void fieldRight();
   void fieldCopy();
   void fieldPaste();
   void showExtractDialog();

#if FW_DEBUG_LEXER_COUNT
   int lexCount{};
#endif

private:
   HWND hFTList{}, hThemesLB{}, hFieldInfo{}, hTipIniFiles{}, hTipHopRight{};
   HBRUSH hbr{};

   // Field Info tracking
   intptr_t caretRecordStartPos{}, caretRecordEndPos{}, caretEolMarkerPos{};
   int caretRecordRegIndex{}, caretFieldIndex{};
   bool panelMounted{}, unlexed{}, utf8Config{}, leftAlign{}, themeEnabled{}, fieldEnabled{};
   string calltipText{}; // Needed for the PostMessage call

   // File Type Info
   struct FileTypeInfo {
      string fileType{};
      wstring fileLabel{};
   };

   vector<FileTypeInfo> vFileTypes{};

   // Doc Info
   struct DocInfo {
      wstring fileName{};
      string docType{};
      string docTheme{};
      string foldStructType{};
      bool folded{};
   };

   vector<DocInfo> vDocInfo{};

   // Styleset data
   struct ThemeInfo {
      wstring name{};
      int styleCount{};
      int rangeStartIndex{};
   };

   vector<ThemeInfo> vThemes{};
   int loadedStyleCount{};

   // Regex data
   string fwVizRegexed{};

   vector<RecordInfo> vRecInfo;

   virtual INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
   void localize();
   bool detectFileType(HWND hScintilla, string& fileType);
   bool detectFileTypeByVizConfig(HWND hScintilla, string& fileType, bool defaultConfig);
   const wstring getCurrentFileName();
   void setDocInfo(bool bDocType, string val);

   bool getDocTheme(wstring& theme);
   string getDocFoldStructType();
   bool getDocFolded();
   void setDocFileType(string fileType);
   void setDocTheme(string theme, string fileType = "");
   void setDocFoldStructType(string foldStructType);
   void setDocFolded(bool bFolding);
   void setADFTCheckbox();
   void setPanelMBCharState();
   void setPanelMBCharIndicator(string fileType);
   void setDefaultBackground();
   void setShowCalltip();
   void initCalltipStyle();

   void enableThemeList(bool enable);
   void syncListFileTypes();
   void syncListThemes();
   void enableFieldControls(bool enable);

   void clearVisualize(bool sync = TRUE);
   int loadTheme(const wstring theme);
   int loadUsedThemes();
   int loadLexer();
   void applyLexer(const intptr_t startLine, intptr_t endLine);
   void clearLexer();
   void visualizeTheme();

   void displayCaretFieldInfo(const intptr_t startLine, const intptr_t endLine);
   void clearCaretFieldInfo();
   void onPanelResize(LPARAM lParam);
   int getFieldEdges(const string fileType, const int fieldIdx, const int rightPullback, intptr_t& leftPos, intptr_t& rightPos);
   void moveToFieldEdge(const string fileType, const int fieldIdx, bool jumpTo, bool rightEdge, bool hilite);
   void setFieldAlign(bool left);
   void popupSamplesMenu();

   string detectFoldStructType(string fileType);
   void applyFolding(string fsType);
   void removeFolding();
   void enableFoldableControls(bool bFoldable);
   void enableFoldedControls(bool bFolded);
   void toggleFolding();
   int foldLevelFromPopup(bool bFold);
   void expandFoldLevel(bool bExpand, int level);
   void foldLevelMenu();
   void unfoldLevelMenu();
   void showFoldStructDialog();

   static DWORD WINAPI threadPositionHighlighter(void*);
};
