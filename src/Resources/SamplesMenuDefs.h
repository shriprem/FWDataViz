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
      "FT001_WEATHER_STATIONS_LIST"
   },
   {
      SINGLE_REC,
      "[Real Data] ☀ Daily Weather Data",
      L"AYM00089664.dly",
      "FT002_DAILY_WEATHER_DATA"
   },
   {
      SINGLE_REC,
      "[Real Data] ☤ ICD-10 Diagnosis Codes",
      L"icd10cm_codes_2021.txt",
      "FT003_ICD-10_DIAGNOSIS_CODES"
   },
   {
      SINGLE_REC,
      "[Real Data] ☣ ICD-10 Order Codes",
      L"icd10cm_order_2021.txt",
      "FT004_ICD-10_ORDER_CODES"
   },
   {
      SINGLE_REC,
      "[Real Data] ₽ ICD-10 Procedure Codes",
      L"icd10pcs_codes_2021.txt",
      "FT005_ICD-10_PROCEDURE_CODES"
   },
   {
      MULTI_REC,
      "[Demo Data - Foldable] Ŧ Treasury IPAC (ANSI) File",
      L"IPAC_ANSI.txt",
      "FT010_TREASURY_IPAC_FILE"
   },
   {
      MULTI_REC,
      "[Mixed Data] ☀ Multiple GSN Daily Weather",
      L"MultiGSN.txt",
      "FT007_MULTIPLE_GSN_DAILY_WEATHER"
   },
   {
      MULTI_REC,
      "[Real Data] ⌂ GSN-Flagged Weather Stations List",
      L"ghcnd-stations.txt",
      "FT006_GSN-FLAGGED_WEATHER_STATIONS"
   },
   {
      MULTI_REC,
      "[Real Data - Foldable] ☣ ICD-10 Billable-Flagged Order Codes",
      L"icd10cm_order_2021.txt",
      "FT008_ICD-10_BILLABLE-FLAGGED_ORDER_CODES"
   },
   {
      MULTI_REC,
      "[Demo Data - Foldable] § Ouroboros Fold Levels",
      L"Ouroboros.txt",
      "FT012_OUROBOROS_FOLD_LEVELS"
   },
   {
      MULTI_LINE,
      "[Demo Data] Ŧ Multi-Line Multi-Byte ĪƤĂĊ File",
      L"IPAC_Unicode.txt",
      "FT011_MULTI-LINE_MULTI-BYTE_FILE"
   },
   {
      MULTI_LINE,
      "[Mixed Data] ☀ Multi-Line GSN Daily Weather",
      L"MultiLineGSN.txt",
      "FT009_MULTI-LINE_GSN_DAILY_WEATHER"
   }
};
