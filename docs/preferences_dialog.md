## Preferences dialog

![Prefereces_dialog](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/preferences_dialog.png)

### Clear button click unchecks Auto-detect File Type [_Default: Unchecked_]
* When this box is checked, clicking the _Clear_ button on the plugin panel will also uncheck the _Auto-detect File Type_ box on the plugin panel, thereby totally clearing the fixed-width data visualization.
* When this is unchecked and the _Auto-detect File Type_ box on the plugin panel is checked, clicking the _Clear_ button on the plugin panel will just re-render the fixed-width data visualization.

### Closing plugin panel clears visualization [_Default: Checked_]
* When this box is checked, closing the plugin panel will also clear the fixed-width data visualization of the current document in Notepad++.
* When this box is unchecked, closing the plugin panel will _NOT_ clear the fixed-width data visualization of the current document in Notepad++.

### Show Multi-byte check box in plugin panel [_Default: Unchecked_]
When this box is checked, the _Multi-Byte Chars_ checkbox will become visible on the plugin panel. Since this is a **_3-state_** checkbox, please review the documentation for it at: [Multi-byte Character Data Visualization](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md#quick-override-from-panel).

### Hop Right: Move to the left edge of the following field [_Default: Unchecked_]
* When this box is unchecked, clicking the right [Hop](https://github.com/shriprem/FWDataViz/blob/master/docs/field_navigation.md#hop-field) button will move cursor to the right edge of the current field. If already at the right edge of the field, the cursor will be moved to the right edge of the following field.
* When this box is checked, clicking the right [Hop](https://github.com/shriprem/FWDataViz/blob/master/docs/field_navigation.md#hop-field) button will move cursor to the left edge of the following field.

### Fold Line Color & Alpha
* Click the colored square to specify the color for the lines that indicate the folding block headers. [_Notepad++ default color: Black_]
* Adjust the slider to specify the color transparency (alpha) for the lines that indicate the folding block headers. [_Notepad++ default value: Opaque_]

_NOTE: Both the color and transparency (alpha) settings will apply globally to all documents in Notepad++._
