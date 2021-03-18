# FWDataViz
#### Fixed Width Data Visualizer plugin for Notepad++
![FWDataViz](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multi_rec_weather_stations.png)

* Visualizes fields within fixed-width data files with distinct colors and font styles.
* <a href="#plugin-panel">Displays record, field and byte information at cursor position.</a>
* <a href="#jump-to-field-popup">Feature to jump to a specific field of the current record.</a>
* [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md) to define and customize file, record and field types.
* [Visualizer Theme Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/theme_config_dialog.md) to define and customize themes and colors for data visualization.
* Easily share customized [File Type Metadata](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_extract_dialog.md) and [Visualizer Theme](https://github.com/shriprem/FWDataViz/blob/master/docs/theme_extract_dialog.md) definitions.
* <a href="#sample-data-files">Sample Data files</a> in the plugin package demonstrating:
   * <a href="#homogenous-record-type-fixed-width-data-files">Homogenous record-type data</a>
   * <a href="#mixed-record-type-fixed-width-data-files">Mixed record-types data</a>
   * <a href="#multi-line-record-type-fixed-width-data-files">Multiple-line-spanning record-types data</a>

## User Interface

#### Toolbar Plugin Icons
![Show_Panel_icon](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/show_panel_toolbutton.png)
![Framed_Caret_icon](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/framed_caret_line_toolbutton.png)

The _Framed Current Line_ feature preserves the color highlights of data fields on the caret line, while still indicating its current line status. This feature is functional even when the fixed-width data visualization is not active.

#### Fixed-Width Data Visualizer Panel:
![Plugin_Panel](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/plugin_panel.png)

* Click on the _View Sample Files_ icon: ![View Sample Files](https://raw.githubusercontent.com/shriprem/FWDataViz/master/src/Resources/file_samples.bmp), and choose from the menu options to view the various <a href="#sample-data-files">sample files</a>.

* Click on the _File Type Metadata Editor_ icon: ![File Type Metadata Editor](https://raw.githubusercontent.com/shriprem/FWDataViz/master/src/Resources/file_config.bmp), to view, modify or create your custom File Type definitions. For more information, see: [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md)

* Click on the _Visualizer Theme Editor_ icon: ![Visualizer Theme Editor](https://raw.githubusercontent.com/shriprem/FWDataViz/master/src/Resources/color_config.bmp), to view, modify or create your custom Visualizer Theme definitions. For more information, see: [Visualizer Theme Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/theme_config_dialog.md)

* Check the _Auto-detect File Type_ box to automatically visualize files with matching file type from the defined list. For more information on this feature, see: [Auto-Detect File Type Configuration](https://github.com/shriprem/FWDataViz/blob/master/docs/auto_detect_file_type.md)

* Check the _Framed Current Line_ box to preserve the color highlights of data fields on the caret line, while still indicating its current line status.

#### Jump to Field popup:
![Jump_to_Field_list](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/jump_to_field_with_list.png)

Useful for navigating between fields on long records with dozens of fields.


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


### Multi-Line Record-type Fixed-width Data Files
Files with data records that span multiple lines. End of record is indicated by a marker. For example: _<END_REC>_

Owing to such a marker, records for these files can be of varying width.

__Weather Stations List with location wiki and Daily Weather data__ _(A contrived format, for concept illustration)_
![Multi_Line](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multi_line_record_file.png)

## Installation
1. Install Notepad++ version 7.9.3 or higher.
2. Open Notepad++.
3. In Notepad++, go to menu **Plugins** » **Plugins Admin...**.
4. In the **Available** tab, check the box for **Fixed-width Data Visualizer**.
5. Click the **Install** button.

## Attributions
* Most of the icons used in this plugin are the originals or derivatives of the [*Fugue Icons*](https://p.yusukekamiyamane.com) designed by **Yusuke Kamiyamane**.

* The core visualizer algorithm of this plugin was first [protyped](https://github.com/shriprem/FWDataViz/blob/master/docs/Visualizer_prototype.py) in Python using **David Brotherstone's** *Python Script* plugin for Notepad++.
