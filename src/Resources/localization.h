﻿#pragma once

// Common
#define MENU_PANEL_NAME             L"Fixed-Width Data Visualizer"
#define MENU_SHOW_PANEL             L"Show FWData&Viz Panel"
#define MENU_CONFIG_FILE_TYPES      L"Configure &File Types"
#define MENU_CONFIG_THEMES          L"Configure Color &Themes"
#define MENU_FIELD_JUMP             L"&Jump to Field"
#define MENU_FIELD_COPY             L"Field &Copy"
#define MENU_FIELD_PASTE            L"Field &Paste"
#define MENU_FIELD_LEFT             L"Hop to &Left edge of the Field"
#define MENU_FIELD_RIGHT            L"Hop to &Right edge of the Field"
#define MENU_DATA_EXTRACTION        L"&Data Extraction"
#define MENU_DEMO_SINGLE_REC_FILES  L"Sample &Single-Record-Type Files"
#define MENU_DEMO_MULTI_REC_FILES   L"Sample &Multiple-Record-Type Files"
#define MENU_DEMO_MULTI_LINE_FILES  L"Sample M&ultiple-Line Record Files"
#define MENU_ABOUT                  L"A&bout"


// Fixed Width Data Visualizer Panel
#define VIZ_PANEL_FILETYPE_LABEL    L"&File Type:"
#define VIZ_PANEL_THEME_LABEL       L"&Visualizer Theme:"
#define VIZ_PANEL_CLEAR_BUTTON      L"Cle&ar"
#define VIZ_PANEL_CLOSE             L"Clos&e"
#define VIZ_PANEL_PREFERENCES       L"Prefere&nces"
#define VIZ_PANEL_AUTO_DETECT_FT    L"Auto-detect File Type"
#define VIZ_PANEL_MCBS_OVERRIDE     L"Multi-Byte Chars"
#define VIZPANEL_DEFAULT_BACKGROUND L"Default Background"
#define VIZPANEL_SHOW_CALLTIP       L"Show Calltip"
#define VIZ_PANEL_FIELD_COPY_TRIM   L"Trim Field Copy"
#define VIZ_PANEL_FIELD_LABEL       L"Cursor Position Data"
#define VIZ_PANEL_JUMP_FIELD_BTN    L"&Jump to Field"
#define VIZ_PANEL_FIELD_COPY_BTN    L"Field &Copy"
#define VIZ_PANEL_FIELD_PASTE_BTN   L"Field &Paste"
#define VIZ_PANEL_PASTE_LEFT_LABEL  L"Left-aligned"
#define VIZ_PANEL_PASTE_RIGHT_LABEL L"Right-aligned"
#define VIZ_PANEL_PASTE_RPAD_LABEL  L"&RPAD:"
#define VIZ_PANEL_PASTE_LPAD_LABEL  L":&LPAD"
#define VIZ_PANEL_EXTRACT_DATA_BTN  L"&Data Extraction"
#define VIZPANEL_FOLD_GROUP_BOX     L"Folding"
#define VIZPANEL_FOLD_APPLY_BTN     L"Apply"
#define VIZPANEL_FOLD_REAPPLY_BTN   L"Re-Apply"
#define VIZPANEL_FOLD_REMOVE_BTN    L"Remove"
#define VIZPANEL_FOLD_DEFINE_BTN    L"Define…"
#define VIZPANEL_FOLD_FOLD_BTN      L"Fold »"
#define VIZPANEL_FOLD_TOGGLE_BTN    L"Toggle Line"
#define VIZPANEL_FOLD_UNFOLD_BTN    L"Unfold »"

