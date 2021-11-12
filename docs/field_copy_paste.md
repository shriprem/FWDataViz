## Copy and Paste Field Data

![Field  Copy](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/field_copy_paste.png)

Quickly copy or paste field data with smart left or right alignment and padding or trimming.

### Field Copy
Copy current field data, with optional trimming based on the currently highlighted field alignment.

* If _Trim Field Copy_ option is checked, the Field Copy process will ignore padding character(s) on either the left or right edge of the field, based on the currently highlighted field alignment selection. The field alignment is automatically set based on which field edge is closer to the current cursor position. But it can also be changed by one of the following ways:
   * Using the `ALT + Left Arrow` or `ALT + Right Arrow` keyboard shortcuts.
   * Clicking the left or right field hop buttons on the panel.
   * Clicking the RPAD or LPAD fields on the panel.

* If more than one single-byte or multi-byte characters are specified for RPAD or LPAD, the Field Copy process will trim by matching for the full (or partial only on the last match) sequence of the specified characters.

* The Field Copy action is accessible via the default keyboard shortcut: `ALT + , (comma)` or by clicking the _Field Copy_ button on the panel.


### Field Paste

Paste the current clipboard text into the current field, replacing its current contents.

* The pasted text will be aligned either to the left or the edge of the field, based on the currently highlighted field alignment selection. The field alignment is automatically set based on which field edge is closer to the current cursor position. But it can also be changed by one of the following ways:
   * Using the `ALT + Left Arrow` or `ALT + Right Arrow` keyboard shortcuts.
   * Clicking the left or right field hop buttons on the panel.
   * Clicking the RPAD or LPAD fields on the panel.

* If the clipboard text is shorter than the field width, it will be paddded with the RPAD or LPAD character(s) on the edge _opposite_ to the alignment.
   * If either the RPAD or LPAD fields is left blank, padding will default to the `SPACE` character.
   * The RPAD and LPAD fields accept more than one single byte or multi-byte characters.

* If the clipboard text is longer than the field width, it will be trimmed to fit the field width. The clipboard text will be trimmed always on the right side (ignoring the alignment selection).

* The Field Paste action is accessible via the default keyboard shortcut: `ALT + . (period)` or by clicking the _Field Paste_ button on the panel.


See also: [Hop Field](https://github.com/shriprem/FWDataViz/blob/master/docs/field_navigation.md#hop-field) | [Menu Item Keyboard Shortcuts](https://github.com/shriprem/FWDataViz/blob/master/docs/menu_shortcuts.md)
