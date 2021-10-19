## Multi-byte Character Data

![Multi-byte_Character_Data](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multibyte_character_data.png)

When files are likely to contain multi-byte character data, the plugin must be switched from using bytes to characters to determine field widths and column positions. To do so, the *Multi-byte Character Data* checkbox in the [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md) should be toggled ON.

When multi-byte character data mode is enabled, character column counts will be used in [Jump To Field](https://github.com/shriprem/FWDataViz/blob/master/README.md#jump-to-field-popup) and [Data Extraction](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md).

It should be noted that the [Scintilla](https://www.scintilla.org/index.html) component, which is at the core of Notepad++, uses *encoded characters* rather than *user-perceived characters* or *grapheme clusters* to count columns and process cursor movements. For more information on this aspect, refer to: [UTF-8 Everywhere](http://utf8everywhere.org).

### Quick Override from panel

![Multi-byte_Chars_Panel_Override](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/multibyte_chars_panel_override.png)

Some users may see the need to quickly toggle between byte-based and character-based modes without having to configure the setting in the [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md). There is such an override option control for quickly toggling the multi-byte character mode on the plugin panel. But this control is hidden by default for a reason explained later [below](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md#three-state-multi-byte-chars-checkbox-control).

#### Uncovering the hidden option
To make the **Multi-Byte Chars** option visible on the plugin panel, manually edit the `PLUGINS_CONFIG_DIR/FWDataViz/Preferences.ini` file to add the following line under the `[Preferences]` section:
```
ShowMBCharsOnPanel=Y
```

#### Three state *Multi-Byte Chars* checkbox control
The *Multi-Byte Chars* checkbox on the plugin panel is a three-state control. Its operation may not be immediately intuitive to many users of this plugin. That is why it is hidden by default. Users willing to take the extra effort to uncover this option can be reasonably expected to have read this section.

The three states are:
* CHECKED (OVERRIDE): When checked, the multi-byte character mode for the plugin is enabled for all file types.

* UNCHECKED (OVERRIDE): When unchecked, the multi-byte character mode for the plugin is disabled for all file types.

* MIXED (PASSTHRU'): In the indeterminate state, the multi-byte character mode activation for the plugin will be based on the setting for the document file type in the [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md).

#### Multi-byte Character Mode Active Indicator
The indicator is a single character that is selectively displayed to the left of the *Multi-Byte Chars* control.

| Indicator | Checkbox State(s) | Remarks |
|---|---|---|
| *  | CHECKED only | Current document is a valid FWDataviz file type. The multi-byte character mode is active due to the panel-level setting in override state. |
| + | MIXED only | Current document is a valid FWDataviz file type. The multi-byte character mode is active due to the file-type-level setting. |
| - | MIXED only | Current document is a valid FWDataviz file type. But the multi-byte character mode is *not* active due to the file-type-level setting. |
| None | CHECKED or MIXED | Current document is *not* a valid FWDataviz file type. |
| None | UNCHECKED | The multi-byte character mode is *not* active due to the panel-level setting in override mode. |

See also: [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md)