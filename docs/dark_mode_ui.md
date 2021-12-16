## _FWDataViz_ in Darkmode

Darkmode for the _FWDataViz_ plugin is rendered by the standalone [NPP_Plugin_Darkmode.dll](https://github.com/shriprem/NPP_Plugin_Darkmode).

This DLL can be shared by multiple plugins, thus reducing the overall memory footprint. Notepad++ will only load a copy of the DLL located either in:
* The folder for the first plugin (_in alphabetical order_) that requires this DLL.
* Or, in the common `<Notepad++_executable_folder>`.

You can verify the singleton loading of the `NPP_Plugin_Darkmode.dll` using the [Process Explorer](https://docs.microsoft.com/en-us/sysinternals/downloads/process-explorer) utility.


#### _Restart of Notepad++ is necessary when darkmode settings are changed_
The _FWDataViz_ plugin is able to render in darkmode based on the settings stored in the `config.xml` file for Notepad++. (The `config.xml` is located either in the `%APPDATA%\Notepad++` folder for full installations of NPP or the `<NPP_Portable>` folder for portable versions of NPP.)

When darkmode is enabled or disabled, or when darkmode tone settings are changed in Notepad++, those changes are only confined to the Notepad++ process memory. These setting changes remain inaccessible to the _FWDataViz_ plugin until Notepad++ is closed. Only when Notepad++ is closed, the setting changes are saved to the `config.xml` file by the Notepad++ process.

Therefore, until the time when Notepad++ API is enhanced to provide these setting changes to the plugins dynamically, a restart of Notepad++ is necessary for the _FWDataViz_ plugin to be able to reflect changes in the darkmode settings.


### Plugin Panel in dark mode (_in Olive tone_)
![Plugin_Panel](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/plugin_panel_dm.png)

### FileType Dialog in dark mode (_in Cyan tone_)
![FileType_Config](https://raw.githubusercontent.com/shriprem/FWDataViz/master/images/file_type_editor_dm.png)
