## Data Extraction

Data Extraction feature allows exporting of data from a select set of Fields and Record Types from a fixed-width data file into a new file.

The field data can be optionally prefixed and suffixed by text labels, separator text and new lines to effectively build an easily readable, quick report from the fixed-width file.

The data extraction feature can be especially useful with mixed-record fixed-width data files wherein column mode selection of field data is not easily possible.

### Sample Extraction Input
![Sample_Data_Extract_Dialog](https://raw.githubusercontent.com/shriprem/FWDataViz/DataExtractFeature/images/data_extract_dialog.png)
In the pictured sample:
* The `\n` in the _Prefix_ of the first line is providing a blank line spacing between DRN-Invoice groups in the output (see below).
* Line Items 3, 5 & 6 were ignored during Data Extraction processing since the _Record Type_ and _Field_ edits were blanks.
* The `\t` in the _Suffix_ for the _Invoice Number_ line item could also have been, alternatively, specified at the start of the _Prefix_ for the _Job Number_ line item, with no difference in the output data.

### Corresponding Output
![Sample_Data_Output](https://raw.githubusercontent.com/shriprem/FWDataViz/DataExtractFeature/images/data_extract_output.png)
