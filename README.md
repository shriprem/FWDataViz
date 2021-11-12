# FWDataViz
#### Fixed Width Data Visualizer plugin for Notepad++
[Current Version: 2.3.0.0](https://github.com/shriprem/FWDataViz/blob/master/VersionHistory.md)
![FWDataViz](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multi_rec_weather_stations.png)

* Visualizes fields within fixed-width data files with distinct colors and font styles.

* <a href="#fixed-width-data-visualizer-panel">Displays record, field and byte information at cursor position.</a>

* [Quick Field Navigation](https://github.com/shriprem/FWDataViz/blob/master/docs/field_navigation.md) to [Jump](https://github.com/shriprem/FWDataViz/blob/master/docs/field_navigation.md#jump-to-field) to any field of the current record or [Hop](https://github.com/shriprem/FWDataViz/blob/master/docs/field_navigation.md#hop-field) to the left or right edge of the current field or adjacent fields.

* [Field Copy](https://github.com/shriprem/FWDataViz/blob/master/docs/field_copy_paste.md#field-copy) and [Field Paste](https://github.com/shriprem/FWDataViz/blob/master/docs/field_copy_paste.md#field-paste) with smart left or right alignment and padding or trimming.

* [Data Extraction](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md) to extract a handful data fields into an easily readable, quick report.

* [Menu Items with Keyboard Shortcuts](https://github.com/shriprem/FWDataViz/blob/master/docs/menu_shortcuts.md) for quick and easy access of frequently used actions.

* [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md) to intuitively define and customize file type metadata with record types and fields.

* Easy sharing of customized File Type metadata with [Extract File Type Data](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_extract_dialog.md) and [Append File Type Data](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_append_dialog.md).

* [Visualizer Theme Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/theme_config_dialog.md) to intuitively define and customize themes and colors for data visualization.

* Easy sharing of customized Themes with [Extract Theme Data](https://github.com/shriprem/FWDataViz/blob/master/docs/theme_extract_dialog.md) and [Append Theme Data](https://github.com/shriprem/FWDataViz/blob/master/docs/theme_append_dialog.md).

* <a href="#sample-data-files">Sample Data files</a> in the plugin package demonstrating:
   * <a href="#homogenous-record-type-fixed-width-data-files">Homogenous record-type data</a>
   * <a href="#mixed-record-type-fixed-width-data-files">Mixed record-types data</a>
   * <a href="#multi-line-record-type-fixed-width-data-files">Multiple-line-spanning record-types data</a>

* Choice of visualization based either on the default single byte (US-ASCII) or [multi-byte](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md) character data.

* Full support for [multi-byte characters](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_support.md) in File Type metadata, Theme Labels, Data Extraction Prefix and Suffix Labels, and left or right padding & trimming.

* [Dark mode rendering of the plugin UI.](https://github.com/shriprem/FWDataViz/blob/master/docs/dark_mode_ui.md)

---

## User Interface

### Toolbar Plugin Icons
![Show_Panel_icon](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/show_panel_toolbutton.png)
![Framed_Caret_icon](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/framed_caret_line_toolbutton.png)

The _Framed Current Line_ feature preserves the color highlights of data fields on the caret line, while still indicating its current line status. This feature is functional even when the fixed-width data visualization is not active.

### Fixed-Width Data Visualizer Panel
![Plugin_Panel](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/plugin_panel.png)

* Click on the _View Sample Files_ icon: ![View Sample Files](https://raw.githubusercontent.com/shriprem/FWDataViz/master/src/Resources/file_samples.bmp), and choose from the menu options to view various <a href="#sample-data-files">sample files</a>.

* Click on the _File Type Metadata Editor_ icon: ![File Type Metadata Editor](https://raw.githubusercontent.com/shriprem/FWDataViz/master/src/Resources/file_config.bmp), to view, modify or create your custom File Type definitions. For more information, see: [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md)

* Click on the _Visualizer Theme Editor_ icon: ![Visualizer Theme Editor](https://raw.githubusercontent.com/shriprem/FWDataViz/master/src/Resources/color_config.bmp), to view, modify or create your custom Visualizer Theme definitions. For more information, see: [Visualizer Theme Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/theme_config_dialog.md)

* Check the _Auto-detect File Type_ box to automatically visualize files with matching file type from the defined list. For more information on this feature, see: [Auto-Detect File Type Configuration](https://github.com/shriprem/FWDataViz/blob/master/docs/auto_detect_file_type.md)

* Check the _Framed Current Line_ box to preserve the color highlights of data fields on the caret line, while still indicating its current line status.

* Check the _Trim Field Copy_ box to automatically ignore the left or right padding characters when copying a field. For more information on this feature, see: [Field Copy](https://github.com/shriprem/FWDataViz/blob/master/docs/field_copy_paste.md#field-copy)

* For more information on the _Jump to Field_ and left & right hop buttons, see: [Quick Field Navigation](https://github.com/shriprem/FWDataViz/blob/master/docs/field_navigation.md)

* For more information on _Field Copy_ and _Field Paste_ buttons, see: [Field Copy and Field Paste](https://github.com/shriprem/FWDataViz/blob/master/docs/field_copy_paste.md)

* For more information on the _Data Extraction_ button, see: [Data Extraction](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md)

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
_(real format, but with fake data)_
![Multi_Rec](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multi_rec_ipac_file.png)

In the above clip, the _Transaction Header_ records are being visualized with a different theme. For more information on this, see: [Record Type Theme](https://github.com/shriprem/FWDataViz/blob/master/docs/record_type_theme.md)

### Multi-Line Record-type Fixed-width Data Files
Files with data records that span multiple lines. End of record is indicated by a marker. For example: _<END_REC>_

Owing to such a marker, records for these files can be of varying width.

__Weather Stations List with location wiki and Daily Weather data__ _(A contrived format, for concept illustration)_
![Multi_Line](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multi_line_record_file.png)

---

## Installation
1. Install Notepad++ version 8.0 or higher.
2. Open Notepad++.
3. In Notepad++, go to menu **Plugins** » **Plugins Admin...**.
4. In the **Available** tab, check the box for **Fixed-width Data Visualizer**.
5. Click the **Install** button.

## Attributions
* Most of the icons used in this plugin are the originals or derivatives of the [*Fugue Icons*](https://p.yusukekamiyamane.com) designed by **Yusuke Kamiyamane**.

* All screen clippings in this repository were made using [FastStone Capture](https://www.faststone.org/FSCaptureDetail.htm). Small-size application with awesome features!
* The core visualizer algorithm of this plugin was first [protyped](https://github.com/shriprem/FWDataViz/blob/master/docs/Visualizer_prototype.py) in Python using **David Brotherstone's** *Python Script* plugin for Notepad++.
