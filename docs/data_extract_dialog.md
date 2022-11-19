## Data Extraction

Data Extraction feature allows exporting of data from a select set of fields and record types of a fixed-width data file into a new document.

The data extraction feature can be especially useful with mixed-record fixed-width data files wherein column mode (ALT+click) selection of field data is not feasible.

The field data can be optionally prefixed and suffixed by text labels, separator text and new lines to effectively build an easily readable, quick report from a dense fixed-width file.

The _Data Extraction_ dialog is launched by clicking the _Data Extraction_ button that appears on the _FWDataViz_ panel when the currently open document has been visualized with a specific _File Type_ by the plugin.

>#### :warning: Caveats
>* The data extraction performance is inversely proportional to the size of the fixed-width data file.
>* The data extraction performance is also inversely proportional to the number of [_line items_](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md#line-items). Although the Data Extraction dialog will allow up to 30 line items, try limiting between 5 and 10 line items.

### Data Extraction dialog
![Data_Extraction_Dialog](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/data_extract_dialog.png)

#### Line Items
* The Data Extraction dialog will allow up to 30 _Line Items_, _i.e.,_ 10 line items per [page](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md#line-item-pages) across 3 pages. In each line item, you can specify a _Prefix Text_, a _Record Type_, a _Field_, and a _Suffix Text_.
* Only line items with a specified _Record Type_ and _Field_ are used for data extraction. _Prefix Text_ and _Suffix Text_ are optional.
* The _Record Type_ is a dropdown list derived from the list of record types of the current document _File Type_.
* The _Field_ is a dropdown list derived from the list of field types of the specified _Record Type_.
* Check the _Trim Leading & Trailing Spaces_ box if you need to trim the leading and trailing spaces of all fields data. The state of this checkbox will be saved as part of the template.
* In _Prefix_ and _Suffix Text_ boxes, use `\n` for new lines and `\t` for TABs.
   * `\n` can be used to achieve a blank line separation between records or, preferably, a group of records.
   * `\t` can also be used at the start of a Prefix on the first field of a record type to achieve indentation for a hierarchical listing in the output data, as seen in the sample below.
* Use the `+` button to insert a blank line item. The current line item and all others below it will shift down by a line. This action may also result in the addition of a new page if the last line item on the current page is not blank; but it will also effectively delete the 30th line item _i.e.,_ the last line item on the last allowed page.
* Use the `-` button to delete the current line item. All line items below the current line item will shift up by one line. This action may also result in the deletion of a following page.
* Use the up and down buttons at the bottom of the line items group box to reorder the line items.
* Use [keyboard shortcuts](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_key_shortcuts.md) for easier navigation and manipulation of line items.

#### Line Item Pages
* The current page number can be inferred from the sequence numbers preceding each line item.
* The _Add Page_ button is disabled on the 3rd page _i.e.,_ the last allowed page.
* The _Delete Page_ button is disabled on the 1st page.
* Use [keyboard shortcuts](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_key_shortcuts.md) for easier navigation and manipulation of pages.

#### Reusable Templates
You can save the specified line items as templates for future reuse. Each template is associated with its document _File Type_.
##### Saving
* Templates will be saved into a common `Extractions.ini` configuration file in the `<NPP_Plugins_folder>/FWDataViz` folder.
* Templates need to have unique names -- even for templates across different _File Types_. Reusing a name while saving a template will overwrite the previously exisiting template with the same name, reassociating the template with its current document _File Type_.
* "[" and "]" characters are disallowed for template names.
* When templates are saved, any totally blank line items in between are skipped. Only the non-blank line items wherein at least one of its _Prefix Text_, _Record Type_, _Field_ and _Suffix Text_ boxes have some specified value are saved into a template.
##### Loading
* The _Load Template_ dropdown will by default list only the templates previously saved with the matching current document _File Type_.
* When the _Show only current FileType Templates_ box is unchecked, all templates (of all different _File Types_) will be listed in the _Load Template_ dropdown. The templates from other _File Types_ are listed with the `[Other]` prefix.
* Uncheck the _Show only current FileType Templates_ box to:
   * Load templates from closely related file types.
   * Load templates orphaned due to changes in name or order of _File Types_ in the [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md).

### Sample Data Extraction Input
![Data_Extraction_Sample](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/data_extract_sample.png)

* The `\n` in the _Prefix_ of the first line is providing a blank line spacing between DRN-Invoice groups in the output (see below).
* Line Items 3, 5 & 6 were ignored during Data Extraction processing since the _Record Type_ and _Field_ boxes were blanks.
* Line Item 4 was ignored during Data Extraction processing since the _Field_ box was blank.
* In Line Item 4, the Work Date field was specified in a template from a closely related but different _File Type_. The position for the _Work Date_ field from that other _File Type_ was beyond the defined set of fields for the _Treasury IPAC File_ format. So the _Field_ edit for that line item did not load from the template.
* The `\t` in the _Suffix_ for the _Invoice Number_ line item could also have been, alternatively, specified at the start of the _Prefix_ for the _Job Number_ line item, with no difference in the resultant output data.


### Sample Data Extraction Output
_[This output data was derived by running the template pictured above on the_ Treasury IPAC File (View Sample Files » Sample Multiple-Record-Type Files » Treasury IPAC File) _that is packaged with this plugin.]_

![Sample_Data_Output](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/data_extract_output.png)

See also: [Data Extract Dialog - Keyboard Navigation](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_key_shortcuts.md) | [Multi-byte Character Data](https://github.com/shriprem/FWDataViz/blob/master/docs/multibyte_character_data.md)