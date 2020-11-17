#pragma once

#include <string>

using std::wstring;

struct SampleFileInfo {
   wstring display_name;
   wstring file_name;
   wstring file_type;
};

const SampleFileInfo gSampleSingleRecs[] =
{
   {
      L"Weather Stations List",
      L"ghcnd-stations.txt",
      L"FT001_WEATHER_STATIONS_LIST"
   },
   {
      L"Daily Weather Data",
      L"AYM00089664.dly",
      L"FT002_DAILY_WEATHER_DATA"
   },
   {
      L"ICD-10 Diagnosis Codes",
      L"icd10cm_codes_2021.txt",
      L"FT003_ICD-10_DIAGNOSIS_CODES"
   },
   {
      L"ICD-10 Order Codes",
      L"icd10cm_order_2021.txt",
      L"FT004_ICD-10_ORDER_CODES"
   },
   {
      L"ICD-10 Procedure Codes",
      L"icd10pcs_codes_2021.txt",
      L"FT005_ICD-10_PROCEDURE_CODES"
   }
};

const SampleFileInfo gSampleMultiRecs[] =
{
   {
      L"GSN-Flagged Weather Stations List",
      L"ghcnd-stations.txt",
      L"FT006_GSN-FLAGGED_WEATHER_STATIONS"
   },
   {
      L"Multiple GSN Daily Weather",
      L"MultiGSN.txt",
      L"FT007_MULTIPLE_GSN_DAILY_WEATHER"
   },
   {
      L"ICD-10 Billable-Flagged Order Codes",
      L"icd10cm_order_2021.txt",
      L"FT008_ICD-10_BILLABLE-FLAGGED_ORDER_CODES"
   }
};

const SampleFileInfo gSampleMultiLines[] =
{
   {
      L"Multi-Line GSN Daily Weather",
      L"MultiLineGSN.txt",
      L"FT009_MULTI-LINE_GSN_DAILY_WEATHER"
   }
};
