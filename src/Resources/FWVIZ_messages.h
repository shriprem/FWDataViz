#pragma once

#define FWVIZMSG                    0x1000

/// <summary>
/// Used by EximFileTypeDialog to Append File Type / Theme / Fold Struct data
/// </summary>
/// <param name="WPARAM">: 0 - Not Used</param>
/// <param name="LAPRAM">: LPTSTR to the temp file name with data to be appended</param>
#define FWVIZMSG_APPEND_EXIM_DATA   (FWVIZMSG + 1)

