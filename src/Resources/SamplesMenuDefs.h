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
   wstring display_name;
   wstring file_name;
   wstring file_type;
};

const SampleFileInfo gSampleFiles[] =
{
   {
      SINGLE_REC,
      L"[Real Data] Weather Stations List",
      L"ghcnd-stations.txt",
      L"FT001_WEATHER_STATIONS_LIST"
   },
   {
      SINGLE_REC,
      L"[Real Data] Daily Weather Data",
      L"AYM00089664.dly",
      L"FT002_DAILY_WEATHER_DATA"
   },
   {
      SINGLE_REC,
      L"[Real Data] ICD-10 Diagnosis Codes",
      L"icd10cm_codes_2021.txt",
      L"FT003_ICD-10_DIAGNOSIS_CODES"
   },
   {
      SINGLE_REC,
      L"[Real Data] ICD-10 Order Codes",
      L"icd10cm_order_2021.txt",
      L"FT004_ICD-10_ORDER_CODES"
   },
   {
      SINGLE_REC,
      L"[Real Data] ICD-10 Procedure Codes",
      L"icd10pcs_codes_2021.txt",
      L"FT005_ICD-10_PROCEDURE_CODES"
   },
   {
      MULTI_REC,
      L"[Dummy Data] Treasury IPAC (ANSI) File",
      L"IPAC_ANSI.txt",
      L"FT010_TREASURY_IPAC_FILE"
   },
   {
      MULTI_REC,
      L"[Dummy Data] Treasury IPAC (Unicode) File",
      L"IPAC_Unicode.txt",
      L"FT010_TREASURY_IPAC_FILE"
   },
   {
      MULTI_REC,
      L"[Mixed Data] Multiple GSN Daily Weather",
      L"MultiGSN.txt",
      L"FT007_MULTIPLE_GSN_DAILY_WEATHER"
   },
   {
      MULTI_REC,
      L"[Real Data] GSN-Flagged Weather Stations List",
      L"ghcnd-stations.txt",
      L"FT006_GSN-FLAGGED_WEATHER_STATIONS"
   },
   {
      MULTI_REC,
      L"[Real Data] ICD-10 Billable-Flagged Order Codes",
      L"icd10cm_order_2021.txt",
      L"FT008_ICD-10_BILLABLE-FLAGGED_ORDER_CODES"
   },
   {
      MULTI_LINE,
      L"[Mixed Data] Multi-Line GSN Daily Weather",
      L"MultiLineGSN.txt",
      L"FT009_MULTI-LINE_GSN_DAILY_WEATHER"
   }
};
