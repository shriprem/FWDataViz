# FWDataViz

![GitHub](https://img.shields.io/github/license/shriprem/FWDataViz)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/shriprem/FWDataViz)
![GitHub all releases](https://img.shields.io/github/downloads/shriprem/FWDataViz/total)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
![GitHub last commit (by committer)](https://img.shields.io/github/last-commit/shriprem/FWDataViz)
![GitHub Workflow Status (with event)](https://img.shields.io/github/actions/workflow/status/shriprem/FWDataViz/CI_build.yml)
![GitHub issues](https://img.shields.io/github/issues/shriprem/FWDataViz)

## Fixed Width Data Visualizer plugin for Notepad++
[Current Version: 2.6.4.0](https://github.com/shriprem/FWDataViz/releases/tag/v2.6.4.0)

![image](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/foldable_orders_file.png)
**ICD-10 Billable-Flagged Order Codes Sample File, with folding applied.**


### Features at the Panel
* Visualization of fields in fixed-width data files with distinct colors and font styles. With or optionally without distinct background colors.

* [Display of record, field and byte information at cursor position](#fixed-width-data-visualizer-panel) on the panel. Also, optionally, inside a calltip within the Notepad++ editor pane.

* [Foldable Record Blocks](https://github.com/shriprem/FWDataViz/blob/master/docs/foldable_record_blocks.md) for files with hierarchical data.

* [Quick Field Navigation](https://github.com/shriprem/FWDataViz/blob/master/docs/field_navigation.md) to [Jump](https://github.com/shriprem/FWDataViz/blob/master/docs/field_navigation.md#jump-to-field) to any field of the current record or [Hop](https://github.com/shriprem/FWDataViz/blob/master/docs/field_navigation.md#hop-field) to the left or right edge of the current field or adjacent fields.

* [Field Copy](https://github.com/shriprem/FWDataViz/blob/master/docs/field_copy_paste.md#field-copy) and [Field Paste](https://github.com/shriprem/FWDataViz/blob/master/docs/field_copy_paste.md#field-paste) with smart left or right alignment and padding or trimming.

* [Default Style Field Types](https://github.com/shriprem/FWDataViz/blob/master/docs/field_type_dialog.md#default-style) for turning off visualization for specific fields like spaces or separators.

* Choice of visualization based either on the default single byte (US-ASCII) or [multi-byte](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md) character data.

* [Menu Items with Keyboard Shortcuts](https://github.com/shriprem/FWDataViz/blob/master/docs/menu_shortcuts.md) for quick and easy access of frequently used actions.

* [Sample Data files](#sample-data-files) in the plugin package demonstrating:
   * [Homogenous record-type data](#homogenous-record-type-fixed-width-data-files)
   * [Mixed record-types data](#mixed-record-type-fixed-width-data-files)
   * [Multiple-line-spanning record-types data](#multi-line-record-type-fixed-width-data-files)
   * [Foldable Record Blocks](https://github.com/shriprem/FWDataViz/blob/master/docs/foldable_record_blocks.md)

### Features beyond the Panel

* [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md) to intuitively define and customize file type metadata with record types and fields.

* [Visualizer Theme Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/theme_config_dialog.md) to intuitively define and customize themes and colors for data visualization.

* [Fold Structures Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/fold_structs_editor.md) to intuitively define and customize Fold Structure definitions.

* Easy sharing of customized File Type or Visualizer Theme or Fold Structure definitions with [Extract Configuration Data](https://github.com/shriprem/FWDataViz/blob/master/docs/config_extract_dialog.md) and [Append Configuration Data](https://github.com/shriprem/FWDataViz/blob/master/docs/config_append_dialog.md).

* [Data Extraction](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md) to extract a handful data fields into an easily readable, quick report.

* Full support for [multi-byte characters](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_support.md) in File Type metadata, Theme Labels, Data Extraction Prefix and Suffix Labels, and left or right padding & trimming.

* [Dark mode rendering of the plugin UI.](https://github.com/shriprem/FWDataViz/blob/master/docs/dark_mode_ui.md)

---

## Plugin Panel
![Plugin_Panel](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/plugin_panel.png)

* Click on the _View Sample Files_ icon: ![View Sample Files](https://raw.githubusercontent.com/shriprem/FWDataViz/master/src/Resources/file_samples.bmp), and choose from the menu options to view various [sample files](#sample-data-files).

* Click on the _File Type Metadata Editor_ icon: ![File Type Metadata Editor](https://raw.githubusercontent.com/shriprem/FWDataViz/master/src/Resources/file_config.bmp), to view, modify or create your custom File Type definitions. For more information, see: [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md).

* Click on the _Visualizer Theme Editor_ icon: ![Visualizer Theme Editor](https://raw.githubusercontent.com/shriprem/FWDataViz/master/src/Resources/color_config.bmp), to view, modify or create your custom Visualizer Theme definitions. For more information, see: [Visualizer Theme Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/theme_config_dialog.md).

* Click the _Preferences_ button to specify preferences for the plugin. For more information, see: [Preferences](https://github.com/shriprem/FWDataViz/blob/master/docs/preferences_dialog.md).

* Check the _Auto-detect File Type_ box to automatically visualize files with matching file type from the defined list. For more information, see: [Auto-Detect File Type Configuration](https://github.com/shriprem/FWDataViz/blob/master/docs/auto_detect_file_type.md).

* To get the _Multi-Byte Chars_ become visible on the plugin panel, see: [Preferences](https://github.com/shriprem/FWDataViz/blob/master/docs/preferences_dialog.md). Since this is a **_3-state_** checkbox, please review the documentation for it at: [Multi-byte Character Data Visualization](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md#quick-override-from-panel).

* Check the _Default Background_ box to render the fixed-width fields with just the text colors while suppressing the background colors of the theme styles.

* Check the _Show Calltip_ box to display the _Cursor Position Data_ in a calltip within the editor, right below the current cursor position. The calltip will be useful during presentations and other situations when there is a need to avoid an additional glance towards the side panel to view the same _Cursor Position Data_.

  > :bulb: To display the calltip text in bold, check the option for _**Use DirectWrite**_ in Notepad++ _Settings » Preferences » MISC._

* Check the _Trim Field Copy_ box to automatically ignore the left or right padding characters when copying a field. For more information, see: [Field Copy](https://github.com/shriprem/FWDataViz/blob/master/docs/field_copy_paste.md#field-copy).

* For more information on the _Jump to Field_ and left & right hop buttons, see: [Quick Field Navigation](https://github.com/shriprem/FWDataViz/blob/master/docs/field_navigation.md).

* For more information on _Field Copy_ and _Field Paste_ buttons, see: [Field Copy and Field Paste](https://github.com/shriprem/FWDataViz/blob/master/docs/field_copy_paste.md).

* For more information on the _Data Extraction_ button, see: [Data Extraction](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md).

* For more information on the buttons in the _Folding_ section, see: [Foldable Record Blocks](https://github.com/shriprem/FWDataViz/blob/master/docs/foldable_record_blocks.md) and [Fold Structures Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/fold_structs_editor.md).

* Clicking the `…` button at the right-bottom corner of the plugin panel will display the refreshed list of file paths for the INI files active for the current fixed-width file. Just hovering over this button will display the last refreshed list.


---

## Sample data files

### Homogenous Record-type Fixed-width Data Files
Files with data in the same record format.

__NOAA Weather Stations List__
![Single_Rec](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/single_rec_weather_stations.png)


### Mixed Record-type Fixed-width Data Files
Files with data in multiple record formats.

__NOAA Weather Stations List__
_(Same data from the preceding sample, but now flagged based on the GSN field)_
![Multi_Rec](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multi_rec_weather_stations.png)


__Treasury IPAC File__
_(Real format, but with fake data. Foldable.)_
![Multi_Rec](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multi_rec_ipac_file.png)

In this sample clip:
* The records are being displayed in foldable blocks. For more information on this, see: [Foldable Record Blocks](https://github.com/shriprem/FWDataViz/blob/master/docs/foldable_record_blocks.md).
* The _Transaction Header_ records are being visualized with a different theme. For more information on this, see: [Record Type Theme](https://github.com/shriprem/FWDataViz/blob/master/docs/record_type_theme.md).

### Multi-Line Record-type Fixed-width Data Files
Files with data records that span multiple lines. End of record is indicated by a marker. For example: _<END_REC>_

Owing to such a marker, records for these files can be of varying width.

__Weather Stations List with location wiki and Daily Weather data__ _(A contrived format, for concept illustration)_
![Multi_Line](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multi_line_record_file.png)

---

## Installation
1. Install Notepad++ version 8.4 or higher.
2. Open Notepad++.
3. In Notepad++, go to menu **Plugins** » **Plugins Admin...**.
4. In the **Available** tab, check the box for **Fixed-width Data Visualizer**.
5. Click the **Install** button.

## Attributions
* Most of the icons used in this plugin are the originals or derivatives of the [*Fugue Icons*](https://p.yusukekamiyamane.com) designed by **Yusuke Kamiyamane**.

* All screen clippings in this repository were made using [FastStone Capture](https://www.faststone.org/FSCaptureDetail.htm). Small-size application with awesome features!
* The core visualizer algorithm of this plugin was first [protyped](https://github.com/shriprem/FWDataViz/blob/master/docs/Visualizer_prototype.py) in Python using **David Brotherstone's** *Python Script* plugin for Notepad++.

<a href='https://ko-fi.com/S6S417WICS' target='_blank'><img height='36' style='border:0px;height:36px;' src='https://storage.ko-fi.com/cdn/kofi5.png?v=6' border='0' alt='Buy Me a Coffee at ko-fi.com' /></a>
