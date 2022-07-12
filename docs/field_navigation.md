## Field Hop and Jump

### Jump to Field
![Jump_to_Field_list](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/jump_to_field_with_list.png)

Useful for navigating between fields on long records with dozens of fields.

#### Optional Settings
1. **_Show sequence numbers in the Select Field list_:** Check this box if you prefer sequence numbers as prefixes to field names in the _Select Fields_ dropdown list.
2. **_Cursor Flash duration (in seconds)_:** Use this slider to specify how long the cursor should blink in block mode after it has jumped to the new field.


### Hop Field
![Jump_to_Field_list](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/field_navigation.png)

More frequently, there is a need to hop to the other edge of the current field or an adjacent field. For example, you may want to quickly toggle between the left and right alignment selections for Field Copy or Field Paste.

For field hops, use the left and right arrow buttons on either side of the _Jump to Field_ button. Or better still, use the `ALT + Left Arrow` or `ALT + Right Arrow` keyboard shortcuts.

* With a left hop, if the cursor is already at the left edge of the current field, it will move to the left edge of the preceding field within the same record.

* With a right hop, if the cursor is already at the right edge of the current field, it will move to the right edge of the following field within the same record.


See also: [Field Copy and Field Paste](https://github.com/shriprem/FWDataViz/blob/master/docs/field_copy_paste.md) | [Menu Item Keyboard Shortcuts](https://github.com/shriprem/FWDataViz/blob/master/docs/menu_shortcuts.md)
