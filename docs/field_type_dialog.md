## Define Field Types

![Jump_to_Field_list](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/field_type_dialog.png)

_Field Types_ are useful for distinguishing generic fields occurring frequently in multiple Record Types and File Types.

In the above clip, field types have been defined for `AMOUNT` and `DATE`. These field types can then be associated with multiple items in the Field Labels list in the [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md) by specifying a colon after the field label and then the _Field Type_.
* The _Field Type_ value is not case sensitive.
* The spaces around the colon separator are optional.

##### Example Usage
```
Sale Date : DATE
Customer ID
Invoice Number
Item Code
Price:AMOUNT
Quantity
Total : Amount
Payment Received Date:Date
```

In the example above:
* The _Sale Date_ and _Payment Received Date_ fields have been associated with the `DATE` field type.
* The _Price_ and _Total_ fields have been associated with the `AMOUNT` field type.

These associated fields will be visualized using the distinctive style defined for the generic field type in the [Define Field Types](https://github.com/shriprem/FWDataViz/blob/master/docs/field_type_dialog.md) dialog. This distinctive field style will override Record and File Type Themes.

In a future version of the plugin, you will also be able to specify a validating regular expression for the generic field types. Then these validations can be used in the [Data Extraction dialog](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md) to list only those record fields passing or not passing these validations.

See also: [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md) | [Data Extraction dialog](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md)