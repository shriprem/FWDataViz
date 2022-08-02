## Fixed Width File Metadata Editor

![FileType_Config](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/file_type_editor.png)

This editor is organized in a three-level hierarchy:

| Level | Brief Description |
|---|---|
| **File Types** | From the list of _File Types_, select a _File Type_ to modify or clone or delete. Or add a new _File Type_. |
| **File Type » Record Types** | From the list of _Record Types_ for the _File Type_ selected in Level 1, select a _Record Type_ to modify or clone or delete. Or add a new _Record Type_ for the selected _File Type_. |
| **Record Type » Fields** | For the _Record Type_ selected in Level 2, define or modify its _Field Labels_ and _Field Widths_. |

* Starting with v2.3.0.0, all fields in the Metadata Editor will accept multi-byte characters.

* The following fields are optional:
  * File Type - Records Terminator
  * [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md)
  * All [Auto-Detect File Type](https://github.com/shriprem/FWDataViz/blob/master/docs/auto_detect_file_type.md) fields
  * Record Type - Starts With
  * Record Type - Regex Key
  * [Record Type - Color Theme](https://github.com/shriprem/FWDataViz/blob/master/docs/record_type_theme.md)

* Fields with shared traits can be tagged so that they always render with a distinct visual styling. Fore more info, see: [Field Types](https://github.com/shriprem/FWDataViz/blob/master/docs/field_type_dialog.md).

* Fields tagged with an undefined field type such as _NO_STYLING_ or _DEFAULT_STYLE_ or _STYLE_DEFAULT_ will be rendered with the default background and foreground colors of the currently **Selected Theme** in Notepad++'s **Style Configurator**. For more info, see: [Field Types - Default Style](https://github.com/shriprem/FWDataViz/blob/master/docs/field_type_dialog.md#default-style).

* Field Labels and Field Widths are synchronized for editing and scrolling.

* Changes in the _Record Type » Fields_ section will disable change of selections at the File Type and Record Type levels until the changes are accepted or reset or the configuration file is saved or reloaded.

* Changes in the _File Type » Record Types_ section will disable change of selections at the File Type and Record Type levels until the changes are accepted or the configuration file is saved or reloaded.

* Changes in the _File Types_ section will disable change of selections at the File Type level until the changes are accepted or the configuration file is saved or reloaded.

* When any of the fields in either the _File Types_ or the _File Type » Record Types_ section are modified, the _New_ button in that section will be replaced by the _Reset_ button. When the changes are accepted, the _New_ button will be restored back in place of the _Reset_ button.

* Previous configurations are automatically backed up with each save. Use _Load Backup Configuration File_ to load a backup file.

**See also**: [Auto-Detect File Type Configuration](https://github.com/shriprem/FWDataViz/blob/master/docs/auto_detect_file_type.md) | [Record Type Theme](https://github.com/shriprem/FWDataViz/blob/master/docs/record_type_theme.md) | [Define Field Types](https://github.com/shriprem/FWDataViz/blob/master/docs/field_type_dialog.md) | [Extract Configuration Data](https://github.com/shriprem/FWDataViz/blob/master/docs/config_extract_dialog.md) | [Append Configuration Data](https://github.com/shriprem/FWDataViz/blob/master/docs/config_append_dialog.md) | [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md)