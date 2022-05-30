## Fixed Width File Metadata Editor

![FileType_Config](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/file_type_editor.png)

This editor is organized in a three-level hierarchy:
1. File Types
2. Record Types
3. Fields

* Starting with v2.3.0.0, all fields in the Metadata Editor will accept multi-byte characters.

* The following fields are optional:
  * File Records Terminator
  * [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md)
  * All [Auto-Detect File Type](https://github.com/shriprem/FWDataViz/blob/master/docs/auto_detect_file_type.md) fields
  * Record Starts With
  * Record Regex Key
  * [Record Color Theme](https://github.com/shriprem/FWDataViz/blob/master/docs/record_type_theme.md)

* Fields with shared traits can be tagged so that they always render with a distinct visual styling. Fore more info, see: [Field Types](https://github.com/shriprem/FWDataViz/blob/master/docs/field_type_dialog.md).

* Fields tagged with an undefined field type such as _NO_STYLING_ or _DEFAULT_STYLE_ or _STYLE_DEFAULT_ will be rendered with the default background and foreground colors of the currently **Selected Theme** in Notepad++'s **Style Configurator**. For more info, see: [Field Types - Default Style](https://github.com/shriprem/FWDataViz/blob/master/docs/field_type_dialog.md#default-style).

* Field Labels and Field Widths are synchronized for editing and scrolling.

* Changes in the Fields section will disable change of selections at the File Types and Record Type levels until the changes are accepted or reset or the configuration file is saved or reloaded.

* Changes in the Record Types section will disable change of selections at the File Types and Record Type levels until the changes are accepted or the configuration file is saved or reloaded.

* Changes in the File Types section will disable change of selections at the File Types levels until the changes are accepted or the configuration file is saved or reloaded.

* Previous configurations are automatically backed up with each save. Use _Load Backup Configuration File_ to load a backup file.

**See also**: [Auto-Detect File Type Configuration](https://github.com/shriprem/FWDataViz/blob/master/docs/auto_detect_file_type.md) | [Record Type Theme](https://github.com/shriprem/FWDataViz/blob/master/docs/record_type_theme.md) | [Define Field Types](https://github.com/shriprem/FWDataViz/blob/master/docs/field_type_dialog.md) | [Extract File Type Data](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_extract_dialog.md) | [Append File Type Data](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_append_dialog.md) | [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md)