#define VIZ_PANEL_INFO_TIP          L"View Readme Online"
#define VIZ_PANEL_FILE_SAMPLES_TIP  L"View Sample Files"
#define VIZ_PANEL_FILE_CONFIG_TIP   L"Open File Type Metadata Editor"
#define VIZ_PANEL_THEME_CONFIG_TIP  L"Open Visualizer Theme Editor"
#define VIZ_PANEL_CLEAR_BTN_TIP     L"If \"Auto-detect File Type\" option is:\r\n● Checked: Redo the file type detection and visualization.\r\n● Not checked: Clear the document's fixed-width visualization.\r\n\r\nTo always clear visualization, modify the setting in \"Preferences\"."
#define VIZ_PANEL_FIELD_TRIM_TIP    L"When copying a field, trim padding character(s) on either the left or right,\r\nbased on which field edge is closer to the cursor position."
#define VIZ_PANEL_FIELD_LEFT_TIP    L"Move cursor to the left edge of the current field.\r\n\r\n● If already at the left edge of the field, the cursor will be moved to the left edge of the preceding field.\r\n\r\nAccessible via keyboard shortcut. See the plugin menu."
#define VIZ_PANEL_FIELD_RIGHT_TIP   L"Move cursor to the right edge of the current field.\r\n\r\n● If already at the right edge of the field, the cursor will be moved to the right edge of the following field.\r\n\r\nUse 'Preferences' » 'Hop Right...' to change this behavior.\r\n\r\nAccessible via keyboard shortcut. See the plugin menu."
#define VIZ_PANEL_FLD_ALT_RIGHT_TIP L"Move cursor to the left edge of the following field.\r\n\r\nUse 'Preferences' » 'Hop Right...' to change this behavior.\r\n\r\nAccessible via keyboard shortcut. See the plugin menu."
#define VIZ_PANEL_FIELD_COPY_TIP    L"Copy current field data.\r\n\r\n● If \"Trim Field Copy\" option is checked, field data will be trimmed off padding character(s) on either the left or right edge, based on which field edge is closer to the cursor position.\r\n\r\nAccessible via keyboard shortcut. See the plugin menu."
#define VIZ_PANEL_FIELD_PASTE_TIP   L"Paste clipboard text into current field, replacing its current contents.\r\n\r\n● Aligned either to the left or the right edge of the field, based on which field edge is closer to the cursor position.\r\n● If the clipboard text is shorter than the field width, it will be paddded on the opposite edge of the aligned edge.\r\n● If the clipboard text is longer, it will be trimmed to the field width.\r\n\r\nAccessible via keyboard shortcut. See the plugin menu."
#define VIZ_PANEL_FIELD_RPAD_TIP    L"● When pasting, RPAD character(s) will be used for padding the right edge of the field.\r\n● If copying with \"Trim Field Copy\" option checked, characters on the right edge of the field data that match (the sequence of) the padding character(s) will be trimmed.\r\n● When RPAD is left blank, a space will be used as the padding character."
#define VIZ_PANEL_FIELD_LPAD_TIP    L"● When pasting, LPAD character(s) will be used for padding the left edge of the field.\r\n● If copying with \"Trim Field Copy\" option checked, characters on the left edge of the field data that match (the sequence of) the padding character(s) will be trimmed.\r\n● When LPAD is left blank, a space will be used as the padding character."
#define VIZ_PANEL_JUMP_FIELD_TITLE  L"Jump to Field"
#define VIZ_PANEL_JUMP_CHANGED_DOC  L"The currently active document is of a different File Type.\r\n\r\nUnable to jump to the specified field."
#define VIZ_PANEL_JUMP_CHANGED_REC  L"Caret has moved to a different Record Type.\r\n\r\nUnable to jump to the specified field."
#define VIZ_PANEL_DARKMODE_CHANGED  L"Please restart Notepad++ to refresh the FWDataViz plugin panel and dialogs with the changed Darkmode settings."
#define VIZ_PANEL_FILE_INFO_TITLE   L"Active INI Files"
#define VIZ_PANEL_FILE_INFO_TIP     L"\nNOTE: Click the button to refresh the files list"


