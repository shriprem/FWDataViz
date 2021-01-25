# FWDataViz
#### Fixed Width Data Visualizer plugin for Notepad++
![FWDataViz](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multi_rec_weather_stations.png)

* Visualizes fields within fixed-width data files with distinct colors and font styles.
* <a href="#plugin-panel">Displays record, field and byte information at cursor position.</a>
* <a href="#jump-to-field-popup">Feature to jump to a specific field of the current record.</a>
* <a href="#file-record--field-configuration-dialog">Configuration dialog to define and customize file, record and field types.</a>
* <a href="#theme--color-configuration-dialog">Configuration dialog to define and customize themes and colors for data visualization.</a>
* Easily share customized <a href="#file-type-extract-dialog">FileType</a> and <a href="#theme-extract-dialog">Color Theme</a> configurations.
* <a href="#sample-data-files">Sample Data files</a> in the plugin package demonstrating:
   * <a href="#homogenous-record-type-fixed-width-data-files">Homogenous record-type data</a>
   * <a href="#mixed-record-type-fixed-width-data-files">Mixed record-types data</a>
   * <a href="#multi-line-record-type-fixed-width-data-files">Multiple-line-spanning record-types data</a>

## User Interface

#### Toolbar Plugin Icons
![Show_Panel_icon](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/show_panel_toolbutton.png)
![Framed_Caret_icon](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/framed_caret_line_toolbutton.png)

The _Framed Current Line_ feature preserves the color highlights of data fields on the caret line, while still indicating its current line status. This feature is functional even when the fixed-width data visualization is not active.

#### Plugin Panel:
![Plugin_Panel](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/plugin_panel.png)

To see the plugin in action, click on the _View Sample Files_ icon, and choose from the menu options to view the <a href="#sample-data-files">various sample files</a>.

#### Jump to Field popup:
![Jump_to_Field_list](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/jump_to_field_with_list.png)

Useful for navigating between fields on long records with dozens of fields.


## Configuration Dialogs

#### File, Record & Field Configuration Dialog:
![FileType_Config](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/file_type_editor.png)

Heirarchical configuration of File Type, Record Type, and Field Labels & Widths. Previous configurations are automatically backed up with each save.

#### File Type Extract Dialog:
![FileType_Extract](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/file_type_extract.png)

Useful for sharing of customized File Type definitions within teams, and between collaborators.

#### Theme & Color Configuration Dialog:
![Theme_Config](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/color_theme_editor.png)

Heirarchical configuration of Themes and Colors. Previous configurations are automatically backed up with each save.

#### Theme Extract Dialog:
![Theme_Extract](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/color_theme_extract.png)

Useful for sharing of customized Theme definitions within teams, and between collaborators.

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

