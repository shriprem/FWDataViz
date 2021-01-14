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

#### Plugin Panel:
![Plugin_Panel](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/plugin_panel.png)

#### Jump to Field popup:
![Jump_to_Field_popup](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/jump_to_field.png)
![Jump_to_Field_list](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/jump_to_field_with_list.png)


## Configuration Dialogs

#### File, Record & Field Configuration Dialog:
![FileType_Config](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/file_type_editor.png)

#### File Type Extract Dialog:
![FileType_Extract](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/file_type_extract.png)

#### Theme & Color Configuration Dialog:
![Theme_Config](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/color_theme_editor.png)

#### Theme Extract Dialog:
![Theme_Extract](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/color_theme_extract.png)


## Sample data files

### Homogenous Record-type Fixed-width Data Files
Data file with data in the same record format.

__NOAA Weather Stations List__
![Single_Rec](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/single_rec_weather_stations.png)


### Mixed Record-type Fixed-width Data Files
Data files with data in multiple record formats.

__NOAA Weather Stations List__
_(Same data from the preceding sample, but now flagged based on the GSN field)_
![Multi_Rec](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multi_rec_weather_stations.png)


__Treasury IPAC File__
_(real format, but with dummy data)_
![Multi_Rec](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multi_rec_ipac_file.png)


### Multi-Line Record-type data sample 
__Weather Stations List with location wiki and Daily Weather data__ _(Contrived format, as sample illustration)_
![Multi_Line](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multi_line_record_file.png)