#define CUR_POS_DATA_BLANK_LINE     L"<Blank Line>"
#define CUR_POS_DATA_UNKOWN_REC     L"<Unknown Record Type>"
#define CUR_POS_DATA_REC_END        L"<Record End>"
#define CUR_POS_DATA_REC_TERM       L"<Record Terminator>"
#define CUR_POS_DATA_REC_TYPE       L"  Record Type: "
#define CUR_POS_DATA_REC_LENGTH     L"Record Length: "
#define CUR_POS_DATA_CURR_DEFINED   L" [Current/Defined]"
#define CUR_POS_DATA_OVERFLOW       L"    Overflow!"
#define CUR_POS_DATA_FIELD_LABEL    L"  Field Label: "
#define CUR_POS_DATA_FIELD_NUM      L"Field #"
#define CUR_POS_DATA_FIELD_START    L"  Field Start: "
#define CUR_POS_DATA_FIELD_WIDTH    L"  Field Width: "
#define CUR_POS_DATA_FIELD_COL      L" Field Column: "
#define CUR_POS_DATA_ANSI_BYTE      L"    ANSI Byte: "
#define CUR_POS_DATA_UTF8_BYTES     L"  UTF-8 Bytes: "


// About Dialog
#define ABOUT_DIALOG_TITLE          L"About"
#define ABOUT_PLUGIN_NAME           L"Fixed Width Data Visualizer Plugin for Notepad++"
#define ABOUT_BTN_LABEL_OK          L"&OK"


// Configuration Dialog
#define FWVIZ_DEF_DIALOG_TITLE      L"File Type Metadata Editor"
#define FWVIZ_DEF_FILE_GROUP_BOX    L"File Type"
#define FWVIZ_DEF_FILE_MOVE_DOWN    L"Move down the selected File Type"
#define FWVIZ_DEF_FILE_MOVE_UP      L"Move up the selected File Type"
#define FWVIZ_DEF_FILE_DESC_LABEL   L"File Type - Label:"
#define FWVIZ_DEF_FILE_EOL_LABEL    L"File Type - Records Terminator:"
#define FWVIZ_DEF_MCBS_CHECKBOX     L"Multi-byte Character Data"
#define FWVIZ_DEF_ADFT_GROUP_LABEL  L"Auto-Detect File Type"
#define FWVIZ_DEF_ADFT_LINE_LABEL   L"Line No:"
#define FWVIZ_DEF_ADFT_REGEX_LABEL  L"Line Regex:"
#define FWVIZ_DEF_FILE_THEME_LABEL  L"File Type - Color Theme:"
#define FWVIZ_DEF_FILE_ACCEPT_BTN   L"Accept"
#define FWVIZ_DEF_FILE_RESET_BTN    L"Reset"
#define FWVIZ_DEF_FILE_NEW_BTN      L"New"
#define FWVIZ_DEF_FILE_CLONE_BTN    L"Clone"
#define FWVIZ_DEF_FILE_DEL_BTN      L"Delete"
#define FWVIZ_DEF_REC_GROUP_BOX     L"File Type » Record Types"
#define FWVIZ_DEF_REC_MOVE_DOWN     L"Move down the selected Record Type"
#define FWVIZ_DEF_REC_MOVE_UP       L"Move up the selected Record Type"
#define FWVIZ_DEF_REC_DESC_LABEL    L"Record Type - Label:"
#define FWVIZ_DEF_REC_START_LABEL   L"Record Type - Starts With:"
#define FWVIZ_DEF_REC_REGEX_LABEL   L"Record Type - Regex Key:"
#define FWVIZ_DEF_REC_THEME_LABEL   L"Record Type - Color Theme:"
#define FWVIZ_DEF_REC_THEME_FROM_FT L"[Use File Type - Color Theme]"
#define FWVIZ_DEF_REC_ACCEPT_BTN    L"Accept"
#define FWVIZ_DEF_REC_RESET_BTN     L"Reset"
#define FWVIZ_DEF_REC_NEW_BTN       L"New"
#define FWVIZ_DEF_REC_CLONE_BTN     L"Clone"
#define FWVIZ_DEF_REC_DEL_BTN       L"Delete"
#define FWVIZ_DEF_FIELD_GROUP_BOX   L"Record Type » Fields"
#define FWVIZ_DEF_FIELD_LABELS_TEXT L"Field Labels:"
#define FWVIZ_DEF_FIELD_WIDTHS_TEXT L"Widths:"
#define FWVIZ_DEF_FIELD_ACCEPT_BTN  L"Accept"
#define FWVIZ_DEF_FIELD_RESET_BTN   L"Reset"
#define FWVIZ_DEF_FIELD_TYPE_BTN    L"&Define Field Types"
#define FWVIZ_DIALOG_SAVE_BTN       L"&Save as Primary Configuration"
#define FWVIZ_DIALOG_RESET_BTN      L"&Reload Primary Configuration"
#define FWVIZ_DIALOG_BKUP_LOAD_BTN  L"&Load Backup Configuration"
#define FWVIZ_DIALOG_BKUP_VIEW_BTN  L"&View Backup Folder"
#define FWVIZ_DIALOG_EXTRACT_BTN    L"E&xtract Selected File Type Data"
#define FWVIZ_DIALOG_APPEND_BTN     L"&Append New File Type Data"
#define FWVIZ_DIALOG_CLOSE_BTN      L"&Close"

