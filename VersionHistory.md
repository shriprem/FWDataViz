# Version History

### Version 1.4.0.1
#### Enhancements:
1. [Easy Keyboard Navigation](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_key_shortcuts.md) in the Data Extraction dialog.
---
### Version 1.4.0.0
#### Enhancements:
1. [Data Extraction](https://github.com/shriprem/FWDataViz/blob/master/docs/data_extract_dialog.md) feature to extract a handful data fields into an easily readable, quick report.
---
### Version 1.0.3.1
#### Enhancements:
1. Light & Dark mode Fluent UI toolbar icons added for the plugin.
2. Including ARM64 build. (_Please note that I am unable to test the ARM64 build since I don't have an ARM64 device, and it is being provided AS IS._)

##### _`ALERT!` This plugin release is only compatible with Notepad++ 8.0 and later releases._

#### Programming Note:
The Notepad++ 8.0 has introduced the Fluent UI icons in Small/Large, Light/Dark & Unfilled/Filled modes, making for a total of 8 possible combinations. But the newly added API function [`NPPM_ADDTOOLBARICON_FORDARKMODE`](https://github.com/notepad-plus-plus/notepad-plus-plus/commit/8a898bae3f84c03c44aaed25001e9fa1ddfa09aa) has allowed the plugins to supply images for only 4 Fluent UI images among the possible set of 8.

Due to the current NPP API limitation, this minor release for FWDataViz is only able to add Fluent UI icons for the Small/Large, Light/Dark Fluent UI modes but not the Filled/Unfilled Fluent UI modes. So the toolbar icons for this plugin will not change when switching between unfilled and filled Fluent UI selections in Notepad++ Preferences dialog.

---
### Version 1.0.3.0
#### Enhancements:
1. [Record Type Themes](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_extract_dialog.md)

---
### Version 1.0.2.0
#### Enhancements:
1. [Automatic Detection of File Types](https://github.com/shriprem/FWDataViz/blob/master/docs/auto_detect_file_type.md)

---
### Version 1.0.1.0
#### Enhancements:
1. *Cursor Position Data* display will shrink or grow to the dock panel width.
2. Long text in *Cursor Position Data* display will scroll horizontally instead of wrapping to the next line.
3. Added *Record Length [Current/Defined]* line in *Cursor Position Data* display.
