@echo off
SET YYYY=%DATE:~0,4%
SET MM=%DATE:~5,2%
SET DD=%DATE:~8,2%
SET HH=%TIME:~0,2%
SET MI=%TIME:~3,2%
SET SS=%TIME:~6,2%

echo #ifndef PARAM_DEF_ > %1
echo #define PARAM_DEF_ >> %1
echo #define COMPILE_TIME "%YYYY%-%MM%-%DD% %HH%:%MI%:%SS%" >> %1
echo #endif >> %1