#define UTILS_REGEX_ERROR           L"Invalid Regular Expression"
#define FWVIZ_DIALOG_NUMERIC_TITLE  L"Numbers only!"
#define FWVIZ_DIALOG_NUMERIC_MSG    L"This edit field will only accept numbers,\r\nwith an optional leading minus sign."
#define FWVIZ_DIALOG_COMMAS_TITLE   L"Commas are item separators"
#define FWVIZ_DIALOG_COMMAS_MESSAGE L"The line will be split into multiple items when the 'Accept' button below is clicked."
#define FWVIZ_DISCARD_CHANGES       L"Discard changes?"
#define FWVIZ_DEFAULT_OVERWRITE     L"Save the Default Configuration changes as the Primary Configuration?"
#define FWVIZ_OPEN_BKUP_CONFIG_DLG  L"Open Backup Configuration File"
#define FWVIZ_SAVE_BKUP_CONFIG_DLG  L"Save Backup Configuration File"
#define FWVIZ_FT_NEW_ACTION         L"Add New File Type"
#define FWVIZ_FT_CLONE_ACTION       L"Clone Selected File Type"
#define FWVIZ_FT_APPEND_ACTION      L"Append File Type(s)"
#define FWVIZ_FT_LIMIT_ERROR        L"Only %i File Types can be defined."
#define FWVIZ_RT_NEW_ACTION         L"Add New Record Types"
#define FWVIZ_RT_CLONE_ACTION       L"Clone Selected Record Types"
#define FWVIZ_RT_LIMIT_ERROR        L"Only %i Record Types can be defined per File Type."
#define FWVIZ_DEF_MCBS_HINT_TITLE   L"Multi-byte Character Data"
#define FWVIZ_DEF_MCBS_HINT_TEXT    L"1. Check this box only if the file type is likely to contain multi-byte character data.\r\n\r\n2. When this box is checked, field widths will be calculated using character counts. And, TABs will be counted as multiple spaces (or characters).\r\n\r\n3. When this box is unchecked, field widths will be calculated using byte counts.\r\n\r\nClick the (i) icon to view the online documentation."
#define FWVIZ_DEF_ADFT_HINT_TITLE   L"ADFT in a Nutshell"
#define FWVIZ_DEF_ADFT_HINT_TEXT    L"1. Line Numbers begin at '1', counting forwards from the start of file. Negative line numbers will count backwards from the end of file. Zero and numbers (+ve or -ve) beyond file line count will fail ADFT matching.\r\n\r\n2. ADFT Regexes with no terminating '$' will be used as *starts-with* match. Whereas, those with terminating '$' will be used as *full-line* match. ADFT Regexes will have an implicit caret(^) at the start if it has not been explicitly included.\r\n\r\nClick the (i) icon to view the online ADFT documentation."
#define FWVIZ_DEF_RECTHEME_HINT_HDR L"Record-type Theme"
#define FWVIZ_DEF_RECTHEME_HINT_TXT L"Specify a different color theme than the file-type theme for select-few record types to make them distinctive.\r\n\r\nFor an example, see: 'Transaction Headers' inside the Samples » Treasury IPAC file.\r\n\r\nClick the (i) icon to view the online documentation."


// Style Definition Component
#define STYLE_DEF_GROUP_BOX         L"Style Definition"
#define STYLE_DEF_BACK_LABEL        L"Back Color:"
#define STYLE_DEF_FORE_LABEL        L"Fore Color:"
#define STYLE_DEF_BOLD              L"Bold"
#define STYLE_DEF_ITALICS           L"Italics"
#define STYLE_DEF_PREVIEW_LABEL     L"Output:"
#define STYLE_DEF_HEX_TITLE         L"Hexadecimal characters only!"
#define STYLE_DEF_HEX_CHARS_ONLY    L"This edit field will only accept characters in the following range: 0-9|A-F|a-f"


