## Foldable Record Blocks
### Brief Usage Notes
The definition scheme for record blocks will reside in the new `FoldStructs.ini` file. Upon the first run of Notepad++ after upgrading the _FWDataViz_  dll, this new file will get copied into either of the two folder locations:
1. `%APPDATA%/Notepad++/plugins/Config/FWDataViz/` (_for non-portable installs of NPP_) or
2. `<Notepad++_executable_folder>/plugins/Config/FWDataViz/` (_for portable installs of NPP_)

#### `FoldStructs.ini` file layout
* In the `Base` section, the `FoldStructCount` should indicate the number of file types that are being defined with folding record blocks.
* Each file type will have its own section in the format **FS**_nnn_, starting with `FS001`.

#### With each file type section:
* `FileType` value should exactly match the corresponding file type section name in the `Visualizer.ini` file.
* `FileLabel` value should _optionally_ match the corresponding `FileLabel` value in the `Visualizer.ini` file.
* `FoldLevelAuto` values can be either `Y` or `N`. This will specify whether, after a file loads into the NPP editor, the plugin should automatically try to do a full file scan and apply the specified folding structure for the matching file type. Recommend setting this to `N` for file types with likely large data files, when this process can take a longer time.
* `HeaderRecords` define a comma separated list of record types that will be headers for a folding record block. The record types should exactly match the corresponding record types for the file type in the `Visualizer.ini` file.
* `RECnnn_Priority` value will specify the order priority for a record block. A block with _lower_ numeric value of priority will have a higher precedence. i.e., A block with _lower_ numeric value of priority will enclose a block with _higher_ numeric value of priority. Recommend using values in increments of  10, so that blocks that need to be added at a later time can be specified with intermediate values. For example, the outermost record block can have a priority of _10_, the next record block that may be enclosed underneath it can have a priority of _20_, and so on.
* `RECnnn_Recursive` value will specify whether record blocks with same priority value should nest inside each other. For most data files, this value will be `N`.  If this value is  set to `Y`, the result will be a nested folding structure similar to the nesting in the pseudocode below.
```BASIC
IF (CONDITION#1) THEN
   IF (CONDITION#2) THEN
      IF (CONDITION#3) THEN
         ...
      END IF
   END IF
END IF
```
* `RECnnn_EndRecords` value will specify record types that will terminate the record block. The end-record types should exactly match the corresponding record types for the file type in the `Visualizer.ini` file. The record blocks will auto-terminate with:
   *  with end of file or
   *  by another record block that outranks it (i.e., one with a _lower_ numeric value of _Priority_) or
   *  by another record block of the same rank when `RECnnn_Recursive=N` for the record block that will be terminated.
* **_Outranking block termination_**: A record block with a _lower_ numeric value of priority will terminate _multiple_ levels of nested record blocks with _higher_ numeric values of priority that are present above it in the data file.
*  **_Peer block termination_**: A peer record block will terminate a _single_ level of record block with an _equal_ numeric value of priority that is present above it in the data file.

---
### Sample Files with Folding
View the sample files and then review their definitions in `FoldStructs.ini` file.
#### 1. ICD-10 Billable-Flagged Order Codes
This file has the most basic record block folding with just two record types, and only one level of folding.
![image](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/foldable_orders_file.png)

#### 2. Treasury IPAC (ANSI) File
This file has three levels of hierarchical folding. Yet the structure is simple since no recursive nesting is involved.
![image](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/foldable_ipac_file.png)

#### 3. Ouroboros Fold Levels
This sample file is named after the icon of [a serpent eating its own tail](https://en.wikipedia.org/wiki/Ouroboros). This contrived sample file is being included to showcase the full possibilities with the record block folding feature of _FWDataViz_.
* Record type 1 (marker `P3`) is _recursive_ and terminated by record type 2.
* Record type 2 (marker `Q4`) is _recursive_ and terminated by record type 3.
* Record type 3 (marker `R5`) is _recursive_ and terminated by record type 4.
* Record type 4 (marker `S6`) is _recursive_ and terminated by record type 1.
* Record type 5 (marker `TF`) is _non-recursive_ and terminates record types 1, 2, 3 & 4.

To gain better sense of the folding structure in this file, turn on the `Show Calltip` option on the side panel, and then click on lines 1 through 10. The last line in the calltip will indicate the fold level for the current line.

![image](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/foldable_ouroboros_file.png)

