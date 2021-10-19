# Version History

### Version 2.2.0.0
#### Enhancements
1. Visualize files with [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md).

2. Use character column counts for *Field Column* values in [Cursor Position Data](https://github.com/shriprem/FWDataViz/blob/master/README.md#fixed-width-data-visualizer-panel) while in [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md) mode.

3. Display UTF-8 Bytes and Unicode value  for multi-byte characters in [Cursor Position Data](https://github.com/shriprem/FWDataViz/blob/master/README.md#fixed-width-data-visualizer-panel) on the plugin panel (*regardless* of whether the multi-byte character mode is switched on or not).

4. Use character column counts in [Jump To Field](https://github.com/shriprem/FWDataViz/blob/master/README.md#jump-to-field-popup) while in [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md) mode.

5. Use character column counts in [Data Extraction](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md) while in [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md) mode.

6. A [hidden option](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md#quick-override-from-panel) on the plugin panel to quickly override the File Type [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md) mode.

#### Fixed
1. Record overshoots occurred with [Jump To Field](https://github.com/shriprem/FWDataViz/blob/master/README.md#jump-to-field-popup) and [Data Extraction](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md) when record terminated earlier than at their defined lengths.

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