// Theme Dialog
#define THEME_DIALOG_TITLE          L"Visualizer Theme Editor"
#define THEME_DEF_GROUP_BOX         L"Themes"
#define THEME_DEF_MOVE_DOWN         L"Move down the selected Theme"
#define THEME_DEF_MOVE_UP           L"Move up the selected Theme"
#define THEME_DEF_DESC_LABEL        L"Theme - Label:"
#define THEME_DEF_ACCEPT_BTN        L"Accept"
#define THEME_DEF_RESET_BTN         L"Reset"
#define THEME_DEF_NEW_BTN           L"New"
#define THEME_DEF_CLONE_BTN         L"Clone"
#define THEME_DEF_DEL_BTN           L"Delete"
#define THEME_STYLE_GROUP_BOX       L"Theme » Styles"
#define THEME_STYLE_MOVE_DOWN       L"Move down the selected Style"
#define THEME_STYLE_MOVE_UP         L"Move up the selected Style"
#define THEME_STYLE_CLONE_BTN       L"Clone"
#define THEME_STYLE_NEW_BTN         L"New"
#define THEME_STYLE_DEL_BTN         L"Delete"
#define THEME_STYLE_DEF_ACCEPT_BTN  L"Accept"
#define THEME_STYLE_DEF_RESET_BTN   L"Reset"
#define THEME_DIALOG_SAVE_BTN       L"&Save as Primary Themes File"
#define THEME_DIALOG_RESET_BTN      L"&Reload Primary Themes File"
#define THEME_DIALOG_BKUP_LOAD_BTN  L"&Load Backup Themes File"
#define THEME_DIALOG_BKUP_VIEW_BTN  L"&View Backup Folder"
#define THEME_DIALOG_EXTRACT_BTN    L"E&xtract Selected Theme Data"
#define THEME_DIALOG_APPEND_BTN     L"&Append New Theme Data"
#define THEME_DIALOG_CLOSE_BTN      L"&Close"

#define THEME_DEF_NEW_ACTION        L"Add New Theme"
#define THEME_DEF_CLONE_ACTION      L"Clone Selected Theme"
#define THEME_DEF_APPEND_ACTION     L"Append Theme(s)"
#define THEME_DEF_LIMIT_ERROR       L"Only %i Themes can be defined."
#define THEME_STYLE_NEW_ACTION      L"Add New Style"
#define THEME_STYLE_CLONE_ACTION    L"Clone Selected Style"
#define THEME_STYLE_LIMIT_ERROR     L"Only %i Styles can be defined per Theme."

#define THEME_DISCARD_CHANGES       L"Discard changes?"
#define THEME_OPEN_BKUP_CONFIG_DLG  L"Open Backup Themes File"
#define THEME_SAVE_BKUP_CONFIG_DLG  L"Save Backup Themes File"


