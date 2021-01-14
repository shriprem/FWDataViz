# FWDataViz
Fixed Width Data Visualizer plugin for Notepad++

* Visualizes fields within fixed-width data files with distinct colors and font styles.
* Displays record, field and byte information at cursor position.
* Feature to jump to a specific field of the current record.
* Configuration dialog to define and edit file, record and field types.
* Configuration dialog to define and edit themes and colors for data visualization.
* Can handle files with:
   * Homogenous record-type data 
   * Mixed record-types data
   * Multiple-line-spanning record-types data

## User Interface

#### Toolbar Plugin Icons
![Show_Panel_icon](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/show_panel_toolbutton.png)
![Framed_Caret_icon](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/framed_caret_line_toolbutton.png)

The Framed Current Line feature preserves the color highlights of data fields on the caret line, while still indicating its current line status.

#### Plugin Panel:
![Plugin_Panel](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/plugin_panel.png)

#### Jump to Field popup:
![Jump_to_Field_popup](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/jump_to_field.png) »»
![Jump_to_Field_list](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/jump_to_field_with_list.png)

Useful for jumping between fields on long records with dozens of fields.


## Configuration Dialogs

#### File, Record & Field Configuration Dialog:
![FileType_Config](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/file_type_editor.png)

Heirarchical configuration of File Type, Record Type, and Field Labels & Widths. Previous configurations are automatically backed up with each save.

#### File Type Extract Dialog:
![FileType_Extract](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/file_type_extract.png)

Useful for sharing of file type definitions between teams & collaborators.

#### Theme & Color Configuration Dialog:
![Theme_Config](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/color_theme_editor.png)

Heirarchical configuration of Themes and Colors. Previous configurations are automatically backed up with each save.

#### Theme Extract Dialog:
![Theme_Extract](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/color_theme_extract.png)

Useful for sharing of theme definitions between teams & collaborators.

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
Files with data records that span multiple lines. End of record is indicated by a marker. For example: _<END_REC>_ Owing to such a marker, records for these files can be of varying width.

__Weather Stations List with location wiki and Daily Weather data__ _(A contrived format, for concept illustration)_
![Multi_Line](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multi_line_record_file.png)

