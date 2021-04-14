## Auto-Detect File Type (ADFT)

With the Auto-Detect File Type (ADFT) feature, the plugin quickly scans a file during file load or tab activation to determine if the file fits the template specification of a particular fixed-width data file type. If there is a match, the plugin will automatically visualize the file with the matching file-type configuration. Thus, this plugin can be a time saver for users handling numerous fixed width data files as part of their workflow.

For ADFT to work, it requires:
* The Fixed Width Data Visualizer plugin to be open in Notepad++.

* The Auto-detect File Type box on the plugin panel to be in the checked state.

* The ADFT configuration for a file type to have been specified in the [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md)


## ADFT in a Nutshell

![ADFT_Edit_Fields](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/file_type_adft.png)

1. Line Numbers begin at '1', counting forwards from the start of file. Negative line numbers count backwards from the end of file. Zero and numbers (+ve or -ve) beyond file line count will fail ADFT matching.

2. ADFT Regexes with no terminating '\$' will be used as _starts-with_ match. Whereas, those with terminating '\$' will be used as _full-line_ match. ADFT Regexes will have an implicit caret (\^) at the start if it has not been explicitly included.

3. The ADFT expression between the optional '\^' and '\$' signs shall be a valid regular expression.

With reference to the sample clip above, for a valid ADFT match on a file:
* Line 1 must start with: _PCA_
* Line 2 must start with: _BIPAC_
* Line 3 must start with an _H_, and must be followed by exactly 46, 47 or 48 characters, before the end of line.

## ADFT Usage Hints

ADFT feature works by utilizing up to three pairs of line numbers and their corresponding regular expressions for a template match with a file type. Reasonable familiarity with regular expressions is therefore desirable to configure the ADFT feature.

Look in the sample files packaged with the plugin for usage examples while you review the usage hints listed below.

* Each specified ADFT regular expression is used to _fully_ match its corresponding line.

* Each specified pair needs to have a proper match in the data for a file to be identified by the ADFT. If one pair fails a match, then that file type will be skipped by ADFT. If a file type can be identified by only one or two ADFT pairs, just use the minimal necessary, leaving the other pairs blank.

* An implicit caret [\^] is assumed for the ADFT regular expressions. If needed, for the sake of your own clarity, explicitly specify a '\^' at the start of the ADFT regular expressions.

* Specify a dollar sign [\$] if the regular expression you are specifying is a _full_ match on the line. If the last character of the specified regular expression is _not_ a dollar sign, ADFT will implicitly deem this to be a _starts-with_ match, and thereby will affix a '.*$' to the end of the specified expression. For a demo of both cases, see the ADFT configuration (pictured above) for the sample _Treasury IPAC File_ wherein only the third expression has an ending dollar sign.

* ADFT line numbers are _not_ zero-indexed. Use '1' for the first line, '2' for the 2nd line, and so on -- just as how they are numbered in Notepad++.

* When negative numbers are specified, ADFT matching will be performed on lines counting backwards from the end of the file. Use '-1' for the last line of the file, '-2' for the second-to-last line, and so on.

* Using negative line numbers in ADFT can get tricky due to a _ghost_ line that may or may not be present at the end of data files. If there is an end-of-line sequence at the very end of a file, then the last line of that file will actually be an empty line. If this is the consistent pattern with your data files, use '-2' to match on the last line with non-blank data. If there is no such consistent pattern with your data files, avoid using negative line numbers for ADFT.

* If the absolute value of the line number is zero or greater than the total line count of a file, the match will fail and ADFT will skip that file type. For example, if a data file has only 75 lines, then the ADFT line number has to be in the range of -75 to -1 or 1 to 75.


With some file types the ADFT can be unambiguous and reliable. With some others, ADFT can be ambiguous and unreliable. Let us review a few sample files amongst this latter category.

* _Daily Weather data_: In this sample file, it is only reasonably possible to check for the constant line length of a record. So for the sake of better certainty, this same check is repeated over three distinct lines before confirming a file type match.

* _ICD-10 Diagnosis Codes_ and _ICD-10 Procedure Codes_: These two sample file types have the same basic record template: `^.{7} .{1,392}$`. In this scenario, whichever file type is listed higher will be identified as the matching file type by ADFT. In such situations, use the File Type Move Up/Down buttons in the [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md) to prioritize your file
types for ADFT.

See also: [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md)