// Fold Structures Editor Dialog
#define FOLD_STRUCT_DIALOG_TITLE    L"Fold Structures Editor"
#define FOLD_DEF_FILE_GROUP_BOX     L"Fold Structures"
#define FOLD_DEF_FILE_MOVE_DOWN     L"Move down the selected Fold Structure"
#define FOLD_DEF_FILE_MOVE_UP       L"Move up the selected Fold Structure"
#define FOLD_DEF_FILE_TYPE_LABEL    L"Fold Structure - File Type:"
#define FOLD_DEF_AUTO_FOLD_APPLY    L"Automatically apply Fold Structure \t\ton matching files"
#define FOLD_DEF_FILE_ACCEPT_BTN    L"Accept"
#define FOLD_DEF_FILE_RESET_BTN     L"Reset"
#define FOLD_DEF_FILE_NEW_BTN       L"New"
#define FOLD_DEF_FILE_RESET_BTN     L"Reset"
#define FOLD_DEF_FILE_CLONE_BTN     L"Clone"
#define FOLD_DEF_FILE_DEL_BTN       L"Delete"
#define FOLD_DEF_HDR_REC_GROUP_BOX  L"Fold Structure » Fold Blocks"
#define FOLD_DEF_HDR_REC_MOVE_DOWN  L"Move down the selected Fold Block"
#define FOLD_DEF_HDR_REC_MOVE_UP    L"Move up the selected Fold Block"
#define FOLD_DEF_HDR_REC_TYPE_LABEL L"Fold Block - Header Record Type:"
#define FOLD_DEF_HDR_PRIORITY_LABEL L"Fold Block - Priority: "
#define FOLD_DEF_HDR_REC_RECURSIVE  L"Recursive Fold Block"
#define FOLD_DEF_HDR_REC_ACCEPT_BTN L"Accept"
#define FOLD_DEF_HDR_REC_RESET_BTN  L"Reset"
#define FOLD_DEF_HDR_REC_NEW_BTN    L"New"
#define FOLD_DEF_HDR_REC_RESET_BTN  L"Reset"
#define FOLD_DEF_HDR_REC_CLONE_BTN  L"Clone"
#define FOLD_DEF_HDR_REC_DEL_BTN    L"Delete"
#define FOLD_IMPLICIT_TRMNTRS_GROUP L"Fold Block » Implicit Terminators"
#define FOLD_IMPLICIT_END_OF_FILE   L"<End of File>"
#define FOLD_EXPLICIT_TRMNTRS_GROUP L"Fold Block » Explicit Terminators"
#define FOLD_EXPLICIT_ENDREC_LABEL  L"Explicit Terminator - Record Type:"
#define FOLD_EXPLICIT_ENDREC_ACCEPT L"Accept"
#define FOLD_EXPLICIT_ENDREC_RESET  L"Reset"
#define FOLD_EXPLICIT_ENDREC_NEW    L"New"
#define FOLD_EXPLICIT_ENDREC_RESET  L"Reset"
#define FOLD_EXPLICIT_ENDREC_DEL    L"Delete"
#define FOLD_DEF_SAVE_BTN           L"&Save as Primary Fold Structs"
#define FOLD_DEF_RESET_BTN          L"&Reload Primary Fold Structs"
#define FOLD_DEF_BACKUP_LOAD_BTN    L"&Load Backup Fold Structs"
#define FOLD_DEF_BACKUP_VIEW_BTN    L"&View Backup Folder"
#define FOLD_DEF_EXTRACT_BTN        L"E&xtract Selected Fold Structure"
#define FOLD_DEF_APPEND_BTN         L"&Append New Fold Structure"

#define FOLD_DEF_DISCARD_CHANGES    L"Discard changes?"
#define FOLD_DEF_OPEN_BKUP_CONFIG   L"Open Backup Fold Structures File"
#define FOLD_DEF_SAVE_BKUP_CONFIG   L"Save Backup Fold Structures File"


// Define Field Type Dialog
#define FIELD_TYPE_DEF_DLG_TITLE    L"Define Field Types"
#define FIELD_TYPE_GROUP_BOX        L"Field &Types"
#define FIELD_TYPE_NEW_BTN          L"&New"
#define FIELD_TYPE_CLONE_BTN        L"&Clone"
#define FIELD_TYPE_DEL_BTN          L"&Delete"
#define FIELD_TYPE_DESC_LABEL       L"Field Type &Label:"
#define FIELD_TYPE_REGEX_LABEL      L"Field &Validation Regex:"
#define FIELD_STYLE_DEF_SAVE_BTN    L"&Save"
#define FIELD_STYLE_DEF_RESET_BTN   L"&Reset"


// Exim File Type Dialog
#define EXIM_APPEND_FT_TITLE        L"Append File Type Metadata"
#define EXIM_EXTRACT_FT_TITLE       L"Extract File Type Metadata"
#define EXIM_EDIT_FT_LABEL          L"File Type Metadata:"
#define EXIM_APPEND_FT_BTN          L"&Append File Type(s)"

