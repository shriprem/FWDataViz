# Version History

### Version 2.5.1.0

1. Fields tagged with an undefined field type such as _NO_STYLING_ or _DEFAULT_STYLE_ or _STYLE_DEFAULT_ will be rendered with the default background and foreground colors of the currently **Selected Theme** in Notepad++'s **Style Configurator**. For more info, see: [Field Types - Default Style](https://github.com/shriprem/FWDataViz/blob/master/docs/field_type_dialog.md#default-style).

2. With the built-in support for *Framed Current Line* in Notepad++ 8.4.1, the *Framed Current Line* checkbox control has been permanently removed from the *FWDataViz* plugin panel and menu.

3. With Notepad++ 8.4.2, plugins now have query support for Darkmode. By utilizing this support, the Darkmode render code in the _FWDataViz_ plugin has been significantly trimmed down. This has resulted in a smaller size of the plugin DLL for this version.

#### Known Issue with the _CSVLint_ plugin
Starting with Notepad++ v8.4.1, if you have both the _FWDataViz_ and _CSVLint_ plugins installed and then open a CSV file while the _FWDataViz_ side panel is open, Notepad++ will crash.

This issue got introduced when the _CSVLint_ plugin made code changes to comply with the Scintilla Lexer upgrade of Notepad++ 8.4. (_FWDataViz_ was not affected by the Scintilla lexer upgrade since it uses Scintilla _**styles**_ instead of the Scintilla _**lexer**_). To resolve this, two issues ([#25](https://github.com/BdR76/CSVLint/issues/25) and [#26](https://github.com/BdR76/CSVLint/issues/26)) have been opened at the _CSVLint_ repository.

As a workaround, if you have the _CSVLint_ plugin installed, close the _FWDataViz_ side panel before opening any CSV files. Alternatively, use the _CSVQuery_ plugin which has no such issues.

**Release:** [2.5.1.0 Release](https://github.com/shriprem/FWDataViz/releases/tag/v2.5.1.0)

---

### Version 2.5.0.0

1. Dynamic change of Dark mode tones: When dark mode settings are changed in the Preferences dialog of Notepad++ (version 8.4.1 or higher), they will be immediately reflected in the _FwDataViz_ plugin user interfaces -- without requiring a restart of Notepad++.

2. With the built-in support for *Framed Current Line* in Notepad++ 8.4.1, the options for toggling the *Framed Current Line* from the *FWDataViz* plugin will be suppressed.

3. Fixed blurry disabled static texts in dark mode.

4. The Preferences option for *Closing plugin panel clears visualization* is now handled even when the *FWDataViz* panel is closed by clicking the 'X' button on its title bar.


**Release:** [2.5.0.0 Release](https://github.com/shriprem/FWDataViz/releases/tag/v2.5.0.0)

---

### Version 2.4.0.2
**Background:** With its v8.3 release, the Notepad++ *x64* binary will support opening 2GB+ files. However, this change causes compability issue with many *x64* plugins, [and they will need to be recompiled with a modified C++ header file](https://community.notepad-plus-plus.org/topic/22471/recompile-your-x64-plugins-with-new-header).

The _FWDataViz_ ***x64*** plugin was affected by this incompatibility issue specifically when trying to extract data in the [Data Extraction dialog](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md). With the suggested recompile of the _FWDataViz_ plugin with the modified C++ header file, the v2.4.0.2 release fixes this issue.

**Release:** [2.4.0.2 Release](https://github.com/shriprem/FWDataViz/releases/tag/v2.4.0.2)

---

### Version 2.4.0.1
1. Updated `NPP_Plugin_Darkmode.dll` to [version 1.1.0.0](https://github.com/shriprem/NPP_Plugin_Darkmode/releases/tag/v1.1.0.0). This fixes darkmode rendering issue in Windows 11.

**Release:** [2.4.0.1 Release](https://github.com/shriprem/FWDataViz/releases/tag/v2.4.0.1)

---

### Version 2.4.0.0

1. [Field Types](https://github.com/shriprem/FWDataViz/blob/master/docs/field_type_dialog.md) for distinguishing fields sharing the same traits.

2. Preferences dialog added to configure three new settings.

3. Regex expression fields are now validated for user input errors.

4. Darkmode for the _FWDataViz_ plugin is now rendered by a standalone DLL that can be shared by multiple plugins, thus reducing the overall memory footprint. Notepad++ will only load a copy of the `NPP_Plugin_Darkmode.dll` located either in:
   * The folder for the first plugin (_in alphabetical order_) that requires this DLL.
   * Or, in the common `<Notepad++_executable_folder>`.

   You can verify the singleton loading of the `NPP_Plugin_Darkmode.dll` using the [Process Explorer](https://docs.microsoft.com/en-us/sysinternals/downloads/process-explorer) utility.

**Plugin Developers**: For more information on using `NPP_Plugin_Darkmode.dll` in your plugins, please refer to the documentation at the [NPP_Plugin_Darkmode](https://github.com/shriprem/NPP_Plugin_Darkmode) repository.

**Release:** [2.4.0.0 Release](https://github.com/shriprem/FWDataViz/releases/tag/v2.4.0.0)

---

### Version 2.3.0.0
#### Enhancements
1. [Field Copy](https://github.com/shriprem/FWDataViz/blob/master/docs/field_copy_paste.md#field-copy) and [Field Paste](https://github.com/shriprem/FWDataViz/blob/master/docs/field_copy_paste.md#field-paste) with smart left or right alignment and padding or trimming.

2. [Hop to the left or right Edge](https://github.com/shriprem/FWDataViz/blob/master/docs/field_navigation.md) of the current field or adjacent fields.

3. [Menu Items with Keyboard Shortcuts](https://github.com/shriprem/FWDataViz/blob/master/docs/menu_shortcuts.md) to invoke frequently used actions, without shifting focus away from the document to access the plugin panel buttons.

4. [Total Support for Multi-byte Characters](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_support.md). _Record Markers, Record Terminators_ and _ADFT Regexes_ can all now include multi-byte characters. Like so with _Prefix_ and _Suffix_ labels during data extraction.

**Release:** [2.3.0.0 Release](https://github.com/shriprem/FWDataViz/releases/tag/v2.3.0.0)

---

### Version 2.2.0.0
#### Enhancements
1. Visualize files with [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md).

2. Use character column counts for *Field Column* values in [Cursor Position Data](https://github.com/shriprem/FWDataViz/blob/master/README.md#fixed-width-data-visualizer-panel) while in [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md) mode.

3. Display UTF-8 Bytes and Unicode value  for multi-byte characters in [Cursor Position Data](https://github.com/shriprem/FWDataViz/blob/master/README.md#fixed-width-data-visualizer-panel) on the plugin panel (*regardless* of whether the multi-byte character mode is switched on or not).

4. Use character column counts in [Jump To Field](https://github.com/shriprem/FWDataViz/blob/master/README.md#jump-to-field-popup) while in [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md) mode.

5. Use character column counts in [Data Extraction](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md) while in [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md) mode.

6. A [hidden option](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md#quick-override-from-panel) on the plugin panel to quickly override the File Type [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md) mode.

#### Fixed
1. Record overshoots occurred with [Jump To Field](https://github.com/shriprem/FWDataViz/blob/master/README.md#jump-to-field-popup) and [Data Extraction](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md) when record terminated earlier than at their full defined lengths.

**Release:** [2.2.0.0 Release](https://github.com/shriprem/FWDataViz/releases/tag/v2.2.0.0)

---

### Version 2.1.2.1
1. **Enhancement:** Cursor will be briefly set to block mode to highlight its position in text when using [Jump to Field](https://github.com/shriprem/FWDataViz#jump-to-field-popup).

2. **Fixed:** The Cursor Position Data was refreshing only when the page was scrolled. This regression got introduced in version 2.1.0.0.

**Release:** [2.1.2.1 Release](https://github.com/shriprem/FWDataViz/releases/tag/v2.1.2.1)

---

### Version 2.1.1.1
1. **Fixed:** If the ADFT option had been enabled, the plugin was clearing out the NPP's lexing (color scheme) of standard file types (such as .xml, .cpp, .js, etc.). The workaround was to close the FWDataViz panel, and restart NPP. Not anymore with this release.

2. **Minor Change:** After the File Types or Themes Configuration were edited and saved, a backup file was created using the [MoveFile](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-movefile) Windows API  to move the *Visualizer.ini* or *Themes.ini* into the *Backup* folder, and the changes were saved to a new file to replace the original config file. With this release, the backup is created instead by using the [CopyFile](https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-copyfile) Windows API. This will allow the usage of hardlinks, if needed for some users, to these config files.

**Release:** [2.1.1.1 Release](https://github.com/shriprem/FWDataViz/releases/tag/v2.1.1.1)

---

### Version 2.1.1.0
1. **Change:** Themes configuration file extension has been changed from *.dat* to *.ini*. During the initial load, the plugin will rename *Themes.dat* file to *Themes.ini*, if and only if the latter is not already existing in the plugin config folder. See the feature request [issue](https://github.com/shriprem/FWDataViz/issues/27).

2. **Change:** In Darkmode, the dropdown arrow for comboboxes has been changed from [U+02C5 (MODIFIER LETTER DOWN ARROWHEAD)](https://www.fileformat.info/info/unicode/char/02C5/index.htm) to [U+25BF (WHITE DOWN-POINTING SMALL TRIANGLE)](https://www.fileformat.info/info/unicode/char/25bf/index.htm).

3. **Fixed:** In some instances, the File Open dialog was not launching at the FWDataViz Config Backup folder when either the _Load Backup Configuration_ (in the [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md)) or the _Load Backup Themes File_ (in the [Visualizer Theme Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/theme_config_dialog.md)) buttons were clicked.

4. **Optimized:** All three ICD-10 Sample Files were edited to retain only the first and last 10,000 lines (for a total 20,000 lines) from their original versions. This will yield around 30% reduction in this plugin's binary packages.

**Release:** [2.1.1.0 Release](https://github.com/shriprem/FWDataViz/releases/tag/v2.1.1.0)

---

### Version 2.1.0.0
1. **Enhancement:** FWDataViz is now able to visualize even when wordwrap is enabled for a document. Hence the "Turnoff Wordwrap" button on the panel became redundant, and so has been removed.
2. **Fixed:** FWDataviz was not visualizing all lines in view when block(s) of lines were folded or hidden (the latter, via NPP menu: *View » Hide Lines*). See: [issue #25](https://github.com/shriprem/FWDataViz/issues/25).

**Release:** [Wordwrap, Fold Lines & Hide Lines Release](https://github.com/shriprem/FWDataViz/releases/tag/v2.1.0.0)

---

### Version 2.0.0.0
#### Enhancements:
1. Darkmode rendering enabled for the plugin panel and dialog boxes.
2. Toolbar icons in filled and unfilled modes.

#### `NOTE`
Notepad++ user configuration settings are saved into the config.xml file by Notepad++ only during the application closing event. The FWDataViz plugin is only able to read these settings changes in the config.xml file during the next launch of Notepad++.

So, when Darkmode and toolbar icon settings are changed, Notepad++ will need to be restarted for the plugin to reflect these settings changes.

See: [Sample screenshots of the plugin UI in dark mode](https://github.com/shriprem/FWDataViz/blob/master/docs/dark_mode_ui.md)

**Release:** [Dark mode Plugin UI Release](https://github.com/shriprem/FWDataViz/releases/tag/v2.0.0.0)

---

### Version 1.4.2.0
#### Enhancements:
1. Page mode added to [Data Extraction](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md) dialog. The dialog now supports up to 30 line items, 10 line items per page across 3 pages.
---
### Version 1.4.0.1
#### Enhancements:
1. [Easy Keyboard Navigation](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_key_shortcuts.md) in the Data Extraction dialog.

---

### Version 1.4.0.0
#### Enhancements:
1. [Data Extraction](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md) feature to extract a handful data fields into an easily readable, quick report.

---

### Version 1.0.3.1
#### Enhancements:
1. Light & Dark mode Fluent UI toolbar icons added for the plugin.
2. Including ARM64 build. (_Please note that I am unable to test the ARM64 build since I don't have an ARM64 device, and it is being provided AS IS._)

##### _`ALERT!` This plugin release is only compatible with Notepad++ 8.0 and later releases._

#### Programming Note:
The Notepad++ 8.0 has introduced the Fluent UI icons in Small/Large, Light/Dark & Unfilled/Filled modes, making for a total of 8 possible combinations. But the newly added API function [`NPPM_ADDTOOLBARICON_FORDARKMODE`](https://github.com/notepad-plus-plus/notepad-plus-plus/commit/8a898bae3f84c03c44aaed25001e9fa1ddfa09aa) has allowed the plugins to supply images for only 4 Fluent UI images among the possible set of 8.

Due to the current NPP API limitation, this minor release for FWDataViz is only able to add Fluent UI icons for the Small/Large, Light/Dark Fluent UI modes but not the Filled/Unfilled Fluent UI modes. So the toolbar icons for this plugin will not change when switching between unfilled and filled Fluent UI selections in Notepad++ Preferences dialog.

---

### Version 1.0.3.0
#### Enhancements:
1. [Record Type Themes](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_extract_dialog.md)

---

### Version 1.0.2.0
#### Enhancements:
1. [Automatic Detection of File Types](https://github.com/shriprem/FWDataViz/blob/master/docs/auto_detect_file_type.md)

---

### Version 1.0.1.0
#### Enhancements:
1. *Cursor Position Data* display will shrink or grow to the dock panel width.
2. Long text in *Cursor Position Data* display will scroll horizontally instead of wrapping to the next line.
3. Added *Record Length [Current/Defined]* line in *Cursor Position Data* display.
