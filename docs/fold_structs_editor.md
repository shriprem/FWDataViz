# Fold Structures Editor

![image](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/fold_struct_editor.png)

This editor is organized in a three-level hierarchy:

| Level | Brief Description |
|---|---|
| **[Fold Structures](#Fold-Structures)** | From the list of _Fold Structures_, select a _Fold Structure_ to modify or clone or delete. Or add a new _Fold Structure_. |
| **[Fold Structure » Fold Blocks](#Fold-Structure--Fold-Blocks)** | From the list of _Fold Blocks_ for the _Fold Structure_ selected in Level 1, select a _Fold Block_ to modify or clone or delete. Or add a new _Fold Block_ for the selected _Fold Structure_. |
| a. **[Fold Block » Implicit Terminators](#Fold-Block--Implicit-Terminators)** | For the _Fold Block_ selected in Level 2, view its implicit terminators. _(This list is for reference only.)_ |
| b. **[Fold Block » Explicit Terminators](#Fold-Block--Explicit-Terminators)** | For the _Fold Block_ selected in Level 2, add or modify or delete its explicit termination records. |

* Changes in the _Fold Block » Implicit Terminators_ section will disable change of selections at the Fold Structure and Fold Block levels until the changes are accepted or reset or the configuration file is saved or reloaded.

* Changes in the _Fold Structure » Fold Blocks_ section will disable change of selections at the Fold Structure and Fold Block levels until the changes are accepted or the configuration file is saved or reloaded.

* Changes in the _Fold Structures_ section will disable change of selections at the Fold Structure level until the changes are accepted or the configuration file is saved or reloaded.

* When any of the fields in either the _Fold Structures_ or the _Fold Structure » Fold Blocks_ or _Fold Block » Implicit Terminators_ section are modified, the _New_ button in that section will be replaced by the _Reset_ button. When the changes are accepted, the _New_ button will be restored back in place of the _Reset_ button.

* Previous configurations are automatically backed up with each save. Use _Load Backup Configuration File_ to load a backup file.


## Fold Structures

### Fold Structure » File Type

:book: Related: [Definitions (_INI File_)](https://github.com/shriprem/FWDataViz/blob/master/docs/foldable_record_blocks.md#Definitions-INI-File) - `FileType`

This selection will link the _Fold Structure_ to one of the _File Types_ defined in the [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md).

Multiple Fold Structures with slightly different settings can be defined on the same _File Type_. In such cases, only the _Fold Structure_ listed first in the _Fold Structures_ list box will become active and get applied on a file of matching _File Type_. Hence the availability of Up & Down buttons below the _Fold Structures_ list box. Otherwise, the ordering of items in this list box does not matter.

> :warning: If the ordinal (_i.e.,_ the sequential positioning in the _File Types_ list) or the _File Label_ for the _File Type_ is modified in the [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md), you will need to manually re-associate the _File Type_ with the _Fold Structure_ here.

### Automatically apply Fold Structure on matching files


 :book: Related: [Definitions (_INI File_)](https://github.com/shriprem/FWDataViz/blob/master/docs/foldable_record_blocks.md#Definitions-INI-File) - `FoldLevelAuto`

This option specifies if the plugin should automatically do a full file scan and apply the specified folding structure for the matching File Type after a file loads in the Notepad++ editor. Recommend unchecking this option for File Types with likely large data files, when this process can take a long time.

## Fold Structure » Fold Blocks

### Fold Block - Header Record Type

 :book: Related: [Definitions (_INI File_)](https://github.com/shriprem/FWDataViz/blob/master/docs/foldable_record_blocks.md#Definitions-INI-File) - `HeaderRecords`

This selection will link the _Fold Block_ to one of the _Record Types_ for the _Fold Structure's File Type_, as defined in the [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md).

Multiple Fold Blocks with slightly different settings can be defined on the same _Record Type_. In such cases, only the _Fold Block_ listed first in the _Fold Block_ list box will become active and get applied on a record of matching _Record Type_. Hence the availability of Up & Down buttons below the _Fold Blocks_ list box. Otherwise, the ordering of items in this list box does not matter.

> :warning: If the ordinal (_i.e.,_ the sequential positioning in the _Record Types_ list) for the _Record Type_ is modified in the [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md), you will need to manually re-associate the _Record Type_ with the _Fold Block_ here.

### Fold Block - Priority

 :book: Related: [Definitions (_INI File_)](https://github.com/shriprem/FWDataViz/blob/master/docs/foldable_record_blocks.md#Definitions-INI-File) - `RECnnn_Priority`

 A numeric value denoting the order priority for a _Fold Block_. A block with _lower_ numeric value of priority will enclose a block with _higher_ numeric value of priority. i.e., A block with _lower_ numeric value of priority will have a _higher_ precedence.

### Recursive Fold Block

 :book: Related: [Definitions (_INI File_)](https://github.com/shriprem/FWDataViz/blob/master/docs/foldable_record_blocks.md#Definitions-INI-File) - `RECnnn_Recursive`

 This option will specify whether the _Fold Blocks_ with same priority value will nest inside each other. Since most data files are _non-recursive_, this option will need to be enabled very rarerly.


## Fold Block » Implicit Terminators

:book: See: [Record Block Termination](https://github.com/shriprem/FWDataViz/blob/master/docs/foldable_record_blocks.md#record-block-termination)

This list is displayed for reference only. It is dynamically built by the system based on the _Priority_ and _Recursive_ values of the _Header Record Types_ for the _Fold Block_.

## Fold Block » Explicit Terminators

 :book: Related: [Definitions (_INI File_)](https://github.com/shriprem/FWDataViz/blob/master/docs/foldable_record_blocks.md#Definitions-INI-File) - `RECnnn_EndRecords`

This selection will link multiple _End Records_ to the _Record Types_ for the _Fold Structure's File Type_, as defined in the [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md).

> :warning: If the ordinal (_i.e.,_ the sequential positioning in the _Record Types_ list) for the _Record Type_ is modified in the [File Type Metadata Editor](https://github.com/shriprem/FWDataViz/blob/master/docs/file_type_config_dialog.md), you will need to manually re-associate the _Record Type_ with the _End Records_ here.

**See also**: [Foldable Record Blocks](https://github.com/shriprem/FWDataViz/blob/master/docs/foldable_record_blocks.md) | [Extract Configuration Data](https://github.com/shriprem/FWDataViz/blob/master/docs/config_extract_dialog.md) | [Append Configuration Data](https://github.com/shriprem/FWDataViz/blob/master/docs/config_append_dialog.md)