#define EXIM_APPEND_THEME_TITLE     L"Append Visualizer Theme Data"
#define EXIM_EXTRACT_THEME_TITLE    L"Extract Visualizer Theme Data"
#define EXIM_EDIT_THEME_LABEL       L"Visualizer Theme Data:"
#define EXIM_APPEND_THEME_BTN       L"&Append Theme(s)"

#define EXIM_APPEND_FOLD_TITLE      L"Append Fold Structure Data"
#define EXIM_EXTRACT_FOLD_TITLE     L"Extract Fold Structure Data"
#define EXIM_EDIT_FOLD_LABEL        L"Fold Structure Data:"
#define EXIM_APPEND_FOLD_BTN        L"&Append Fold Struct(s)"

#define EXIM_LOAD_FILE_BTN          L"&Load from a File"
#define EXIM_SAVE_FILE_BTN          L"&Save to a File"
#define EXIM_CLOSE_BTN              L"&Close"
#define EXIM_CANCEL_BTN             L"&Cancel"


// Preferences Dialog
#define PREFERENCES_DIALOG_TITLE    L"Preferences"
#define PREFERENCES_CLEARVIZ_AUTO   L"\"Clear\" button click unchecks Auto-detect File Type"
#define PREFERENCES_CLEARVIZ_PANEL  L"Closing plugin panel clears visualization"
#define PREFERENCES_MBCHARS_STATE   L"Show Multi-byte checkbox in plugin panel"
#define PREFERENCES_HOP_LEFT_EDGE   L"Hop Right: Move to the left edge of the following field"
#define PREFERENCES_CLOSE_BTN       L"&Close"
#define PREF_CLEARVIZ_AUTO_TIP      L"● When this box is unchecked, clicking the \"Clear\" button will redo the auto-file detection, followed by document visualization.\r\n\r\n● When this box is checked, clicking the \"Clear\" button will also uncheck the \"Auto-detect File Type\" checkbox, and also clear the document visualization."
#define PREF_CLEARVIZ_PANEL_TIP     L"● Leave this box unchecked if you are only closing the plugin panel for more document viewing area but wish to retain the document visualization.\r\n\r\n● When this box is checked, closing the plugin panel will also clear the document visualization."
#define PREF_MBCHARS_STATE_TIP      L"● When this box is checked, the \"Multi-Byte Chars\" 3-state checkbox will be displayed on the plugin panel. The 3-state checkbox is useful for quickly toggling between the single-byte and multi-byte modes of visualization of a document with multi-byte character data."
#define PREF_HOP_RT_LEFT_EDGE_TIP   L"● When this box is unchecked, clicking the right \"Field Hop\" button will move cursor to the right edge of the current field. If already at the right edge of the field, the cursor will be moved to the right edge of the following field.\r\n\r\n● When this box is checked, clicking the right \"Field Hop\" button will move cursor to the left edge of the following field."


// Jump Field Dialog
#define JUMP_FIELD_DIALOG_TITLE     L"Jump to Field"
#define JUMP_FIELD_SELECT_LABEL     L"&Select Field:"
#define JUMP_FIELD_PREF_SEQ_NUM     L"Show sequence numbers in the Select Field list"
#define JUMP_LABEL_CARET_FLASH      L"Cursor Flash duration (in seconds)"
#define JUMP_FIELD_GO_BTN           L"&Jump"
#define JUMP_FIELD_CLOSE_BTN        L"&Cancel"
#define JUMP_TIP_CARET_FLASH        L"When the cursor is set at its default width of '1', it can be hard to spot the cursor at its new position. To aid quick spotting, the cursor is briefly set to block mode at the destination position.\r\n\r\nSpecify the number of seconds to have the cursor flashing in block mode at the destination position."


