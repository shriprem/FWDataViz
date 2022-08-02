## Visualizer Theme Editor

![Theme_Config](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/color_theme_editor.png)

This editor is organized in a three-level hierarchy:

| Level | Brief Description |
|---|---|
| **Themes** | From the list of _Themes_, select a _Theme_ to modify or clone or delete. Or add a new _Theme_. |
| **Theme » Styles** | From the list of _Styles_ for the _Theme_ selected in Level 1, select a _Style_ to modify or clone or delete. Or add a new _Style_ for the selected _Theme_. |
| **Style Definition** | For the _Style_ selected in Level 2, define or modify its _Style Definition_. |

* The _Theme - Label_ field will accept Unicode characters.

* When the _Theme - Label_ field content is modified, the _New_ button is replaced by the _Reset_ button. When the change is accepted, the _New_ button will be restored back in place of the _Reset_ button.

* The color swatches in the Styles and Styles Definition section are clickable.

* Changes in the Style Definition section will disable change of selections at the Styles and Theme levels until the changes are accepted or reset or the themes file is saved or reloaded.

* Previous configurations are automatically backed up with each save. Use _Load Backup Themes File_ to load a backup file.

**See also**: [Extract Configuration Data](https://github.com/shriprem/FWDataViz/blob/master/docs/config_extract_dialog.md) | [Append Configuration Data](https://github.com/shriprem/FWDataViz/blob/master/docs/config_append_dialog.md)

> :sparkles: **_`TRIVIA`_**: The quirky sentences displayed in the _Output_ box are [*Pangrams*](https://en.wikipedia.org/wiki/Pangram). Clicking on the _Output_ box will randomly shuffle through the embedded set.