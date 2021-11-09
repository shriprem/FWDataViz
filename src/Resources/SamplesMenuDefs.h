#pragma once

#include <string>

using std::wstring;

const enum SampleFileTypes {
   SINGLE_REC,
   MULTI_REC,
   MULTI_LINE
};

struct SampleFileInfo {
   SampleFileTypes sample_type;
   string display_name;
   wstring file_name;
   string file_type;
};

const SampleFileInfo gSampleFiles[] =
{
   {
      SINGLE_REC,
      "[Real Data] ⌂ Weather Stations List",
      L"ghcnd-stations.txt",
      "FT001_⌂_WEATHER_STATIONS_LIST"
   },
   {
      SINGLE_REC,
      "[Real Data] ☀ Daily Weather Data",
      L"AYM00089664.dly",
      "FT002_☀_DAILY_WEATHER_DATA"
   },
   {
      SINGLE_REC,
      "[Real Data] ☤ ICD-10 Diagnosis Codes",
      L"icd10cm_codes_2021.txt",
      "FT003_☤_ICD-10_DIAGNOSIS_CODES"
   },
   {
      SINGLE_REC,
      "[Real Data] ☣ ICD-10 Order Codes",
      L"icd10cm_order_2021.txt",
      "FT004_☣_ICD-10_ORDER_CODES"
   },
   {
      SINGLE_REC,
      "[Real Data] ₽ ICD-10 Procedure Codes",
      L"icd10pcs_codes_2021.txt",
      "FT005_₽_ICD-10_PROCEDURE_CODES"
   },
   {
      MULTI_REC,
      "[Dummy Data] ╦ Treasury IPAC (ANSI) File",
      L"IPAC_ANSI.txt",
      "FT010_╦_TREASURY_IPAC_FILE"
   },
   {
      MULTI_REC,
      "[Mixed Data] ☀ Multiple GSN Daily Weather",
      L"MultiGSN.txt",
      "FT007_☀_MULTIPLE_GSN_DAILY_WEATHER"
   },
   {
      MULTI_REC,
      "[Real Data] ⌂ GSN-Flagged Weather Stations List",
      L"ghcnd-stations.txt",
      "FT006_⌂_GSN-FLAGGED_WEATHER_STATIONS"
   },
   {
      MULTI_REC,
      "[Real Data] ☣ ICD-10 Billable-Flagged Order Codes",
      L"icd10cm_order_2021.txt",
      "FT008_☣_ICD-10_BILLABLE-FLAGGED_ORDER_CODES"
   },
   {
      MULTI_LINE,
      "[Dummy Data] ╦ Multi-Line Multi-Byte Ï₱₳₡ File",
      L"IPAC_Unicode.txt",
      "FT011_╦_MULTI-LINE_MULTI-BYTE_Ï₱₳₡_FILE"
   },
   {
      MULTI_LINE,
      "[Mixed Data] ☀ Multi-Line GSN Daily Weather",
      L"MultiLineGSN.txt",
      "FT009_☀_MULTI-LINE_GSN_DAILY_WEATHER"
   }
};