// Data Extraction Dialog
#define DATA_EXTRACT_DIALOG_TITLE   L"Fixed-Width Data Extraction"
#define DATA_EXTRACT_PREFIX_LABEL   L"Prefix Text¹"
#define DATA_EXTRACT_RECORD_LABEL   L"Record Type"
#define DATA_EXTRACT_FIELD_LABEL    L"Field"
#define DATA_EXTRACT_FIELD_TRIM     L"Trim Leading && Trailing Spaces"
#define DATA_EXTRACT_SUFFIX_LABEL   L"Suffix Text¹"
#define DATA_EXTRACT_ADD_LINE_ITEM  L"Insert Line Item\t[CTRL+Insert]"
#define DATA_EXTRACT_DEL_LINE_ITEM  L"Delete Line Item\t[CTRL+Delete]"
#define DATA_EXTRACT_PAGE_PREV      L"Previous Page\t[CTRL+Page Up]"
#define DATA_EXTRACT_PAGE_NEXT      L"Next Page\t[CTRL+Page Down]"
#define DATA_EXTRACT_PAGE_ADD       L"Add Page\t[CTRL+SHIFT+Insert]"
#define DATA_EXTRACT_PAGE_DEL       L"Delete Page\t[CTRL+SHIFT+Delete]"
#define DATA_EXTRACT_MOVE_DOWN      L"Move down the selected Line Item\t[CTRL+Down Arrow]"
#define DATA_EXTRACT_MOVE_UP        L"Move up the selected Line Item\t[CTRL+Up Arrow]"
#define DATA_EXTRACT_EXTRACT_BTN    L"E&xtract Data"
#define DATA_EXTRACT_CLOSE_BTN      L"&Close"
#define DATA_EXTRACT_TEMPLATE_GROUP L"Reusable Templates"
#define DATA_EXTRACT_TEMPLATE_CURR  L"Show only current FileType Templates"
#define DATA_EXTRACT_TEMPLATE_OTHER "[Other] "
#define DATA_EXTRACT_TEMPLATE_LOAD  L"Load Template:"
#define DATA_EXTRACT_TEMPLATE_NAME  L"Template Name:"
#define DATA_EXTRACT_TEMPLATE_SAVE  L"&Save Template"
#define DATA_EXTRACT_TEMPLATE_NEW   L"&New"
#define DATA_EXTRACT_TEMPLATE_DEL   L"&Delete"
#define DATA_EXTRACT_NEW_LINE_TAB   L"Tip¹:\nIn Prefix and Suffix Text boxes,\nuse \\n for New Lines and \\t for TABs."
#define DATA_EXTRACT_KEYBOARD_TIP   L"Easy Keyboard Shortcuts"
#define DATA_EXTRACT_DELETE_PROMPT  L"Delete Template?"
#define DATA_EXTRACT_INVTEMPL_TITLE L"Invalid Template Name!"
#define DATA_EXTRACT_INVTEMPL_MSG   L"\"[\" and \"]\" are disallowed in Template Name."
#define DATA_EXTRACT_CHANGED_DOC    L"The currently active document is of a different File Type.\r\n\r\nUnable to proceed with data extraction."


// Info Button Links
#define VIZPANEL_INFO_README        L"https://github.com/shriprem/FWDataViz/blob/master/README.md"
#define VIZPANEL_FOLD_INFO_README   L"https://github.com/shriprem/FWDataViz/blob/master/docs/foldable_record_blocks.md"
#define FWVIZ_DEF_INFO_README       L"https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md"
#define FWVIZ_DEF_MCBS_INFO_README  L"https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md"
#define FWVIZ_DEF_ADFT_INFO_README  L"https://github.com/shriprem/FWDataViz/blob/master/docs/auto_detect_file_type.md"
#define FWVIZ_RT_THEME_INFO_README  L"https://github.com/shriprem/FWDataViz/blob/master/docs/record_type_theme.md"
#define THEME_DEF_INFO_README       L"https://github.com/shriprem/FWDataViz/blob/master/docs/theme_config_dialog.md"
#define FOLD_DEF_INFO_README        L"https://github.com/shriprem/FWDataViz/blob/master/docs/fold_structs_editor.md"
#define CONFIG_APPEND_INFO_README   L"https://github.com/shriprem/FWDataViz/blob/master/docs/config_append_dialog.md"
#define CONFIG_EXTRACT_INFO_README  L"https://github.com/shriprem/FWDataViz/blob/master/docs/config_extract_dialog.md"
#define FIELD_TYPE_INFO_README      L"https://github.com/shriprem/FWDataViz/blob/master/docs/field_type_dialog.md"
#define DATA_EXTRACT_INFO_README    L"https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md"
#define DATA_EXTRACT_KEYNAV_README  L"https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_key_shortcuts.md"
